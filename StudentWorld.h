#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <list>

class Actor;
class GhostRacer;
class Borderline;

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
    virtual void cleanUp(); 
        /*  
            destroy all actors currently in game
        */
    virtual ~StudentWorld();
    virtual GhostRacer* getGhostRacer() { return m_ghostracer; }

private:

    //keep track of all game objects
    std::list<Actor*> m_actors;
    GhostRacer* m_ghostracer;
};

#endif // STUDENTWORLD_H_
