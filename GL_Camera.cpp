#include "GL_Camera.h"
#include "GL_Terrain.h"
#include "GL_House.h" 
#include "GL_Plant.h"
#include "GL_Flag.h"
#include <math.h>
#include <windows.h>


GL_Camera::GL_Camera() :m_walkbias(0.0f), m_radius(4.0f), m_gravity(0.1f)
{
	m_Velocity = GL_Vector3(0, m_gravity, 0);
	m_ground = false;
	m_bpressw = false;
	m_bpresss = false;
	m_bpressa = false;
	m_bpressd = false;
	m_bpressq = false;

	//��ʼ�������λ�� ��ʼλ���ڵ��ε�������
	float x = 400;
	float z = -1058;
	//�����������ȷ����GL_Terrain����GL_Camera֮ǰ�ͳ�ʼ����
	float y = GL_Terrain::pTerrain->GetHeight(x, z) + 100;
	PositionCamera(x, y, z, x, y, z - 1, 0, 1, 0);
}

GL_Camera::~GL_Camera()
{
}



void GL_Camera::PositionCamera(float positionX, float positionY, float positionZ,
	float viewX, float viewY, float viewZ,
	float upVectorX, float upVectorY, float upVectorZ)
{
	GL_Vector3 vPosition = GL_Vector3(positionX, positionY, positionZ);
	GL_Vector3 vView = GL_Vector3(viewX, viewY, viewZ);
	GL_Vector3 vUpVector = GL_Vector3(upVectorX, upVectorY, upVectorZ);

	m_vPosition = vPosition;
	m_vView = vView;
	m_vUpVector = vUpVector;
}

void GL_Camera::CheckForViewRotate() {
	POINT mousePos; /* ���浱ǰ���λ�� */
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /* �õ���Ļ��ȵ�һ�� */
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /* �õ���Ļ�߶ȵ�һ�� */
	float angleY = 0.0f; /* �����������ת�Ƕ� */
	float angleZ = 0.0f; /* �����������ת�Ƕ� */
	static float currentRotX = 0.0f;

	/* �õ���ǰ���λ�� */
	GetCursorPos(&mousePos);
	ShowCursor(TRUE);

	/* ������û���ƶ�,���ø��� */
	if ((mousePos.x == middleX) && (mousePos.y == middleY))
		return;
	/* �������λ������Ļ���� */
	SetCursorPos(middleX, middleY);

	/* �õ�����ƶ����� */
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;
	static float lastRotX = 0.0f; /* ���ڱ�����ת�Ƕ� */
	lastRotX = currentRotX;

	/* ���������������ת�Ƕ� */
	currentRotX += angleZ;

	/* ���������ת���ȴ���1.0,���ǽ�ȡ��1.0����ת */
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		/* ���ݱ���ĽǶ���ת���� */
		if (lastRotX != 1.0f)
		{
			/* ͨ������ҵ�����ת����ֱ������ */
			GL_Vector3 vAxis = m_vView - m_vPosition;
			vAxis = Cross(vAxis, m_vUpVector);
			vAxis = Normalize(vAxis);

			//��ת
			RotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/*�����ת����С��-1.0,��Ҳ��ȡ��-1.0����ת */
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if (lastRotX != -1.0f)
		{

			/* ͨ������ҵ�����ת����ֱ������ */
			GL_Vector3 vAxis = m_vView - m_vPosition;
			vAxis = Cross(vAxis, m_vUpVector);
			vAxis = Normalize(vAxis);

			//��ת
			RotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/* �������תangleZ�� */
	else
	{
		/* �ҵ�����ת����ֱ���� */
		GL_Vector3 vAxis = m_vView - m_vPosition;
		vAxis = Cross(vAxis, m_vUpVector);
		vAxis = Normalize(vAxis);

		//��ת
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}
	/* ����������ת����� */
	RotateView(angleY, 0, 1, 0);

}

//��������x, y, z����תangle�Ƕ�
void GL_Camera::RotateView(float angle, float x, float y, float z)
{
	GL_Vector3 vNewView;

	// Get the view vector (The direction we are facing)
	GL_Vector3 vView = m_vView - m_vPosition;

	// Calculate the sine and cosine of the angle once
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	// Find the new x position for the new rotated point
	vNewView.x = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * sinTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * sinTheta)	* vView.z;

	// Find the new y position for the new rotated point
	vNewView.y = ((1 - cosTheta) * x * y + z * sinTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * sinTheta)	* vView.z;

	// Find the new z position for the new rotated point
	vNewView.z = ((1 - cosTheta) * x * z - y * sinTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * sinTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

	// Now we just add the newly rotated vector to our position to set
	// our new rotated view of our camera.
	m_vView = m_vPosition + vNewView;
}

void GL_Camera::StrafeCamera(float speed)
{
	// Initialize a variable for the cross product result
	GL_Vector3 vCross = Cross(m_vView - m_vPosition, m_vUpVector);

	// Normalize the strafe vector
	m_vStrafe = Normalize(vCross);
	// Add the strafe vector to our position
	m_vPosition.x += m_vStrafe.x * speed;
	m_vPosition.z += m_vStrafe.z * speed;

	// Add the strafe vector to our view
	m_vView.x += m_vStrafe.x * speed;
	m_vView.z += m_vStrafe.z * speed;
}

void GL_Camera::MoveCamera(float speed)
{
	// Get the current view vector (the direction we are looking)
	GL_Vector3 vVector = m_vView - m_vPosition;
	vVector = Normalize(vVector);

	m_vPosition.x += vVector.x * speed;		// Add our acceleration to our position's X
	m_vPosition.y += vVector.y * speed;		// Add our acceleration to our position's Y
	m_vPosition.z += vVector.z * speed;		// Add our acceleration to our position's Z
	m_vView.x += vVector.x * speed;			// Add our acceleration to our view's X
	m_vView.y += vVector.y * speed;			// Add our acceleration to our view's Y
	m_vView.z += vVector.z * speed;			// Add our acceleration to our view's Z
}


void GL_Camera::CheckHouseCollision(GL_Vector3 *pVertices, int numOfVerts)
{
	// Go through all the triangles
	for (int i = 0; i < numOfVerts; i += 3)
	{
		// Store of the current triangle we testing
		GL_Vector3 vTriangle[3] = { pVertices[i], pVertices[i + 1], pVertices[i + 2] };

		CheckTriCollision(vTriangle);
	}
}

void GL_Camera::CheckTriCollision(GL_Vector3 vTriangle[])
{
	// 1) STEP ONE - Finding the sphere's classification

	// We want the normal to the current polygon being checked
	GL_Vector3 vNormal = Normal(vTriangle);

	// This will store the distance our sphere is from the plane
	float distance = 0.0f;

	// This is where we determine if the sphere is in FRONT, BEHIND, or INTERSECTS the plane
	int classification = ClassifySphere(m_vPosition, vNormal, vTriangle[0], m_radius, distance);

	// If the sphere intersects the polygon's plane, then we need to check further
	if (classification == INTERSECTS)
	{
		// 2) STEP TWO - Finding the psuedo intersection point on the plane

		// Now we want to project the sphere's center onto the triangle's plane
		GL_Vector3 vOffset = vNormal * distance;

		// Once we have the offset to the plane, we just subtract it from the center
		// of the sphere.  "vIntersection" is now a point that lies on the plane of the triangle.
		GL_Vector3 vIntersection = m_vPosition - vOffset;

		// 3) STEP THREE - Check if the intersection point is inside the triangles perimeter

		if (InsidePolygon(vIntersection, vTriangle, 3) ||
			EdgeSphereCollision(m_vPosition, vTriangle, 3, m_radius / 2))
		{
			// If we get here, we have collided!  To handle the collision detection
			// all it takes is to find how far we need to push the sphere back.
			// GetCollisionOffset() returns us that offset according to the normal,
			// radius, and current distance the center of the sphere is from the plane.
			vOffset = GetCollisionOffset(vNormal, m_radius, distance);

			// Now that we have the offset, we want to ADD it to the position and
			// view vector in our camera.  This pushes us back off of the plane.  We
			// don't see this happening because we check collision before we render
			// the scene.
			m_vPosition = m_vPosition + vOffset;
			m_vView = m_vView + vOffset;

			// Here we make sure that we don't slide slowly down walls when we
			// jump and collide into them.  We only want to say that we are on
			// the ground if we actually have stopped from falling.  A wall wouldn't
			// have a high y value for the normal, it would most likely be 0.
			if (vNormal.y < 0.2f)  //�ܶ���һ���ϰ���
			{
				// We can try and step over the wall we collided with
				m_stepover = true;
			}
			// If the normal of the plane has a Y value of 1, that means it's just the
			// flat ground and we don't need to check if we can step over it, it's flat!
			else  
			{
				//����ĵ����ǹ����ϵĵ���
				//��˼�ǲ���׹���ˣ������ر��ݶ�����������ȵ�
				//ע�⣺�ر��ݶ������ӵ��������ǳ�������ģ����Է�����������
				m_ground = true;
			}
		}
	}
}

void GL_Camera::AABBCheckBorderCollision(GL_Vector3 oldPos, GL_Vector3 oldView)
{
	/*�������ײ���飬ʹ�ӵ�λ�������ڵ�ͼ�ڲ�*/ 
	int tem = 1; // �߽�ȡһ�����浥λ
	/*x�����ж�*/
	if (m_vView.x > (m_TerrainSize - tem)*m_TerrainScale || m_vView.x <  tem*m_TerrainScale)
	{
		m_vPosition.x = oldPos.x;
		m_vView.x = oldView.x;
	}

	/*z�����ж�*/
	if (m_vView.z > -tem*m_TerrainScale || m_vView.z < -(m_TerrainSize - tem)*m_TerrainScale)
	{
		m_vPosition.z = oldPos.z;
		m_vView.z = oldView.z;
	}

	/*Ϊ�¼���ķ���ģ����Ӱ�Χ����ײ����*/
	if (m_vPosition.x > box.minX && m_vPosition.x <  box.maxX && m_vPosition.z >  box.minZ && m_vPosition.z < box.maxZ)
	{
		m_vPosition.x = oldPos.x;
		m_vView.x = oldView.x;
		m_vPosition.z = oldPos.z;
		m_vView.z = oldView.z;
	}
}

void GL_Camera::CheckTerrainCollision()
{
	GL_Vector3 vTriangle[3];
	for (int i = 0; i < m_TerrainSize - 1; i++)
	{
		for (int j = 0; j < m_TerrainSize - 1; j++)
		{
			vTriangle[0] = GL_Terrain::pTerrain->m_TerrainCoord[i*m_TerrainSize + j];
			vTriangle[1] = GL_Terrain::pTerrain->m_TerrainCoord[(i + 1)*m_TerrainSize + j];
			vTriangle[2] = GL_Terrain::pTerrain->m_TerrainCoord[i*m_TerrainSize + j + 1];
			CheckTriCollision(vTriangle);

			vTriangle[0] = GL_Terrain::pTerrain->m_TerrainCoord[i*m_TerrainSize + j + 1];
			vTriangle[1] = GL_Terrain::pTerrain->m_TerrainCoord[(i + 1)*m_TerrainSize + j];
			vTriangle[2] = GL_Terrain::pTerrain->m_TerrainCoord[(i + 1)*m_TerrainSize + j + 1];
			CheckTriCollision(vTriangle);
		}
	}
}

void GL_Camera::TraceSphere()
{
	CheckCollision();
	if (m_ground)
		if (m_Velocity.y < 0)
			m_Velocity.y = m_gravity;
	GL_Vector3 lastpos = m_vPosition;
	GL_Vector3 lastview = m_vView;
}

void GL_Camera::CheckCollision()
{
	m_ground = false;

	CheckHouseCollision(GL_House::pHouse->GetHouseVertex(), 192);
	CheckTerrainCollision();
}


void GL_Camera::CheckForMovement()
{
	GL_Vector3 oldPos, oldView;
	oldPos = m_vPosition;
	oldView = m_vView;

	m_Velocity.y -= m_gravity;   //����ٵģ������ǣ�-=
	if (m_Velocity.y <= -m_radius)  //����Ϊ�˷�ֹ�ٶȹ��죬������ȥ�����
		m_Velocity.y = -m_radius;
	m_vPosition = m_vPosition + m_Velocity;
	m_vView = m_vView + m_Velocity;

	//SHORT GetKeyState��int nVirtKey����
	//����ֵ������ֵ�����˸����������״̬��״̬���£�
	//������λΪ1���������DOWN״̬������ΪUP״̬��
	//������λΪ1�����������������CAPS LOCK����
	m_Speed = 1.5f;

	if (!(GetKeyState('Q') & 0x80))
		m_bpressq = false;

	// Check if we hit the Up arrow or the 'w' key
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {

		// Move our camera forward by a positive SPEED
		MoveCamera(m_Speed);
		m_bpressw = true;
	}
	else
		m_bpressw = false;

	// Check if we hit the Down arrow or the 's' key
	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {

		// Move our camera backward by a negative SPEED
		MoveCamera(-m_Speed);
		m_bpresss = true;
	}
	else
		m_bpresss = false;

	// Check if we hit the Left arrow or the 'a' key
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {

		// Strafe the camera left
		StrafeCamera(-m_Speed);
		m_bpressa = true;
	}
	else
		m_bpressa = false;

	// Check if we hit the Right arrow or the 'd' key
	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {

		// Strafe the camera right
		StrafeCamera(m_Speed);
		m_bpressd = true;
	}
	else
		m_bpressd = false;

	TraceSphere();
	AABBCheckBorderCollision(oldPos, oldView);
}


void GL_Camera::Update()
{
	CheckForViewRotate();

	CheckForMovement();
}


/////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*
///
///	This updates the camera according to the 
///
/////////////// LOOK \\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*

void GL_Camera::Look()
{
	// Give openGL our camera position, then camera view, then camera up vector
	gluLookAt(m_vPosition.x, m_vPosition.y, m_vPosition.z,
		m_vView.x, m_vView.y, m_vView.z,
		m_vUpVector.x, m_vUpVector.y, m_vUpVector.z);
}