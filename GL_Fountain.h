/*#ifndef GL_FOUNTAIN_H
#define GL_FOUNTAIN_H

#include "GL_Vector3.h"
#include "GL_Pool.h"
#include "GL_Texture.h"
#include "GL_Terrain.h"
/********
	When a fountain has many drops, a whole pool area is often put so strongly 
    down that it is below the bowl and becomes invisible! This is unrealistic 
	anyway, so the "AffectOscillator" method does not allow to put an oscillator
	too deep. The corresponding line of code should be replaced when using
	the pool for other purposes.
*******/


/**

  "GL_Fountain" is the description for the fountain's water in the air.
  The rest, which is most of the water, resists in the bowl.

**/


//Constants:
/*#define NUM_X_OSCILLATORS		21
#define NUM_Z_OSCILLATORS		21
#define OSCILLATOR_DISTANCE		0.12
#define OSCILLATOR_WEIGHT       0.0001

#define MAXX					(NUM_X_OSCILLATORS*OSCILLATOR_DISTANCE)
#define MAXZ					(NUM_Z_OSCILLATORS*OSCILLATOR_DISTANCE)

#define POOL_HEIGHT				0.3

class GL_Fountain;

class CDrop
{
private:
	GLfloat time;  //How many steps the drop was "outside", when it falls into the water, time is set back to 0
	GL_Vector3 ConstantSpeed;  //See the fountain doc for explanation of the physics
	GLfloat AccFactor;
public:
	void SetConstantSpeed (GL_Vector3 NewSpeed);
	void SetAccFactor(GLfloat NewAccFactor);
	void SetTime(GLfloat NewTime);
	void GetNewPosition(GL_Vector3 * PositionVertex, float dtime, GL_Fountain * pAirFountain); 
};

class GL_Fountain
{
public:
	GL_Fountain();
	~GL_Fountain();
	void InitFountain();
	void Draw();
	void Update(float dtime);
public:
	GL_Vector3 Position;
    GL_Pool    m_Pool; 
private:
	
	void Initialize(GLint Steps, GLint RaysPerStep, GLint DropsPerRay, 
					GLfloat AngleOfDeepestStep, 
					GLfloat AngleOfHighestStep,
					GLfloat RandomAngleAddition,
					GLfloat AccFactor);
	void RenderBowl();  //喷泉池
	void RenderFountain();
private:
	GL_Vector3 * FountainVertices;
	CDrop * FountainDrops;
	GLint m_NumDropsComplete;
	
	GL_Texture m_texture;
	GLuint     m_tex[2];

	int   numfountains;     //喷泉的个数
	float *xpos;            //位置 
	float *zpos;

	//GLfloat LightAmbient[4];
	//GLfloat LightDiffuse[4];
	//GLfloat LightPosition[4];
};

#endif    //GL_FOUNTAIN_H*/