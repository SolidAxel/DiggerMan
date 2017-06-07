#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp
/* can only use this GraphObject's member functions in our program
GraphObject(int imageID, int startX, int startY, DIRECTION startDirection, float size = 1.0, unsigned int depth = 0) {
public:
void setVisible(bool shouldIDisplay);
void getX() const;
void getY() const;
void moveTo(int x, int y);
DIRECTION getDirection() const; // Directions: up, down, left, right void
setDirection(DIRECTION d); // Directions: up, down, left, right
}
*/

/**********************************************
*       ACTOR CLASS                           *
**********************************************/
Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, unsigned int Ticks)
	: GraphObject(imageID, startX, startY, dir, size, depth), m_world(world), m_alive(true), ticks(Ticks)
{
	setVisible(true);
}
Actor::~Actor() {
	setVisible(false);
}
void Actor::isAnnoyed() {

}
void Actor::hitByBoulder() {
	isDead();
}
void Actor::doSomething() {}
StudentWorld* Actor::getWorld() {
	return m_world;
}
bool Actor::isAlive() const {
	return m_alive;
}
void Actor::isDead() {
	m_alive = false;
	setVisible(false);
}
void Actor::moveToIfCan(int x, int y)
{
	bool canMove = getWorld()->canActMoveTo(this, x, y);	//checks conditions
	if (!canMove)
		return;
	moveTo(x, y);
}

/****************************************************
*       CHARACTERS CLASS                            *
****************************************************/
Character::Character(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int health, unsigned int Ticks)
	: Actor(world, imageID, startX, startY, dir, size, depth, Ticks), m_health(health) 
	//maybe add health to ctor to initialize m_health?
{
	setVisible(true);
}
Character::~Character()
{
	setVisible(false);
}
int Character::getHealth()const {
	return m_health;
}
void Character::setHealth(int health) {
	m_health = health;
}
void Character::changeHealth(int health) {
	m_health -= health;
}
void Character::decHealth()
{
	m_health--;
}
void Character::isAnnoyed() {

}
void Character::hitByBoulder() {
	changeHealth(100);//chage the health by 100 points if hit by boulder
	isDead();//set player to dead to decrement one life
	getWorld()->playSound(SOUND_PLAYER_GIVE_UP);//play sound of character giving up
}
void Character::doSomething() {

}


/****************************************************
*       DIGGERMAN CLASS                             *
****************************************************/
DiggerMan::DiggerMan(StudentWorld* world, unsigned int sOil) 
	: Character(world, IMID_PLAYER, 30, 60, right, 1.0, 0, 10, 0), m_oil(sOil)
{
	cerr << "DiggerMan position (x,y): \n " << "( " << getX() << ',' << getY() << " )" << endl;
	m_squirt = 5;
	m_sonar = 1;
	m_goldNug = 0;
	//m_oil = 0;
	//m_HitPt = 10;
	setVisible(true);
}
DiggerMan::~DiggerMan() {
	setVisible(false);
}
void DiggerMan::doSomething()
{
	if (!isAlive())
		return;
	getWorld()->removeDirt(getX(), getY());
	keyInput();

}
void DiggerMan::keyInput()
{
	int i;
	Direction d = getDirection();
	if (isSonarOn)//NEW
		sonarLifeTimeTicks--;//NEW
	if (sonarLifeTimeTicks == 0 && isSonarOn)//NEW
	{
		isSonarOn = false;
		getWorld()->Sonar(isSonarOn, getX(), getY(), sonarRadius);
	}
	if (getWorld()->getKey(i))
	{
		switch (i)
		{
		case KEY_PRESS_ESCAPE:	//restart level/game
			isDead();
			break;
		case KEY_PRESS_SPACE:	//fire water
			if (m_squirt <= 0)
				break;
			decSquirts();
			switch (d)
			{//(StudentWorld*world, int StartX, int StartY,Direction dir) 
			case left:
				getWorld()->addActorToGame(new Squirt(getWorld(), getX() - 4, getY(), d));
				break;
			case right:
				getWorld()->addActorToGame(new Squirt(getWorld(), getX() + 4, getY(), d));
				break;
			case down:
				getWorld()->addActorToGame(new Squirt(getWorld(), getX(), getY() - 4, d));
				break;
			case up:
				getWorld()->addActorToGame(new Squirt(getWorld(), getX(), getY() + 4, d));
				break;
			default:
				break;
			}
			getWorld()->playSound(SOUND_PLAYER_SQUIRT);
			break;
		case KEY_PRESS_LEFT:		//move or change directions
			if (d == left)
				moveToIfCan(getX() - 1, getY());
			else
				setDirection(left);
			break;
		case KEY_PRESS_RIGHT:
			if (d == right)
				moveToIfCan(getX() + 1, getY());
			else
				setDirection(right);
			break;
		case KEY_PRESS_UP:
			if (d == up)
				moveToIfCan(getX(), getY() + 1);
			else
				setDirection(up);
			break;
		case KEY_PRESS_DOWN:
			if (d == down)
				moveToIfCan(getX(), getY() - 1);
			else
				setDirection(down);
			break;
		case 'z':
		case 'Z':			//use sonar kit
			if (m_sonar > 0)
			{
				isSonarOn = true;//NEW
				sonarLifeTimeTicks = 30;//NEW
				decSonar();//NEW
				getWorld()->playSound(SOUND_SONAR);//NEW
				getWorld()->Sonar(isSonarOn, getX(), getY(), sonarRadius);//NEW
			}
			break;
		case KEY_PRESS_TAB:		//bribe with gold
			if (getGoldPicked() > 0)
			{
				decGoldNug();
				getWorld()->addActorToGame(new GoldNuggets(getWorld(), getX(), getY(), 2));
			}
			break;

		}
	}
}
void DiggerMan::isAnnoyed()
{
	changeHealth(2);
	if (getHealth() <= 0)
	{
		isDead();
		getWorld()->playSound(SOUND_PLAYER_ANNOYED);
	}
}
unsigned int DiggerMan::getSquirts() const
{
	return m_squirt;
}
unsigned int DiggerMan::getSonarsCount()const
{

	return m_sonar;
}
unsigned int DiggerMan::getOilCount()const
{
	return m_oil;
}
unsigned int DiggerMan::getGoldPicked()const
{
	return m_goldNug;
}
void DiggerMan::decSquirts() {
	m_squirt--;
}
void DiggerMan::incSquirts(int squirts) {
	m_squirt += squirts;
	//m_squirt++;
}
void DiggerMan::incGoldNug() {
	m_goldNug++;
}
void DiggerMan::decGoldNug() {
	m_goldNug--;
}
void DiggerMan::decSonar() {
	m_sonar--;
}
void DiggerMan::incSonar() {
	m_sonar++;
}
void DiggerMan::incOil() {
	m_oil++;
}
void DiggerMan::decOil() {
	m_oil--;
}

/****************************************************
*       PROTESTER CLASS                             *
****************************************************/
Protester::Protester(StudentWorld * world,const int img,int life) : Character(world,img,60,60,left,1.0,0,life,0)
{
	setVisible(true);
	setTicksToWaitBewtweenMoves();
}
Protester::~Protester()
{
	setVisible(false);
}
bool Protester::isItInRestState()
{
	if (ticksToWaitBetweenMoves > 0)
		return true;
	else
	{
		setTicksToWaitBewtweenMoves();
		nonRestingTicks++;
	}
	return false;
}
void Protester::setTicksToWaitBewtweenMoves()
{
	ticksToWaitBetweenMoves = static_cast<int>(fmax(0, 3 - getWorld()->getLevel()));
}

void Protester::moveInThisDirection(Direction dir)
{
	switch (dir)
	{
	case up:
		setDirection(up);
		if (getWorld()->canMoveXYdir(getX(), getY(), up))
			moveUp();
		break;
	case down:
		setDirection(down);
		if (getWorld()->canMoveXYdir(getX(), getY(), down))
			moveDown();
		break;
	case right:
		setDirection(right);
		if (getWorld()->canMoveXYdir(getX(), getY(), right))
			moveRight();

		break;
	case left:
		setDirection(left);
		if (getWorld()->canMoveXYdir(getX(), getY(), left))
			moveLeft();
		break;
	case none:
		setDirection(right);
		if (getWorld()->canMoveXYdir(getX(), getY(), right))
			moveRight();
		else isDead();
		return;
		break;
	}
	return;
}
bool Protester::moveLeft()
{
	if (getWorld()->canActMoveTo(this, getX() - 1, getY()))
	{
		moveTo(getX() - 1, getY());
		return true;
	}
	return false;
}
bool Protester::moveRight()
{
	if (getWorld()->canActMoveTo(this, getX() + 1, getY()))
	{
		moveTo(getX() + 1, getY());
		return true;
	}
	return false;
}
bool Protester::moveUp()
{
	if (getWorld()->canActMoveTo(this, getX(), getY() + 1))
	{
		moveTo(getX(), getY() + 1);
		return true;
	}
	return false;
}
bool Protester::moveDown()
{
	if (getWorld()->canActMoveTo(this, getX(), getY() - 1))
	{
		moveTo(getX(), getY() - 1);
		return true;
	}
	return false;
}

/****************************************************
*       REGULAR-PROTESTER CLASS                    *
****************************************************/
RegularProtester::RegularProtester(StudentWorld* world) : Protester(world,IMID_PROTESTER,5), leaveOilField(false), hasYelled(false), waitToYell(0)
{
	setVisible(true);
	setTicksToWaitBewtweenMoves();
}
RegularProtester::~RegularProtester()
{
	setVisible(false);
}

void RegularProtester::doSomething()
{
	//cerr << "From inside protester do somenthing();" << endl;
	if (!isAlive())
	{
		//cerr << "FROM INSIDE !isAlive dosomenthing if: " << endl;
		return;
	}
	else if (isItInRestState())
	{
		//cerr << "From inside IsItINrestate if protester do somenthing();" << endl;
		decTicksBetweenMoves();
		if (hasYelled && waitToYell != 0)
			waitToYell--;
		return;
	}
	else if (leaveOilField)
	{
		//cerr << "From inside leaveoilfield if protester do somenthing();" << endl;
		if (getX() == 60 && getY() == 60)
		{
			isDead();
			return;
		}
		else
		{
			moveInThisDirection(getWorld()->nextDirectionToTake(this, leaveOilField));
			return;
		}
	}
	else if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), 16))//checks to see if protester is within the radius of diggerman by 4 units
	{
		if (getWorld()->isProtestorFacingDiggerMan(getX(), getY(), getDirection()))
		{
			if (waitToYell == 0)
			{
				waitToYell = 15;
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				getWorld()->annoyDiggerMan();
				hasYelled = true;
				return;
			}
		}
	}
	else if (getWorld()->isProtestorInHorizontalLineOfSight(getX()) || getWorld()->isProtestorInVerticalLineOfSight(getY()))
	{
		if (!getWorld()->IsItCloseToDiggerMan(getX(), getY(), 4))
			if (getWorld()->canProtestorReachDiggerMan(getX(), getY()))
			{
				setDirection(getWorld()->setDirectionOfProtetorForChase(getX(), getY()));
				if (getDirection() == left)
				{
					moveLeft();
					numSquares = 0;
					return;
				}
				else if (getDirection() == right)
				{
					moveRight();
					numSquares = 0;
					return;
				}
				else if (getDirection() == down)
				{
					moveDown();
					numSquares = 0;
					return;
				}
				else if (getDirection() == up)
				{
					moveUp();
					numSquares = 0;
					return;
				}
			}
	}
	else
	{
		if (numSquares > 0)
		{
			if (getDirection() == left)
			{
				if (!moveLeft())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == right)
			{
				if (!moveRight())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == down)
			{
				if (!moveDown())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == up)
			{
				if (!moveUp())
					numSquares = 0;
				else
					numSquares--;
			}
		}
		else
		{
			setDirection(getWorld()->setDirectionOfProtetorForRegularWalking(getX(), getY()));
			setNumSquaresToMoveInCurrentDirection();
			if (getWorld()->protestorIsAtIntersection(getX(), getY(), getDirection()))
			{
				if (getNonRestingTicks() % 200 == 0)
				{
					setDirection(getWorld()->setDirectionAtIntersection(getX(), getY(), getDirection()));
					setNumSquaresToMoveInCurrentDirection();
				}
			}
			if (getDirection() == left)
			{
				if (!moveLeft())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == right)
			{
				if (!moveRight())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == down)
			{
				if (!moveDown())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == up)
			{
				if (!moveUp())
					numSquares = 0;
				else
					numSquares--;
			}
		}
	}
}
void RegularProtester::isAnnoyed()
{
	if (getHealth() <= 0 && leaveOilField !=true)
	{
		leaveOilField = true;
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setTicksBetweenMoves(0);
		//cerr << "FROM PROTESTER IS ANNOYED AND GIVING UP:" << endl;
		doSomething();
	}
	else
	{
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		decHealth();
		decHealth();
        setTicksBetweenMoves(getNonRestingTicks() +static_cast<int>(fmax(50, 100 - getWorld()->getLevel() * 10)));
	}
}
void RegularProtester::setleaveOilField(bool m)
{
	leaveOilField = m;
}
bool RegularProtester::getLeaveOilField()const
{
	return leaveOilField;
}

/****************************************************
*       HARDCORE-PROTESTER CLASS                   *
****************************************************/
HardcoreProtester::HardcoreProtester(StudentWorld* world) : Protester(world, IMID_HARD_CORE_PROTESTER, 20), leaveOilField(false), hasYelled(false), waitToYell(0)
{
	setVisible(true);
	setTicksToWaitBewtweenMoves();
}
HardcoreProtester::~HardcoreProtester()
{
	setVisible(false);
}
void HardcoreProtester::isAnnoyed()
{
	if (getHealth() <= 0 && leaveOilField != true)
	{
		leaveOilField = true;
		getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        setTicksBetweenMoves(0);
        //cerr << "FROM PROTESTER IS ANNOYED AND GIVING UP:" << endl;
		doSomething();
	}
	else
	{
		getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
		decHealth();
		decHealth();
        setTicksBetweenMoves(getTicksBetweenMoves() + static_cast<int>(fmax(50, 100 - getWorld()->getLevel() * 10)));
	}
}
void HardcoreProtester::doSomething()
{
	
	//cerr << "From inside protester do somenthing();" << endl;
	if (!isAlive())
	{
		//cerr << "FROM INSIDE !isAlive dosomenthing if: " << endl;
		return;
	}
	else if (isItInRestState())
	{
		//cerr << "From inside IsItINrestate if protester do somenthing();" << endl;
        decTicksBetweenMoves();
        if (hasYelled && waitToYell != 0)
			waitToYell--;
		return;
	}
	else if (leaveOilField)
	{
		//cerr << "From inside leaveoilfield if protester do somenthing();" << endl;
		if (getX() == 60 && getY() == 60)
		{
			isDead();
			return;
		}
		else
		{
			/*setDirection(getWorld()->nextDirectionToTake(this, leaveOilField));
			if (getDirection() == left)
			{
			moveLeft();

			return;
			}
			else if (getDirection() == right)
			{
			moveRight();

			return;
			}
			else if (getDirection() == down)
			{
			moveDown();

			return;
			}
			else if (getDirection() == up)
			{
			moveUp();

			return;
			}*/
			moveInThisDirection( getWorld()->nextDirectionToTake(this, leaveOilField));
			return;
		}
	}
	else if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), 4))//checks to see if protester is within the radius of diggerman by 4 units
	{
		if (getWorld()->isProtestorFacingDiggerMan(getX(), getY(), getDirection()))
		{
			if (waitToYell == 0)
			{
				waitToYell = 15;
				getWorld()->playSound(SOUND_PROTESTER_YELL);
				getWorld()->annoyDiggerMan();
				hasYelled = true;
				return;
			}
		}
	}
	//this for hardcore protester only
	else if (findDiggerMan())
	{
		cerr << "From inside doSomenthing findDiggerman if block!!!!!" << endl;
		return;
	}
	else if (getWorld()->isProtestorInHorizontalLineOfSight(getX()) || getWorld()->isProtestorInVerticalLineOfSight(getY()))
	{
		if (!getWorld()->IsItCloseToDiggerMan(getX(), getY(), 4))
			if (getWorld()->canProtestorReachDiggerMan(getX(), getY()))
			{
				setDirection(getWorld()->setDirectionOfProtetorForChase(getX(), getY()));
				if (getDirection() == left)
				{
					moveLeft();
					numSquares = 0;
					return;
				}
				else if (getDirection() == right)
				{
					moveRight();
					numSquares = 0;
					return;
				}
				else if (getDirection() == down)
				{
					moveDown();
					numSquares = 0;
					return;
				}
				else if (getDirection() == up)
				{
					moveUp();
					numSquares = 0;
					return;
				}
			}
	}
	else
	{
		if (numSquares > 0)
		{
			if (getDirection() == left)
			{
				if (!moveLeft())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == right)
			{
				if (!moveRight())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == down)
			{
				if (!moveDown())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == up)
			{
				if (!moveUp())
					numSquares = 0;
				else
					numSquares--;
			}
		}
		else
		{
			setDirection(getWorld()->setDirectionOfProtetorForRegularWalking(getX(), getY()));
			setNumSquaresToMoveInCurrentDirection();
			if (getWorld()->protestorIsAtIntersection(getX(), getY(), getDirection()))
			{
				if (getNonRestingTicks() % 200 == 0)
				{
					setDirection(getWorld()->setDirectionAtIntersection(getX(), getY(), getDirection()));
					setNumSquaresToMoveInCurrentDirection();
				}
			}
			if (getDirection() == left)
			{
				if (!moveLeft())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == right)
			{
				if (!moveRight())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == down)
			{
				if (!moveDown())
					numSquares = 0;
				else
					numSquares--;
			}
			else if (getDirection() == up)
			{
				if (!moveUp())
					numSquares = 0;
				else
					numSquares--;
			}
		}
	}

}
bool HardcoreProtester::findDiggerMan()
{
	M = 16 + (getWorld()->getLevel() * 2);
	if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), M))
	{
		moveInThisDirection(getWorld()->nextDirectionToTake(this, false));
		return true;
	}
	else
		return false;
}
bool HardcoreProtester::getLeaveOilField()const
{
	return leaveOilField;
}


/*****************************************************
*       DIRT CLASS                                   *
* -This class makes up the "background" for the game *
*****************************************************/
Dirt::Dirt(StudentWorld* world, int StartX, int StartY) : Actor(world, IMID_DIRT, StartX, StartY, right, 0.25, 3, 0)
{
	setVisible(true);
}
Dirt::~Dirt() {
	setVisible(false);
}
void Dirt::doSomething() {
	//NOt implemented for dirt
}

/****************************************************
*       ITEMS CLASS                                 *
****************************************************/

Items::Items(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) 
	: Actor(world, imageID, startX, startY, dir, size, depth, 0), m_count(0)
{
	setVisible(true);
}
Items::~Items() {
	setVisible(false);
}
unsigned int Items::getItemCount() {
	return m_count;
}
void Items::setItemCount(unsigned int count)
{
	m_count = count;
}
void Items::doSomething()
{

}



/*****************************************************
*       GoldNuggets CLASS                            *
*                                                    *
*****************************************************/
GoldNuggets::GoldNuggets(StudentWorld * world, int startX, int startY, int state)
	: Items(world, IMID_GOLD, startX, startY, right, 1, 0), State(state)
{
	cerr << "GoldNugget position (x,y): \n " << "( " << getX() << ',' << getY() << " )" << endl;
	if (State == 1)
	{
		setVisible(false);
		setScore(10); //when diggerman picks up gold
	}
	if (State == 2)
	{
		cerr << "State 2\n";
		m_waiting = true;
	}
}
GoldNuggets::~GoldNuggets()
{
	setVisible(false);
}
void GoldNuggets::doSomething()
{
	if (isAlive())
	{
		if (State == 1)
		{
			//checks to see if diggerman is close
			//if he is within radius of 4 then setvisiable
			if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), 4))
			{
				setVisible(true);
				
			}
			//if diggerman is within radius of 3
			//update score and add gold to inventory
			if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), 3))
			{
				
				
				getWorld()->playSound(SOUND_GOT_GOODIE);
				getWorld()->addScore(getScore());
				getWorld()->addInvetory(getID());
				isDead();
			}
		}
		//if the gold is dropped by the diggerman
		//temporary state or (State 2)
		else if (State == 2)
		{
			if (ticks < 100 && m_waiting)
			{
				if (getWorld()->IsItCloseToProtester(getX(), getY(), 3) && pickedUp == false)
				{
					m_waiting = false;
					isDead();
					getWorld()->aProtestorActivatesNugget(rpScore);
				}
				else if (getWorld()->IsItCloseToHardcoreProtester(getX(), getY(), 3) && pickedUp == false)
				{
					m_waiting = false;
					isDead();
					getWorld()->aHardcoreProtestorActivatesNugget(hpScore);
				}
				else
					ticks++;
			}
			else if (ticks >= 100)
			{
				m_waiting = false;
				isDead();
			}
		}
	}
	else //if dead
		return;
}
void GoldNuggets::setPickedUp(bool flag)
{
	pickedUp = flag;
}


/*****************************************************
*       Boulder CLASS                                *
* -This class makes up the "background" for the game *
*****************************************************/

//(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
Boulders::Boulders(StudentWorld*world, int StartX, int StartY)
	:Items(world, IMID_BOULDER, StartX, StartY, down, 1.0, 1), m_stable(true), m_falling(false), m_waiting(false)
{
	cerr << "Boulder position (x,y): \n " << "( " << getX() << ',' << getY() << " )" << endl;
	setScore(500);
}
Boulders:: ~Boulders() {
	setVisible(false);
}
void Boulders::doSomething()
{
	//cerr << "From Boulders DOSOMenthing " << endl;
	if (isAlive())
	{
		if (m_stable)
		{
			if (getWorld()->isThereDirtX(getX(), getY() - 1))
				return;
			else
				m_stable = false;
		}
		if (!m_stable)
			m_waiting = true;
        if (m_waiting){
			if (getTick() < 30)
				incTick();
        }
			else
			{
				m_waiting = false;
				m_falling = true;
			}
		if (m_falling)
		{
			getWorld()->playSound(SOUND_FALLING_ROCK);
			if (!getWorld()->canActMoveTo(this, getX(), getY() - 1))//if can't move die
				isDead();
			else
				moveTo(getX(), getY() - 1);

			getWorld()->annoyAllNearbyActs(this, getX(), getY(), 3);//annoy those it falls on
		}
	}
	else//if not alive return;
		return;
}


/*****************************************************
*       Squirt CLASS                                 *
*                                                    *
*****************************************************/
Squirt::Squirt(StudentWorld*world, int StartX, int StartY, Direction dir) 
	: Items(world, IMID_WATER_SPURT, StartX, StartY, dir, 1.0, 1)
{
	m_travelDistance = 4;
}
Squirt:: ~Squirt()
{
	setVisible(false);
}
void Squirt::doSomething()
{
	if (!isAlive())
	{
		return;
	}
	else
		if (getDirection() == 1)//up
		{
			if (getTick() < 4)
			{
				if (getWorld()->IsItCloseToProtester(getX(), getY(), 3))
				{
					getWorld()->annoyAllNearbyActs(this, getX(), getY(), 3);
					isDead();
					return;
				}
				else if (getWorld()->canActMoveTo(this, getX(), getY() + 1))
				{
					moveTo(getX(), getY() + 1);
					incTick();
				}
				else
					isDead();
			}
			else
				isDead();
		}
		else if (getDirection() == 2)//down
		{
			if (getTick() < 4)
			{
				if (getWorld()->IsItCloseToProtester(getX(), getY(), 3))
				{
					getWorld()->annoyAllNearbyActs(this, getX(), getY(), 3);
					isDead();
					return;
				}
				else if (getWorld()->canActMoveTo(this, getX(), getY() - 1))
				{
					moveTo(getX(), getY() - 1);
					incTick();
				}
				else
					isDead();
			}
			else
				isDead();
		}
		else if (getDirection() == 3)//left
		{
			if (getTick() < 4)
			{
				if (getWorld()->IsItCloseToProtester(getX(), getY(), 3))
				{
					getWorld()->annoyAllNearbyActs(this, getX(), getY(), 3);
					isDead();
					return;
				}
				else if (getWorld()->canActMoveTo(this, getX() - 1, getY()))
				{
					moveTo(getX() - 1, getY());
					incTick();
				}
				else
					isDead();
			}
			else
				isDead();
		}
		else //right
		{
			if (getTick() < 4)
			{
				if (getWorld()->IsItCloseToProtester(getX(), getY(), 3))
				{
					getWorld()->annoyAllNearbyActs(this, getX(), getY(), 3);
					isDead();
					return;
				}
				else if (getWorld()->canActMoveTo(this, getX() + 1, getY()))
				{
					moveTo(getX() + 1, getY());
					incTick();
				}
				else
					isDead();
			}
			else
				isDead();
		}

	

}

/*****************************************************
*       OilBarrels CLASS                             *
*                                                    *
*****************************************************/
OilBarrels::OilBarrels(StudentWorld * world, int startX, int startY)
	: Items(world, IMID_BARREL, startX, startY, right, 1.0, 2)//NEW
{
	setVisible(false);
	setScore(1000);
}
void OilBarrels::doSomething()
{
	if (isAlive())
	{

		//checks to see if diggerman is close
		//if he is within radius of 4 then setvisiable
		//if (getWorld()->AreObjectsClose(getX(), getY(), 4) && pickedUp == false)

		if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), 4))
		{
			setVisible(true);
			
		}
		//if diggerman is within radius of 3
		//update score and add gold to inventory
		if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), 3))
		{
			
			
			getWorld()->playSound(SOUND_FOUND_OIL);
			getWorld()->addScore(getScore());
			getWorld()->addInvetory(getID());
			isDead();
		}
	}
	else //if dead
		return;
}
OilBarrels::~OilBarrels()
{
	setVisible(false);
}


/*****************************************************
*       SonarCharge CLASS                            *
*                                                    *
*****************************************************/
SonarCharge::SonarCharge(StudentWorld * world)//new
	: Items(world, IMID_SONAR, 0, 60, right, 1, 1)//new
{
	setVisible(true);//new
	ticksinTempState = static_cast<int>(fmax(100, 300 - 10 * world->getLevel()));//new
	setScore(75);
}
void SonarCharge::doSomething()//new
{
	if (isAlive())
	{
		if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), 3))//if its near the diggerman by 3 units in any direction(raidally)
		{
			isDead();//kill it bro because it already got caught by DM
			getWorld()->playSound(SOUND_GOT_GOODIE);//play sound for picking up the goodies ;)P
			getWorld()->increaseScore(getScore());//increase this guys score cuz he just poicked up a sonar ma boi
			getWorld()->addInvetory(getID());//send the id back to the function so it can know what it is so it can know what to add for the DM
		}
		else//if not then continue to the countdown of the ticks
		{
			if (ticksinTempState > 0)//checks if it ran out of ticks already
			{
				ticksinTempState--;//decrements it by 1 if it hasnt ran out yet
			}
			else//if its reahced zero then kill it bro
				isDead();//bam dead!
		}
	}
	else
		return;
}
SonarCharge::~SonarCharge()
{
	setVisible(false);
}

/*****************************************************
*       WaterPool CLASS                              *
*                                                    *
*****************************************************/
WaterPool::WaterPool(StudentWorld* world, int startX, int startY) : Items(world, IMID_WATER_POOL, startX, startY, right, 1.0, 2)
{
	setVisible(true);
	ticksinTempState = static_cast<int>(fmax(100, 300 - 10 * world->getLevel()));//determines the number of ticks the pool of water should be visible
	cerr << "number of ticks for the water pool: " << ticksinTempState << endl;
	setScore(100);
}
WaterPool::~WaterPool()
{
	//cerr << "From inside waterpool destructor" << endl;
	setVisible(false);
}
void WaterPool::doSomething()
{
	if (!isAlive())
		return;
	else {
		//if diggerman is within radius of 3
		//update score and add water to inventory
		if (getWorld()->IsItCloseToDiggerMan(getX(), getY(), 3))
		{
			
			getWorld()->playSound(SOUND_GOT_GOODIE);
			getWorld()->addScore(getScore());
			getWorld()->addInvetory(getID());
			isDead();
		}
		if (ticksinTempState < 0)//if ticks are less than zero then water is dead (disappears)
		{
			cerr << "WHEN TICKS RUN OUT WATER DISAPPEARS" << endl;
			isDead();
		}
		else
			ticksinTempState--;//decrement the ticks
	}
}


