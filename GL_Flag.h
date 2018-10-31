#ifndef GL_FLAG_H
#define GL_FLAG_H
#include "GL_Texture.h"
#include "GL_Vector3.h"

typedef	struct _Particle {//�������
	float	fMass;
	float	fInvMass;
	GL_Vector3	vPosition;
	GL_Vector3	vVelocity;
	GL_Vector3	vAcceleration;
	GL_Vector3	vForces;
	//GL_Vector3  vNormal;
	BOOL	bLocked;
} Particle;

typedef struct _ParticleRef {
	int	r;		// row index
	int c;		// column index
} ParticleRef;

typedef	struct	_Spring {
	ParticleRef		p1;			// reference to connected particle #1
	ParticleRef		p2;			// reference to connected particle #2
	float	k;					// tensile spring constant
	float	d;					// damping factor	
	float	L;					// rest length of spring
} Spring;

const int       NUMCOLUMNS            =10;   //�������񣬽����ӷֳ�10��7�е�����
const int       NUMROWS               =7;
const int       NUMSTRUCTURALSPRINGS  =NUMCOLUMNS*(NUMROWS+1) + NUMROWS*(NUMCOLUMNS+1) + NUMCOLUMNS*NUMROWS*2;

const float		GRAVITY		          =	-32.174f;
const float		SPRINGTENSIONCONSTANT =	600.0f  ;  //����ϵ��  t
const float		SPRINGSHEARCONSTANT	  =	600.0f  ;  //          s  
const float		SPRINGDAMPINGCONSTANT =	2.0f    ;  //          d
const float		DRAGCOEFFICIENT		  =	0.01f   ;

class GL_Flag
{
public:
	GL_Flag();
	~GL_Flag();
	void InitFlag();
	void Update();
	void DrawFlag();
	void SetFlagSize(float width,float height){m_flagwidth=width;m_flagheight=height;}
	///////////////////
    static GL_Flag *pFlag;
	int    GetNumber(){return numflags;}
	float  *GetHeight(){return height;}
	float  *GetXpos  (){return xpos;}
	float  *GetZpos  (){return zpos;}

private:
	void InitFlagData();                  //��ʼ����������
	
	void DrawFlagPole(int m);
	
    //////////////
	void	Initialize();
	// calcs total forces and moments
    void	CalcForces(Particle particles[NUMROWS+1][NUMCOLUMNS+1]);			
	int     tb_Rnd(int min, int max);
    void	StepSimulation(float dt);			// step dt time in the simulation
    void	SetWindVector(float x, float y, float z);


private:
	GL_Texture  m_texture;   //����
    GLuint	m_flag[3];	     // ��������
	GLuint  m_flagpole;      //�������

	int   numflags;         //���ĵĸ���
	float *height;          //���ĵĸ߶�
	float *xpos;            //λ�� 
	float *zpos;

    /////////////////

	float               m_flagwidth;    //���ĵĿ��
	float               m_flagheight;   //���ĵĸ߶�

	Particle			Particles[NUMROWS+1][NUMCOLUMNS+1];
    Spring				StructuralSprings[NUMSTRUCTURALSPRINGS];
    GL_Vector3			WindVector;
    float				WindForceFactor;

    float               MASSPERFACE;
	float               CSTEP;
	float               RSTEP;
};

#endif  //GL_FLAG_H