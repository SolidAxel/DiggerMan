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
	for (int row = 0; row < 64; row++)
	{
		for (int column = 0; column < 64; column++)
		{
			//form the mineshaft
			if (row >= 30 && row < 34 && column >= 4 && column < 60){
				m_dirt[row][column] = nullptr;
				continue;
			}
			if (column>=60) {
				m_dirt[row][column] = nullptr;
				continue;
			}

			m_dirt[row][column] = new Dirt(this, row, column);
		}
	}
	//create a  player:
	dm = new DiggerMan(this);
    pr = new Protester(this);
    hr = new HardcoreProtester(this);
	addActorToGame(dm);
	int GxPosition = 0, GyPosition = 0, Gradius = 4;
	//Gold Nuggets
	//addActorToGame(new GoldNuggets(this, 34, 40, 1));
	int G = static_cast<int> (fmax(5 - getLevel() / 2, 2));
	for (int i = 0; i < G; i++)
	{
		GxPosition = rand() % 60;
		GyPosition = rand() % 56;
		//while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
		while ((GxPosition >= 30 && GxPosition < 34 && GyPosition>4 && GyPosition < 60) || GyPosition>60 || GyPosition == 0) {
			GxPosition = rand() % 60;
			GyPosition = rand() % 56;
		}
		//check that there's no other items within a radius of 6
		if (AreObjectsClose(GxPosition, GyPosition, Gradius))
		{
			GxPosition = rand() % 60;
			GyPosition = rand() % 56;
			//while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
			while ((GxPosition >= 30 && GxPosition < 34 && GyPosition >= 4 && GyPosition < 60) || GyPosition>60 || GyPosition == 0)
			{
				GxPosition = rand() % 60;
				GyPosition = rand() % 56;
			}
		}
		addActorToGame(new GoldNuggets(this, GxPosition, GyPosition, 1));
		//remove dirt just to show where gold is for testing
		//remember to remove the removeDirt function!!
		removeDirt(GxPosition, GyPosition);
	}
	//Boulders
	int xPosition = 0, yPosition = 0, radius = 6;
	int B = static_cast<int>(fmin(getLevel() / 2 + 2, 7));
	cerr << "Number of Boulders this round = " << B << endl;
	for (int i = 0; i < B; i++)
	{
		xPosition = rand() % 60;
		yPosition = rand() % 56;
		//while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
		while ((xPosition >= 30 && xPosition < 34 && yPosition>4 && yPosition < 60) || yPosition>60 || yPosition == 0){
			xPosition = rand() % 60;
			yPosition = rand() % 56;
		}
		//check that there's no other items within a radius of 6
		if (AreObjectsClose(xPosition, yPosition, radius))
		{
			xPosition = rand() % 60;
			yPosition = rand() % 56;
			//while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
			while ((xPosition >= 30 && xPosition < 34 && yPosition >= 4 && yPosition < 60) || yPosition>60 || yPosition == 0)
			{
				xPosition = rand() % 60;
				yPosition = rand() % 56;
			}
		}
		addActorToGame(new Boulders(this, xPosition, yPosition));
		removeDirt(xPosition, yPosition);
	}
	

	return GWSTATUS_CONTINUE_GAME;

}
int StudentWorld::move(){
	// This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
	//decLives();
	//cerr << "FROM move() " << endl;
	DisplayText();
	if (dm->isAlive())
	{
		dm->doSomething();
	}
	else
	{
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
    if (hr -> isAlive()) {
        int wait = ticksToWaitBetweenMoves;
        hr -> ticks = hr -> numSquaresToMoveInCurrentDirection();
        while(wait > 0){
            wait--;
        }
        if (hr -> ticks > 0) {
            hr -> doSomething();
            hr -> ticks--;
        }
        else
        {
            hr -> randomizeDir();
        }
    }
    if (pr-> isAlive()) {
        int wait = ticksToWaitBetweenMoves;
        pr -> ticks = pr -> numSquaresToMoveInCurrentDirection();
        while(wait > 0){
            wait--;
        }
        if (pr -> ticks > 0) {
            pr -> doSomething();
            pr-> ticks--;
        }
        else
            pr -> randomizeDir();

    }
	auto it = m_v.begin();
	while (it != m_v.end())
	{
		if (!(*it)->isAlive())
		{
			delete *it;
			it = m_v.erase(it);
		}
		else
		{
			(*it)->doSomething();
			it++;
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()
{
	for (int r = 0; r < 64; r++)
	{
		for (int c = 0; c < 60; c++)
		{
			delete m_dirt[r][c];
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
void StudentWorld::addActorToGame(Actor *Act)
{
	m_v.push_back(Act);
}
bool StudentWorld::AreObjectsClose(int x, int y, int radius){
	bool isClose = false;
	vector<Actor*>::iterator it = m_v.begin();
	while (it != m_v.end() )
	{
		if ((*it)->getID() != 7)
		{
			//distance = sqrt(pow((x - xPosition), 2) + pow((y - yPosition), 2));
			//distance = EuclidanDistance(x, y, xPosition, yPosition);
			if (EuclidanDistance(x, y, (*it)->getX(), (*it)->getY()) <= radius) {
				isClose = true;
				cerr << "From studentworld::AreObjectsClose, is the object close : " << isClose << endl;
				return isClose;
			}
		}
	
		it++;
	}
	//cerr << "From studentworld::AreObjectsClose, is the object close : " << isClose << endl;
	return isClose;
}
int StudentWorld::annoyAllNearbyActs(Actor* Act, int x, int y, int radius)
{
	int count = 0;
	if (Act->getID() == IMID_BOULDER)
	{
		if (EuclidanDistance(x, y, dm->getX(), dm->getY()) <= radius)
		{
			dm->hitByBoulder();
			count++;
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
				count++;
			}
		}
	}
	cerr << "number of dm annoyed by boulder: " << count << endl;
	return count;
}
void StudentWorld::removeDirt(int x, int y)
{
	for (int r = x; r < x + 4; r++)
	{
		for (int c = y; c < y + 4; c++)
		{
			if (m_dirt[r][c] != nullptr && c < 60 && r < 64)
			{
				playSound(SOUND_DIG);
				delete m_dirt[r][c];
				m_dirt[r][c] = nullptr;
			}
		}
	}
}
bool StudentWorld::canActMoveTo(Actor* Act,int x, int y)
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
	if (isThereDirtGrid(x, y) == true) 
		return false;
	return true;
}
bool StudentWorld::isThereDirtGrid(int x, int y)
{
	//out of bounds return no dirt grid
	if (x < 0 || y < 0 || x > 60 || y > 60)		//only works if I change y>60 to y>=60
		return false;

	for (int r = x; r < x + 4; r++)
	{
		for (int c = y;c < y + 4; c++)
		{
			if (m_dirt[r][c] != nullptr) {
				std::cout << "I returned" << std::endl;
				return true;
			}
		}
	}
	return false;
}
bool StudentWorld::isThereDirtX(int x, int y)
{
	for (int r = x; r < x + 4; r++)
	{
		if (r < 64 && m_dirt[r][y] != nullptr)
			return true;
	}
	return false;
}
bool StudentWorld::isThereBoulderTop(int x, int y)
{
	vector<Actor*>::iterator it;
	it = m_v.begin();
	while (it != m_v.end())
	{
		if ((*it)->getID() == IMID_BOULDER)
		{
			for (int r = x - 3; r < x + 4; r++)
			{
				if (r >= 0 && r < 64 && (*it)->getY() == y - 4 && (*it)->getX() == r)
					return true;
			}
		}
		it++;
	}
	return false;
}
bool StudentWorld::isThereBoulder(int x, int y)
{
	vector<Actor*>::iterator it;
	it = m_v.begin();
	while (it != m_v.end())
	{
		if ((*it)->getID() == IMID_BOULDER)
		{
			if (EuclidanDistance(x, y, (*it)->getX(), (*it)->getY()) <= 3)	//make it check if within radius of 3
				return true;
		}
		it++;
	}
	return false;
}
void StudentWorld::DisplayText()
{
	int score = getScore();
	int level = getLevel();
	int lives = getLives();
	int health = dm->getHealth();
	int squirts = dm->getSquirts();
	int gold = dm->getGoldPicked();
	int sonar = dm->getSonarsCount();

	stringstream os;
	os << "  Level: " << setw(2) << level;
	os << "  Lives: " << setw(1) << lives;
	os << "  Health: " << setw(3) << health * 10 << '%';
	os << "  Water: " << setw(2) << squirts;
	os << "  Gold: " << setw(2) << gold;
	os << "  Sonar: " << setw(2) << sonar;
	os.fill('0');
	os << "  Scr: " << setw(6) << score;

	//os << "   Oil Left: "<<setw(2) << barrels;
	setGameStatText(os.str());
}
void StudentWorld::addScore(int score)
{
	increaseScore(score);
}
void StudentWorld::addInvetory(int ID)
{
	if (ID == 5)
	{
		//if ID is barrel do something
	}
	//if ID is gold then increase gold count
	if (ID == 7)
	{
		dm->incGoldNug();
	}
	//etc...
}
double StudentWorld::EuclidanDistance(int x1, int y1, int x2, int y2){
	return sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2));
}
