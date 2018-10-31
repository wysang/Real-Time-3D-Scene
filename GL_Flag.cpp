#include "GL_Flag.h"
#include "GL_Terrain.h"
#include <math.h>
#include <GL/glut.h>

GL_Flag *GL_Flag::pFlag=NULL;

GL_Flag::GL_Flag():m_flagwidth(16),m_flagheight(20),WindForceFactor(100.0f)
{
    MASSPERFACE=100.0f/((NUMCOLUMNS*NUMROWS) * 2);
	CSTEP      =m_flagwidth  / NUMCOLUMNS;
	RSTEP	   =m_flagheight / NUMROWS   ;
	Initialize();      //旗帜  

	InitFlagData();    //旗杆

	pFlag=this;
}

GL_Flag::~GL_Flag()
{
	for(int i=0;i<3;i++)  //删除纹理对象
	{
		glDeleteTextures(1, &m_flag[i]);  
	}
	glDeleteTextures(1,&m_flagpole);

	delete []height;
	delete []xpos;
	delete []zpos;
}

void GL_Flag::InitFlagData()
{
	FILE *fileflag;
	char oneline[255];
	fileflag = fopen("data/house/flag.txt", "rt");
	Readstr(fileflag,oneline);
	sscanf(oneline, "%d", &numflags);
	//位置 
	height=new float[numflags];
	xpos  =new float[numflags]; 
	zpos  =new float[numflags];     

	for(int m=0;m<numflags;m++)
	{
		Readstr(fileflag,oneline);
        sscanf(oneline,"%f %f %f",&height[m],&xpos[m],&zpos[m]);
	}
	fclose(fileflag);
}

void GL_Flag::InitFlag()
{
	m_texture.MakeTextureBind("data/house/China.bmp", m_flag[0]);
	m_texture.MakeTextureBind("data/house/China.bmp",m_flag[1]);
	m_texture.MakeTextureBind("data/house/China.bmp",m_flag[2]);
	m_texture.MakeTextureBind("data/house/flagpole.bmp",m_flagpole);
}

void GL_Flag::DrawFlag()
{
	glEnable(GL_TEXTURE_2D);
	for (int i=0;i<1;i++)
	{
		float x=xpos[i]*m_TerrainScale,y,z=zpos[i]*m_TerrainScale;
		y=GL_Terrain::pTerrain->GetHeight(x,z)-1+height[i]-m_flagheight;

		// 画旗帜
	    glPushMatrix();
		
		glTranslatef(x, y, z);
		int r, c;	
		
		glBindTexture(GL_TEXTURE_2D, m_flag[i%3]);  //总共3个旗面
        glBegin(GL_TRIANGLE_STRIP);
		for(c=0; c<=NUMCOLUMNS-1; c++)
		{
			for(r=0; r<=NUMROWS; r++)
			{
				float x1=Particles[r][c].vPosition.x;
				float y1=Particles[r][c].vPosition.y;
				float z1=Particles[r][c].vPosition.z;

				float x2=Particles[r][c+1].vPosition.x;
				float y2=Particles[r][c+1].vPosition.y;
				float z2=Particles[r][c+1].vPosition.z;
				
				glTexCoord2f(r/(float)NUMROWS, 1-c/(float)NUMCOLUMNS);
				glVertex3f(x1, y1, z1);
				
				glTexCoord2f(r/(float)NUMROWS, 1-(c+1)/(float)NUMCOLUMNS);
				glVertex3f(x2, y2, z2);
			}
		}
		glEnd();
	    glPopMatrix();

		// 旗杆
		glBindTexture(GL_TEXTURE_2D, m_flagpole);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glPushMatrix();
		glTranslatef(x,GL_Terrain::pTerrain->GetHeight(x,z)-1,z);
			DrawFlagPole(i);
		glPopMatrix();
	} 
	glDisable(GL_TEXTURE_2D);
}

void GL_Flag::DrawFlagPole(int m)
{
	float radius=height[m]/80.0f; 

	float angle=0.0174533f;
	//总共12个点，可以构成一个QUAD_STRIP
	glBegin(GL_QUAD_STRIP);          
	    for(float i=8;i>0;i--)    
		{
			float x=-radius*sinf(i*45*angle); //得到圆周上一点
			float z=-radius*cosf(i*45*angle);
	        glTexCoord2f(i/8.0f,0);  
			glVertex3f(x,0,z);
	        glTexCoord2f(i/8.0f,8);
			glVertex3f(x,height[m],z);
		}
	    glTexCoord2f(0.0f,0); 
		//这个点就是for循环里的第一个点，和起点连起来，形成一周
	    glVertex3f(0,0,-radius);               
	    glTexCoord2f(0.0f,8);
		//这个点就是for循环里的第一个点，和起点连起来，形成一周
		glVertex3f(0,height[m],-radius);   
    glEnd();
}

void GL_Flag::Initialize()
{
	int	r, c;
	float f;
	GL_Vector3			L;
	int				count;

	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{
			// 计算节点质量
			if((r == 0) && (c == 0)) 
				f = 1;
			else if((r == NUMROWS) && (c == 0)) 
				f = 2;
			else if((r == 0) && (c == NUMCOLUMNS)) 
				f = 2;
			else if((r == NUMROWS) && (c == NUMCOLUMNS)) 
				f = 1;
			else if(((r == 0) || (r == NUMROWS)) && ((c != 0) && (c != NUMCOLUMNS))) 
				f = 3;
			else
				f = 6;

			Particles[r][c].fMass = (f * MASSPERFACE) / 3;
			Particles[r][c].fInvMass = 1 / Particles[r][c].fMass;
			
			// 初始节点的位置
			Particles[r][c].vPosition.x = c * CSTEP;
			Particles[r][c].vPosition.y = r * RSTEP;
			Particles[r][c].vPosition.z = 0.0f;

			// 初速度设为零
			Particles[r][c].vVelocity.x = 0.0f;
			Particles[r][c].vVelocity.y = 0.0f;
			Particles[r][c].vVelocity.z = 0.0f;

			Particles[r][c].vAcceleration.x = 0.0f;
			Particles[r][c].vAcceleration.y = 0.0f;
			Particles[r][c].vAcceleration.z = 0.0f;
			
			Particles[r][c].vForces.x = 0.0f;
			Particles[r][c].vForces.y = 0.0f;
			Particles[r][c].vForces.z = 0.0f;
			if(c == 0 )
				Particles[r][c].bLocked = TRUE;
			else
				Particles[r][c].bLocked = FALSE;
		}
	}
	
	// 设定构造的弹簧，连接临近的粒子
	count = 0;
	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{			
			if(c<NUMCOLUMNS)
			{
				StructuralSprings[count].p1.r = r;
				StructuralSprings[count].p1.c = c;
				StructuralSprings[count].p2.r = r;
				StructuralSprings[count].p2.c = c+1;
				StructuralSprings[count].k = SPRINGTENSIONCONSTANT;
				StructuralSprings[count].d = SPRINGDAMPINGCONSTANT;				
				L = Particles[r][c].vPosition - Particles[r][c+1].vPosition;
				StructuralSprings[count].L = Magnitude(L);				
				count++;
			}
			if(r<NUMROWS)
			{
				StructuralSprings[count].p1.r = r;
				StructuralSprings[count].p1.c = c;
				StructuralSprings[count].p2.r = r+1;
				StructuralSprings[count].p2.c = c;
				StructuralSprings[count].k = SPRINGTENSIONCONSTANT;
				StructuralSprings[count].d = SPRINGDAMPINGCONSTANT;				
				L = Particles[r][c].vPosition - Particles[r+1][c].vPosition;
				StructuralSprings[count].L = Magnitude(L);				
				count++;
			}
			if(r<NUMROWS && c<NUMCOLUMNS)
			{
				StructuralSprings[count].p1.r = r;
				StructuralSprings[count].p1.c = c;
				StructuralSprings[count].p2.r = r+1;
				StructuralSprings[count].p2.c = c+1;
				StructuralSprings[count].k = SPRINGSHEARCONSTANT;
				StructuralSprings[count].d = SPRINGDAMPINGCONSTANT;				
				L = Particles[r][c].vPosition - Particles[r+1][c+1].vPosition;
				StructuralSprings[count].L = Magnitude(L);				
				count++;
			}
			if(c>0 && r<NUMROWS)
			{
				StructuralSprings[count].p1.r = r;
				StructuralSprings[count].p1.c = c;
				StructuralSprings[count].p2.r = r+1;
				StructuralSprings[count].p2.c = c-1;
				StructuralSprings[count].k = SPRINGSHEARCONSTANT;
				StructuralSprings[count].d = SPRINGDAMPINGCONSTANT;				
				L = Particles[r][c].vPosition - Particles[r+1][c-1].vPosition;
				StructuralSprings[count].L = Magnitude(L);				
				count++;
			}
		}
	}
	//风向
	WindVector.x = 1.0;
	WindVector.y = 1.0;
	WindVector.z = 1.0;

}

/*计算弹簧质点所受到的力*/
void GL_Flag::CalcForces(Particle particles[NUMROWS+1][NUMCOLUMNS+1])
{
	int	r, c, i, r1, c1, r2, c2;
	GL_Vector3	dragVector;
	GL_Vector3	f1, f2, d, v;
	float	L;

	// 所有的力设为零
	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{				
			particles[r][c].vForces.x = 0;
			particles[r][c].vForces.y = 0;
			particles[r][c].vForces.z = 0;
		}
	}

	// 计算重力和阻力
	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{				
			
			if(particles[r][c].bLocked == FALSE)
			{
				// 重力
				particles[r][c].vForces.y +=  (GRAVITY * particles[r][c].fMass);

				// 阻力
				dragVector = -particles[r][c].vVelocity;
				dragVector = Normalize(dragVector);
				particles[r][c].vForces += dragVector * (Magnitude(particles[r][c].vVelocity)*Magnitude(particles[r][c].vVelocity)) * DRAGCOEFFICIENT;

				// 设定风向
				SetWindVector((float)tb_Rnd(0, 10), 0, (float)tb_Rnd(0, 1));
				WindVector=Normalize(WindVector);

				//向量加法获得力
				particles[r][c].vForces += WindVector * (float)tb_Rnd(0, (int)WindForceFactor);			
			}
		}
	}

	// 计算弹簧拉力
	for(i = 0; i<NUMSTRUCTURALSPRINGS; i++)
	{
		r1 = StructuralSprings[i].p1.r;
		c1 = StructuralSprings[i].p1.c;
		r2 = StructuralSprings[i].p2.r;
		c2 = StructuralSprings[i].p2.c;

		d = particles[r1][c1].vPosition - particles[r2][c2].vPosition;
		v = particles[r1][c1].vVelocity - particles[r2][c2].vVelocity;
		L = StructuralSprings[i].L;

		f1 = -(StructuralSprings[i].k * (Magnitude(d) - L) + StructuralSprings[i].d * ( Dot(v,d) / Magnitude(d) )) * ( d / Magnitude(d) );
		f2 = -f1;
		
		if(particles[r1][c1].bLocked == FALSE)
			particles[r1][c1].vForces += f1;

		if(particles[r2][c2].bLocked == FALSE)
			particles[r2][c2].vForces += f2;
	}
}

// 生成规定范围内的随机数
int GL_Flag::tb_Rnd(int min, int max)
{
	int	number;		
    number = (((abs(rand())%(max-min+1))+min));    
    
    if(number>max) {
    	number = max;
    }
    
    if(number<min) {
    	number = min;
    }	
		
	return number;
}

//风向设置
void GL_Flag::SetWindVector(float x, float y, float z)
{
	WindVector.x = x;
	WindVector.y = y;
	WindVector.z = z;
}

//模拟旗帜上的质点受力运动
void GL_Flag::StepSimulation(float dt)
{
	GL_Vector3	Ae;
	int		r, c;
	int		check = 0;

	// 计算所有的力 
	CalcForces(Particles);

	// 积分
	for(r=0; r<=NUMROWS; r++)
	{
		for(c=0; c<=NUMCOLUMNS; c++)
		{				
			Ae = Particles[r][c].vForces * Particles[r][c].fInvMass;
			Particles[r][c].vAcceleration = Ae;
			Particles[r][c].vVelocity += Ae * dt;
			Particles[r][c].vPosition += Particles[r][c].vVelocity * dt;
		}
	}
}

void GL_Flag::Update()
{
	StepSimulation(0.020f);
}
