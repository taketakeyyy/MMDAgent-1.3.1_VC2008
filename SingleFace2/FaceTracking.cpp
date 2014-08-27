
//------------------------------------------------------------------------------
// <copyright file="SingleFace.cpp" company="Microsoft">
//     Copyright (c) Microsoft Corporation.  All rights reserved.
// </copyright>
//------------------------------------------------------------------------------

// Defines the entry point for the application.
//

/* definitions */
#define _USE_MATH_DEFINES


#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif /* _WIN32 */

/* headers */
#include <iostream>
#include <math.h>
#include "MMDAgent.h"
#include "BoneController.h"
#include "SingleFace.h"
#include "FaceTracking_Thread.h"
//#include "KinectSensor.h"
#include <list>


/* variables */

typedef struct _ControllerList {
   BoneController head;
   BoneController eye;
   struct _ControllerList *next;
} ControllerList;

static ControllerList *controllerList;
static bool enable;
static bool updating;

int per;
int mousePosX, mousePosY, mousePosZ;
int nowX,nowY,nowZ;
int preX,preY,preZ;

int countlist=0;
std::list<int> intlistx; 
std::list<int> intlisty; 

static FaceTracking_Thread FaceTracking_thread;


/* setHeadController: set bone controller to head */
static void setHeadController(BoneController *controller, PMDModel *model)
{
   const char *bone[] = {"頭"};
   controller->setup(model, bone, 1, 0.150f, 0.008f, 0.0f, 0.0f, 1.0f, 20.0f, 60.0f, 0.0f, -45.0f, -60.0f, 0.0f, 0.0f, -1.0f, 0.0f);
}

/* setEyeController: set eye controller to eyes */
static void setEyeController(BoneController *controller, PMDModel *model)
{
   const char *bone[] = {"右目", "左目"};
   controller->setup(model, bone, 2, 0.180f, 0.008f, 0.0f, 0.0f, 1.0f, 5.0f, 5.0f, 0.0f, -5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f);
}

/* changeLookAt: switch LookAt */
static void changeLookAt(PMDObject *objs, int num, MMDAgent *mmdagent)
{
   int i;
   ControllerList *tmp;

   for(i = 0, tmp = controllerList; i < num; i++) {
      if(objs[i].isEnable() == true && tmp != NULL) {
         if(enable == true) {
            tmp->head.setEnableFlag(false);
            tmp->eye.setEnableFlag(false);
         } else {
            tmp->head.setEnableFlag(true);
            tmp->eye.setEnableFlag(true);
         }
      }
      if(tmp != NULL)
         tmp = tmp->next;
   }
   updating = true;
   enable = !enable;
   if(enable)
      mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, "%s", PLUGINFACE_NAME);
   else
      mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, "%s", PLUGINFACE_NAME);
}



/***** extAppStart: プラグインロード & スレッド開始 *****/
EXPORT void extAppStart(MMDAgent *mmdagent)
{
FaceTracking_thread.setupAndStart(mmdagent);
   controllerList = NULL;
   enable = false;
   updating = false;

}

/***** extProcCommand: コマンドメッセージ受信時処理 *****/
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{

   if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
      if(MMDAgent_strequal(args, PLUGINFACE_NAME) && enable == false)
         changeLookAt(mmdagent->getModelList(), mmdagent->getNumModel(), mmdagent);
   } else if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
      if(MMDAgent_strequal(args, PLUGINFACE_NAME) && enable == true)
         changeLookAt(mmdagent->getModelList(), mmdagent->getNumModel(), mmdagent);
   }


}

/* extProcEvent: process event message */
EXPORT void extProcEvent(MMDAgent *mmdagent, const char *type, const char *args)
{
   int i, id;
   char *p, *buf, *save;
   PMDObject *objs;
   ControllerList *tmp1, *tmp2 = NULL;

   objs = mmdagent->getModelList();
   if(MMDAgent_strequal(type, MMDAGENT_EVENT_KEY)) {
      if(MMDAgent_strequal(args, "U") || MMDAgent_strequal(args, "u")) {

		  init();//kataoka
		  changeLookAt(objs, mmdagent->getNumModel(), mmdagent);

      }
   } else if(MMDAgent_strequal(type, MMDAGENT_EVENT_MODELCHANGE) || MMDAgent_strequal(type, MMDAGENT_EVENT_MODELADD)) {
      buf = MMDAgent_strdup(args);
      p = MMDAgent_strtok(buf, "|", &save);
      if(p) {
         id = mmdagent->findModelAlias(p);
         if(id != -1) {
            for(i = 0, tmp1 = controllerList; i <= id; i++) {
               if(tmp1 == NULL) {
                  tmp1 = new ControllerList;
                  tmp1->next = NULL;
                  if(i == 0)
                     controllerList = tmp1;
                  else
                     tmp2->next = tmp1;
               }
               if(i == id) {
                  setHeadController(&tmp1->head, objs[id].getPMDModel());
                  setEyeController(&tmp1->eye, objs[id].getPMDModel());
                  tmp1->head.setEnableFlag(enable);
                  tmp1->eye.setEnableFlag(enable);
               }
               tmp2 = tmp1;
               tmp1 = tmp1->next;
            }
         }
      }
      if(buf != NULL)
         free(buf);
   }
}



/* extUpdate: update motions */
EXPORT void extUpdate(MMDAgent *mmdagent, double deltaFrame)
{
   int i;
   float rate;
   PMDObject *objs;
   btVector3 targetPos, pointPos;
   int windowWidth, windowHeight;

   ControllerList *tmp;
   bool hasUpdates;

   if (updating == false)
      return;
   if (controllerList == NULL)
      return;

			  aa();//キネクトupdate


   // set target position 
   mmdagent->getWindowSize(&windowWidth, &windowHeight);



 //以下はミミクリ用に何秒か遅らせるやつだよ！消しちゃだめー
   /*
   
   intlistx.push_back(mousePosX);
intlisty.push_back(mousePosY);


mousePosX=*intlistx.begin(); 
mousePosY=*intlisty.begin(); 

   if(countlist>1000){//1000で大体8秒ぐらい。125フレームレート？
	   intlistx.pop_front();
   	   intlisty.pop_front();
   }else{
	   countlist++;
   }
*/

   

nowY=-(cos(rotationz)*sin(rotationx)*cos(rotationy)+sin(rotationz)*sin(rotationy))*1000;
nowX=-(sin(rotationz)*sin(rotationx)*cos(rotationy)-cos(rotationz)*sin(rotationy))*1000;
nowZ=cos(rotationx)*cos(rotationy)*100;//なんかわからんけど、ここ小さくしないと首の動く角度がすごく微妙




   pointPos.setValue(nowX, -nowY+7, nowZ-110);//この位置を見つめるように顔を動かすよ！
   mmdagent->getScreenPointPosition(&targetPos, &pointPos);

   // calculate direction of all controlled bones 
   hasUpdates = false;
   objs = mmdagent->getModelList();
   for (i = 0, tmp = controllerList; i < mmdagent->getNumModel(); i++) {
      if (objs[i].isEnable() == true && tmp != NULL) {
         if (tmp->head.update(&targetPos, (float) deltaFrame) == true)
            hasUpdates = true;
         if (tmp->eye.update(&targetPos, (float) deltaFrame) == true)
            hasUpdates = true;
      }
      if(tmp != NULL)
         tmp = tmp->next;
   }


   if (hasUpdates == false) {
      /* no further update, stop updating */
      updating = false;
   }
   
}



/***** extAppEnd: プラグイン終了 & スレッド開放 *****/


EXPORT void extAppEnd(MMDAgent *mmdagent)
{
	FaceTracking_thread.stopAndRelease();
}

/*


EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   ControllerList *tmp1, *tmp2;

   for(tmp1 = controllerList; tmp1 != NULL; tmp1 = tmp2) {
      tmp2 = tmp1->next;
      delete tmp1;
   }
   controllerList = NULL;
   enable = false;
}
*/


