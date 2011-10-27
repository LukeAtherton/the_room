#include "testApp.h"
#include "JointHitDetector.h"
#include <sys/stat.h> 
//--------------------------------------------------------------
void testApp::setup()
{
	recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
	recordDepth.setup(&recordContext);
	recordImage.setup(&recordContext);
	
	recordUser.setup(&recordContext);
	
	recordContext.toggleRegisterViewport();
	recordContext.toggleMirror();

	ofBackground(0, 0, 0);

	room = new TheRoom();
}

void testApp::exit()
{
   delete room;
}

//--------------------------------------------------------------
void testApp::update()
{		
	// update all nodes
	recordContext.update();
	recordDepth.update();
	recordImage.update();
	recordUser.update();
	
	TheMessenger->Poll();
   
	for (int i = 0; (i < recordUser.getNumberOfTrackedUsers()) && (i < MAX_PEOPLE); i++) {
		ofxTrackedUser* user = recordUser.getTrackedUser(i+1);
		if (user != NULL) {
			room->updatePerson(i, user->left_upper_torso.found,
			ofxVec3f (user->left_upper_torso.position[0].X, user->left_upper_torso.position[0].Y, user->left_upper_torso.position[0].Z),
			ofxVec3f (user->right_lower_arm.position[1].X, user->right_lower_arm.position[1].Y, user->right_lower_arm.position[1].Z),
			ofxVec3f (user->left_lower_arm.position[1].X, user->left_lower_arm.position[1].Y, user->left_lower_arm.position[1].Z),
			ofxVec3f (user->right_lower_arm.position[0].X, user->right_lower_arm.position[0].Y, user->right_lower_arm.position[0].Z),
			ofxVec3f (user->left_lower_arm.position[0].X, user->left_lower_arm.position[0].Y, user->left_lower_arm.position[0].Z),
			ofxVec3f (user->right_lower_leg.position[0].X, user->right_lower_leg.position[0].Y, user->right_lower_leg.position[0].Z),
			ofxVec3f (user->left_lower_leg.position[0].X, user->left_lower_leg.position[0].Y, user->left_lower_leg.position[0].Z)				
			);
		}
	}

	TheMessenger->Dispatch();

}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(255, 255, 255);
	
	glPushMatrix();
	glScalef(1, 1, 1);
	
	recordDepth.draw(0,0,640,480);
	recordUser.draw();			
}

//--------------------------------------------------------------
void testApp::OnMessage(const ofxOscMessage& msg) {
	/*
	 * Uncomment if you want to recieve incoming OSC messages.
	 *
   if (msg.getAddress() == "/depth_mode")
   {
      assert(msg.getNumArgs() == 1);
      assert(msg.getArgType(0) == OFXOSC_TYPE_INT32);
      int mode = msg.getArgAsInt32(0);
      if (mode >= 0 && mode < kNumDepthModes)
         mDepthMode = (DepthMode)mode;
   }
	 */
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

