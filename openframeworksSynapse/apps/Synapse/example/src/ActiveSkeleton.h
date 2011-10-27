/*
 *  ActiveSkeleton.h
 *  emptyExample
 *
 *  Created by Ryan Challinor on 5/18/11.
 *  Copyright 2011 aww bees. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxVec3f.h"

class ActiveSkeleton
{
public:
   ActiveSkeleton();
   ~ActiveSkeleton();
   
   bool IsTracked() { return mUser != NULL; }
   
   void SetUserGenerator(ofxUserGenerator* pUserGenerator) { mUserGenerator = pUserGenerator; }
   void SetDepthGenerator(ofxDepthGenerator* pDepthGenerator) { mDepthGenerator = pDepthGenerator; }
   void SetActiveUser(ofxTrackedUser* pUser) { mUser = pUser; }
      
   ofxVec3f GetRealWorldPos(XnSkeletonJoint joint, float& confidence) const;
   ofxVec3f GetProjectivePos(XnSkeletonJoint joint) const;
private:
   ofxUserGenerator* mUserGenerator;
   ofxDepthGenerator* mDepthGenerator;
   ofxTrackedUser* mUser;
};

extern ActiveSkeleton* TheActiveSkeleton;

