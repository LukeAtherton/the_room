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

	mBundledHost = BUNDLED_HOST;
	mUnBundledHost = UNBUNDLED_HOST;
	mBundledSendPort = BUNDLED_SEND_PORT;
	mUnBundledSendPort = UNBUNDLED_SEND_PORT;

	ifstream ifs("room.ini");
	if(ifs.good()){
		readConfFile(ifs);
	}else{
		cout << "Config File Error\n";
		// throw my_exceptions("cannot open file");
	}

	cout << "MAX IP:" << mUnBundledHost << "\n";
	cout << "MAX PORT:" << mUnBundledSendPort << "\n";
	cout << "QUARTZ IP:" << mBundledHost << "\n";
	cout << "QUARTZ PORT:" << mBundledSendPort << "\n";
   
	// open an outgoing connection to HOST:SEND_PORT
	mUnBundledSender.setup(mUnBundledHost, mUnBundledSendPort);
	mBundledSender.setup(mBundledHost, mBundledSendPort);
   
	// open connection listening to RECEIVE_PORT
	mReceiver.setup(RECEIVE_PORT);

	bundle = ofxOscBundle();
}

UDPMessenger::~UDPMessenger() {
	//assert(TheMessenger == this);
	TheMessenger = NULL;
}

void UDPMessenger::readConfFile(std::istream& is) 
{
	string key;
	string value;
	string identifier;

  for(;;) {
    string line;
	char c;

    getline(is, line);
    if(!is) break;

    istringstream iss(line);

	c=iss.peek();
	if(c == '['){
		iss.ignore(1);
		getline(iss,identifier,']');
		//cout << "Identifier found: " << identifier << "\n";
	}else{
		getline(iss, key, '=');
		//cout << "Key found: " << key << "\n";
		getline(iss, value);
		//cout << "Value found: " << value << "\n";
		if(identifier == "max"){
			if(key == "ip"){
				mUnBundledHost = value;
			}else{
				if(key == "port"){
					mUnBundledSendPort = atoi(value.c_str());
				}
			}
		}else{
			if(identifier == "quartz"){
				if(key == "ip"){
					mBundledHost = value;
				}else{
					if(key == "port"){
						mBundledSendPort = atoi(value.c_str());
					}
				}
			}
		}
	}
  }
  if(!is.eof()) cout << "Config File Error\n";
  //if(!is.eof()) throw my_exceptions("error reading file");
}

void UDPMessenger::Dispatch() {
	//mBundledSender.sendBundle(bundle);
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
	cout << label << ":" << val << "\n";
	ofxOscMessage msg;
	msg.setAddress(label);
	msg.addFloatArg(val);
	mUnBundledSender.sendMessage(msg);
	//mBundledSender.sendMessage(msg);
	//bundle.addMessage(msg);
}

void UDPMessenger::SendIntMessage(string label, int val) {
	cout << label << ":" << val << "\n";
	ofxOscMessage msg;
	msg.setAddress(label);
	msg.addIntArg(val);
	mUnBundledSender.sendMessage(msg);
	//mBundledSender.sendMessage(msg);
	//bundle.addMessage(msg);
}

void UDPMessenger::SendStringMessage(string label, string val) {
	cout << label << ":" << val << "\n";
	ofxOscMessage msg;
	msg.setAddress(label);
	msg.addStringArg(val);
	mUnBundledSender.sendMessage(msg);
	//mBundledSender.sendMessage(msg);
	//bundle.addMessage(msg);
}

void UDPMessenger::SendVectorMessage(string label, ofxVec3f val) {
	//cout << label << ":" << val.x << "-" << val.y << "-" << val.z << "\n";
	ofxOscMessage msg;
	msg.setAddress(label);
	msg.addFloatArg(val.x);
	msg.addFloatArg(val.y);
	msg.addFloatArg(val.z);	
	mUnBundledSender.sendMessage(msg);
	//mBundledSender.sendMessage(msg);
	bundle.addMessage(msg);
}