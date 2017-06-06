#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <list>
#include <vector>
#include <queue>
#include <sstream>
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
// contains your StudentWorld class declaration

const int UNDISCOVERED = 0;//0 represents an UNDISCOVERED path
const int BLOCKED = 4096;//Big number (4096) that represents a blocked path
//use for coordinates in the maze search algorith:
struct Coordinates
{
public:
	Coordinates(int Xpos, int Ypos) :m_Xpos(Xpos), m_Ypos(Ypos){}
	int Xposition()const
	{
		return m_Xpos;
	}
	int Yposition()const
	{
		return m_Ypos;
	}
private:
	int m_Xpos, m_Ypos;
};
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir) : GameWorld(assetDir)
	{
		m_ObjectsClose = false;
		firsttick = true;//NEW
		m_numberoftickssincelastprotester = 0;//NEW

	}

	////helper functions:
	bool AreObjectsClose(int x, int y, int radius);
	bool canActMoveTo(Actor* Act, int x, int y);
	bool canMoveXYdir(int x, int y, Actor::Direction direction);
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
	GraphObject::Direction nextDirectionToTake(Actor * protester, bool leavingOilField);
	int numberOfProtestorsinField();//return number of protestors currently in field----------NEW
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
	void updateMaze();// updates the array for the protesters to use to find what direction they should move in
	void updateDiggerManFinder();// updates the array for the hardcore protesters to find DiggerMan

private:
	void DisplayText();
	bool m_ObjectsClose;
	vector<Actor*> m_v;//rest of the actors for the game
	DiggerMan * dm;// Diggerman
	Dirt * m_dirt[VIEW_HEIGHT][VIEW_WIDTH];//2d array of dirt pointers ( 64 * 64)
	RegularProtester * pr;
	HardcoreProtester * hr;
	int B, G, O;
	int m_numberoftickssincelastprotester;//number of ticks since last protester was added--NEW
	bool firsttick;//--New
	//map to search  the maze
	int mapmaze[VIEW_WIDTH][VIEW_HEIGHT];
	//Map to find Diggerman for Hardcore protester
	int mapTofindDiggerMan[VIEW_WIDTH][VIEW_HEIGHT];

};

#endif // STUDENTWORLD_H_
