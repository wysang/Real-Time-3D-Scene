#ifndef GL_CAMERA_H
#define GL_CAMERA_H

#include "GL_Vector3.h"

class GL_Camera 
{
public:
	GL_Camera();	
	~GL_Camera();
    float GetSpeed(){ return m_Speed;}
	// These are data access functions for our camera's private data
	GL_Vector3 Position()   {	return m_vPosition;		}
	GL_Vector3 View()		{	return m_vView;			}
	GL_Vector3 UpVector()   {	return m_vUpVector;		}
	GL_Vector3 Strafe()	    {	return m_vStrafe;		}
	
	// This changes the position, view, and up vector of the camera.
	// This is primarily used for initialization
	void PositionCamera(float positionX, float positionY, float positionZ,
			 		    float viewX,     float viewY,     float viewZ,
						float upVectorX, float upVectorY, float upVectorZ);

	// 每帧都要条用update函数
	void Update();

	// 调用glulookat的函数
	void Look();

	struct AABBbox {
		float maxX = 532.0;
		float minX = 409.0;
		float maxZ = -138.0;
		float minZ = -261.0;
	};
	AABBbox box;

private:

	//初始化视点半径
	void SetCameraRadius(float radius) {	m_radius = radius;	};

	//绕向量（x, y, z）旋转angle角度
	void RotateView(float angle, float X, float Y, float Z);

	// This moves the camera's view by the mouse movements (First person view)
	void CheckForViewRotate();

	// This strafes the camera left or right depending on the speed (+/-) 
	void StrafeCamera(float speed);

	// This will move the camera forward or backward depending on the speed
	void MoveCamera(float speed);

	// This checks for keyboard movement
	void CheckForMovement();
	void CheckHouseCollision(GL_Vector3 *pVertices, int numOfVerts);
    void CheckTerrainCollision();

	void AABBCheckBorderCollision(GL_Vector3 oldPos,GL_Vector3 oldView);
    void CheckTriCollision(GL_Vector3 vTriangle[]);
    // This traces a sphere along a ray to check for collision with the polygons
	void TraceSphere();

	// This checks to see if we can step up over a collision (like a step)
	void TryToStep(GL_Vector3 lastpos,GL_Vector3 lastview);

    void CheckCollision();
private:					
	// 视点坐标
	GL_Vector3 m_vPosition;

	// 视角方向
	GL_Vector3 m_vView;						

	// 视点向上的向量
	GL_Vector3 m_vUpVector;		
	
	// The camera's strafe vector
	// 相机的水平向量，一般可由视点向量与相机向上的向量之间的叉积得到。
	GL_Vector3 m_vStrafe;						

    float      m_Speed;       //相机移动速度
	float      m_walkbias;    //行走时的晃动

	// 视点球体
	float m_radius;	
	// 视点速度
	GL_Vector3 m_Velocity;
	bool       m_ground;      //是否到达地面
	bool       m_stepover;    //是否能越过障碍

	float      m_gravity;     //重力

	bool       m_bpressw;     //是否按下w键
	bool       m_bpresss;     //是否按下s键
	bool       m_bpressa;     //是否按下a键
	bool       m_bpressd;     //是否按下d键
	bool       m_bpressq;	  //是否按下q键
};

#endif    // GL_CAMERA_H
