#ifndef _H_OFXTRACKEDUSER
#define _H_OFXTRACKEDUSER

#include <XnOpenNI.h>
#include <XnCppWrapper.h>
#include "ofxOpenNIContext.h"
#include "ofMain.h"

struct ofxLimb {
	ofxLimb(XnSkeletonJoint nStartJoint, XnSkeletonJoint nEndJoint)
	:start_joint(nStartJoint)
	,end_joint(nEndJoint)
	,found(false)
	{
		position[0].X = position[1].X = 0;
		position[0].Y = position[1].Y = 0;
		position[0].Z = position[1].Z = 0;
	}

	ofxLimb(){};
	~ofxLimb(){};

	XnSkeletonJoint start_joint;
	XnSkeletonJoint end_joint;
	XnPoint3D position[2];
	bool found;

	void debugDraw() {
		if(!found)
			return;
		if(position[0].X > 0.0f && position[0].Y > 0.0f && position[1].X > 0.0f && position[1].Y > 0.0f){
			glPushMatrix();
			glLineWidth(10);
			glColor4f(0,1,0,0.3f);
			glBegin(GL_LINES);
			glVertex2i(position[0].X * 1.5f, position[0].Y * 1.5f);
			glVertex2i(position[1].X * 1.5f, position[1].Y * 1.5f);
			glEnd();
			glPopMatrix();
		}
	}

};

class ofxOpenNIContext;
class ofxTrackedUser {
public:

	void debugDraw();
	bool IsTracked();

	ofxLimb neck;

	// left arm + shoulder
	ofxLimb left_shoulder;
	ofxLimb left_upper_arm;
	ofxLimb left_lower_arm;

	// right arm + shoulder
	ofxLimb right_shoulder;
	ofxLimb right_upper_arm;
	ofxLimb right_lower_arm;

	// torso
	ofxLimb left_upper_torso;
	ofxLimb right_upper_torso;

	// left lower torso + leg
	ofxLimb left_lower_torso;
	ofxLimb left_upper_leg;
	ofxLimb left_lower_leg;

	// right lower torso + leg
	ofxLimb right_lower_torso;
	ofxLimb right_upper_leg;
	ofxLimb right_lower_leg;

	ofxLimb hip;
	XnUserID id;


private:

	ofxTrackedUser(ofxOpenNIContext* pContext);

	void updateBonePositions();
	void updateLimb(ofxLimb& rLimb);

	ofxOpenNIContext*		context;
	xn::UserGenerator		user_generator;
	xn::DepthGenerator		depth_generator;

	friend class ofxUserGenerator;
};

#endif
