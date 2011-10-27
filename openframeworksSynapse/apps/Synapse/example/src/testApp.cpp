#include "testApp.h"
#include "JointHitDetector.h"

//--------------------------------------------------------------
void testApp::setup()
{
   recordContext.setup();	// all nodes created by code -> NOT using the xml config file at all
	recordDepth.setup(&recordContext);
	recordImage.setup(&recordContext);
	
	recordUser.setup(&recordContext);
	
	recordContext.toggleRegisterViewport();
	recordContext.toggleMirror();

	mActiveSkeleton.SetUserGenerator(&recordUser);
   mActiveSkeleton.SetDepthGenerator(&recordDepth);
   
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_HAND, XN_SKEL_TORSO, "/righthand"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_HAND, XN_SKEL_TORSO, "/lefthand"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_ELBOW, XN_SKEL_TORSO, "/rightelbow"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_ELBOW, XN_SKEL_TORSO, "/leftelbow"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_KNEE, XN_SKEL_TORSO, "/rightknee"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_KNEE, XN_SKEL_TORSO, "/leftknee"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_FOOT, XN_SKEL_TORSO, "/rightfoot"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_FOOT, XN_SKEL_TORSO, "/leftfoot"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_HEAD, XN_SKEL_RIGHT_HIP, "/head"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_TORSO, XN_SKEL_TORSO, "/torso", 75));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_NECK, XN_SKEL_TORSO, "/neck"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO, "/leftshoulder"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO, "/rightshoulder"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_LEFT_HIP, XN_SKEL_TORSO, "/lefthip"));
   mHitDetector.push_back(new JointHitDetector(XN_SKEL_RIGHT_HIP, XN_SKEL_TORSO, "/righthip"));
   mClosestHand = new JointHitDetector(XN_SKEL_RIGHT_HAND, XN_SKEL_TORSO, "/closesthand");
   mHitDetector.push_back(mClosestHand);
	
	ofBackground(0, 0, 0);
	
}

void testApp::exit()
{
   for (int i=0; i<mHitDetector.size(); ++i)
      delete mHitDetector[i];
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
   
	float dt = .01666f;

	ofxTrackedUser* pTracked = NULL;
	for (int i=0; i<recordUser.getNumberOfTrackedUsers(); ++i)
	{
		ofxTrackedUser* pUser = recordUser.getTrackedUser(i+1);
		if (pUser != NULL && pUser->IsTracked())
		{
			pTracked = pUser;
			break;
		}
	}
   
	mActiveSkeleton.SetActiveUser(pTracked);
   
   bool sTracking = false;
   if (mActiveSkeleton.IsTracked())
	{
		float dummy;
		if (TheActiveSkeleton->GetRealWorldPos(XN_SKEL_RIGHT_HAND, dummy).z <
			TheActiveSkeleton->GetRealWorldPos(XN_SKEL_LEFT_HAND, dummy).z)
			mClosestHand->SetJoint(XN_SKEL_RIGHT_HAND);
		else
			mClosestHand->SetJoint(XN_SKEL_LEFT_HAND);
      
		for (int i=0; i<mHitDetector.size(); ++i)
		{
			mHitDetector[i]->Poll(dt);
		}

      if (!sTracking)
      {
         sTracking = true;
         TheMessenger->SendIntMessage("/tracking_skeleton", 1);
      }
	}
   else
   {
      if (sTracking)
      {
         sTracking = false;
         TheMessenger->SendIntMessage("/tracking_skeleton", 0);
      }
   }
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofSetColor(255, 255, 255);
	
	glPushMatrix();
	glScalef(1, 1, 1);
	
	recordDepth.draw(0,0,640,480);
   recordUser.draw();			

	for (int i=0; i<mHitDetector.size(); ++i)
		mHitDetector[i]->Draw();
		
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

