/*
 *  UDPMessenger.cpp
 *  emptyExample
 *
 *  Created by Ryan Challinor on 5/17/11.
 *  Copyright 2011 aww bees. All rights reserved.
 *
 */
#include "UDPMessenger.h"

UDPMessenger* TheMessenger = NULL;

UDPMessenger::UDPMessenger() {
	fprintf(stderr, "creating messenger\n");
	//assert(TheMessenger == NULL);
	TheMessenger = this;
   
	// open an outgoing connection to HOST:SEND_PORT
	mSender.setup(HOST, SEND_PORT);
	mSender2.setup(HOST2, SEND_PORT2);
   
	// open connection listening to RECEIVE_PORT
	mReceiver.setup(RECEIVE_PORT);

	bundle = ofxOscBundle();
}

UDPMessenger::~UDPMessenger() {
	//assert(TheMessenger == this);
	TheMessenger = NULL;
}

void UDPMessenger::Dispatch() {
	mSender.sendBundle(bundle);
	mSender2.sendBundle(bundle);

	// Clear the bundle after we have dispatched it.
	bundle.clear();
}

void UDPMessenger::Poll() {
	while (mReceiver.hasWaitingMessages()) {
		// get the next message
		ofxOscMessage msg;
		mReceiver.getNextMessage( &msg );
      
		string label = msg.getAddress();
      
		for (std::list<Listener>::iterator i = mListeners.begin(); i != mListeners.end(); ++i) {
			const Listener& listener = *i;
			if (listener.mLabel == label) {
				listener.mReceiver->OnMessage(msg);
			}
		}
	}
}

void UDPMessenger::AddListener(IMessageReceiver* receiver, string label) {
	Listener listener;
	listener.mReceiver = receiver;
	listener.mLabel = label;
	mListeners.push_back(listener);
}

void UDPMessenger::RemoveListener(IMessageReceiver* receiver) {
	for (std::list<Listener>::iterator i = mListeners.begin(); i != mListeners.end(); ++i) {
		const Listener& listener = *i;
		if (listener.mReceiver == receiver) {
			i = mListeners.erase(i);
		}
	}
}

void UDPMessenger::RemoveListener(IMessageReceiver* receiver, string label) {
	for (std::list<Listener>::iterator i = mListeners.begin(); i != mListeners.end(); ++i) {
		const Listener& listener = *i;
		if (listener.mLabel == label && listener.mReceiver == receiver) {
			i = mListeners.erase(i);
		}
	}
}

void UDPMessenger::SendFloatMessage(string label, float val) {	
	ofxOscMessage msg;
	msg.setAddress(label);
	msg.addFloatArg(val);
	bundle.addMessage(msg);
}

void UDPMessenger::SendIntMessage(string label, int val) {
	ofxOscMessage msg;
	msg.setAddress(label);
	msg.addIntArg(val);
	bundle.addMessage(msg);
}

void UDPMessenger::SendStringMessage(string label, string val) {
	ofxOscMessage msg;
	msg.setAddress(label);
	msg.addStringArg(val);
	bundle.addMessage(msg);
}

void UDPMessenger::SendVectorMessage(string label, ofxVec3f val) {
	ofxOscMessage msg;
	msg.setAddress(label);
	msg.addFloatArg(val.x);
	msg.addFloatArg(val.y);
	msg.addFloatArg(val.z);	
	bundle.addMessage(msg);
}