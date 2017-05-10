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
Actor::Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth)
	: GraphObject(imageID, startX, startY, dir, size, depth), m_world(world), m_alive(true)
{
	setVisible(true);
}
Actor::~Actor() {
	setVisible(false);
}
void Actor::isAnnoyed() {

}
void Actor::hitByBoulder() {

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
	std::cout << x << " " << y << std::endl;
	bool canMove = getWorld()->canActMoveTo(this, x, y);	//checks conditions
	if (!canMove)
	{
		std::cout<<canMove<<std::endl;
		return;
	}
	moveTo(x, y);
}

/****************************************************
*       CHARACTERS CLASS                            *
****************************************************/
Character::Character(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int health) : Actor(world, imageID, startX, startY, dir, size, depth), m_health(health) //maybe add health to ctor to initialize m_health?
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
DiggerMan::DiggerMan(StudentWorld* world) : Character(world, IMID_PLAYER, 30, 60, right, 1.0, 0, 10)
{
	m_squirt = 5;
	m_sonar = 1;
	m_goldNug = 0;
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
void DiggerMan::keyInput() {
	int i;
	Direction d = getDirection();
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


				getWorld()->playSound(SOUND_SONAR);
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
	if (getHealth() <= 0) {
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
unsigned int DiggerMan::getGoldPicked()const
{
	return m_goldNug;
}
void DiggerMan::decSquirts() {
	m_squirt--;
}
void DiggerMan::incSquirts() {

	m_squirt++;
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

/*****************************************************
*       DIRT CLASS                                   *
* -This class makes up the "background" for the game *
*****************************************************/
Dirt::Dirt(StudentWorld* world, int StartX, int StartY) : Actor(world, IMID_DIRT, StartX, StartY, right, 0.25, 3)
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
*       REGULAR-PROTESTER CLASS                     *
****************************************************/


Protester::Protester(StudentWorld* world) : Character(world, IMID_PROTESTER, 60, 60 , left, 1.0, 0,5)
{
    setVisible(true);
}
Protester::~Protester(){
    setVisible(false);
}
void Protester::doSomething(){
    int numSquares = numSquaresToMoveInCurrentDirection();
    if (!isAlive())
        return;
    /*if (numSquares > 0) {*/
        if (getDirection() == left) {
            moveLeft();
            numSquares--;
        }
        else if (getDirection() == right){
            moveRight();
            numSquares--;
        }
        else if (getDirection() == down){
            moveDown();
            numSquares--;
        }
        else if(getDirection() == up){
            moveUp();
            numSquares--;
        }
    //}
}
void Protester::isAnnoyed()
{
    changeHealth(2);
    if (getHealth() <= 0){
        isDead();
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    }
}
void Protester::randomizeDir()
{
    int i = (rand() % 4) + 1000;    // randomizes the direction
    switch (i)
    {
        case KEY_PRESS_LEFT:		//move or change directions
                setDirection(left);
            break;
        case KEY_PRESS_RIGHT:
                setDirection(right);
            break;
        case KEY_PRESS_UP:
                setDirection(up);
            break;
        case KEY_PRESS_DOWN:
                setDirection(down);
            break;
            
    }
    
}
void Protester::moveLeft(){
    moveToIfCan(getX() - 1, getY());
}
void Protester::moveRight(){
    moveToIfCan(getX() + 1, getY());
}
void Protester::moveUp(){
    moveToIfCan(getX(), getY() + 1);
}
void Protester::moveDown(){
    moveToIfCan(getX(), getY() - 1);
}
/****************************************************
*       HARDCORE-PROTESTER CLASS                    *
****************************************************/
HardcoreProtester::HardcoreProtester(StudentWorld* world) : Character(world, IMID_HARD_CORE_PROTESTER, 60, 60, left,1, 0, 20)
{
    setVisible(true);
}
HardcoreProtester::~HardcoreProtester(){
    setVisible(false);
}
void HardcoreProtester::doSomething(){
    int numSquares = numSquaresToMoveInCurrentDirection();
    if (!isAlive())
        return;
    if (numSquares > 0) {
        if (getDirection() == left) {
            moveLeft();
            numSquares--;
        }
        else if (getDirection() == right){
            moveRight();
            numSquares--;
        }
        else if (getDirection() == down){
            moveDown();
            numSquares--;
        }
        else if(getDirection() == up){
            moveUp();
            numSquares--;
        }
    }
}
void HardcoreProtester::moveLeft(){
    moveToIfCan(getX() - 1, getY());
}
void HardcoreProtester::moveRight(){
    moveToIfCan(getX() + 1, getY());
}
void HardcoreProtester::moveUp(){
    moveToIfCan(getX(), getY() + 1);
}
void HardcoreProtester::moveDown(){
    moveToIfCan(getX(), getY() - 1);
}
void HardcoreProtester::isAnnoyed()
{
    changeHealth(2);
    if (getHealth() <= 0){
        isDead();
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
    }
}
void HardcoreProtester::randomizeDir()
{
        int i = (rand() % 4) + 1000;    // randomizes the direction
        switch (i)
        {
            case KEY_PRESS_LEFT:		//move or change directions
                setDirection(left);
                break;
            case KEY_PRESS_RIGHT:
                setDirection(right);
                break;
            case KEY_PRESS_UP:
                setDirection(up);
                break;
            case KEY_PRESS_DOWN:
                setDirection(down);
                break;
                
        }
    
}

/****************************************************
*       ITEMS CLASS                                 *
****************************************************/

Items::Items(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth) :Actor(world, imageID, startX, startY, dir, size, depth), m_count(0)
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
	:Items(world, IMID_GOLD, startX, startY, right, 1, 0), State(state)
{
	if (State == 1)
	{
		
		setVisible(false);
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
			if (getWorld()->AreObjectsClose(getX(), getY(), 4) && pickedUp == false)
			{
				setVisible(true);
				cerr << "Colision\n";
			}
			//if diggerman is within radius of 3
			//update score and add gold to inventory
			if (getWorld()->AreObjectsClose(getX(), getY(), 3) && pickedUp == false)
			{
				cerr << "picked up\n";
				setPickedUp(true);
				getWorld()->playSound(SOUND_GOT_GOODIE);
				getWorld()->addScore(pickScore);
				getWorld()->addInvetory(getID());
				isDead();
			}
		}
		//if the gold is dropped by the diggerman
		//temporary state or (State 2)
		else if (m_waiting)
		{
			if (ticks < 100)
			{
				ticks++;
			}
			else if (ticks >= 100)
			{
				m_waiting = false;
				isDead();
			}
		}
		
		
		
		
	}
	

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
	:Items(world, IMID_BOULDER, StartX, StartY, down, 1.0, 1), m_stable(true), m_falling(false), m_waiting(false), ticks(0)
{}
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
		if (m_waiting)
        {
			if (ticks < 30)
				ticks++;
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
Squirt::Squirt(StudentWorld*world, int StartX, int StartY, Direction dir) : Items(world, IMID_WATER_SPURT, StartX, StartY, dir, 1.0, 1)
{
	m_travelDistance = 4;

}
Squirt:: ~Squirt() {

}
void Squirt::doSomething() {

}

/*****************************************************
*       OilBarrels CLASS                             *
*                                                    *
*****************************************************/
OilBarrels::OilBarrels(StudentWorld * world, int startX, int startY)
	:Items(world, IMID_BARREL, startX, startY, right, .25, 3) {}
unsigned int OilBarrels::getStartOilCount()
{
	return getItemCount();
}
OilBarrels::~OilBarrels()
{
	setVisible(false);
}

/*****************************************************
*       SquirtGun CLASS                              *
*                                                    *
*****************************************************/
SquirtGun::SquirtGun(StudentWorld * world, int startX, int startY, Direction dir)
	:Items(world, IMID_WATER_SPURT, startX, startY, dir, 1, 0) {}
SquirtGun::~SquirtGun()
{
	setVisible(true);
}

/*****************************************************
*       SonarCharge CLASS                            *
*                                                    *
*****************************************************/
SonarCharge::SonarCharge(StudentWorld * world, int startX, int startY)
	:Items(world, IMID_SONAR, startX, startY, right, .25, 1) {}
SonarCharge::~SonarCharge()
{
	setVisible(false);
}

/*****************************************************
*       WaterPool CLASS                              *
*                                                    *
*****************************************************/
WaterPool::WaterPool(StudentWorld * world, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth)
	:Items(world, IMID_WATER_POOL, startX, startY, right, 1, 1) {}
WaterPool::~WaterPool()
{
	setVisible(false);

}
