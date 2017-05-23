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

int StudentWorld::init()//
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
    //create a  player:
    dm = new DiggerMan(this);
    //addActorToGame(dm);
    //hr = new HardcoreProtester(this);
    pr = new Protester(this, ticksToWaitBetweenMoves);
    hr = new HardcoreProtester(this, ticksToWaitBetweenMoves);
    addActorToGame(pr);
    addActorToGame(hr);
    //Boulders
    int xPosition = 0, yPosition = 0, radius = 6;
    int B = static_cast<int>(fmin(getLevel() / 2 + 2, 7));
    cerr << "Number of Boulders this round = " << B << endl;
    std::cout << dm -> getDirection() << endl;
    cerr << left << endl;
    for (int i = 0; i < B; i++)
    {
        xPosition = rand() % 60;
        yPosition = rand() % 56;
        //while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
        while ((xPosition>26 && xPosition < 34) || yPosition < 20 || yPosition>56)
        {
            xPosition = rand() % 60;
            yPosition = rand() % 56;
            if (AreObjectsClose(xPosition, yPosition, radius)){
                xPosition = rand() % 60;
                yPosition = rand() % 56;
            }
        }
        ////check that there's no other items within a radius of 6
        if (AreObjectsClose(xPosition, yPosition, radius) && m_ObjectsClose == true){
            
            xPosition = rand() % 60;
            yPosition = rand() % 56;
            
            while ((xPosition>26 && xPosition < 34) || yPosition < 20 || yPosition>56)
            {
                xPosition = rand() % 60;
                yPosition = rand() % 56;
                if (AreObjectsClose(xPosition, yPosition, radius)){
                    xPosition = rand() % 60;
                    yPosition = rand() % 56;
                }
            }
        }
        addActorToGame(new Boulders(this, xPosition, yPosition));
        removeDirt(xPosition, yPosition);
    }
    
    int GxPosition = 0, GyPosition = 0, Gradius = 6;
    //Gold Nuggets
    int G = static_cast<int> (fmax(5 - getLevel() / 2, 2));
    cerr << "Number of GoldNuggets this round = " << G << endl;
    
    for (int i = 0; i < G; i++)
    {
        GxPosition = rand() % 60;
        GyPosition = rand() % 56;
        //while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
        while ((GxPosition>26 && GxPosition < 34) || GyPosition < 0 || GyPosition>56)
        {
            GxPosition = rand() % 60;
            GyPosition = rand() % 56;
        }
        if (isThereBoulder(GxPosition, GyPosition)){
            if (AreObjectsClose(GxPosition, GyPosition, Gradius) && m_ObjectsClose == true)
            {
                GxPosition = rand() % 60;
                GyPosition = rand() % 56;
                //while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
                while ((GxPosition >26 && GxPosition < 34) || GyPosition < 0 || GyPosition > 56)
                {
                    GxPosition = rand() % 60;
                    GyPosition = rand() % 56;
                }
            }
        }
        //check that there's no other items within a radius of 6
        if (AreObjectsClose(GxPosition, GyPosition, Gradius) && m_ObjectsClose == true)
        {
            GxPosition = rand() % 60;
            GyPosition = rand() % 56;
            //while position is in the mineshaft coordinates, or at the bottom, out bounds of Dirt "map" set a new postion
            while ((GxPosition >26 && GxPosition < 34) || GyPosition < 0 || GyPosition > 56)
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
    
    return GWSTATUS_CONTINUE_GAME;
    
}
int StudentWorld::move(){
    // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    /*if (!m_firstick)
     {
     m_firstick = true;
     pr = new Protester(this);
     addActorToGame(pr);
     }*/
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
    if (hr->isAlive()) {
        hr->ticks = hr->numSquaresToMoveInCurrentDirection();
        if (hr->ticks > 0) {
            hr->doSomething();
            hr->ticks--;
        }
        else
        {
            hr->randomizeDir();
        }
    }
    if (pr->isAlive()) {
        
        pr->ticks = pr->numSquaresToMoveInCurrentDirection();
        
        if (pr->ticks > 0) {
            pr->doSomething();
            pr->ticks--;
        }
        else
            pr->randomizeDir();
        
    }
    //Adding Goodies to game (water pool and/or sonar charge)
    int G = getLevel() * 25 + 300;//according to specs
    int xPosition, yPosition;
    int randomnumber = rand()%100;//to test the probability of when to add water/sonar
    cerr <<"Probability: "<< randomnumber % G << endl;//to display test probability
    if (randomnumber % G == 1)
    {
        if (rand() % 100 == 20)//20 percent of the time or 1/5
            addActorToGame(new SonarCharge(this));//according to specs sonar can only appear at x=0,y=60
        else{//waterpool can appear anywhere in the grid where dirt is removed
            xPosition = rand() % 61;
            yPosition = rand() % 61;
            while (isThereDirtGrid(xPosition, yPosition))//checks if there exist dirt at x,y positions
            {
                xPosition = rand() % 61;
                yPosition = rand() % 61;
            }
            addActorToGame(new WaterPool(this, xPosition, yPosition));//adds water pool to position where there is no dirt
        }
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
//function to cleanup all memory allocated for the game.
void StudentWorld::cleanUp()
{
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
    
    /*delete dm;
     delete pr;
     delete hr;*/
}

//this function adds an Actor to the vector
void StudentWorld::addActorToGame(Actor *Act)
{
    m_v.push_back(Act);
}
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
    
    double distance=EuclidanDistance(x, y, dm->getX(), dm->getY());
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
bool StudentWorld::isActorCloseAndFacing(int x, int y, int radius){//new
    bool isClose = false;
    vector<Actor*>::iterator it = m_v.begin();
    while (it != m_v.end()){
        if ((*it)->getID() == IMID_PROTESTER || (*it)->getID() == IMID_HARD_CORE_PROTESTER){
            
            double distance = EuclidanDistance((*it)->getX(), (*it)->getY(),dm->getX(),dm->getY());
            if (distance <= radius){
                if ((*it)->getY() == dm->getY()) {
                    if ((*it)->getX() > dm->getX() && ((*it) -> getDirection() ==  3)) {
                        isClose = true;
                    }
                    else if ((*it)->getX() < dm->getX() && (*it) -> getDirection() == 4){
                        isClose = true;
                    }
                }
                else if((*it)->getX() == dm->getX()){
                    if ((*it)->getY() > dm->getY() && ((*it) -> getDirection() ==  1)) {
                        isClose = true;
                    }
                    else if ((*it)->getY() < dm->getY() && (*it) -> getDirection() == 2){
                        isClose = true;
                    }
                }
            }
        
        }
        it++;
    }
    return isClose;
}
bool StudentWorld::isActorClose(int x, int y, int radius, int &dir){//new
    bool isClose = false;
    vector<Actor*>::iterator it = m_v.begin();
    while (it != m_v.end()){
        if ((*it)->getID() == IMID_PROTESTER || (*it)->getID() == IMID_HARD_CORE_PROTESTER){
            
            double distance = EuclidanDistance((*it)->getX(), (*it)->getY(),dm->getX(),dm->getY());
            if (distance <= radius){
                if ((*it)->getY() == dm->getY()) {
                    if ((*it)->getX() > dm->getX()) {
                        dir = 1;
                        isClose = true;
                    }
                    else if ((*it)->getX() < dm->getX()){
                        dir = 2;
                        isClose = true;
                    }
                }
                else if((*it)->getX() == dm->getX()){
                    if ((*it)->getY() > dm->getY()) {
                        dir = 3;
                        isClose = true;
                    }
                    else if ((*it)->getY() < dm->getY()){
                        dir = 4;
                        isClose = true;
                    }
                }
            }
            
        }
        it++;
    }
    return isClose;
}
//bool StudentWorld::isActorCloseForHardcore(int x, int y, int radius, int &dir1){
//    bool isClose = false;
//    vector<Actor*>::iterator it = m_v.begin();
//    while (it != m_v.end()){
//        if ((*it)->getID() == IMID_HARD_CORE_PROTESTER){
//            
//            double distance = EuclidanDistance((*it)->getX(), (*it)->getY(),dm->getX(),dm->getY());
//            if (distance <= radius){
//                if ((*it)->getY() == dm->getY()) {
//                    if ((*it)->getX() > dm->getX()) {
//                        dir1 = 1;
//                        isClose = true;
//                    }
//                    else if ((*it)->getX() < dm->getX()){
//                        dir1 = 2;
//                        isClose = true;
//                    }
//                }
//                else if((*it)->getX() == dm->getX()){
//                    if ((*it)->getY() > dm->getY()) {
//                        dir1 = 3;
//                        isClose = true;
//                    }
//                    else if ((*it)->getY() < dm->getY()){
//                        dir1 = 4;
//                        isClose = true;
//                    }
//                }
//            }
//            
//        }
//        it++;
//    }
//    return isClose;
//}
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
                it--;
            }
        }
    }
    cerr << "number of dm annoyed by boulder: " << count << endl;
    return count;
}
void StudentWorld::removeDirt(int x, int y)
{
    for (int c = x; c < x + 4; c++)
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
    if (ID == IMID_BARREL)
    {
        //if ID is barrel do something
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
