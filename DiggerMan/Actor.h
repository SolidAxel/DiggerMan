#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameWorld.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
/* can only use this GraphObject's member functions
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
class StudentWorld;
///////////////////////////////CLASS ACTOR (BASE CLASS FOR ALL OBJECTS) //////////////////////////////////////
class Actor : public GraphObject {//Base class for all objects inherits from graphobject
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, unsigned int Ticks);
	virtual ~Actor();
	virtual void doSomething();
	virtual void isAnnoyed();
	virtual void hitByBoulder();
	virtual int getTick() const { return ticks; }
	virtual void incTick() { ticks++; }
	StudentWorld * getWorld();
	bool isAlive() const;
	void isDead();
	void moveToIfCan(int x, int y);
private:
	StudentWorld* m_world;
	bool m_alive;
	int ticks;
};

//////////////////////////CLASS CHARACTERS (INHERITS FROM BASE CLASS (ACTORS) AND IS THE BASE FOR ALL THE "Moving" CHARACTERS, i.e PROTESTER, DIGGERMAN) ///////////////////
class Character : public Actor {
public:
	Character(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int health, unsigned int Ticks);
	virtual ~Character();
	//virtual void doSomething();
	int getHealth()const;
	void setHealth(int health);
	void changeHealth(int health);
	void decHealth();
	virtual void doSomething();
	virtual void isAnnoyed();
	virtual void hitByBoulder();
private:
	unsigned int m_health;
};

////////////////////////////////CLASS DIGGERMAN (INHERITS FROM CHARACTERS CLASS)////////////////////
class DiggerMan : public Character
{
public:
	DiggerMan(StudentWorld* world, unsigned int sOil);
	virtual ~DiggerMan();
	virtual void doSomething();
	virtual void isAnnoyed();
	unsigned int getSquirts()const;
	unsigned int getSonarsCount()const;
	unsigned int getGoldPicked()const;
	unsigned int getOilCount()const;
	void decSquirts();
	void incSquirts(int squirts);
	void incGoldNug();
	void decGoldNug();
	void decSonar();
	void incSonar();
	void incOil();
	void decOil();
	void keyInput();

private:
	unsigned int m_squirt;
	unsigned int m_goldNug;
	unsigned int m_sonar;
	unsigned int m_oil;
	bool isSonarOn = false;
	int sonarLifeTimeTicks;
	int sonarRadius = 24;

};
////////////////////////////////CLASS DIRT/////////////////////////////////////////////
class Dirt : public Actor
{
public:
	Dirt(StudentWorld* world, int StartX, int StartY);
	virtual ~Dirt();
	virtual void doSomething();

};

/////////////////////// CLASS PROTESTER//////////////////

class Protester :public Character
{
public:
    Protester(StudentWorld * world, const int img, int life);
    ~Protester();
    bool moveLeft();
    bool moveRight();
    bool moveUp();
    bool moveDown();
    bool isItInRestState();
    virtual void doSomething(){};
    void isAnnoyed(){};
    void setTicksToWaitBewtweenMoves();
    void moveInThisDirection(Direction dir);
    virtual void setNumSquaresToMoveInCurrentDirection()
    {
        numSquares = rand() % 60 + 8;
    }
    int getTicksBetweenMoves(){
        return ticksToWaitBetweenMoves;
    }
    int getNonRestingTicks(){
        return nonRestingTicks;
    }
    int getNumSquares(){
        return numSquares;
    }
    void decTicksBetweenMoves(){
        ticksToWaitBetweenMoves--;
    }
    void setTicksBetweenMoves(int x){
        ticksToWaitBetweenMoves = x;
    }
private:
    int ticksToWaitBetweenMoves;
    int nonRestingTicks = 0;
    int numSquares;

};
/////////////////////// CLASS REGULAR-PROTESTER//////////////////
class RegularProtester : public Protester {
public:
	RegularProtester(StudentWorld* world);
	virtual void doSomething();
	virtual void isAnnoyed();
    virtual void setNumSquaresToMoveInCurrentDirection(){
        numSquares = rand() % 60 + 8;
    }
    virtual ~RegularProtester();
	bool getLeaveOilField()const;
	void setleaveOilField(bool);
    
private:
    int numSquares;
    int waitToYell;
	bool leaveOilField;
	bool hasYelled;
};
///////////////////CLASS HARCORE PORTESTER/////////////////////////
class HardcoreProtester : public Protester {
public:
	HardcoreProtester(StudentWorld* world);
    virtual void doSomething();
    virtual void isAnnoyed();
	bool findDiggerMan();
    virtual void setNumSquaresToMoveInCurrentDirection(){
        numSquares = rand() % 60 + 8;
    }
    virtual ~HardcoreProtester();
	bool getLeaveOilField()const;
    
private:
    int numSquares;
    int waitToYell;
    bool leaveOilField;
    bool hasYelled;
	int M;
};
////////////////////////////// CLASS ITEMS (INHERITS FROM BASE CLASS (ACTORS) AND IS THE BASE FOR ALL NON-MOVING CHARACTERS IN GAME, i.e DIRT, BOULDER/////////////////////////
class Items : public Actor {
public:
	Items(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
	virtual ~Items();
	virtual void doSomething();
	unsigned int getItemCount();
	void setItemCount(unsigned int count);
	virtual unsigned int getScore() const { return score; }
	virtual void setScore(unsigned int s) { score = s;}
private:
	unsigned int m_count;
	unsigned int score;


};

////////////////////////////CLASS GOLDNUGGETS////////////////////////////
class GoldNuggets : public Items// GoldNuggets Class Derived from Items
{
public:
	GoldNuggets(StudentWorld *world, int startX, int startY, int state);
	virtual ~GoldNuggets();
	virtual void doSomething();
	void setPickedUp(bool flag);

private:
	const int hpScore = 50,//For bribing a Hardcore Protester with a Gold Nugget: 50 points
		      rpScore = 25;//For bribing a Regular Protester with a Gold Nugget: 25 points
	int State; // 1 for permanent state 2 for temp state
	bool m_waiting;
	bool pickedUp = false;
	int ticks = 0;
};

///////////////////////////////CLASS BOULDER ////////////////////////////////////////
class Boulders : public Items
{
public:
	Boulders(StudentWorld*world, int StartX, int StartY);
	virtual ~Boulders();
	virtual void doSomething();
private:
	bool m_stable;
	bool m_falling;
	bool m_waiting;
	

};

/////////////////CLASS SQUIRT////////////////////
class Squirt :public Items
{
public:
	//(new Squirt(getX() - 4, getY(), d, getWorld())
	Squirt(StudentWorld*world, int StartX, int StartY, Direction dir);
	virtual ~Squirt();
	virtual void doSomething();

private:
	int m_travelDistance;
	int direction;
	

};
/////////////////////////////////////////OilBarrels///////////////////////////////////
class OilBarrels : public Items// OilBarrels Class Derived from Items
{
private:
	bool m_waiting;
public:
	OilBarrels(StudentWorld* world, int startX, int startY);
	virtual void doSomething();
	virtual ~OilBarrels();
};


//////////////////////////////////////SonarCharge///////////////////////////////////////
class SonarCharge : public Items//SonarCharge Class Derived from Items
{
private:
	bool pickedUp = false;//new
	int ticksinTempState;//new
	
public:
	SonarCharge(StudentWorld* world);//new
	virtual void doSomething();//new
	virtual ~SonarCharge();
};
/////////////////////////////////////WaterPool//////////////////////////////////////////
class WaterPool : public Items// WaterPool Class Derived from Items
{
public:
	WaterPool(StudentWorld* world, int startX, int startY);
	virtual ~WaterPool();
	virtual void doSomething();
	
private:
	
	int ticksinTempState;//to determine number of ticks pool of water should be visible
	

};

#endif // ACTOR_H_




