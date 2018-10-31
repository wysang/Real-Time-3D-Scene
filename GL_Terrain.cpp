#include "GL_Terrain.h"
#define GL_CLAMP_TO_EDGE 0x812F

GL_Terrain *GL_Terrain::pTerrain=NULL;

//为什么读取纹理的部分和初始化地形不能放在构造函数里面？
//因为它们含有一些opengl的状态机设置，例如启用顶点数组和生成纹理对象等等
//当把它们放到构造函数里面的时候，语句：Me * example = new Me(class_name);
//而m_Terrain是Me的一个成员变量，那么初始化m_Terrain，调用m_Terrain的默认构造函数
//则对opengl的状态机设置如启用顶点数组和生成纹理对象等等只会在GL_Terrain类里面起作用
//*********在类Me里没有效果，必须在类Me里面能够调用到含有opengl状态机设置的函数才可以*****
GL_Terrain::GL_Terrain()
{	
    InitTerrainData();
	pTerrain=this;
}

GL_Terrain::~GL_Terrain()
{
	if(m_TerrainImage)
	{
		if(m_TerrainImage->data)
		{
			free(m_TerrainImage->data);
			m_TerrainImage->data=0;
		}
		free(m_TerrainImage);
		m_TerrainImage=0;
	}
	
	//下面的等价于glDeleteTextures(6,m_Skybox)
	for(int i=0;i<6;i++) 
		glDeleteTextures(1, &m_Skybox[i]);   //删除纹理对象
	glDeleteTextures(2, m_Groundtexture);
}

void GL_Terrain::InitTerrainData()
{
	//读取图像灰度作为地形高度
	m_TerrainImage = auxDIBImageLoadA("data/terrain/Terrain32.bmp");
	//初始化地形网格数据
	int vertex;
	for(int z=0; z<m_TerrainSize;z++)
	{
		for(int x=0; x<m_TerrainSize;x++)
		{ 
			vertex=z*m_TerrainSize+x;
			m_TerrainCoord [vertex].x = float(x)*m_TerrainScale;
			//下面的:每个像素有r g b三个分量，乘以3跳过，只取第一个分量 
			//高度是0~255,太大,所以除以一个变量缩小
			//注意为了确保值在数组之内，通常设定要读的图的大小为：m_TerrainSize*m_TerrainSize
            m_TerrainCoord [vertex].y = float(m_TerrainImage->data[vertex*3]/1.0f);
			m_TerrainCoord [vertex].z = -float(z)*m_TerrainScale;
		}
	}
}

//初始化地形,初始化m_TerrainVertex、m_TerrainCoord、m_TerrainTexCoord
void GL_Terrain::InitTerrain()
{
	char* fileName[] = {"data/skybox/2front.bmp",
		                "data/skybox/2right.bmp", 
	                    "data/skybox/2back.bmp",
	                    "data/skybox/2left.bmp",
	                    "data/skybox/2top.bmp",
	                    "data/terrain/SAND3.bmp"};
	for(int i=0;i<6;i++)
		m_Texture.MakeTextureBind(fileName[i],m_Skybox[i]);
	m_Texture.MakeTextureBind("data/terrain/SAND3.bmp",m_Groundtexture[0]);
    m_Texture.MakeTextureBind("data/terrain/SAND3.bmp",m_Groundtexture[1]);
}

void GL_Terrain::DrawGround()
{ 
    glEnable(GL_CULL_FACE);//启用多边形剔除
	//glFrontFace(GL_CCW);   //默认的逆时针方向为前面，即正面   
	glCullFace(GL_FRONT);  //将正面剔出，因为我们地形顶点的顺序为顺时针，即反面朝上
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, m_Skybox[5]);
	DrawGrid(0,23,0,31);
    DrawGrid(23,31,0,4);
	DrawGrid(23,31,18,31);
    DrawGrid(29,31,4,18);

	glBindTexture(GL_TEXTURE_2D, m_Groundtexture[0]);
	DrawGrid(23,25,4,18);
	DrawGrid(25,29,4,6);

	glBindTexture(GL_TEXTURE_2D, m_Groundtexture[1]);
    DrawGrid(25,29,6,18);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE); 
}

void GL_Terrain::DrawGrid(int minrow,int maxrow,int mincol,int maxcol)
{
	glBegin(GL_TRIANGLES);
	for(int i = minrow; i < maxrow; i ++)
	{
		for(int j = mincol; j < maxcol; j++)
		{
			glTexCoord2f(float(i),float(j));
			glVertex3f(m_TerrainCoord[i*m_TerrainSize+j].x,
				       m_TerrainCoord[i*m_TerrainSize+j].y,
					   m_TerrainCoord[i*m_TerrainSize+j].z);

			glTexCoord2f(float(i),float(j+1));
			glVertex3f(m_TerrainCoord[(i+1)*m_TerrainSize+j].x,
				       m_TerrainCoord[(i+1)*m_TerrainSize+j].y,
					   m_TerrainCoord[(i+1)*m_TerrainSize+j].z);
	
			glTexCoord2f(float(i+1),float(j));
			glVertex3f(m_TerrainCoord[i*m_TerrainSize+j+1].x,
				       m_TerrainCoord[i*m_TerrainSize+j+1].y,
					   m_TerrainCoord[i*m_TerrainSize+j+1].z);

			glTexCoord2f(float(i+1),float(j));
			glVertex3f(m_TerrainCoord[i*m_TerrainSize+j+1].x,
				       m_TerrainCoord[i*m_TerrainSize+j+1].y,
					   m_TerrainCoord[i*m_TerrainSize+j+1].z);

			glTexCoord2f(float(i),float(j+1));
			glVertex3f(m_TerrainCoord[(i+1)*m_TerrainSize+j].x,
				       m_TerrainCoord[(i+1)*m_TerrainSize+j].y,
					   m_TerrainCoord[(i+1)*m_TerrainSize+j].z);
	
			glTexCoord2f(float(i+1),float(j+1));
			glVertex3f(m_TerrainCoord[(i+1)*m_TerrainSize+j+1].x,
				       m_TerrainCoord[(i+1)*m_TerrainSize+j+1].y,
					   m_TerrainCoord[(i+1)*m_TerrainSize+j+1].z);
		}
	}
	glEnd();
}

/* 进行双线性插值得到地面高度 */
float GL_Terrain::GetHeight(float x,float z)
{
 //如果该点在小方格的中间，那么它的高度y值可通过该小方格的四个顶点插值来得到
 //具体的是通过该点画两条分别平行于x轴和z轴的线，将小方格分为四个矩形
 //然后通过四个矩形的面积的大小作为比重（系数）来插值得到该点的高度值
	float CameraX = x/m_TerrainScale;
	float CameraZ =-z/m_TerrainScale;  
	int Col0 = int(CameraX);  //得到列坐标
	int Row0 = int(CameraZ);  //行作标
	int Col1 = Col0 + 1;
	int Row1 = Row0 + 1; 
	if (Col1 > m_TerrainSize)	Col1 = 0;  //行数列数不要超过边界
	if (Row1 > m_TerrainSize)	Row1 = 0;
	float h00=m_TerrainCoord[Col0 + Row0*m_TerrainSize].y;  //高度值
	float h01=m_TerrainCoord[Col1 + Row0*m_TerrainSize].y;
	float h11=m_TerrainCoord[Col1 + Row1*m_TerrainSize].y;
	float h10=m_TerrainCoord[Col0 + Row1*m_TerrainSize].y;
	float tx =CameraX - int(CameraX);  //得到小数部分
	float ty =CameraZ - int(CameraZ);
	float txty = tx * ty;      //下面的是根据四个小矩形的比重来插值得到高度
	//最后一行需要注意的是反比关系，该点高度和属于那个面积较小的矩形的关系密切
	return (h00*(1.0f-ty-tx+txty)+h01*(tx-txty)+h11*txty+h10*(ty-txty)); 
}


//创建天空盒
//ratio 为地面下的部分与地面上的部分的比值,一般取为0.2即可
void GL_Terrain::CreateSkyBox(float ratio)
{
	float width =float(m_TerrainSize*m_TerrainScale*2);
	float height=float(m_TerrainSize*m_TerrainScale*(ratio+0.6));
	float length=float(m_TerrainSize*m_TerrainScale*2);

	//天空盒的原点（x,y,z）
	float x = -m_TerrainSize*m_TerrainScale*0.5f;        
	float y = -m_TerrainSize*m_TerrainScale*ratio;
	float z = -m_TerrainSize*m_TerrainScale*1.5f;
	
	// 使用纹理
	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[0]);  //Z轴的正方向
	//下面两句是天空盒子无缝连接的关键所在
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glEnd();
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[1]);  //right
	//下面两句是天空盒子无缝连接的关键所在
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
	glEnd();
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[2]);  //Z轴负向的那面
	//下面两句是天空盒子无缝连接的关键所在
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[3]);  //left
	//下面两句是天空盒子无缝连接的关键所在
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	    glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);	
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
	glEnd();
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[4]);  //top
	//下面两句是天空盒子无缝连接的关键所在
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);	 
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y+height,z+length);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
}


