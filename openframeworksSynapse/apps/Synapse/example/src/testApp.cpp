#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
   recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
	recordDepth.setup(&recordContext);
	recordImage.setup(&recordContext);
	
	recordUser.setup(&recordContext);
	recordUser.setMaxNumberOfUsers(MAX_PEOPLE);
	
	recordContext.toggleRegisterViewport();
	recordContext.toggleMirror();

	for(int activeSkeleton = 0; activeSkeleton < MAX_PEOPLE; activeSkeleton++){
		mActiveSkeletons.push_back(new ActiveSkeleton(&recordUser, &recordDepth, activeSkeleton));
	}

	roomEmpty = true;
	TheMessenger->SendStringMessage("/room_empty", "true");

	ofBackground(0, 0, 0);
}

void testApp::exit()
{
   for (int i=0; i<mActiveSkeletons.size(); ++i)
      delete mActiveSkeletons[i];
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
   
	int currentTime = ofGetElapsedTimeMillis();
	float dt = (float) (currentTime - lastPollTime) / 1000.0f;

	for (int currentUser=0; currentUser<recordUser.getNumberOfTrackedUsers(); ++currentUser)
	{
		ofxTrackedUser* pUser = recordUser.getTrackedUser(currentUser+1);
		if (pUser != NULL && pUser->IsTracked())
		{
			mActiveSkeletons[currentUser]->SetActiveUser(pUser);
			mActiveSkeletons[currentUser]->UpdateSkeleton(dt);
			if(roomEmpty){
				roomEmpty = false;
				TheMessenger->SendStringMessage("/room_empty", "false");
			}
		}else{
			mActiveSkeletons[currentUser]->SetActiveUser(NULL);
		}
	}

	if(recordUser.getNumberOfTrackedUsers() == 0 && !roomEmpty){
		roomEmpty = true;
		TheMessenger->SendStringMessage("/room_empty", "true");
	}

	TheMessenger->Dispatch();
	lastPollTime = currentTime;
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(255, 255, 255);
	
	glPushMatrix();
	glScalef(1, 1, 1);
	
	recordDepth.draw(0,0,640,480);
   recordUser.draw();	

	for (int currentSkeleton=0; currentSkeleton<mActiveSkeletons.size(); ++currentSkeleton){
		if(mActiveSkeletons[currentSkeleton]->IsTracked()){
			mActiveSkeletons[currentSkeleton]->Draw();
		}
	}
		
	glPopMatrix();
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

