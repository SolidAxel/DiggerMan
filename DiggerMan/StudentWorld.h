#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <list>
#include <vector>
#include <sstream>
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
// contains your StudentWorld class declaration

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {
        m_ObjectsClose = false;
        //m_firstick = false;
        
    }
    
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    double EuclidanDistance(int x1, int y1, int x2, int y2);
    void addActorToGame(Actor * Act);
    bool AreObjectsClose(int x, int y, int radius);
    int annoyAllNearbyActs(Actor* Act, int x, int y, int radius);
    void removeDirt(int x, int y);
    bool isThereDirtGrid(int x, int y);
    bool isThereDirtX(int x, int y);
    bool canActMoveTo(Actor* Act, int x, int y);
    bool isThereBoulderTop(int x, int y);
    bool isThereBoulder(int x, int y);
    void addScore(int score);
    void addInvetory(int ID);
    bool IsItCloseToDiggerMan(int x, int y, int radius);
private:
    void DisplayText();
    bool m_ObjectsClose;
    vector<Actor*> m_v;//rest of the actors for the game
    DiggerMan * dm;// Diggerman
    Dirt * m_dirt[VIEW_HEIGHT][VIEW_WIDTH];//2d array of dirt pointers ( 64 * 64)
    Protester * pr;
    HardcoreProtester * hr;
    int ticksToWaitBetweenMoves = static_cast<int>(fmax(0, 3 - getLevel()));
    //bool m_firstick;
    
};

#endif // STUDENTWORLD_H_
