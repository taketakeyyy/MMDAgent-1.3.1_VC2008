
#include "KinectSensor.h"
 
#include <stdlib.h>
#include "SingleFace.h"
 
KinectSensor kinect;

	float rotationx;
	float rotationy;
	float rotationz;
		int per3=0;
float		    m_ReportedPitchAverage = 0;
float    m_ReportedYawAverage = 0;
float    m_ReportedRollAverage = 0;
int			m_SamePositionCount=0;


void init(void)//initializeは初めてuが押された時
{ 
    kinect.initialize();
}



int aa()//update
{
		if (per3==10){//pluginのupdate毎に更新するのはfacetrackingが間に合わないから何回かに1回だけ。
			kinect.update();
			per3=0;
		}

	per3++;

	if(per3==1){

    if ( kinect.IsFaceTracked() ) {

        FLOAT scale = 0.0f;
        FLOAT rotation[3] = { 0 };
        FLOAT transration[3] = { 0 };
        kinect.getResult()->Get3DPose( &scale, rotation, transration );//顔の大きさ？と向き、位置、とったどー！
//顔の大きさ？と位置は使ってないよ

		//こっから顔方向のスムージング
        float smoothingFactor = 1.0f;
        m_SamePositionCount++;
        smoothingFactor /= m_SamePositionCount;
        smoothingFactor = max(smoothingFactor, 0.002f);

        m_ReportedPitchAverage += smoothingFactor*(rotation[0]-m_ReportedPitchAverage);
        m_ReportedYawAverage += smoothingFactor*(-rotation[1]-m_ReportedYawAverage);
        m_ReportedRollAverage += smoothingFactor*(rotation[2]-m_ReportedRollAverage);

    rotationx = (rotation[0]-m_ReportedPitchAverage)/180.0f;//Pitch
    rotationy = (-rotation[1]-m_ReportedYawAverage)/180.0f;//Yaw
    rotationz = (rotation[2]-m_ReportedRollAverage)/180.0f;//Roll
//ここまでスムージング

	}

/*
if (rotationx==0 && rotationy==0 && rotationz==0)
{
			rotationx=-1000;
		rotationy=-1000;
		rotationz=-1000;

}
*/
	}
	
	
	return 0;
}


