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
class StudentWorld;//
///////////////////////////////CLASS ACTOR (BASE CLASS FOR ALL OBJECTS) //////////////////////////////////////
class Actor : public GraphObject {//Base class for all objects inherits from graphobject
public:
    Actor(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual ~Actor();
    virtual void doSomething();
    virtual void isAnnoyed();
    virtual void hitByBoulder();
    StudentWorld * getWorld();
    bool isAlive() const;
    void isDead();
    void moveToIfCan(int x, int y);
private:
    StudentWorld* m_world;
    bool m_alive;
};

//////////////////////////CLASS CHARACTERS (INHERITS FROM BASE CLASS (ACTORS) AND IS THE BASE FOR ALL THE "Moving" CHARACTERS, i.e PROTESTER, DIGGERMAN) ///////////////////
class Character : public Actor {
public:
    Character(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int health);
    virtual ~Character();
    //virtual void doSomething();
    int getHealth()const;
    void setHealth(int health);
    void changeHealth(int health);
    void decHealth();
    virtual void doSomething();
    virtual void isAnnoyed() = 0;
    virtual void hitByBoulder();
private:
    unsigned int m_health;
};

////////////////////////////////CLASS DIGGERMAN (INHERITS FROM CHARACTERS CLASS)////////////////////
class DiggerMan : public Character
{
public:
    DiggerMan(StudentWorld* world);
    virtual ~DiggerMan();
    virtual void doSomething();
    virtual void isAnnoyed();
    unsigned int getSquirts()const;
    unsigned int getSonarsCount()const;
    unsigned int getGoldPicked()const;
    void decSquirts();
    void incSquirts(int squirts);
    void incGoldNug();
    void decGoldNug();
    void decSonar();
    void incSonar();
    void keyInput();
private:
    unsigned int m_squirt;
    unsigned int m_goldNug;
    unsigned int m_sonar;
    
};
////////////////////////////////CLASS DIRT/////////////////////////////////////////////
class Dirt : public Actor
{
public:
    Dirt(StudentWorld* world, int StartX, int StartY);
    virtual ~Dirt();
    virtual void doSomething();
private:
};

/////////////////////// CLASS PROTESTER//////////////////
class Protester : public Character {
public:
    Protester(StudentWorld* world, int wait);
    virtual void doSomething();
    virtual void isAnnoyed();
    virtual void randomizeDir();
    virtual void moveLeft();
    virtual void moveRight();
    virtual void moveUp();
    virtual void moveDown();
    int tickWaiting;
    virtual void shout();
    int waiting;
    virtual ~Protester();
    int ticks = 0;
    int ticksSinceLastShout = 0;
    bool isFacingDiggerman();
    int numSquaresToMoveInCurrentDirection(){
        int i = rand() % 60;
        while (!(i < 8)){
            i = rand() % 60;
        }
        return i;
    }
    
};

class HardcoreProtester : public Character {
public:
    HardcoreProtester(StudentWorld* world, int wait);
    virtual void doSomething();
    virtual void isAnnoyed();
    virtual void randomizeDir();
    virtual void moveLeft();
    virtual void moveRight();
    virtual void moveUp();
    virtual void moveDown();
    virtual ~HardcoreProtester();
    int ticks = 0;
    int ticksWaiting;
    int waiting;
    int numSquaresToMoveInCurrentDirection(){
        int i = rand() % 60;
        while (!(i < 8)){
            i = rand() % 60;
        }
        return i;
    }
};
////////////////////////////// CLASS ITEMS (INHERITS FROM BASE CLASS (ACTORS) AND IS THE BASE FOR ALL NON-MOVING CHARACTERS IN GAME, i.e DIRT, BOULDER/////////////////////////
class Items : public Actor {
public:
    Items(StudentWorld* world, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth);
    virtual ~Items();
    virtual void doSomething();
    unsigned int getItemCount();
    void setItemCount(unsigned int count);
private:
    unsigned int m_count;
    
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
    pickScore = 10,//For Picking a nugget up
    rpScore = 25;//For bribing a Regular Protester with a Gold Nugget: 25 points
    bool pickedUp = false;
    int State; // 1 for permanent state 2 for temp state
    bool m_waiting;
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
    int ticks;
    const int score = 500;
    
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
    
};
/////////////////////////////////////////OilBarrels///////////////////////////////////
class OilBarrels : public Items// OilBarrels Class Derived from Items
{
private:
    const int score = 1000;//For collecting a barrel of oil
public:
    OilBarrels(StudentWorld* world, int startX, int startY);
    unsigned int getStartOilCount();
    virtual ~OilBarrels();
};

//////////////////////////////////////SonarCharge///////////////////////////////////////
class SonarCharge : public Items//SonarCharge Class Derived from Items
{
private:
    const int score = 75;//For picking up Sonar Charge
public:
    SonarCharge(StudentWorld* world, int startX, int startY);
    virtual ~SonarCharge();
};
/////////////////////////////////////WaterPool//////////////////////////////////////////
class WaterPool : public Items// WaterPool Class Derived from Items
{
public:
    WaterPool(StudentWorld* world, int startX, int startY);
    virtual ~WaterPool();
    virtual void doSomething();
    void setPickedUp(bool flag);
private:
    //bool pickedUp;
    int ticksinTempState;//to determine number of ticks pool of water should be visible
    const int score = 100;//For filling up Squirt Gun
    
};

#endif // ACTOR_H_




