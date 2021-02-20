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

StatObj::StatObj(int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Actor(imageID, startX, startY, dir, size, depth)
{

}

Borderline::Borderline(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: StatObj(imageID, startX, startY, dir, size, depth)
{
	m_vSpeed = -4;
	m_hSpeed = 0;
	setAlive(true);;
	setAvoidance(false);
	setStudentWorld(env);

}


void Borderline::doSomething() {
	int vert_speed = m_vSpeed - getStudentWorld()->getGhostRacer()->getSpeed();
	int horiz_speed = m_hSpeed;
	double cur_y, cur_x;
	GraphObject::getAnimationLocation(cur_x, cur_y);
	cur_y += vert_speed;
	cur_x += horiz_speed;
	GraphObject::moveTo(cur_x, cur_y);
	if (cur_y < 0 || cur_x < 0 || cur_x > VIEW_WIDTH || cur_y > VIEW_HEIGHT)
		setAlive(false);
	return;

}

Cab::Cab(int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Actor(imageID, startX, startY, dir, size, depth)
{

}

GhostRacer::GhostRacer(StudentWorld* env, int imageID, double startX, double startY, int dir, double size, unsigned int depth)
	: Cab(imageID, startX, startY, dir, size, depth)
{
	m_health = 100;
	m_sprays = 10;
	m_speed = 0;
	setAlive(true);
	setAvoidance(true);
	setStudentWorld(env);
}



void GhostRacer::doSomething()
{
	double m_x, m_y;
	GraphObject::getAnimationLocation(m_x, m_y);
	int m_dir = GraphObject::getDirection();

	if (m_health <= 0)
	{
		Actor::setAlive(false);
		return;
	}

	if (m_x <= ROAD_CENTER - ROAD_WIDTH / 2)
	{
		if (m_dir > 90)
			m_health -= 10;
		GraphObject::setDirection(82);
		//playsound SOUND_VEHICLE_CRASH
		return move();
	}
	else if (m_x >= ROAD_CENTER + ROAD_WIDTH / 2)
	{
		if (m_dir < 90)
			m_health -= 10;
		GraphObject::setDirection(98);
		//playsound SOUND_VEHICLE_CRASH
		return move();
	}

	move();

	int key;
	if (!(getStudentWorld()->getKey(key)))
		return;
	switch (key) {
		// need a key press space
	case KEY_PRESS_LEFT:
		if (m_dir < 114)
			GraphObject::setDirection(m_dir + 8);
		return move();
		break;
	case KEY_PRESS_RIGHT:
		if (m_dir > 66)
			GraphObject::setDirection(m_dir - 8);
		return move();
		break;
	case KEY_PRESS_UP:
		if (m_speed < 5)
			m_speed++;
		return move();
		break;
	case KEY_PRESS_DOWN:
		if (m_speed > -1)
			m_speed--;
		return move();
		break;
	default:
		return move();
	}

}

void GhostRacer::move() 
{
	double max_shift_per_tick = 4.0;
	int direction = getDirection();
	double delta_x = cos(direction * (M_PI/180)) * max_shift_per_tick;
	double cur_x, cur_y;
	GraphObject::getAnimationLocation(cur_x, cur_y);
	GraphObject::moveTo(cur_x + delta_x, cur_y);
	return;
}

