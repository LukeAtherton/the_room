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
#include "TheRoom.h"
#include <sstream>

string person_info[MAX_PEOPLE][8] = {
	
	{"/p1_speed", "/p1_torso", "/p1_righthand", "/p1_lefthand", "/p1_rightelbow",
	"/p1_leftelbow", "/p1_rightkne", "/p1_leftknee"},

	{"/p2_speed", "/p2_torso", "/p2_righthand", "/p2_lefthand", "/p2_rightelbow",
	"/p2_leftelbow", "/p2_rightknee", "/p2_leftknee"},

	{"/p3_speed", "/p3_torso", "/p3_righthand", "/p3_lefthand", "/p3_rightelbow",
	"/p3_leftelbow", "/p3_rightknee", "/p3_leftknee"},

	{"/p4_speed", "/p4_torso", "/p4_righthand", "/p4_lefthand", "/p4_rightelbow",
	"/p4_leftelbow", "/p4_rightknee", "/p4_leftknee"},

	{"/p5_speed", "/p5_torso", "/p5_righthand", "/p5_lefthand", "/p5_rightelbow",
	"/p5_leftelbow", "/p5_rightknee", "/p5_leftknee"},

	{"/p6_speed", "/p6_toso", "/p6_righthand", "/p6_lefthand", "/p6_rightelbow",
	"/p6_leftelbow", "/p6_rightknee", "/p6_leftknee"}

};

int Person::totalInRoom = 0;

Person::Person() {
	Person(0);
}

Person::Person(int id) {
	person_id = id;
	avg_speed = 0.0f;
	sum_speed = 0.0f;
	last_torso_position = ofxVec3f(0.0f, 0.0f, 0.0f);
	inside_room = false;

	char id_string [5];
	itoa (person_id, id_string, 10);

	mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_HAND, XN_SKEL_TORSO, (string)"/p" + id_string + "_righthand"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_HAND, XN_SKEL_TORSO, (string)"/p" + id_string + "_lefthand"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_ELBOW, XN_SKEL_TORSO, (string)"/p" + id_string + "_rightelbow"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_ELBOW, XN_SKEL_TORSO, (string)"/p" + id_string + "_leftelbow"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_KNEE, XN_SKEL_TORSO, (string)"/p" + id_string + "_rightknee"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_KNEE, XN_SKEL_TORSO, (string)"/p" + id_string + "_leftknee"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_FOOT, XN_SKEL_TORSO, (string)"/p" + id_string + "_rightfoot"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_FOOT, XN_SKEL_TORSO, (string)"/p" + id_string + "_leftfoot"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_HEAD, XN_SKEL_RIGHT_HIP, (string)"/p" + id_string + "_head"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_TORSO, XN_SKEL_TORSO, (string)"/p" + id_string + "_torso", 75));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_NECK, XN_SKEL_TORSO, (string)"/p" + id_string + "_neck"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO, (string)"/p" + id_string + "_leftshoulder"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO, (string)"/p" + id_string + "_rightshoulder"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_HIP, XN_SKEL_TORSO, (string)"/p" + id_string + "_lefthip"));
	mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_HIP, XN_SKEL_TORSO, (string)"/p" + id_string + "_righthip"));
	

   //mClosestHand = new JointHitDetector(XN_SKEL_RIGHT_HAND, XN_SKEL_TORSO, "/closesthand");
   //mHitDetector.push_back(mClosestHand);

}

Person::~Person() {
}

void Person::update(bool in_room, 
					ofxVec3f torso_position, 
					ofxVec3f righthand_pos_world, 
					ofxVec3f lefthand_pos_world, 
					ofxVec3f rightelbow_pos_world,
					ofxVec3f leftelbow_pos_world,
					ofxVec3f rightknee_pos_world,
					ofxVec3f leftknee_pos_world) {

	// Person is currently inside the room - update and tell OSC.
	if (in_room) {

		if(!inside_room){
			inside_room = true;
			Person::totalInRoom++;
		}

		// Determine the current speed of the person on each axis.
		speed = last_torso_position - torso_position;
		speed /= DT;

		float sq_length = fabs(speed.squareLength());
		speeds.push_back(sq_length);

		if (sq_length){
			sum_speed += sq_length;

			if (speeds.size() >= ROLLING_AVG) {
				sq_length = speeds.front();
				sum_speed -= sq_length;
				speeds.pop_front();
			}

			avg_speed = sum_speed / ((float) speeds.size());
		}
		//fprintf(stdout, "%f \n", avg_speed);

		// Update state of person.
		last_torso_position = torso_position;
		
		// Send information to Ableton Live via OSC.
		TheMessenger->SendVectorMessage(person_info[person_id][0], avg_speed);
		TheMessenger->SendVectorMessage(person_info[person_id][1], last_torso_position);
		TheMessenger->SendVectorMessage(person_info[person_id][2], righthand_pos_world - last_torso_position);
		TheMessenger->SendVectorMessage(person_info[person_id][3], lefthand_pos_world - last_torso_position);
		TheMessenger->SendVectorMessage(person_info[person_id][4], rightelbow_pos_world - last_torso_position);
		TheMessenger->SendVectorMessage(person_info[person_id][5], leftelbow_pos_world - last_torso_position);
		TheMessenger->SendVectorMessage(person_info[person_id][6], rightknee_pos_world - last_torso_position);
		TheMessenger->SendVectorMessage(person_info[person_id][7], leftknee_pos_world - last_torso_position);

	}else{
		// If the person is currently inside the room, and they left - clear state:	
		if (inside_room) {
			last_torso_position = ofxVec3f(0.0f, 0.0f, 0.0f);
			speed = ofxVec3f(0.0f, 0.0f, 0.0f);
			avg_speed = 0.0f;
			sum_speed = 0.0f;
			speeds.clear();
			inside_room = false;
			Person::totalInRoom--;
		}
	}
}

bool TheRoom::isEmpty() {
	/*for (int i = 0; i < MAX_PEOPLE; i++) {
		if (people_in_room[i].inside_room) {
			return false;
			break;
		}
	}
	return true;*/
	if(Person::totalInRoom==0){
		return true;
	}else{
		return false;
	}
}

// ------------------------------------------------------------------------------------------------
TheRoom::TheRoom() {
	for (int i = 0; i < MAX_PEOPLE; i++) {
		people_in_room[i] = Person(i);
	}
	sent_stop = true;
}

TheRoom::~TheRoom() {
}

void TheRoom::updatePerson(int id, bool in_room, 
		ofxVec3f torso_position, 
		ofxVec3f righthand_pos_world, 
		ofxVec3f lefthand_pos_world, 
		ofxVec3f rightelbow_pos_world,
		ofxVec3f leftelbow_pos_world,
		ofxVec3f rightknee_pos_world,
		ofxVec3f leftknee_pos_world) {

	//	Calculates position and speed of the person in the room.
	if (id < MAX_PEOPLE) {
		people_in_room[id].update(in_room, 
			torso_position, 
			righthand_pos_world, 
			lefthand_pos_world, 
			rightelbow_pos_world,
			leftelbow_pos_world,
			rightknee_pos_world,
			leftknee_pos_world);
	}

	// Workout if the room is empty or not and send it to Ableton Live via OSC.
	if (!sent_stop && isEmpty()) {
		TheMessenger->SendStringMessage("/empty", "true");
		sent_stop = true;		
	} else if (in_room) {
		sent_stop = false;
	}
}

void TheRoom::OnMessage(const ofxOscMessage& msg) {
}