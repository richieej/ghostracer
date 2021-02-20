#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <list>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    
}

StudentWorld::~StudentWorld()
{

}

int StudentWorld::init()
{
    //create ghostracer
    list<Actor*> m_actors;
    m_ghostracer = new GhostRacer(this);
    m_actors.push_front(m_ghostracer);
    //list<Actor*>::iterator test = m_actors.begin();
    //(*test)->doSomething();

    
    //add yellow borderlines
    init_yellowlines();
    //add white borderlines;
    init_whitelines();
    

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::init_yellowlines()
{
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    for (int j = 0;j < N;j++)
    {
        //add left
        m_actors.push_front(new Borderline(this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, j * SPRITE_HEIGHT));
        //add right
        m_actors.push_front(new Borderline(this, IID_YELLOW_BORDER_LINE, RIGHT_EDGE, j * SPRITE_HEIGHT));
    }
    return;
}

void StudentWorld::init_whitelines()
{
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    for (int j = 0;j < M;j++)
    {
        //add left
        m_actors.push_back(new Borderline(this, IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH / 3, j * (4 * SPRITE_HEIGHT)));
        //add right
        m_actors.push_back(new Borderline(this, IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3, j * (4 * SPRITE_HEIGHT)));
    }
    return;
}

int StudentWorld::move()
{
    if (!m_ghostracer->getAlive())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }

    //iterate through m_actors and doSomething
    m_ghostracer->doSomething();
    list<Actor*>::iterator it = m_actors.begin();
    (*it)->doSomething();
    for (; it != m_actors.end();)
    {
        (*it)->doSomething();
        it++;
    }
    
    
    //add borderline
    double LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    double RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    double new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    double waste, last_y;
    (m_actors.back())->getAnimationLocation(waste, last_y);
    double delta_y = new_border_y - last_y;
    if (delta_y >= SPRITE_HEIGHT)
    {
        //left
        m_actors.push_front(new Borderline(this, IID_YELLOW_BORDER_LINE, LEFT_EDGE, new_border_y));
        //right
        m_actors.push_front(new Borderline(this, IID_YELLOW_BORDER_LINE, RIGHT_EDGE, new_border_y));
    }
    if (delta_y >= 4 * SPRITE_HEIGHT)
    {
        //left
        m_actors.push_back(new Borderline(this, IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH / 3, new_border_y));
        //right
        m_actors.push_back(new Borderline(this, IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3, new_border_y));
    }
    

    //iterate through m_actors and dispose
    it = m_actors.begin();
    for (; it != m_actors.end();)
    {
        if (!((*it)->getAlive()))
        {
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }

    return GWSTATUS_CONTINUE_GAME;

}

void StudentWorld::cleanUp()
{
    auto it = m_actors.begin();
    for (; it != m_actors.end();)
    {
        delete* it;
        it = m_actors.erase(it);
    }
    delete m_ghostracer;
}
