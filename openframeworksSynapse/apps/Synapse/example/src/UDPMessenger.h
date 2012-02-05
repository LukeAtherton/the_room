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
#include <time.h>

#define UNBUNDLED_HOST "localhost"
#define BUNDLED_HOST "localhost"
#define UNBUNDLED_SEND_PORT 12345
#define BUNDLED_SEND_PORT 22345
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

	void readConfFile(std::istream& is);
   
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
	ofxOscSender mUnBundledSender;
	ofxOscSender mBundledSender;
	ofxOscReceiver mReceiver;
	ofxOscBundle bundle;
   
	std::list<Listener> mListeners;

	string mBundledHost;
	string mUnBundledHost;
	int mBundledSendPort;
	int mUnBundledSendPort;

	time_t lastSendBundled;
	time_t lastSendUnBundled;

};

extern UDPMessenger* TheMessenger;