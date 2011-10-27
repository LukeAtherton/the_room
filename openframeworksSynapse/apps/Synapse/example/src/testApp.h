#ifndef _TEST_APP
#define _TEST_APP

//#define USE_IR // Uncomment this to use infra red instead of RGB cam...

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "UDPMessenger.h"
#include "ActiveSkeleton.h"

#include "TheRoom.h"

class JointHitDetector;


class testApp : public ofBaseApp, public IMessageReceiver{
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

	void gotMessage(ofxOscMessage msg);
	void OnMessage(const ofxOscMessage& msg);
	
	ofxOpenNIContext	recordContext;
	ofxDepthGenerator	recordDepth;

	ofxImageGenerator	recordImage;
	ofxUserGenerator	recordUser;
	
   UDPMessenger mMessenger;
   TheRoom* room;

};

#endif
