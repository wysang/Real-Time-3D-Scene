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

	// ÿ֡��Ҫ����update����
	void Update();

	// ����glulookat�ĺ���
	void Look();

	struct AABBbox {
		float maxX = 532.0;
		float minX = 409.0;
		float maxZ = -138.0;
		float minZ = -261.0;
	};
	AABBbox box;

private:

	//��ʼ���ӵ�뾶
	void SetCameraRadius(float radius) {	m_radius = radius;	};

	//��������x, y, z����תangle�Ƕ�
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
	// �ӵ�����
	GL_Vector3 m_vPosition;

	// �ӽǷ���
	GL_Vector3 m_vView;						

	// �ӵ����ϵ�����
	GL_Vector3 m_vUpVector;		
	
	// The camera's strafe vector
	// �����ˮƽ������һ������ӵ�������������ϵ�����֮��Ĳ���õ���
	GL_Vector3 m_vStrafe;						

    float      m_Speed;       //����ƶ��ٶ�
	float      m_walkbias;    //����ʱ�Ļζ�

	// �ӵ�����
	float m_radius;	
	// �ӵ��ٶ�
	GL_Vector3 m_Velocity;
	bool       m_ground;      //�Ƿ񵽴����
	bool       m_stepover;    //�Ƿ���Խ���ϰ�

	float      m_gravity;     //����

	bool       m_bpressw;     //�Ƿ���w��
	bool       m_bpresss;     //�Ƿ���s��
	bool       m_bpressa;     //�Ƿ���a��
	bool       m_bpressd;     //�Ƿ���d��
	bool       m_bpressq;	  //�Ƿ���q��
};

#endif    // GL_CAMERA_H
