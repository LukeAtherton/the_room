/*
 *  JointHitDetector.h
 *  emptyExample
 *
 *  Created by Ryan Challinor on 5/17/11.
 *  Copyright 2011 aww bees. All rights reserved.
 *
 */

#ifndef JointHitDetector_Included
#define JointHitDetector_Included

#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "UDPMessenger.h"
#include "ActiveSkeleton.h"

class ActiveSkeleton;

class JointHitDetector : public IMessageReceiver
{
public:
   JointHitDetector(ActiveSkeleton* skeleton, XnSkeletonJoint joint, XnSkeletonJoint refJoint, string name, float requiredLength = 150);
   ~JointHitDetector();
   
   void Poll(float dt);
   void Draw() const;
   
   void SetJoint(XnSkeletonJoint joint);
   void OnMessage(const ofxOscMessage& msg);
   
private:
   enum HitDirection
   {
      kHitForward,
      kHitUp,
      kHitRight,
      kHitDown,
      kHitLeft,
      kHitBack,
      kNumHitDirections
   };
   
   bool DetectHit(ofxVec3f& vDir);
   bool LongEnough(const ofxVec3f& vec) const;
   void DrawHitDirection(HitDirection hitDirection) const;
   bool MatchesDir(const ofxVec3f& vMatchDir, const ofxVec3f& vCheckDir, const ofxVec3f& vRefDir) const;
   void BroadcastTuningInfo();
   bool NeedDistFromRef() const { return mJoint == XN_SKEL_RIGHT_HAND || mJoint == XN_SKEL_LEFT_HAND; }
   
   XnSkeletonJoint mJoint;
   XnSkeletonJoint mRefJoint;
   std::vector<ofxVec3f> mPoints;
   float mHitAreaDisplay[kNumHitDirections];
   int mPointHistorySize;
   float mRequiredLength;
   string mName;
   float mMessageWorldJointPos;
   float mMessageBodyJointPos;
   float mMessageScreenJointPos;
   float mTimer;

   ActiveSkeleton* mSkeleton;
};

#endif