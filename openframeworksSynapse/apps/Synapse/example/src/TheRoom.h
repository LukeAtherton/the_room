/*
 * Copyright (c) 2011
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#pragma once

#include "ofMain.h"
#include "UDPMessenger.h"
#include "ofxVec3f.h"
#include <list>
#include "JointHitDetector.h"

#define MAX_PEOPLE 6		// The maximum number of people in the room.
#define UPDATE_RATE 30		// The desired frame rate for the application.
#define DT 0.03333f			// The time between frames (1.0 / UPDATE_RATE).
#define ROLLING_AVG 50		// The number of samples we average across for the speed.

/**
 * Class that represents somebody inside the room.
 */
class Person {
public:
	/**
	 * Constructor.
	 */
	Person();

	/**
	 * Constructor
	 *
	 * @id The id of the person in the room.
	 */
	Person(int id);

	/**
	 * Destructor.
	 */
	~Person();

	void update(bool in_room, 
		ofxVec3f torso_position, 
		ofxVec3f righthand_pos_world, 
		ofxVec3f lefthand_pos_world, 
		ofxVec3f rightelbow_pos_world,
		ofxVec3f leftelbow_pos_world,
		ofxVec3f rightknee_pos_world,
		ofxVec3f leftknee_pos_world);

	// The ID of the person.
	int person_id;

	// Is the person inside the room?
	bool inside_room;

	// The last known coordinates of the person in the room.
	ofxVec3f last_torso_position;

	// The current speed of the person in the room.
	ofxVec3f speed;

	list<float> speeds;
	float avg_speed;
	float sum_speed;

	static int totalInRoom;

	std::vector<JointHitDetector*> mHitDetector;

};

/**
 * Class that represents the current state of the room, is it empty? How many people are present?
 * Is it chaotic, or is it zen?
 */
class TheRoom : public IMessageReceiver {
public:
	
	/**
	 * Constructor.
	 */
	TheRoom();

	/**
	 * Destructor.
	 */
	~TheRoom();

	/**
	 * @return true if nobody is in the room, false otherwise.
	 */
	bool isEmpty();

	/**
	 * Updates the position of a person in the room.
	 *
	 * @id The ID of the person we are updating (0 -> 6).
	 * @in_room true if the person is in the room, false otherwise.
	 * @x_pos The x position of the person in the room.
	 * @y_pos The y position of the person in the room.
	 * @z_pos The z position of the person in the room.
	 */
	void updatePerson(int id, bool in_room, 
		ofxVec3f torso_position, 
		ofxVec3f righthand_pos_world, 
		ofxVec3f lefthand_pos_world, 
		ofxVec3f rightelbow_pos_world,
		ofxVec3f leftelbow_pos_world,
		ofxVec3f rightknee_pos_world,
		ofxVec3f leftknee_pos_world);

	/**
	 * Call back for incoming OscMessage.
	 */
	void OnMessage(const ofxOscMessage& msg);

	// All the people in the room.
	Person people_in_room[MAX_PEOPLE];

	// Have we sent a stop message to ableton?
	bool sent_stop;
	bool room_full;
	float zen_level;
	int currentlyInRoom;

};
