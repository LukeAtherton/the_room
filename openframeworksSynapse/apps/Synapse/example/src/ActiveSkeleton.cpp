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
   assert(TheActiveSkeleton == NULL);
   TheActiveSkeleton = this;
}

ActiveSkeleton::~ActiveSkeleton()
{
   assert(TheActiveSkeleton == this);
   TheActiveSkeleton = NULL;
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
   
   vPos.x = pos[0].X; vPos.y = pos[0].Y; vPos.z = pos[0].Z; 
   return vPos;
}
