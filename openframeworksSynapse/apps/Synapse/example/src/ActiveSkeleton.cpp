/*
 *  ActiveSkeleton.cpp
 *  emptyExample
 *
 *  Created by Ryan Challinor on 5/18/11.
 *  Copyright 2011 aww bees. All rights reserved.
 *
 */

#include "ActiveSkeleton.h"

ActiveSkeleton* TheActiveSkeleton = NULL;

ActiveSkeleton::ActiveSkeleton()
   : mUserGenerator(NULL)
   , mUser(NULL)
   , mDepthGenerator(NULL)
{

}

ActiveSkeleton::ActiveSkeleton(ofxUserGenerator* userGenerator, ofxDepthGenerator* depthGenerator, int skeletonID)
	: mUserGenerator(userGenerator)
	, mDepthGenerator(depthGenerator)
	, mSkeletonID(skeletonID)
	, mUser(NULL)
{

	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_RIGHT_HAND, XN_SKEL_TORSO, "_righthand"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_LEFT_HAND, XN_SKEL_TORSO, "_lefthand"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_RIGHT_ELBOW, XN_SKEL_TORSO, "_rightelbow"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_LEFT_ELBOW, XN_SKEL_TORSO, "_leftelbow"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_RIGHT_KNEE, XN_SKEL_TORSO, "_rightknee"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_LEFT_KNEE, XN_SKEL_TORSO, "_leftknee"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_RIGHT_FOOT, XN_SKEL_TORSO, "_rightfoot"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_LEFT_FOOT, XN_SKEL_TORSO, "_leftfoot"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_HEAD, XN_SKEL_RIGHT_HIP, "_head"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_TORSO, XN_SKEL_TORSO, "_torso", 75));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_NECK, XN_SKEL_TORSO, "_neck"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_LEFT_SHOULDER, XN_SKEL_TORSO, "_leftshoulder"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_RIGHT_SHOULDER, XN_SKEL_TORSO, "_rightshoulder"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_LEFT_HIP, XN_SKEL_TORSO, "_lefthip"));
	mHitDetector.push_back(new JointHitDetector(this, XN_SKEL_RIGHT_HIP, XN_SKEL_TORSO, "_righthip"));
	mClosestHand = new JointHitDetector(this, XN_SKEL_RIGHT_HAND, XN_SKEL_TORSO, "_closesthand");
	mHitDetector.push_back(mClosestHand);

}

ActiveSkeleton::~ActiveSkeleton()
{

   for (int currentDetector=0; currentDetector<mHitDetector.size(); ++currentDetector)
      delete mHitDetector[currentDetector];
}

ofxVec3f ActiveSkeleton::GetRealWorldPos(XnSkeletonJoint joint, float& confidence) const
{
   assert(mUserGenerator != NULL);
   
   if (mUser == NULL)
      return ofxVec3f(0,0,0);   
   ofxVec3f vRealWorld;
   XnSkeletonJointPosition pos;
   mUserGenerator->getXnUserGenerator().GetSkeletonCap().
      GetSkeletonJointPosition(mUser->id,joint,pos);
   vRealWorld.x = pos.position.X;
   vRealWorld.y = pos.position.Y;
   vRealWorld.z = pos.position.Z;
   confidence = pos.fConfidence;
   
   return vRealWorld;
}

ofxVec3f ActiveSkeleton::GetProjectivePos(XnSkeletonJoint joint) const
{
   assert(mDepthGenerator != NULL);
   
   float confidence;
   ofxVec3f vPos = GetRealWorldPos(joint, confidence);
   XnPoint3D pos[1];
   pos[0].X = vPos.x; pos[0].Y = vPos.y; pos[0].Z = vPos.z; 
   mDepthGenerator->getXnDepthGenerator().ConvertRealWorldToProjective(1, pos, pos);
   
   vPos.x = pos[0].X * 1.5f; vPos.y = pos[0].Y * 1.5f; vPos.z = pos[0].Z * 1.5f; 
   return vPos;
}

void ActiveSkeleton::PollJointHitDetectors(float dt){
	for (int i=0; i < mHitDetector.size(); ++i)
	{
		mHitDetector[i]->Poll(dt);
	}
}

void ActiveSkeleton::UpdateClosestHand(){
	float dummy;
	if (this->GetRealWorldPos(XN_SKEL_RIGHT_HAND, dummy).z <
		this->GetRealWorldPos(XN_SKEL_LEFT_HAND, dummy).z)
		mClosestHand->SetJoint(XN_SKEL_RIGHT_HAND);
	else
		mClosestHand->SetJoint(XN_SKEL_LEFT_HAND);
}

void ActiveSkeleton::UpdateSkeleton(float dt){
	this->UpdateClosestHand();
	this->PollJointHitDetectors(dt);
}

void ActiveSkeleton::Draw(){
	for (int i=0; i<mHitDetector.size(); ++i)
		mHitDetector[i]->Draw();
}
