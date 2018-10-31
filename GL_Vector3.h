#ifndef GL_VECTOR3_H
#define GL_VECTOR3_H

#define PI 3.1415926535897932					// This is our famous PI

#define BEHIND		0
#define INTERSECTS	1
#define FRONT		2


typedef struct _Texcoord      //Œ∆¿ÌΩ·ππ
{
	float x,y;
}Texcoord;

// This is our basic 3D point/vector class
class GL_Vector3
{
public:
	
	// A default constructor
	GL_Vector3() {x=0.0;y=0.0;z=0.0;}

	// This is our constructor that allows us to initialize our data upon creating an instance
	GL_Vector3(float X, float Y, float Z) 
	{ 
		x = X; y = Y; z = Z;
	}

	// Here we overload the + operator so we can add vectors together 
	GL_Vector3 operator+(GL_Vector3 vVector)
	{
		// Return the added vectors result.
		return GL_Vector3(vVector.x + x, vVector.y + y, vVector.z + z);
	}

	GL_Vector3& operator+=(GL_Vector3 vVector)
	{
		// Return the added vectors result.
		x +=vVector.x;
		y +=vVector.y;
		z +=vVector.z;
		return *this;
	}

	// Here we overload the - operator so we can subtract vectors 
	GL_Vector3 operator-(GL_Vector3 vVector)
	{
		// Return the subtracted vectors result
		return GL_Vector3(x - vVector.x, y - vVector.y, z - vVector.z);
	}

	GL_Vector3& operator-=(GL_Vector3 vVector)
	{
		x -=vVector.x;
		y -=vVector.y;
		z -=vVector.z;
		return *this;
	}
	
	// Here we overload the * operator so we can multiply by scalars
	GL_Vector3 operator*(float num)
	{
		// Return the scaled vector
		return GL_Vector3(x * num, y * num, z * num);
	}

	GL_Vector3& operator*=(float num)
	{
		x *=num;
		y *=num;
		z *=num;
		return *this;
	}

	// Here we overload the / operator so we can divide by a scalar
	GL_Vector3 operator/(float num)
	{
		// Return the scale vector
		return GL_Vector3(x / num, y / num, z / num);
	}

	GL_Vector3& operator/=(float num)
	{
		x /=num;
		y /=num;
		z /=num;
		return *this;
	}

	GL_Vector3 operator-()
	{
		return GL_Vector3(-x,-y,-z);
	}

	GL_Vector3& operator=(GL_Vector3 vVector)
	{
		x =vVector.x;
		y =vVector.y;
		z =vVector.z;
		return *this;
	}

	float x, y, z;						

};

//////////////collision////////////////////////
//////////////collision////////////////////////

    GL_Vector3 operator*(float s, GL_Vector3 u);

	// This returns the absolute value of "num"
    float Absolute(float num);

    //	This returns a perpendicular vector from 2 given vectors by taking the cross product.
    GL_Vector3 Cross(GL_Vector3 vVector1, GL_Vector3 vVector2);

    //	This returns the magnitude of a normal (or any other vector)
    float Magnitude(GL_Vector3 vNormal);

    //	This returns a normalize vector (A vector exactly of length 1)
    GL_Vector3 Normalize(GL_Vector3 vNormal);

    //	This returns the normal of a polygon (The direction the polygon is facing)
    GL_Vector3 Normal(GL_Vector3 vPolygon[]);

    // This returns the distance between 2 3D points
    float Distance(GL_Vector3 vPoint1, GL_Vector3 vPoint2);

    // This returns the point on the line segment vA_vB that is closest to point vPoint
    GL_Vector3 ClosestPointOnLine(GL_Vector3 vA, GL_Vector3 vB, GL_Vector3 vPoint);

    // This returns the distance the plane is from the origin (0, 0, 0)
    // It takes the normal to the plane, along with ANY point that lies on the plane (any corner)
    float PlaneDistance(GL_Vector3 Normal, GL_Vector3 Point);

    // This takes a triangle (plane) and line and returns true if they intersected
    bool IntersectedPlane(GL_Vector3 vPoly[], GL_Vector3 vLine[], GL_Vector3 &vNormal, float &originDistance);

    // This returns the dot product between 2 vectors
    float Dot(GL_Vector3 vVector1, GL_Vector3 vVector2);

    // This returns the angle between 2 vectors
    double AngleBetweenVectors(GL_Vector3 Vector1, GL_Vector3 Vector2);

    // This returns an intersection point of a polygon and a line (assuming intersects the plane)
    GL_Vector3 IntersectionPoint(GL_Vector3 vNormal, GL_Vector3 vLine[], double distance);

    // This returns true if the intersection point is inside of the polygon
    bool InsidePolygon(GL_Vector3 vIntersection, GL_Vector3 Poly[], long verticeCount);

    // Use this function to test collision between a line and polygon
    bool IntersectedPolygon(GL_Vector3 vPoly[], GL_Vector3 vLine[], int verticeCount);

    // This function classifies a sphere according to a plane. (BEHIND, in FRONT, or INTERSECTS)
    int ClassifySphere(GL_Vector3 &vCenter, 
				   GL_Vector3 &vNormal, GL_Vector3 &vPoint, float radius, float &distance);

    // This takes in the sphere center, radius, polygon vertices and vertex count.
    // This function is only called if the intersection point failed.  The sphere
    // could still possibly be intersecting the polygon, but on it's edges.
    bool EdgeSphereCollision(GL_Vector3 &vCenter, 
						 GL_Vector3 vPolygon[], int vertexCount, float radius);

    // This returns true if the sphere is intersecting with the polygon.
    bool SpherePolygonCollision(GL_Vector3 vPolygon[], 
							GL_Vector3 &vCenter, int vertexCount, float radius);

///// * //////// * //////// * NEW * ///// * //////// * //////// *

    // This returns the offset the sphere needs to move in order to not intersect the plane
    GL_Vector3 GetCollisionOffset(GL_Vector3 &vNormal, float radius, float distance);

///// * //////// * //////// * NEW * ///// * //////// * //////// *


#endif //GL_VECTOR3_H



