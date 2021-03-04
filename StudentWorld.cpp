#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <math.h>
#include <list>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

# define M_PI  3.14159265358979323846

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
    m_bonus = 5000;
    m_soulstosave = 2 * getLevel() + 5;
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
    if (m_soulstosave == 0)
    {
        return GWSTATUS_FINISHED_LEVEL;
    }

    if (!m_ghostracer->getAlive())
    {
        decLives();
        GameWorld::playSound(SOUND_PLAYER_DIE);
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
    
    //potentially add actors
    int chance = max(150 - getLevel() * 10, 40);
    int ChanceOilSlick = randInt(0, chance - 1);
    if (ChanceOilSlick == 0)
        m_actors.push_front(new OilSlick(this, IID_OIL_SLICK, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    chance = 100 + 10 * getLevel();
    int ChanceHolyWater = randInt(0, chance - 1);
    if (ChanceHolyWater == 0)
        m_actors.push_front(new HolyWaterG(this, IID_HOLY_WATER_GOODIE, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));
    
    chance = max(200 - getLevel() * 10, 30);
    int ChanceHumanPed = randInt(0, chance - 1);
    if (ChanceHumanPed == 0)
        m_actors.push_front(new HumanPed(this, IID_HUMAN_PED, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));

    chance = max(100 - getLevel() * 10, 20);
    int chanceZombiePed = randInt(0, chance - 1);
    if (chanceZombiePed == 0)
        m_actors.push_front(new ZombiePed(this, IID_ZOMBIE_PED, randInt(0, VIEW_WIDTH), VIEW_HEIGHT));

    int ChanceLostSoul = randInt(0, 99);
    if (ChanceLostSoul == 0)
        m_actors.push_front(new LostSoulG(this, IID_SOUL_GOODIE, randInt(LEFT_EDGE, RIGHT_EDGE), VIEW_HEIGHT));

    chance = max(100 - getLevel() * 10, 20);
    int ChanceZombieCab = randInt(0, chance - 1);
    if (ChanceZombieCab == 0)
    {
        for (int i = 0;i < 3;i++)
        {
            int cur_lane = randInt(0, 2);
            int roadctr;
            if (cur_lane == 0)
                roadctr = ROAD_CENTER - ROAD_WIDTH/3;
            else if (cur_lane == 1)
                roadctr = ROAD_CENTER;
            else if (cur_lane == 2)
                roadctr = ROAD_CENTER + ROAD_WIDTH/3;
            if (closestAvoidBottom(roadctr))
                break;
        }
    }

    if (m_bonus >= 0)
        m_bonus--;
    updateStatusLine();

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

void StudentWorld::updateStatusLine()
{
    ostringstream oss;
    oss << "Score: " << getScore() << "  " << "Lvl: " << getLevel() << "  " << "Souls2Save: " << m_soulstosave << "  " << "Lives: " << getLives() << "  " << "Health: " << getGhostRacer()->getHealth() << "  " << "Sprays: " << getGhostRacer()->getSprays() << "  " << "Bonus: " << m_bonus;
    string s = oss.str();
    setGameStatText(s);
}


bool StudentWorld::closestAvoidBottom(double lane)
{
    auto it = m_actors.begin();
    double bottom = 100;
    double gr_x, gr_y;
    getGhostRacer()->getAnimationLocation(gr_x, gr_y);
    if (gr_x < lane + ROAD_WIDTH / 6 && gr_x > lane - ROAD_WIDTH / 6)
        return false;
    while (it != m_actors.end())
    {
        double cur_x, cur_y;
        (*it)->getAnimationLocation(cur_x, cur_y);
        if ((*it)->getAvoidance() == true && cur_x < lane + ROAD_WIDTH / 6 && cur_x > lane - ROAD_WIDTH / 6)
        {
            if (cur_y < bottom)
                bottom = cur_y;
        }
        it++;
    }
    if (bottom == 100 || bottom > VIEW_HEIGHT / 3)
    {
        m_actors.push_front(new ZombieCab(this, IID_ZOMBIE_CAB, lane, 32 - SPRITE_HEIGHT / 2));
        it = m_actors.begin();
        (*it)->setVspeed(getGhostRacer()->getSpeed() + randInt(2, 4));
        return true;
    }


    double top = 0;
    it = m_actors.begin();
    while (it != m_actors.end())
    {   
        double cur_x, cur_y;
        (*it)->getAnimationLocation(cur_x, cur_y);
        if ((*it)->getAvoidance() == true && cur_x < lane + ROAD_WIDTH / 6 && cur_x > lane - ROAD_WIDTH / 6)
        {
            if (cur_y > top)
                top = cur_y;
        }
        it++;
    }
    if (top == 0 || top > VIEW_HEIGHT * 2 / 3)
    {
        m_actors.push_front(new ZombieCab(this, IID_ZOMBIE_CAB, lane, VIEW_HEIGHT - SPRITE_HEIGHT / 2));
        it = m_actors.begin();
        (*it)->setVspeed(getGhostRacer()->getSpeed() - randInt(2, 4));
        return true;
    }
    return false;
}



void StudentWorld::holywaterCheck(HolyWaterProjectile* HW)
{
    auto it = m_actors.begin();
    while (it != m_actors.end())
    {
        if (HW->detOverlap(HW, *it) == true && (*it)->getAffectedbyHW() == true)
        {
            if ((*it)->getAlive() == true)
            {
                (*it)->hitByHW();
                HW->setAlive(false);
                return;
            }
        }
        else
            it++;
    }
}

void StudentWorld::addHolyWater()
{
    double x, y, dir;
    getGhostRacer()->getAnimationLocation(x, y);
    x += SPRITE_HEIGHT * cos(getGhostRacer()->getDirection() * M_PI / 180);
    y += SPRITE_HEIGHT * sin(getGhostRacer()->getDirection() * M_PI / 180);
    dir = getGhostRacer()->getDirection();
    m_actors.push_front(new HolyWaterProjectile(this, IID_HOLY_WATER_PROJECTILE, x, y, dir));
    playSound(SOUND_PLAYER_SPRAY);
    getGhostRacer()->setSprays(getGhostRacer()->getSprays() - 1);
}


void StudentWorld::zombieCabCheck(ZombieCab* ZC)
{
    if (ZC->getVspeed() > getGhostRacer()->getSpeed())
    {
        auto it = m_actors.begin();
        while (it != m_actors.end())
        {
            double t_x, t_y, zc_x, zc_y;
            (*it)->getAnimationLocation(t_x, t_y);
            ZC->getAnimationLocation(zc_x, zc_y);
            if (t_x < zc_x + ROAD_WIDTH / 2 && t_x > zc_x - ROAD_WIDTH / 2 && (*it)->getAvoidance() == true && (*it)->getAlive() == true)
            {
                if (t_y - zc_y > 0 && t_y - zc_y < 96)
                    ZC->setVspeed(ZC->getVspeed() - 0.5);
                else if (zc_y - t_y < 96 && zc_y - t_y > 0)
                    ZC->setVspeed(ZC->getVspeed() + 0.5);
            }

            it++;
        }
    }
}

void StudentWorld::addOilSlick(double x, double y)
{
    m_actors.push_front(new OilSlick(this, IID_OIL_SLICK, x, y));
}

void StudentWorld::addHealingG(double x, double y)
{
    m_actors.push_front(new HealingG(this, IID_HEAL_GOODIE, x, y));
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
