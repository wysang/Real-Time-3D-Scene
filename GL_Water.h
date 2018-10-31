#ifndef GL_WATER_H
#define GL_WATER_H
#include "GL_Texture.h"
#include "GL_Load3DS.h"

const int SIZEX=300;     //ˮ�������С
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
	GL_Texture  m_texture;               //����
    GLuint	    m_watertexture[34];	 

	float       m_watergrid[SIZEX][SIZEZ][3];   // ˮ�����񶥵�����
	float       m_buffer[SIZEX][SIZEZ];

	float       m_num;
	float       m_waterlevel;
	bool        m_bwaterlevel;
	float       m_waterheight;   //ˮ��߶�
	
};

#endif  //GL_WATER_H