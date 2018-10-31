#include "GL_Plant.h"
#include "GL_Terrain.h"
#include <math.h>

GL_Plant *GL_Plant::pPlant=NULL;

GL_Plant::GL_Plant():m_radiusTree(1.0f),m_height(100.f),m_branchWidth(100.f)
{
	InitPlantData();
	pPlant=this;
}

GL_Plant::~GL_Plant()
{
	for(int i=0;i<3;i++)  //删除纹理对象
	{
		glDeleteTextures(1, &m_treeBody[i]);
		glDeleteTextures(1, &m_treeBranch[i]);   
	}

	delete []height;
	delete []type;
	delete []xpos;
	delete []zpos;
	delete []detail;
}

// 载入纹理与树的信息

bool GL_Plant::InitPlant()
{
	//树干纹理
	if(!m_texture.MakeTextureBind("data/plants/treebody0.bmp",m_treeBody[0]))
		return false;  //pine
	if(!m_texture.MakeTextureBind("data/plants/treebody1.bmp",m_treeBody[1]))
		return false;  //poplar
	if(!m_texture.MakeTextureBind("data/plants/treebody0.bmp",m_treeBody[2]))
		return false;  //.........
	//树枝纹理
	if(!m_texture.MakeAlphaTextureBind("data/plants/summer/branch0.bmp","data/plants/summer/branchAlpha0.bmp",m_treeBranch[0],unsigned char(50)))
		return false; //pine
	if(!m_texture.MakeAlphaTextureBind("data/plants/summer/branch1.bmp","data/plants/summer/branchAlpha1.bmp",m_treeBranch[1],unsigned char(100)))
		return false; //poplar
	if(!m_texture.MakeAlphaTextureBind("data/plants/summer/branch2.bmp","data/plants/summer/branchAlpha2.bmp",m_treeBranch[2],unsigned char(50)))
		return false; //maple

	return true;
}

void GL_Plant::InitPlantData()
{
	FILE *filein;
	char oneline[255];
	filein = fopen("data/plants/plant.txt", "rt");
	Readstr(filein,oneline);
	sscanf(oneline, "%d", &numtrees);
	height=new float[numtrees];
	type  =new int  [numtrees];           //树的类型
	xpos  =new float[numtrees];           //位置 
	zpos  =new float[numtrees];     
	detail=new int  [numtrees];           //树的细节

	for(int m=0;m<numtrees;m++)
	{
		Readstr(filein,oneline);
        sscanf(oneline,"%f %d %f %f %d",
			   &height[m],&type[m],&xpos[m],&zpos[m],&detail[m]);
	}
	fclose(filein);
}

void texture_colorkey()
{
	GLint width, height;
	GLubyte* pixels = 0;

	// 获得纹理的大小信息
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	// 分配空间并获得纹理像素
	pixels = (GLubyte*)malloc(width*height * 4);
	if (pixels == 0)
		return;
	glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);

	// 修改像素中的Alpha值
	// 其中pixels[i*4], pixels[i*4+1], pixels[i*4+2], pixels[i*4+3]
	//    分别表示第i个像素的蓝、绿、红、Alpha四种分量，0表示最小，255表示最大
	{
		GLint i;
		GLint count = width * height;
		for (i = 0; i<count; ++i)
		{
			if (pixels[i * 4] <= 10
				&& pixels[i * 4 + 1] <= 10
				&& pixels[i * 4 + 2] <= 10)
				pixels[i * 4 + 3] = 0;
			else
				pixels[i * 4 + 3] = 255;
		}
	}

	// 将修改后的像素重新设置到纹理中，释放内存
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_BGRA_EXT, GL_UNSIGNED_BYTE, pixels);
	free(pixels);
}

void GL_Plant::RenderAll(double cx, double cz)
{
	glEnable(GL_TEXTURE_2D);
	DrawTree(0, 270, -1059, 0,cx,cz);
	glDisable(GL_TEXTURE_2D);
}

//画一棵树 i==0对应pine  i==1对应poplar  i==2对应maple
//(x,z)为树的位置，j为树枝的茂盛程度，j==0 很茂盛，j==1 一般，j==2  枝叶很少
void GL_Plant::DrawTree(int i,float x,float z,int j, double cx, double cz)
{
	glPushMatrix();
	//这里减1为了解决悬空问题
	glTranslatef(x,(GL_Terrain::pTerrain->GetHeight(x,z))-1,z); 
    glBindTexture(GL_TEXTURE_2D, m_treeBody[i]);
	RenderTreebody();
	
	glEnable(GL_ALPHA_TEST);   
	//billboard方法画树
	glAlphaFunc(GL_GREATER , 0.1f);
    //对于非透明的部分，以全亮度绘制，否则效果不好
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, m_treeBranch[i]);
	RenderBranch(i,j,x,z,cx,cz);
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
	glPopMatrix();
}


// 绘制树身
void GL_Plant::RenderTreebody()
{   
	float angle=0.0174533f;
	//总共12个点，可以构成一个QUAD_STRIP，当然不一定是12个点，想更精细一点的话
	//就设很多个点，如可以是16个点，对应的i<8 ,角度为45度
	glBegin(GL_QUAD_STRIP);          
	    for(float i=8;i>0;i--)    
		{
			float x=-m_radiusTree*sinf(i*45*angle); //得到圆周上一点
			float z=-m_radiusTree*cosf(i*45*angle);
	        glTexCoord2f(i*0.1f,0);  
			glVertex3f(x,0,z);
	        glTexCoord2f(i*0.1f+0.1f,8);
			glVertex3f(x*0.15f,m_height,z*0.15f);
		}
	    glTexCoord2f(0.0f,0); 
		//这个点就是for循环里的第一个点，和起点连起来，形成一周
	    glVertex3f(0,0,-m_radiusTree);               
	    glTexCoord2f(0.1f,8);
		//这个点就是for循环里的第一个点，和起点连起来，形成一周
		glVertex3f(0,m_height,-m_radiusTree*0.15f);   
    glEnd();
}


//绘制树枝
void GL_Plant::RenderBranch(int i,int j,double x,double z, double cx, double cz)
{
	glPushMatrix();	
    //这里需要注意的是：下面的平移旋转矩阵是互相影响的
	//要想独立的话，每个树枝都要用glPushMatrix()和glPopMatrix()给包起来
	switch (i)
	{
	case 0: 
		glTranslatef(0, m_height*0.431f, 0);
		glRotatef(0, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.2f);

		glTranslatef(0, m_height*0.03f, 0);
		glRotatef(170, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.35f);

		glTranslatef(0, m_height*0.03f, 0);
		glRotatef(80, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.38f);

		glTranslatef(0, m_height*0.025f, 0);
		glRotatef(130, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.38f);

		glTranslatef(0, m_height*0.03f, 0);
		glRotatef(170, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.3f);

		glTranslatef(0, m_height*0.03f, 0);
		glRotatef(80, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.32f);

		glTranslatef(0, m_height*0.03f, 0);
		glRotatef(130, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.3f);

		glTranslatef(0, m_height*0.027f, 0);
		glRotatef(200, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.3f);

		glTranslatef(0, m_height*0.027f, 0);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.2f);

		glTranslatef(0, m_height*0.04f, 0);
		glRotatef(150, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.28f);

		glTranslatef(0, m_height*0.041f, 0);
		glRotatef(80, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.28f);

		glTranslatef(0, m_height*0.041f, 0);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.3f);

		glTranslatef(0, m_height*0.027f, 0);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.18f);

		glTranslatef(0, m_height*0.04f, 0);
		glRotatef(150, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.25f);

		glTranslatef(0, m_height*0.04f, 0);
		glRotatef(80, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.22f);

		glTranslatef(0, m_height*0.041f, 0);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.2f);

		glTranslatef(0, m_height*0.03f, 0);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.18f);

		glTranslatef(0, m_height*0.02f, 0);
		glRotatef(90, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.1f);

		glTranslatef(0, m_height*0.02f, 0);
		glRotatef(180, 0.0f, 1.0f, 0.0f);
		RenderPineBranch(m_height*0.14f);
		break;
	}
   
	glPopMatrix();
}

//松树枝
void GL_Plant::RenderPineBranch(float width)
{
	glBegin(GL_TRIANGLE_STRIP);
							
	    glTexCoord2f(0,0);
	    glVertex3f(width*0.5f,-width*0.4f,0.7071f*width);

	    glTexCoord2f(0,1);
	    glVertex3f(width,-width*0.1f,0);

	    glTexCoord2f(1,0);
	    glVertex3f(0,0,0);

	    glTexCoord2f(1,1);
	    glVertex3f(width*0.5f,-width*0.4f,-0.7071f*width);

	glEnd();
}


void GL_Plant::DrawTop(float width,float height)
{
	glBegin(GL_QUADS);
	    glTexCoord2f(1,0);
	    glVertex3f(0,0,0);

	    glTexCoord2f(0,0);
	    glVertex3f(0.7071f*width,0.5f*height,0);

	    glTexCoord2f(0,1);
	    glVertex3f(0,height,0);

	    glTexCoord2f(1,1);
	    glVertex3f(-0.7071f*width,0.5f*height,0);

	    glTexCoord2f(1,0);
	    glVertex3f(0,0,0);

	    glTexCoord2f(0,0);
	    glVertex3f(0,height*0.5f,0.7071f*width);

	    glTexCoord2f(0,1);
	    glVertex3f(0,height,0);

	    glTexCoord2f(1,1);
	    glVertex3f(0,height*0.5f,-0.7071f*width);

	glEnd();
}





