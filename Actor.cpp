#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"
#include <cmath>
#include <iostream>

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

# define M_PI  3.14159265358979323846


Actor::Actor(int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: GraphObject(imageID, startX, startY, dir, size, depth)
{

}

void Actor::hitByHW()
{}

void Actor::genmove(Actor* A) 
{
	double vert_speed = A->getVspeed() - getStudentWorld()->getGhostRacer()->getSpeed();
	double horiz_speed = A->getHspeed();
	double cur_y, cur_x;
	A->GraphObject::getAnimationLocation(cur_x, cur_y);
	cur_y += vert_speed;
	cur_x += horiz_speed;
	A->GraphObject::moveTo(cur_x, cur_y);
	if (cur_y < 0 || cur_x < 0 || cur_x > VIEW_WIDTH || cur_y > VIEW_HEIGHT)
		A->setAlive(false);
	return;
}

bool Actor::detOverlap(Actor* A, Actor* B)
{
	double a_x, a_y, b_x, b_y;
	A->getAnimationLocation(a_x, a_y);
	B->getAnimationLocation(b_x, b_y);
	double delta_x = abs(a_x - b_x);
	double delta_y = abs(a_y - b_y);
	double sum_Radii = A->getRadius() + B->getRadius();
	return (delta_x < sum_Radii * 0.25 && delta_y < sum_Radii * 0.6);
}

StatObj::StatObj(int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Actor(imageID, startX, startY, dir, size, depth)
{

}

GRActivatedObj::GRActivatedObj(int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: StatObj(imageID, startX, startY, dir, size, depth)
{

}

OilSlick::OilSlick(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: GRActivatedObj(imageID, startX, startY, dir, size, depth)
{
	setHspeed(0);
	setVspeed(-4);
	setAvoidance(false);
	setStudentWorld(env);
	setAlive(true);
	setAffectedbyHW(false);
}

void OilSlick::doSomething() {
	genmove(this);
	if (detOverlap(getStudentWorld()->getGhostRacer(), this))
	{
		getStudentWorld()->playSound(SOUND_OIL_SLICK);
		getStudentWorld()->getGhostRacer()->setDirection(randInt(-20, 20));
		double cur_dir = getStudentWorld()->getGhostRacer()->getDirection();
		/*while (cur_dir > -5 && cur_dir < 5)
		{
			getStudentWorld()->getGhostRacer()->setDirection(randInt(-20, 20));
			double cur_dir = getStudentWorld()->getGhostRacer()->getDirection();
		}*/
		if (cur_dir < 60)
			getStudentWorld()->getGhostRacer()->setDirection(60);
		else if (cur_dir > 120)
			getStudentWorld()->getGhostRacer()->setDirection(120);
	}
	
}

HealingG::HealingG(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: GRActivatedObj(imageID, startX, startY, dir, size, depth)
{
	setHspeed(0);
	setVspeed(-4);
	setAvoidance(false);
	setStudentWorld(env);
	setAlive(true);
	setAffectedbyHW(true);
}

void HealingG::doSomething()
{
	genmove(this);
	if (getAlive() == false)
		return;
	if (detOverlap(getStudentWorld()->getGhostRacer(), this))
	{
		getStudentWorld()->getGhostRacer()->setHealth(getStudentWorld()->getGhostRacer()->getHealth() + 10);
		setAlive(false);
		getStudentWorld()->playSound(SOUND_GOT_GOODIE);
		getStudentWorld()->increaseScore(250);
		getStudentWorld()->getGhostRacer()->setHealth(getStudentWorld()->getGhostRacer()->getHealth() + 10);
	}
}

void HealingG::hitByHW()
{
	setAlive(false);
}

HolyWaterG::HolyWaterG(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: GRActivatedObj(imageID, startX, startY, dir, size, depth)
{
	setHspeed(0);
	setVspeed(-4);
	setAvoidance(false);
	setAlive(true);
	setStudentWorld(env);
	setAffectedbyHW(true);
}

void HolyWaterG::doSomething()
{
	genmove(this);
	if (getAlive() == false)
		return;
	if (detOverlap(getStudentWorld()->getGhostRacer(), this))
	{
		getStudentWorld()->getGhostRacer()->setSprays(getStudentWorld()->getGhostRacer()->getSprays() + 10);
		setAlive(false);
		getStudentWorld()->playSound(SOUND_GOT_GOODIE);
		getStudentWorld()->increaseScore(50);
	}
}

void HolyWaterG::hitByHW()
{
	setAlive(false);
}

LostSoulG::LostSoulG(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: GRActivatedObj(imageID, startX, startY, dir, size, depth)
{
	setHspeed(0);
	setVspeed(-4);
	setAvoidance(false);
	setStudentWorld(env);
	setAlive(true);
	setAffectedbyHW(false);
}

void LostSoulG::doSomething()
{
	genmove(this);
	if (getAlive() == false)
		return;
	if (detOverlap(getStudentWorld()->getGhostRacer(), this))
	{
		//increase number of lost souls saved
		getStudentWorld()->decSouls();
		setAlive(false);
		getStudentWorld()->playSound(SOUND_GOT_SOUL);
		getStudentWorld()->increaseScore(100);
	}
	setDirection(getDirection() + 10);

}





Borderline::Borderline(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: StatObj(imageID, startX, startY, dir, size, depth)
{
	setVspeed(-4);
	setHspeed(0);
	setAlive(true);;
	setAvoidance(false);
	setStudentWorld(env);
	setAffectedbyHW(false);
}


void Borderline::doSomething() {
	genmove(this);
}



Pedestrian::Pedestrian(int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Actor(imageID, startX, startY, dir, size, depth)
{

}


HumanPed::HumanPed(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Pedestrian(imageID, startX, startY, dir, size, depth)
{
	setMovementPD(0);
	setVspeed(-4);
	setHspeed(0);
	setHealth(2);
	setAlive(true);
	setAvoidance(true);
	setStudentWorld(env);
	setAffectedbyHW(true);
}

void HumanPed::doSomething() {
	if (!getAlive())
		return;
	//overlap
	if (detOverlap(getStudentWorld()->getGhostRacer(), this))
	{
		//getStudentWorld()->decLives();
		getStudentWorld()->getGhostRacer()->setAlive(false);
		return;
	}
	//move
	genmove(this);
	setMovementPD(getMovementPD() - 1);
	if (getMovementPD() > 0)
		return;
	//new movement plan
	setHspeed(randInt(-3, 3));
	while (getHspeed() == 0)
		setHspeed(randInt(-3, 3));
	setMovementPD(randInt(4, 32));
	if (getHspeed() < 0)
		setDirection(180);
	else
		setDirection(0);

	//other circumstances

}

void HumanPed::hitByHW()
{
	setDirection(getDirection() + 180);
	setHspeed(getHspeed() * -1);
	getStudentWorld()->playSound(SOUND_PED_HURT);
}



ZombiePed::ZombiePed(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Pedestrian(imageID, startX, startY, dir, size, depth)
{
	setMovementPD(0);
	setVspeed(-4);
	setHspeed(0);
	setAlive(true);
	setHealth(2);
	setAvoidance(true);
	setStudentWorld(env);
	setAffectedbyHW(true);
	m_ticksBeforeGrunt = 0;
}

void ZombiePed::doSomething() {
	if (getAlive() == false)
		return;
	if (detOverlap(getStudentWorld()->getGhostRacer(), this))
	{
		getStudentWorld()->getGhostRacer()->setHealth(getStudentWorld()->getGhostRacer()->getHealth() - 5);
		setHealth(getHealth() - 2);
		setAlive(false);
		return;
	}

	ZPattack();
	genmove(this);

	if (getMovementPD() > 0) {
		setMovementPD(getMovementPD() - 1);
		return;
	}
	setHspeed(randInt(-3, 3));
	while (getHspeed() == 0)
		setHspeed(randInt(-3, 3));
	setMovementPD(randInt(4, 32));
	if (getHspeed() < 0)
		setDirection(180);
	else
		setDirection(0);

}

	
void ZombiePed::ZPattack() {
	double cur_x, cur_y, gr_x, gr_y;
	getAnimationLocation(cur_x, cur_y);
	getStudentWorld()->getGhostRacer()->getAnimationLocation(gr_x, gr_y);
	if (abs(cur_x - gr_x) <= 30 && cur_y > gr_y)
	{
		setDirection(270);
		if (cur_x < gr_x)
			setHspeed(1);
		else if (cur_x > gr_x)
			setHspeed(-1);
		else
			setHspeed(0);
		m_ticksBeforeGrunt--;

		if (m_ticksBeforeGrunt <= 0) {
			getStudentWorld()->playSound(SOUND_ZOMBIE_ATTACK);
			m_ticksBeforeGrunt = 20;
		}
	}
}

void ZombiePed::hitByHW()
{
	setHealth(getHealth() - 1);
	if (getHealth() <= 0)
	{
		setAlive(false);
		getStudentWorld()->playSound(SOUND_PED_DIE);
		if (!detOverlap(this, getStudentWorld()->getGhostRacer()))
		{
			int chance = randInt(0, 4);
			double cur_x, cur_y;
			getAnimationLocation(cur_x, cur_y);
			if (chance == 0)
				getStudentWorld()->addHealingG(cur_x, cur_y);
			getStudentWorld()->increaseScore(150);
		}
	}
	else
		getStudentWorld()->playSound(SOUND_PED_HURT);

}


Cab::Cab(int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Actor(imageID, startX, startY, dir, size, depth)
{

}


ZombieCab::ZombieCab(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Cab(imageID, startX, startY, dir, size, depth)
{
	setHspeed(0);
	setVspeed(0);
	setHealth(3);
	setMovementPD(0);
	m_damagedGR = false;
	setAvoidance(true);
	setStudentWorld(env);
}

void ZombieCab::doSomething()
{
	if (getAlive() == false)
		return;
	if (detOverlap(getStudentWorld()->getGhostRacer(), this))
	{
		if (m_damagedGR == false)
		{
			getStudentWorld()->playSound(SOUND_VEHICLE_CRASH);
			getStudentWorld()->getGhostRacer()->setHealth(getStudentWorld()->getGhostRacer()->getHealth() - 20);
			double cur_x, cur_y, gr_x, gr_y;
			getAnimationLocation(cur_x, cur_y);
			getStudentWorld()->getGhostRacer()->getAnimationLocation(gr_x, gr_y);
			if (cur_x < gr_x)
			{
				setHspeed(-5);
				setDirection(120 + randInt(0, 19));
			}
			else if (cur_x >= gr_x)
			{
				setHspeed(5);
				setDirection(60 - randInt(0, 19));
			}
			m_damagedGR = true;
		}
	}
	genmove(this);
	if (getAlive() == false)
		return;

	getStudentWorld()->zombieCabCheck(this);
	setMovementPD(getMovementPD() - 1);
	if (getMovementPD() > 0)
		return;
	setMovementPD(randInt(4, 32));
	setVspeed(getVspeed() + randInt(-1, 4));
}

void ZombieCab::hitByHW()
{
	setHealth(getHealth() - 1);
	if (getHealth() <= 0)
	{
		setAlive(false);
		getStudentWorld()->playSound(SOUND_VEHICLE_DIE);
		double cur_x, cur_y;
		getAnimationLocation(cur_x, cur_y);
		int chance = randInt(0, 4);
		if (chance == 0)
		{
			getStudentWorld()->addOilSlick(cur_x, cur_y);
		}
		getStudentWorld()->increaseScore(200);
		return;
	}
	else
	{
		getStudentWorld()->playSound(SOUND_VEHICLE_HURT);
	}
}



GhostRacer::GhostRacer(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Cab(imageID, startX, startY, dir, size, depth)
{
	setHealth(100);
	setAlive(true);
	setAvoidance(true);
	setStudentWorld(env);
	setAffectedbyHW(false);
	m_sprays = 10;
	m_speed = 0;
}


void GhostRacer::doSomething()
{
	double m_x, m_y;
	GraphObject::getAnimationLocation(m_x, m_y);
	int m_dir = GraphObject::getDirection();

	if (getHealth() <= 0)
	{
		Actor::setAlive(false);
		return;
	}

	if (m_x <= ROAD_CENTER - ROAD_WIDTH / 2)
	{
		if (m_dir > 90)
			setHealth(getHealth() - 10);
		GraphObject::setDirection(82);
		//playsound SOUND_VEHICLE_CRASH
		getStudentWorld()->playSound(SOUND_VEHICLE_CRASH);
		return GRmove();
	}
	else if (m_x >= ROAD_CENTER + ROAD_WIDTH / 2)
	{
		if (m_dir < 90)
			setHealth(getHealth() - 10);
		GraphObject::setDirection(98);
		//playsound SOUND_VEHICLE_CRASH
		getStudentWorld()->playSound(SOUND_VEHICLE_CRASH);
		return GRmove();
	}

	GRmove();

	int key;
	if (!(getStudentWorld()->getKey(key)))
		return;
	switch (key) {
		// need a key press space
	case KEY_PRESS_LEFT:
		if (m_dir < 114)
			GraphObject::setDirection(m_dir + 8);
		return GRmove();
		break;
	case KEY_PRESS_RIGHT:
		if (m_dir > 66)
			GraphObject::setDirection(m_dir - 8);
		return GRmove();
		break;
	case KEY_PRESS_UP:
		if (m_speed < 5)
			m_speed++;
		return GRmove();
		break;
	case KEY_PRESS_DOWN:
		if (m_speed > -1)
			m_speed--;
		return GRmove();
		break;
	case KEY_PRESS_SPACE:
		if (getSprays() > 0)
			getStudentWorld()->addHolyWater();
		GRmove();
		break;
	default:
		return GRmove();
	}

}

void GhostRacer::GRmove() 
{
	double max_shift_per_tick = 4.0;
	int direction = getDirection();
	double delta_x = cos(direction * (M_PI/180)) * max_shift_per_tick;
	double cur_x, cur_y;
	GraphObject::getAnimationLocation(cur_x, cur_y);
	GraphObject::moveTo(cur_x + delta_x, cur_y);
	return;
}


HolyWaterProjectile::HolyWaterProjectile(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Actor(imageID, startX, startY, dir, size, depth)
{
	setAlive(true);
	setAvoidance(false);
	setStudentWorld(env);
	setmaxTraveldistance(160);
	setTravelled(0);
}

void HolyWaterProjectile::doSomething()
{
	if (getAlive() == false)
		return;
	getStudentWorld()->holywaterCheck(this);

	moveForward(SPRITE_HEIGHT);
	setTravelled(getTravelled() + SPRITE_HEIGHT);
	double cur_x, cur_y;
	getAnimationLocation(cur_x, cur_y);
	if (cur_y < 0 || cur_x < 0 || cur_x > VIEW_WIDTH || cur_y > VIEW_HEIGHT)
		setAlive(false);

	if (getTravelled() >= 160)
	{
		setAlive(false);
	}

}


