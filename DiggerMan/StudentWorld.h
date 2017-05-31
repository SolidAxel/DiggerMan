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
		m_numberoftickssincelastprotester = 0;//NEW
	}
	////helper functions:
	bool AreObjectsClose(int x, int y, int radius);
	bool canActMoveTo(Actor* Act, int x, int y);
	bool canProtestorReachDiggerMan(int x, int y);
	bool isThereBoulder(int x, int y);
	bool isThereBoulderTop(int x, int y);
	bool isThereDirt(int x, int y);
	bool isThereDirtGrid(int x, int y);
	bool isThereDirtX(int x, int y);
	bool IsItCloseToDiggerMan(int x, int y, int radius);
	bool IsItCloseToHardcoreProtester(int x, int y, int radius);
	bool IsItCloseToProtester(int x, int y, int radius);
	bool isProtestorFacingDiggerMan(int x, int y, int direction);
	bool isProtestorInHorizontalLineOfSight(int x);
	bool isProtestorInVerticalLineOfSight(int y);
	bool protestorIsAtIntersection(int x, int y, int direction);
	double EuclidanDistance(int x1, int y1, int x2, int y2);
	GraphObject::Direction setDirectionOfProtetorForChase(int x, int y);
	GraphObject::Direction setDirectionOfProtetorForRegularWalking(int x, int y);
	GraphObject::Direction setDirectionAtIntersection(int x, int y, int direction);
	int numberOfProtestorsinField();//return number of protestors cuttently in field----------NEW
	int OilBarrelsLeft();//returns the number of oil barrels in the field----NEW
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	void addActorToGame(Actor * Act);
	void addInvetory(int ID);
	void addScore(int score);
	void aHardcoreProtestorActivatesNugget(int score);
	void annoyAllNearbyActs(Actor* Act, int x, int y, int radius);
	void annoyDiggerMan();
	void aProtestorActivatesNugget(int score);
	void randPositions(int numLvl, int ID);
	void removeDirt(int x, int y);
	void Sonar(bool state, int x, int y, int radius);//state true is activation; state false is deactivation 
private:
	void DisplayText();
	bool m_ObjectsClose;
	vector<Actor*> m_v;//rest of the actors for the game
	DiggerMan * dm;// Diggerman
	Dirt * m_dirt[VIEW_HEIGHT][VIEW_WIDTH];//2d array of dirt pointers ( 64 * 64)
	Protester * pr;
	HardcoreProtester * hr;
	//int ticksToWaitBetweenMoves = static_cast<int>(fmax(0, 3 - getLevel()));
	//bool m_firstick;
	int B, G, O;
	int m_numberoftickssincelastprotester;//number of ticks since last protester was added--NEW

};

#endif // STUDENTWORLD_H_
