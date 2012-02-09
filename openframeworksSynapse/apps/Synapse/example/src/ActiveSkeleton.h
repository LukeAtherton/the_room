/*
 *  ActiveSkeleton.h
 *  emptyExample
 *
 *  Created by Ryan Challinor on 5/18/11.
 *  Copyright 2011 aww bees. All rights reserved.
 *
 */

#ifndef ActiveSkeleton_Included
#define ActiveSkeleton_Included

#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxVec3f.h"
#include "JointHitDetector.h"
#include "ofUtils.h"

class JointHitDetector;

class ActiveSkeleton
{
public:
   ActiveSkeleton();
   ActiveSkeleton(ofxUserGenerator* userGenerator, ofxDepthGenerator* depthGenerator, int skeletonID);

   ~ActiveSkeleton();
   
   bool IsTracked() { return mUser != NULL; }
   
   void SetUserGenerator(ofxUserGenerator* pUserGenerator) { mUserGenerator = pUserGenerator; }
   void SetDepthGenerator(ofxDepthGenerator* pDepthGenerator) { mDepthGenerator = pDepthGenerator; }
   void SetActiveUser(ofxTrackedUser* pUser){mUser = pUser;}
   ofxTrackedUser* GetActiveUser(){return mUser;}
   int GetSkeletonID(){return mSkeletonID;}
   ofxVec3f GetRealWorldPos(XnSkeletonJoint joint, float& confidence) const;
   ofxVec3f GetProjectivePos(XnSkeletonJoint joint) const;

   void UpdateSkeleton(float dt);
   void UpdateClosestHand();
   void PollJointHitDetectors(float dt);

   void Draw();

private:
   ofxUserGenerator* mUserGenerator;
   ofxDepthGenerator* mDepthGenerator;
   ofxTrackedUser* mUser;

   std::vector<JointHitDetector*> mHitDetector;
   JointHitDetector* mClosestHand;
   int mSkeletonID;

};

#endif
