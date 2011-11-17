/*
 *  UDPMessenger.h
 *  emptyExample
 *
 *  Created by Ryan Challinor on 5/17/11.
 *  Copyright 2011 aww bees. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxVec3f.h"

#define HOST2 "localhost"
#define HOST "localhost"
#define SEND_PORT 12345
#define SEND_PORT2 22345
#define RECEIVE_PORT 12346

class IMessageReceiver {
public:
	virtual void OnMessage(const ofxOscMessage& msg) = 0;
};

struct Listener {
	IMessageReceiver* mReceiver;
	string mLabel;
};

class UDPMessenger {
public:
	UDPMessenger();
	~UDPMessenger();
   
	void Poll();
	void Dispatch();
   
	void SendFloatMessage(string label, float val);
	void SendIntMessage(string label, int val);
	void SendStringMessage(string label, string val);
	void SendVectorMessage(string label, ofxVec3f val);
   
	void AddListener(IMessageReceiver* receiver, string label);
	void RemoveListener(IMessageReceiver* receiver);
	void RemoveListener(IMessageReceiver* receiver, string label);
   
private:
	ofxOscSender mSender;
	ofxOscSender mSender2;
	ofxOscReceiver mReceiver;
	ofxOscBundle bundle;
   
	std::list<Listener> mListeners;
};

extern UDPMessenger* TheMessenger;