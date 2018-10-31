#include "GL_Water.h"
#include "GL_Terrain.h"

GL_Water::GL_Water()
{

	m_num=0;
	m_waterlevel=0;
	m_bwaterlevel=false;
	m_waterheight=GL_Terrain::pTerrain->GetFloorHeight()-25.0f;  //水面高度
	InitWaterGrid();

}

GL_Water::~GL_Water()
{
	glDeleteTextures(34,m_watertexture);
}


void GL_Water::InitWaterGrid()
{
	float x=SIZEX/10.0f;
	float z=SIZEZ/10.0f;
	for (int i=0; i<SIZEX; i++)
	{
		for (int j=0; j<SIZEZ; j++)
		{
			m_watergrid[i][j][0]=(float(i)/x)*m_TerrainScale+330;     //x
			m_watergrid[i][j][1]= GL_Terrain::pTerrain->GetFloorHeight()-70;   //y
			m_watergrid[i][j][2]=(float(j)/z)*m_TerrainScale-1053;   //z 

		}
	}
}

void GL_Water::InitWater()
{
	char* fileName[] = {"data/water/0.bmp","data/water/1.bmp",
		                "data/water/2.bmp","data/water/3.bmp",
	                    "data/water/4.bmp","data/water/5.bmp",
						"data/water/6.bmp","data/water/7.bmp",
						"data/water/8.bmp","data/water/9.bmp",
						"data/water/10.bmp","data/water/11.bmp",
						"data/water/12.bmp","data/water/13.bmp",
						"data/water/14.bmp","data/water/15.bmp",
						"data/water/16.bmp","data/water/17.bmp",
						"data/water/18.bmp","data/water/19.bmp",
						"data/water/20.bmp","data/water/21.bmp",
						"data/water/22.bmp","data/water/23.bmp",
						"data/water/24.bmp","data/water/25.bmp",
						"data/water/26.bmp","data/water/27.bmp",
						"data/water/28.bmp","data/water/29.bmp",
	                    "data/water/30.bmp","data/water/31.bmp"};
	for(int i=0;i<32;i++)
		m_texture.MakeTextureBind(fileName[i],m_watertexture[i]);
	m_3ds.Load("data/water/","fish.3DS");
	
}

void GL_Water::Update()
{

}

void GL_Water::DrawWater()
{

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    //混合函数   
	//通过m_num变量控制时间帧贴图置换
    glBindTexture(GL_TEXTURE_2D, m_watertexture[int(m_num)]);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glPushAttrib(GL_CURRENT_BIT);
  
    glColor4f(1.0f,1.0f,1.0f,0.6f);

	glPushMatrix();
	glBegin(GL_QUADS);
	  for(int i=0;i<SIZEX-1;i++)
	  {
		  for(int j=0;j<SIZEZ-1;j++)
		  {
			  float i1=float(i)*20.0f/SIZEX,j1=float(j)*26.0f/SIZEZ,
                    i2=float(i+1)*20.0f/SIZEX,j2=float(j+1)*26.0f/SIZEZ;

			  //一个面片的四个点
			  glTexCoord2f(i1,j1);
	          glVertex3f(m_watergrid[i][j][0],
				         m_watergrid[i][j][1],
						 m_watergrid[i][j][2]);

			  glTexCoord2f(i1,j2);
	          glVertex3f(m_watergrid[i][j+1][0],
				         m_watergrid[i][j+1][1],
						 m_watergrid[i][j+1][2]);
			  
			  glTexCoord2f(i2,j2);
	          glVertex3f(m_watergrid[i+1][j+1][0],
				         m_watergrid[i+1][j+1][1],
						 m_watergrid[i+1][j+1][2]);
			  
			  glTexCoord2f(i2,j1);
	          glVertex3f(m_watergrid[i+1][j][0],
				         m_watergrid[i+1][j][1],
						 m_watergrid[i+1][j][2]);
		  }
	  }
	glEnd();
	glPopMatrix();

	glPopAttrib();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);

	// 咸鱼 & 海棠
	m_3ds.Scene(300, GL_Terrain::pTerrain->GetFloorHeight()+250, -600, 0.5f, 180.0f, 0.0f, 1.0f, 0.0f);

    glDisable(GL_TEXTURE_2D);

	//通过m_num变量控制时间帧显示不同贴图
    if(m_num>=31)
	    m_num=0;
    else
		m_num+=1.0f;
}

//生成min~max之间的随机浮点数
float GL_Water::MakeRand(float min, float max)
{
	float	number;		
    number=min+(max-min)*rand()/float(RAND_MAX);
  	
	return number;
}

