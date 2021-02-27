#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

class Actor;
class GhostRacer;
class Borderline;
class HolyWaterProjectile;
class ZombieCab;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual int init();
        /*
            construct representation of the current level
            populate with initial objects using data structures u come up with
            must return GWSTATUS_CONTINUE_GAME
        */
    virtual void init_yellowlines();
    virtual void init_whitelines();
    virtual int move();
        /*
            each call runs a single tick of the game
            calls each actor's doSomething method
            introduce new actors (appearing at top of screen)
            dispose of actors that disappear beyond bottom of screen
            dispose of actors that disappear organically
        */
    virtual void updateStatusLine();
    virtual void holywaterCheck(HolyWaterProjectile* HW);
    virtual void addHolyWater();
    virtual void zombieCabCheck(ZombieCab* ZC);
    virtual void addOilSlick(double x, double y);
    virtual void addHealingG(double x, double y);
    virtual bool closestAvoidBottom(double lane);

    virtual void cleanUp(); 
        /*  
            destroy all actors currently in game
        */
    virtual GhostRacer* getGhostRacer() { return m_ghostracer; }
    virtual void decSouls() { m_soulstosave--; }
    virtual ~StudentWorld();
   

private:

    //keep track of all game objects
    std::list<Actor*> m_actors;
    GhostRacer* m_ghostracer;
    int m_bonus;
    int m_soulstosave;
};

#endif // STUDENTWORLD_H_
