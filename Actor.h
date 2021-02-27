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
			GRactivatedobj
				healthkit
				holy water refill
				oil slick
				lost souls
			road borderlines
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
	virtual bool detOverlap(Actor* A, Actor* B);
	virtual void genmove(Actor* A);
	virtual void hitByHW();

	virtual int getHealth() { return m_health; }
	virtual void setHealth(int set) { m_health = set; }
	virtual bool getAlive() { return m_alive; }
	virtual void setAlive(bool set) { m_alive = set; }
	virtual bool getAvoidance() { return m_avoidance; }
	virtual void setAvoidance(bool set) { m_avoidance = set; }
	virtual StudentWorld* getStudentWorld() { return m_studentworld; }
	virtual void setStudentWorld(StudentWorld* env) { m_studentworld = env; }
	virtual double getVspeed() { return m_Vspeed; }
	virtual void setVspeed(int set) { m_Vspeed = set; }
	virtual double getHspeed() { return m_Hspeed; }
	virtual void setHspeed(int set) { m_Hspeed = set; }
	virtual bool getAffectedbyHW() { return m_affectedByHolyWater; }
	virtual void setAffectedbyHW(bool set) { m_affectedByHolyWater = set; }
	virtual int getMovementPD() { return m_movementPD; }
	virtual void setMovementPD(int set) { m_movementPD = set; }

	
private:
	bool m_alive;
	double m_Vspeed;
	double m_Hspeed;
	int m_health;
	bool m_avoidance;
	bool m_affectedByHolyWater;
	int m_movementPD;
	StudentWorld* m_studentworld;
};

class StatObj : public Actor 
{
public:
	StatObj(int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 0);
	virtual void doSomething()=0;


private:

};

class GRActivatedObj : public StatObj
{
public:
	GRActivatedObj(int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 0);
	virtual void doSomething() = 0;
};

class OilSlick : public GRActivatedObj
{
public:
	OilSlick(StudentWorld* env, int imageID, double startX, double startY, int dir = 0, double size = 2.0, unsigned int depth = 2);
	virtual void doSomething();
};

class HealingG : public GRActivatedObj
{
public:
	HealingG(StudentWorld* env, int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 2);
	virtual void doSomething();
	virtual void hitByHW();
};

class HolyWaterG : public GRActivatedObj
{
public:
	HolyWaterG(StudentWorld* env, int imageID, double startX, double startY, int dir = 90, double size = 2.0, unsigned int depth = 2);
	virtual void doSomething();
	virtual void hitByHW();
};

class LostSoulG : public GRActivatedObj
{
public:
	LostSoulG(StudentWorld* env, int imageID, double startX, double startY, int dir = 0, double size = 4.0, unsigned int depth = 2);
	virtual void doSomething();
};


class Borderline : public StatObj
{
public:
	Borderline(StudentWorld* env, int imageID, double startX, double startY, int dir = 0, double size = 2.0, unsigned int depth = 2);
	virtual void doSomething();

private:
};




class Pedestrian : public Actor {
public:
	Pedestrian(int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 0);
	virtual void doSomething() = 0;
	

private:
	
};

class HumanPed : public Pedestrian {
public:
	HumanPed(StudentWorld* env, int imageID, double startX, double startY, int dir = 0, double size = 2.0, unsigned int depth = 0);
	virtual void doSomething();
	virtual void hitByHW();

private:
};

class ZombiePed : public Pedestrian {
public:
	ZombiePed(StudentWorld* env, int imageID, double startX, double startY, int dir = 0, double size = 3.0, unsigned int depth = 0);
	virtual void doSomething();
	virtual void ZPattack();
	virtual void hitByHW();

private:
	int m_ticksBeforeGrunt;

};










class Cab : public Actor {
public:
	Cab(int imageID, double startX, double startY, int dir = 0, double size = 1.0, unsigned int depth = 0);
	virtual void doSomething()=0;
};


class ZombieCab : public Cab {
public:
	ZombieCab(StudentWorld* env, int imageID, double startX, double startY, int dir = 90, double size = 4.0, unsigned int depth = 0);
	virtual void doSomething();
	virtual void hitByHW();

private:
	bool m_damagedGR;

};


class GhostRacer : public Cab {
public:
	GhostRacer(StudentWorld* env, int imageID = IID_GHOST_RACER, double startX = ROAD_CENTER, double startY = 32, int dir = 90, double size = 4, unsigned int depth = 0);
	virtual void doSomething();
	virtual void GRmove();
	virtual int getSpeed() { return m_speed; }
	virtual int getSprays() { return m_sprays; }
	virtual void setSprays(int set) { m_sprays = set; }

private:
	int m_sprays;
	int m_speed;
};


class HolyWaterProjectile : public Actor {
public:
	HolyWaterProjectile(StudentWorld* env, int imageID, double startX, double startY, int dir, double size = 1, unsigned int depth = 1);
	virtual void doSomething();
	virtual void setmaxTraveldistance(int set) { m_maxtraveldistance = set; }
	virtual int getmaxTraveldistance() { return m_maxtraveldistance; }
	virtual void setTravelled(int set) { m_travelled = set; }
	virtual int getTravelled() { return m_travelled; }



private:
	int m_maxtraveldistance;
	int m_travelled;

};



#endif // ACTOR_H_
