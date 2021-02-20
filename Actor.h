#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

/*Ghost Racer, zombie
	pedestrians, human pedestrians, zombie cabs, holy water projectiles, healing
	goodies, holy water refill goodies, yellow and white road border lines, oil slicks,
	and lost souls*/

	/*Actor : public graphobject (part 1)
		stat obj
			goodie
				healthkit
				holy water refill
			oil slick
			road borderlines
			lost souls
		pedestrian
			human
			zomb
		cab
			ghostracer (part 1)
			zomb
		holywater projectile
		*/

class Actor : public GraphObject 
{
public:
	Actor(int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 0);
	virtual void doSomething()=0;
	virtual int getHealth() { return m_health; }
	virtual bool getAlive() { return m_alive; }
	virtual void setAlive(bool set) { m_alive = set; }
	virtual void setAvoidance(bool set) { m_avoidance = set; }
	virtual StudentWorld* getStudentWorld() { return m_studentworld; }
	virtual void setStudentWorld(StudentWorld* env) { m_studentworld = env; }
	
private:
	bool m_alive;
	int m_health;
	bool m_avoidance;
	StudentWorld* m_studentworld;
};

class StatObj : public Actor 
{
public:
	StatObj(int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 0);
	virtual void doSomething()=0;
};

class Borderline : public StatObj 
{
public:
	Borderline(StudentWorld* env, int imageID, double startX, double startY, int dir = 0, double size = 2.0, unsigned int depth = 2);
	virtual void doSomething();

private:
	int m_vSpeed;
	int m_hSpeed;

};



class Cab : public Actor {
public:
	Cab(int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 0);
	virtual void doSomething()=0;
};

class GhostRacer : public Cab {
public:
	GhostRacer(StudentWorld* env, int imageID = IID_GHOST_RACER, double startX = ROAD_CENTER, double startY = 32, int dir = 90, double size = 4, unsigned int depth = 0);
	virtual void doSomething();
	virtual void move();
	virtual int getSpeed() { return m_speed; }
	virtual int getHealth() { return m_health; }

private:
	int m_health;
	int m_sprays;
	int m_speed;
};


#endif // ACTOR_H_
