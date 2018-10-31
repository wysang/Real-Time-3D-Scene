#ifndef GL_WATER_H
#define GL_WATER_H
#include "GL_Texture.h"
#include "GL_Load3DS.h"

const int SIZEX=300;     //水面网格大小
const int SIZEZ=300;

class GL_Water
{
public:
	GL_Water();
	~GL_Water(); 
	
	void InitWater();
	void DrawWater();
    void Update();
public:
	C3DSModel	m_3ds;

private:
	void InitWaterGrid();
	float    MakeRand(float min, float max);
    
private:
	GL_Texture  m_texture;               //纹理
    GLuint	    m_watertexture[34];	 

	float       m_watergrid[SIZEX][SIZEZ][3];   // 水面网格顶点数组
	float       m_buffer[SIZEX][SIZEZ];

	float       m_num;
	float       m_waterlevel;
	bool        m_bwaterlevel;
	float       m_waterheight;   //水面高度
	
};

#endif  //GL_WATER_H