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

	//初始化相机的位置 初始位置在地形的中心内
	float x = 400;
	float z = -1058;
	//下面的这句必须确保类GL_Terrain在类GL_Camera之前就初始化了
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
	POINT mousePos; /* 保存当前鼠标位置 */
	int middleX = GetSystemMetrics(SM_CXSCREEN) >> 1; /* 得到屏幕宽度的一半 */
	int middleY = GetSystemMetrics(SM_CYSCREEN) >> 1; /* 得到屏幕高度的一半 */
	float angleY = 0.0f; /* 摄像机左右旋转角度 */
	float angleZ = 0.0f; /* 摄像机上下旋转角度 */
	static float currentRotX = 0.0f;

	/* 得到当前鼠标位置 */
	GetCursorPos(&mousePos);
	ShowCursor(TRUE);

	/* 如果鼠标没有移动,则不用更新 */
	if ((mousePos.x == middleX) && (mousePos.y == middleY))
		return;
	/* 设置鼠标位置在屏幕中心 */
	SetCursorPos(middleX, middleY);

	/* 得到鼠标移动方向 */
	angleY = (float)((middleX - mousePos.x)) / 1000.0f;
	angleZ = (float)((middleY - mousePos.y)) / 1000.0f;
	static float lastRotX = 0.0f; /* 用于保存旋转角度 */
	lastRotX = currentRotX;

	/* 跟踪摄像机上下旋转角度 */
	currentRotX += angleZ;

	/* 如果上下旋转弧度大于1.0,我们截取到1.0并旋转 */
	if (currentRotX > 1.0f)
	{
		currentRotX = 1.0f;

		/* 根据保存的角度旋转方向 */
		if (lastRotX != 1.0f)
		{
			/* 通过叉积找到与旋转方向垂直的向量 */
			GL_Vector3 vAxis = m_vView - m_vPosition;
			vAxis = Cross(vAxis, m_vUpVector);
			vAxis = Normalize(vAxis);

			//旋转
			RotateView(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/*如果旋转弧度小于-1.0,则也截取到-1.0并旋转 */
	else if (currentRotX < -1.0f)
	{
		currentRotX = -1.0f;

		if (lastRotX != -1.0f)
		{

			/* 通过叉积找到与旋转方向垂直的向量 */
			GL_Vector3 vAxis = m_vView - m_vPosition;
			vAxis = Cross(vAxis, m_vUpVector);
			vAxis = Normalize(vAxis);

			//旋转
			RotateView(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	/* 否则就旋转angleZ度 */
	else
	{
		/* 找到与旋转方向垂直向量 */
		GL_Vector3 vAxis = m_vView - m_vPosition;
		vAxis = Cross(vAxis, m_vUpVector);
		vAxis = Normalize(vAxis);

		//旋转
		RotateView(angleZ, vAxis.x, vAxis.y, vAxis.z);
	}
	/* 总是左右旋转摄像机 */
	RotateView(angleY, 0, 1, 0);

}

//绕向量（x, y, z）旋转angle角度
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
			if (vNormal.y < 0.2f)  //很陡的一个障碍物
			{
				// We can try and step over the wall we collided with
				m_stepover = true;
			}
			// If the normal of the plane has a Y value of 1, that means it's just the
			// flat ground and we don't need to check if we can step over it, it's flat!
			else  
			{
				//这里的地面是广义上的地面
				//意思是不用坠落了，包括地表，屋顶，箱子上面等等
				//注意：地表，屋顶，箱子的正方向都是朝向外面的，所以法向量是正数
				m_ground = true;
			}
		}
	}
}

void GL_Camera::AABBCheckBorderCollision(GL_Vector3 oldPos, GL_Vector3 oldView)
{
	/*反向的碰撞检验，使视点位置限制在地图内部*/ 
	int tem = 1; // 边界取一个地面单位
	/*x方向判断*/
	if (m_vView.x > (m_TerrainSize - tem)*m_TerrainScale || m_vView.x <  tem*m_TerrainScale)
	{
		m_vPosition.x = oldPos.x;
		m_vView.x = oldView.x;
	}

	/*z方向判断*/
	if (m_vView.z > -tem*m_TerrainScale || m_vView.z < -(m_TerrainSize - tem)*m_TerrainScale)
	{
		m_vPosition.z = oldPos.z;
		m_vView.z = oldView.z;
	}

	/*为新加入的房子模型添加包围盒碰撞检验*/
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

	m_Velocity.y -= m_gravity;   //会加速的，所以是：-=
	if (m_Velocity.y <= -m_radius)  //这是为了防止速度过快，而穿过去的情况
		m_Velocity.y = -m_radius;
	m_vPosition = m_vPosition + m_Velocity;
	m_vView = m_vView + m_Velocity;

	//SHORT GetKeyState（int nVirtKey）；
	//返回值：返回值给出了给定虚拟键的状态，状态如下：
	//若高序位为1，则键处于DOWN状态，否则为UP状态。
	//若低序位为1，则键被触发。例如CAPS LOCK键。
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