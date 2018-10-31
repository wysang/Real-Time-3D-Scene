#ifndef GL_HOUSE_H
#define GL_HOUSE_H
#include "GL_Texture.h"
#include "GL_Vector3.h"
#include "GL_Load3DS.h"
class GL_House
{
public:
	GL_House();
	~GL_House(); 
	//////////////
	static GL_House *pHouse;
	GL_Vector3 *GetHouseVertex(){return m_house_vertex;}
    GL_Vector3 *GetTowerVertex(){return m_tower_vertex;}
	GL_Vector3 *GetStairVertex(){return m_stair_vertex;}
	
	void InitHouse();
	void DrawHouse();
public:
	GL_Vector3  *m_door_vertex;
	Texcoord    *m_door_texcoord;
	C3DSModel	m_house;
	C3DSModel   m_qiu;
	C3DSModel	m_ren;
private:
    void InitHouseData();                 //初始化房子数据
    void DrawDoor();
private:
	GL_Texture  m_texture;               //纹理
    GLuint	    m_housetexture[7];	     
	GLuint      m_towertexture[6];
	int         numvertex;
	GL_Vector3  *m_house_vertex;
	Texcoord    *m_house_texcoord;

	////
	GL_Vector3  *m_tower_vertex;
	Texcoord    *m_tower_texcoord;
	////
	GL_Vector3  *m_stair_vertex;
	Texcoord    *m_stair_texcoord;
};


#endif  //GL_HOUSE_H