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
private:
	void DisplayText();
	vector<Actor*> m_v;//rest of the actors for the game
	DiggerMan * dm;// Diggerman
    Protester * pr;
    HardcoreProtester * hr;
	Dirt * m_dirt[VIEW_HEIGHT][VIEW_WIDTH];//2d array of dirt pointers ( 64 * 64)
    int ticksToWaitBetweenMoves = fmax(0, 3 - getLevel());

};

#endif // STUDENTWORLD_H_
