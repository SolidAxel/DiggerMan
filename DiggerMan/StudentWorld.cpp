#include "StudentWorld.h"
#include <string>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp
/*
methods to use form GAMEWORLD class:
int getLives();
void decLives();//reduces players lives by 1
void incLives();//increases player's lives by 1
getScore();//determines players' score
getLevel();//determines players current level #
increaseScore(int howmuch);//increases players score according to action
setGameStatText(string text);//used to specify text to be displayed
getkey(int&value);//determines key used by player
playSound(int soundID);//plays sound effect
*/

int StudentWorld::init()
{
	//displays dirt:
	for (int column = 0; column < 64; column++)//x is for the columns of dirt
	{
		for (int row = 0; row < 64; row++)//y is for the rows of dirt
		{
			//form the mineshaft
			if (column >= 30 && column < 34 && row >= 4 && row < 60){
				m_dirt[column][row] = nullptr;
				continue;
			}
			if (row >= 60){
				m_dirt[column][row] = nullptr;
				continue;
			}
			m_dirt[column][row] = new Dirt(this, column, row);
		}
	}

	//Boulders
	int xPosition = 0, yPosition = 0, radius = 6;
	int B = static_cast<int>(fmin(getLevel() / 2 + 2, 7));
	cerr << "Number of Boulders this round = " << B << endl;
	randPositions(B, IMID_BOULDER);
	//Gold Nuggets
	int G = static_cast<int> (fmax(5 - getLevel() / 2, 2));
	cerr << "Number of GoldNuggets this round = " << G << endl;
	randPositions(G, IMID_GOLD);
	int O = static_cast<int>(fmin(2 + getLevel(), 18));
	cerr << "Number of Oil Barrels this round = " << O << endl;
	randPositions(O, IMID_BARREL);

	//create a  player:
	dm = new DiggerMan(this, O);

	//Create regular protester in the very first tick of the game;
	pr = new RegularProtester(this);
	addActorToGame(pr);


	return GWSTATUS_CONTINUE_GAME;

}
int StudentWorld::move()
{
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.

	if (OilBarrelsLeft() > 0)
	{
		DisplayText();
		//NEW:
		m_numberoftickssincelastprotester++;//increase the number of ticks
		if (dm->isAlive())
			dm->doSomething();
		else
		{
			decLives();
			return GWSTATUS_PLAYER_DIED;
		}
		//traverse the vector so that all the Actors in it, Do Somenthing! ----------NEW
		for (vector<Actor*>::iterator it = m_v.begin(); it != m_v.end(); it++)
		{
			if ((*it)->isAlive())
				(*it)->doSomething();
		}

		//Adding Goodies to game (water pool and/or sonar charge)
		int G = static_cast<int>(getLevel() * 25 + 300);//according to specs
		int xPosition, yPosition;
		//int randomnumber = rand() % G;//to test the probability of when to add water/sonar//__________________unneccsary
		//cerr << "Probability: " << randomnumber % G << endl;//to display test probability
		if (rand() % G == 0)//this will suffice
		{
			if (rand() % 100 < 20)//20 percent of the time or 1/5
				addActorToGame(new SonarCharge(this));//new
			else
			{//waterpool can appear anywhere in the grid where dirt is removed as long as is not in the boulder's empty space
				xPosition = rand() % 61;
				yPosition = rand() % 61;
				if (!isThereBoulder(xPosition, yPosition))//if no boulder found at x,y position, water can be added
				{
					while (isThereDirtGrid(xPosition, yPosition))//checks if there exist dirt at x,y positions
					{
						xPosition = rand() % 61;
						yPosition = rand() % 61;
					}
					addActorToGame(new WaterPool(this, xPosition, yPosition));//adds water pool to position where there is no dirt
				}
			}
		}

		//FOLLOWING LINES ARE NEW CODE:
		//Adding Protesters to game according to specs
		int T = static_cast<int>(fmax(25, 200 - getLevel()));// T ticks till new protestor is added
		int P = static_cast<int>(fmin(15, 2 + getLevel()*1.5));//P number of protestors that should be on the oil field

		//if number of protesters is less than P, number of protestors on the field
		//and if T ticks have passed since last protestor was added to field then protestors could be added to field.
				//The following lines were for test purpose only:
				//cerr << "# m_protestorcount < P : " << m_protestorscount << " < " << P <<endl;//test purpose only
				//if (m_numberoftickssincelastprotester >= T)//Test purpose only
				//	cerr << "# m_numberoftickssincelastprotester > T : " << m_numberoftickssincelastprotester << " > " << T << endl;//test purpose only
				//
		int m_protestorscount = numberOfProtestorsinField();//number of protestors currently in the field
		if (m_protestorscount<P && m_numberoftickssincelastprotester>T)
		{
			int probabilityOfHardcore = static_cast<int>(fmin(90, getLevel() * 10 + 30));//probability of hardcore protestor from appearing on the field.
			int number = rand() % 100;//test purpose only
			cerr << "number: " << number << " probability: " << probabilityOfHardcore;//test purpose only
			if (number < probabilityOfHardcore)
			{
				//add new harcore protester to field:
				hr = new HardcoreProtester(this);
				addActorToGame(hr);
			}
			else
			{
				//add regular protestor to field:
				pr = new RegularProtester(this);
				addActorToGame(pr);
			}
			m_numberoftickssincelastprotester = 0;//reset the tick count for protestors
		}
		//end of NEW

		//cleans up any dead Actor/Object in the Game as long as DiggerMan is still Alive
		//if DiggerMan is out of lives then cleanup method is called.
		auto it = m_v.begin();
		while (it != m_v.end())
		{
			if (!(*it)->isAlive())
			{
				delete *it;
				it = m_v.erase(it);
			}
			else
				it++;
		}
		return GWSTATUS_CONTINUE_GAME;
	}
	else
		playSound(SOUND_FINISHED_LEVEL);
	return GWSTATUS_FINISHED_LEVEL;
}

//function to cleanup all memory allocated for the game.
void StudentWorld::cleanUp()
{
	//deallocates memory for the oil field
	for (int r = 0; r < 64; r++)
	{
		for (int c = 0; c < 64; c++)
		{
			delete m_dirt[c][r];
		}
	}

	vector<Actor*>::iterator it;
	it = m_v.begin();
	while (it != m_v.end())
	{
		delete *it;
		it = m_v.erase(it);
	}

	delete dm;
	
}

//this function adds an Actor to the vector
void StudentWorld::addActorToGame(Actor *Act)
{
	m_v.push_back(Act);
}

//This function gives a random position in the oil field to  the Boulder, GoldNugget, and Oil Barrel
void StudentWorld::randPositions(int numLvl, int ID)
{
	int x = 0, y = 0, radius = 6;
	for (int i = 0; i < numLvl; i++)
	{
		x = rand() % 60;
		y = rand() % 56;
		//while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
		//Boulders have to be higher than y = 20
		if (ID == IMID_BOULDER)
		{
			while ((x >26 && x < 34) || y < 20 || y > 56)
			{
				x = rand() % 60;
				y = rand() % 56;
				if (AreObjectsClose(x, y, radius)){
					x = rand() % 60;
					y = rand() % 56;
				}
			}
		}
		else
			while ((x >26 && x < 34) || y < 0 || y > 56)
			{
				x = rand() % 60;
				y = rand() % 56;
			}


		////check that there's no other items within a radius of 6
		if (AreObjectsClose(x, y, radius) && m_ObjectsClose == true){

			x = rand() % 60;
			y = rand() % 56;
			if (ID == IMID_BOULDER)
			{
				while ((x>26 && x < 34) || y < 20 || y>56)
				{
					x = rand() % 60;
					y = rand() % 56;
					if (AreObjectsClose(x, y, radius)){
						x = rand() % 60;
						y = rand() % 56;
					}
				}
			}
			else
				while ((x >26 && x < 34) || y < 0 || y > 56)
				{
					x = rand() % 60;
					y = rand() % 56;
				}

		}
		if (ID == IMID_BOULDER)
		{
			addActorToGame(new Boulders(this, x, y));
			removeDirt(x, y);
		}
		//if ID is gold then increase gold count
		else if (ID == IMID_GOLD)
		{
			addActorToGame(new GoldNuggets(this, x, y, 1));
			//removeDirt(x, y);
		}
		//if ID is water pool then icrease the number of squirts by 5
		else if (ID == IMID_BARREL)
		{
			addActorToGame(new OilBarrels(this, x, y));
			removeDirt(x, y);
		}
		//addActorToGame(new Boulders(this, x, y));
		//removeDirt(x, y);
	}
}

void StudentWorld::Sonar(bool state, int x, int y, int radius)//new
{
	vector<Actor*>::iterator it = m_v.begin();
	while (it != m_v.end())
	{
		if (EuclidanDistance(x, y, (*it)->getX(), (*it)->getY()) <= radius)
			if ((*it)->getID() == IMID_GOLD || (*it)->getID() == IMID_BARREL)
				(*it)->setVisible(state);
		it++;
	}

}



//checks if object is close to diggerman position
//Can't use AreObjectsClose because diggerman is not in the vector of actors
bool StudentWorld::IsItCloseToDiggerMan(int x, int y, int radius){

	double distance = EuclidanDistance(x, y, dm->getX(), dm->getY());
	if (distance <= radius)
		return true;
	else
		return false;
}
bool StudentWorld::IsItCloseToProtester(int x, int y, int radius){

	double distance = EuclidanDistance(x, y, pr->getX(), pr->getY());
	if (distance <= radius)
		return true;
	else
		return false;
}
//this function iterates through the vector of actors to check if objects in the vector are close to each other(distance)
bool StudentWorld::AreObjectsClose(int x, int y, int radius){
	bool isClose = false;
	vector<Actor*>::iterator it = m_v.begin();
	while (it != m_v.end())
	{
		/*if ((*it)->getID() == IMID_GOLD){
		double distance = EuclidanDistance((*it)->getX(), (*it)->getY(),dm->getX(),dm->getY());
		if (distance<=radius)
		m_ObjectsClose = true;
		}
		else{*/
		if (EuclidanDistance(x, y, (*it)->getX(), (*it)->getY()) <= radius) {
			isClose = true;
			m_ObjectsClose = true;
			//cerr << "From studentworld::AreObjectsClose, is the object close : " << isClose << endl;
			return isClose;
		}
		//}

		it++;
	}
	//cerr << "From studentworld::AreObjectsClose, is the object close : " << isClose << endl;
	m_ObjectsClose = false;
	return isClose;
}
void StudentWorld::annoyAllNearbyActs(Actor* Act, int x, int y, int radius)
{
	//int count = 0;
	if (Act->getID() == IMID_BOULDER)
	{
		if (EuclidanDistance(x, y, dm->getX(), dm->getY()) <= radius)
		{
			dm->hitByBoulder();
			//count++;
		}
		for (vector<Actor*>::iterator it = m_v.begin(); it != m_v.end(); it++)
		{
			if ((*it)->getID() != IMID_PROTESTER && (*it)->getID() != IMID_HARD_CORE_PROTESTER)
			{
				continue;
			}
			if (EuclidanDistance(x, y, (*it)->getX(), (*it)->getY()) <= radius)
			{
				(*it)->hitByBoulder();
				//count++;
				it = m_v.erase(it);
			}
		}
	}
	//cerr << "number of dm annoyed by boulder: " << count << endl;
	else if (Act->getID() == IMID_WATER_SPURT)
	{
		vector<Actor*>::iterator it = m_v.begin();
		while (it != m_v.end())
		{
			
			if ((*it)->getID() == IMID_PROTESTER || (*it)->getID() == IMID_HARD_CORE_PROTESTER)
			{
			   
					if ((*it)->isAlive())
					{
						(*it)->isAnnoyed();
						it++;
					}
					else
					
						it = m_v.erase(it);	
			}
			else
				it++;
		}
	}
	//return count;
}
//This Function removes dirt
void StudentWorld::removeDirt(int x, int y)
{
	for (int c = x; c< x + 4; c++)
	{
		for (int r = y; r < y + 4; r++)
		{
			if (m_dirt[c][r] != nullptr && c < 64 && r < 64)
			{
				playSound(SOUND_DIG);
				delete m_dirt[c][r];
				m_dirt[c][r] = nullptr;
			}
		}
	}
}
//this function checks that the Actor can move in the x,y direction
bool StudentWorld::canActMoveTo(Actor* Act, int x, int y)
{
	//out of bounds play
	if (x < 0 || x > 60 || y < 0 || y > 60)
		return false;

	//cannot be dirt or boulder
	if (Act->getID() == IMID_BOULDER)
	{
		if (isThereBoulderTop(x, y))
			return false;
	}
	else
	{
		if (isThereBoulder(x, y))
			return false;
	}

	//Diggerman can get through dirt
	if (Act->getID() == IMID_PLAYER)
		return true;

	//otherwise, for squirts, protesters, etc.:
	if (isThereDirtGrid(x, y))
		return false;
	return true;
}
bool StudentWorld::isThereDirtGrid(int x, int y)
{
	//out of bounds return no dirt grid
	if (x < 0 || y < 0 || x > 60 || y > 60)
		return false;

	for (int c = x; c < x + 4; c++)
	{
		for (int r = y; r < y + 4; r++)
		{
			if (m_dirt[c][r] != nullptr)
				return true;
		}
	}
	return false;
}

//this function checks if there is dirt in the x position 
bool StudentWorld::isThereDirtX(int x, int y)
{

	for (int c = x; c < x + 4; c++)
	{
		if (c < 64 && m_dirt[c][y] != nullptr)
			return true;
	}
	return false;
}

//this function checks if there is a boulder on top of the object
bool StudentWorld::isThereBoulderTop(int x, int y)
{
	//iterate through vector of actors till boulder is found
	//then check 
	vector<Actor*>::iterator it;
	it = m_v.begin();
	while (it != m_v.end())
	{
		if ((*it)->getID() == IMID_BOULDER)
		{
			for (int c = x - 3; c < x + 4; c++)
			{
				if (c >= 0 && c < 64 && (*it)->getY() == y - 4 && (*it)->getX() == c)
					return true;
			}
		}
		it++;
	}
	return false;
}

//this function checks if there is a boulder within the radius of 3 of an object
bool StudentWorld::isThereBoulder(int x, int y)
{
	//iterate through the vector of actors until a boulder is found, then check if boulders are within the radius
	vector<Actor*>::iterator it;
	it = m_v.begin();
	while (it != m_v.end())
	{
		if ((*it)->getID() == IMID_BOULDER)//when boulder is found
		{
			if (EuclidanDistance(x, y, (*it)->getX(), (*it)->getY()) <= 3)	//make it check if within radius of 3
				return true;
		}
		it++;
	}
	return false;
}

//this function display info on the text bar in the game
void StudentWorld::DisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = dm->getHealth();
	int squirts = dm->getSquirts();
	int gold = dm->getGoldPicked();
	int sonar = dm->getSonarsCount();
	int Oil = OilBarrelsLeft();
	//int Oil = dm->getOilCount();

	stringstream os;
	os << " Level: " << setw(2) << level;
	os << " Lives: " << setw(1) << lives;
	os << " Health: " << setw(3) << health * 10 << '%';
	os << " Water: " << setw(2) << squirts;
	os << " Gold: " << setw(2) << gold;
	os << " Sonar: " << setw(2) << sonar;
	os << " Oil Left: " << setw(1) << Oil;
	os.fill('0');
	os << " Scr: " << setw(6) << score;

	//os << "   Oil Left: "<<setw(2) << barrels;
	setGameStatText(os.str());
}

//this function increments the player's score
void StudentWorld::addScore(int score)
{
	increaseScore(score);
}

//this function adds items/goodies to DiggerMan's inventory
void StudentWorld::addInvetory(int ID)
{
	if (ID == IMID_BARREL)
	{
		dm->decOil();
	}
	//if ID is gold then increase gold count
	if (ID == IMID_GOLD)
	{
		dm->incGoldNug();
	}
	//if ID is water pool then icrease the number of squirts by 5
	if (ID == IMID_WATER_POOL)
	{
		dm->incSquirts(5);
	}
	if (ID == IMID_SONAR)//new
	{
		dm->incSonar();
	}
	//etc...
}

//This function calculates the distance/radius between two objects
double StudentWorld::EuclidanDistance(int x1, int y1, int x2, int y2){
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}
//this function is used to annoy DiggerMan
void StudentWorld::annoyDiggerMan()
{
	dm->isAnnoyed();
}

bool StudentWorld::isProtestorFacingDiggerMan(int x, int y, int direction)
{
	if (x == dm->getX())//scenarios where its on same x-axis
	{
		if (y < dm->getY() && direction == 1)//if dm is above and protestor looking up
			return true;
		else if (y > dm->getY() && direction == 2)//if dm is below and protestor looking down
			return true;
		else if (y == dm->getY())//its right on diggerman so doesnt matter what direction
			return true;
	}
	else if (y == dm->getY())//scenarios where its on same y-axis
	{
		if (x < dm->getY() && direction == 4)//if dm is to the right and protestor looking right
			return true;
		else if (x > dm->getY() && direction == 3)//if dm is to the left and protestor looking left
			return true;
	}
	else if (x > dm->getX() && y > dm->getY())//if protestor is upper right of dm
	{
		if (direction == 3 || direction == 2)//shoulsd be looking left or down
			return true;
	}
	else if (x < dm->getX() && y > dm->getY())//if protestor is upper left of dm
	{
		if (direction == 4 || direction == 2)//should be looking right or down
			return true;
	}
	else if (x < dm->getX() && y < dm->getY())//if protestor is lower left of dm
	{
		if (direction == 1 || direction == 4)//should be looking up or right
			return true;
	}
	else if (x < dm->getX() && y < dm->getY())//if protestor is lower right of dm
	{
		if (direction == 3 || direction == 1)//should be looking up or left
			return true;
	}
	return false;//send false back if nada
}

bool StudentWorld::isProtestorInVerticalLineOfSight(int y)
{
	if (y == dm->getY())
		return true;
	return false;
}

bool StudentWorld::isProtestorInHorizontalLineOfSight(int x)
{
	if (x == dm->getX())
		return true;
	return false;
}

bool StudentWorld::canProtestorReachDiggerMan(int x, int y)
{
	if (isProtestorInHorizontalLineOfSight(x))//the protestor is in horizontal line of sight
	{
		if (y < dm->getY())
			for (int i = 0; i < (dm->getY() - y); i++)
			{
				if (isThereBoulder(x, y + i + 1))
					return false;
				if (isThereDirt(x, y + i + 1))
					return false;
			}
		else
			for (int i = 0; i < (y - dm->getY()); i++)
			{
				if (isThereBoulder(x, y - (i + 1)))
					return false;
				if (isThereDirt(x, y - (i + 1)))
					return false;
			}
	}
	else if (isProtestorInVerticalLineOfSight(y))
	{
		if (x < dm->getX())
			for (int i = 0; i < (dm->getX() - x); i++)
			{
				if (isThereBoulder(x + i + 1, y))
					return false;
				if (isThereDirt(x + i + 1, y))
					return false;
			}
		else
			for (int i = 0; i < (x - dm->getX()); i++)
			{
				if (isThereBoulder(x - (i + 1), y))
					return false;
				if (isThereDirt(x - (i + 1), y))
					return false;
			}
	}
	return true;//if nothing returned false then nothing must be in the way
}
void StudentWorld::aProtestorActivatesNugget(int score)
{
	playSound(SOUND_PROTESTER_FOUND_GOLD);
	addScore(score);
	pr->leaveOilField = true;
}

void StudentWorld::aHardcoreProtestorActivatesNugget(int score)
{
	playSound(SOUND_PROTESTER_FOUND_GOLD);
	addScore(score);
	//hr->ticksToWaitBetweenMoves += static_cast<int>(fmax(50, 100 - getLevel() * 10));;
}

GraphObject::Direction StudentWorld::setDirectionOfProtetorForChase(int x, int y)
{
	if (isProtestorInHorizontalLineOfSight(x))
	{
		if (y < dm->getY())
			return GraphObject::Direction(1);
		else
			return GraphObject::Direction(2);
	}
	if (x < dm->getX())
		return GraphObject::Direction(4);
	else
		return GraphObject::Direction(3);
	return GraphObject::Direction(0);
}

GraphObject::Direction StudentWorld::setDirectionOfProtetorForRegularWalking(int x, int y)
{
	bool DontGo = true;
	int newDirection;
	while (DontGo)
	{
		newDirection = rand() % 4 + 1;
		if (x == 0)
		{
			while (newDirection == 3)
				newDirection = rand() % 4 + 1;
		}
		else if (x == 64)
		{
			while (newDirection == 4)
				newDirection = rand() % 4 + 1;
		}
		else if (y == 60)
		{
			while (newDirection == 1)
				newDirection = rand() % 4 + 1;
		}
		else if (y == 0)
		{
			while (newDirection == 2)
				newDirection = rand() % 4 + 1;
		}
		if (newDirection == 1)
		{
			if (!isThereDirt(x, y + 1) && !isThereBoulder(x, y + 1))
				DontGo = false;

		}
		else if (newDirection == 2)
		{
			if (!isThereDirt(x, y - 1) && !isThereBoulder(x, y - 1))
				DontGo = false;
		}
		else if (newDirection == 3)
		{
			if (!isThereDirt(x - 1, y) && !isThereBoulder(x - 1, y))
				DontGo = false;
		}
		else
		{
			if (!isThereDirt(x + 1, y) && !isThereBoulder(x + 1, y))
				DontGo = false;
		}
	}
	return GraphObject::Direction(newDirection);
}

bool StudentWorld::protestorIsAtIntersection(int x, int y, int direction)
{
	if (direction == 1 || direction == 2)//up or down then check left and right
	{
		if ((!isThereDirt(x + 1, y) && !isThereBoulder(x + 1, y)) || (!isThereDirt(x - 1, y) && !isThereBoulder(x - 1, y)))
			return true;
	}
	else//left or rigth check up and down
		if ((!isThereDirt(x, y + 1) && !isThereBoulder(x, y + 1)) || (!isThereDirt(x, y - 1) && !isThereBoulder(x, y - 1)))
			return true;

	return false;
}

GraphObject::Direction StudentWorld::setDirectionAtIntersection(int x, int y, int direction)
{
	if (direction == 1 || direction == 2)//going up or down
	{
		if ((!isThereDirt(x + 1, y) && !isThereBoulder(x + 1, y)) && (!isThereDirt(x - 1, y) && !isThereBoulder(x - 1, y)))//if there both viable then randomly choose one
		{
			return GraphObject::Direction(rand() % 4 + 3);
		}
		else if (!isThereDirt(x + 1, y) && !isThereBoulder(x + 1, y))//if its viable to the right
			return GraphObject::Direction(4);
		else//if its viable to go left
			return GraphObject::Direction(3);
	}
	else//going left or right
	{
		if ((!isThereDirt(x, y + 1) && !isThereBoulder(x, y + 1)) && (!isThereDirt(x, y - 1) && !isThereBoulder(x, y - 1)))//if both are viable then choose randomly
		{
			return GraphObject::Direction(rand() % 2 + 1);
		}
		else if (!isThereDirt(x, y + 1) && !isThereBoulder(x, y + 1))//if its viable to go up
			return GraphObject::Direction(1);
		else//viable to go down
			return GraphObject::Direction(2);
	}
	return GraphObject::Direction(0);
}

//This function checks if dirt is present at the x,y positions
bool StudentWorld::isThereDirt(int x, int y)
{
	if (m_dirt[x][y] != nullptr)
		return true;
	return false;
}

//returns the number of protesters in the field//----------------------NEW
int StudentWorld::numberOfProtestorsinField()//----------------------NEW
{
	int count = 0;
	for (vector<Actor*>::iterator it = m_v.begin(); it != m_v.end(); it++)
		if ((*it)->getID() == IMID_PROTESTER || (*it)->getID() == IMID_HARD_CORE_PROTESTER)
			count++;
	return count;
}

//this function returns the current number of oil barrels in the field
int StudentWorld::OilBarrelsLeft()
{
	int count = 0;
	for (vector<Actor*>::iterator it = m_v.begin(); it != m_v.end(); it++)
		if ((*it)->getID() == IMID_BARREL)
			count++;
	return count;
}
