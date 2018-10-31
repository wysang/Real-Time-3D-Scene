#include "GL_House.h"
#include "GL_Terrain.h"
#include "GL_Camera.h"
#include "Me.h"
GL_House *GL_House::pHouse=NULL;



GL_House::GL_House()
{
	InitHouseData();
	pHouse=this;
}

GL_House::~GL_House()
{
	for(int i=0;i<7;i++)  //删除纹理对象
	{
		glDeleteTextures(1, &m_housetexture[i]); 
	}
	glDeleteTextures(8,m_towertexture);

	delete []m_house_vertex;
	delete []m_house_texcoord;

	delete []m_door_vertex;
	delete []m_door_texcoord;

	//
	delete []m_tower_vertex;
	delete []m_tower_texcoord;

	delete []m_stair_vertex;
	delete []m_stair_texcoord;
}

void GL_House::InitHouseData()
{
	///////////读取文件中的数据//////////
	FILE *filevertex,*filetex;
	char oneline[255];
	filevertex = fopen("data/house/house_vertex.txt", "rt");
	filetex    = fopen("data/house/house_texcoord.txt", "rt");
	Readstr(filevertex,oneline);
	sscanf(oneline, "%d", &numvertex);
	m_house_vertex=new GL_Vector3[numvertex];  
	m_house_texcoord=new Texcoord[numvertex];
    
	//地面高度h
	float h=GL_Terrain::pTerrain->GetFloorHeight();
	for(int m=0;m<numvertex;m++)
	{
		Readstr(filevertex,oneline);
        sscanf(oneline,"%f %f %f",&m_house_vertex[m].x,
			                      &m_house_vertex[m].y,
								  &m_house_vertex[m].z);

		m_house_vertex[m].x=(m_house_vertex[m].x+4)*m_TerrainScale;
		m_house_vertex[m].y= m_house_vertex[m].y*m_TerrainScale+h;
		m_house_vertex[m].z=(m_house_vertex[m].z-25)*m_TerrainScale;

		Readstr(filetex,oneline);
		sscanf(oneline,"%f %f",&m_house_texcoord[m].x,&m_house_texcoord[m].y);
	}

	filevertex = fopen("data/house/door_vertex.txt", "rt");
	filetex    = fopen("data/house/door_texcoord.txt", "rt");
	Readstr(filevertex,oneline);
	sscanf(oneline, "%d", &numvertex);
	m_door_vertex=new GL_Vector3[numvertex];  
	m_door_texcoord=new Texcoord[numvertex];
    //门
	for(int m=0;m<numvertex;m++)
	{
		Readstr(filevertex,oneline);
        sscanf(oneline,"%f %f %f",&m_door_vertex[m].x,
			                      &m_door_vertex[m].y,
								  &m_door_vertex[m].z);

		m_door_vertex[m].x=(m_door_vertex[m].x+4)*m_TerrainScale;
		m_door_vertex[m].y= m_door_vertex[m].y*m_TerrainScale+h;
		m_door_vertex[m].z=(m_door_vertex[m].z-25)*m_TerrainScale;

		Readstr(filetex,oneline);
		sscanf(oneline,"%f %f",&m_door_texcoord[m].x,&m_door_texcoord[m].y);
	}

	fclose(filevertex);
	fclose(filetex);
}

void GL_House::InitHouse()
{
	
	m_texture.MakeTextureBind("data/house/bricks0.bmp", m_housetexture[0]);
	m_texture.MakeTextureBind("data/house/wall00.bmp", m_housetexture[1]);
	m_texture.MakeTextureBind("data/house/stone1.bmp", m_housetexture[2]);
	m_texture.MakeTextureBind("data/house/eaves.bmp", m_housetexture[3]);
	m_texture.MakeTextureBind("data/house/door1.bmp", m_housetexture[4]);
	m_texture.MakeTextureBind("data/house/top.bmp", m_housetexture[5]);
	m_texture.MakeTextureBind("data/house/wallinner1.bmp", m_housetexture[6]);

	m_texture.MakeTextureBind("data/house/stairedge.bmp", m_towertexture[0]);
	m_texture.MakeTextureBind("data/house/stairedge.bmp", m_towertexture[2]);
	m_texture.MakeTextureBind("data/house/door11.bmp", m_towertexture[4]);
	m_texture.MakeTextureBind("data/house/top1.bmp", m_towertexture[5]);

	m_house.Load("data/3ds/","白房子.3DS");
	m_qiu.Load("data", "ball2.3DS");
	m_ren.Load("data/3ds/","dama.3DS");

}

void GL_House::DrawDoor()
{
	//door
	glBindTexture(GL_TEXTURE_2D, m_housetexture[4]);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 12; i += 3)
	{
		glTexCoord2f(m_door_texcoord[i].x, m_door_texcoord[i].y);
		glVertex3f(m_door_vertex[i].x, m_door_vertex[i].y, m_door_vertex[i].z);

		glTexCoord2f(m_door_texcoord[i + 1].x, m_door_texcoord[i + 1].y);
		glVertex3f(m_door_vertex[i + 1].x, m_door_vertex[i + 1].y, m_door_vertex[i + 1].z);

		glTexCoord2f(m_door_texcoord[i + 2].x, m_door_texcoord[i + 2].y);
		glVertex3f(m_door_vertex[i + 2].x, m_door_vertex[i + 2].y, m_door_vertex[i + 2].z);
	}
	glEnd();
}

void GL_House::DrawHouse()
{
	glEnable(GL_TEXTURE_2D);
	//wall
	glBindTexture(GL_TEXTURE_2D, m_housetexture[0]);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i < 48; i += 3)
	{
		glTexCoord2f(m_house_texcoord[i].x, m_house_texcoord[i].y);
		glVertex3f(m_house_vertex[i].x, m_house_vertex[i].y, m_house_vertex[i].z);

		glTexCoord2f(m_house_texcoord[i + 1].x, m_house_texcoord[i + 1].y);
		glVertex3f(m_house_vertex[i + 1].x, m_house_vertex[i + 1].y, m_house_vertex[i + 1].z);

		glTexCoord2f(m_house_texcoord[i + 2].x, m_house_texcoord[i + 2].y);
		glVertex3f(m_house_vertex[i + 2].x, m_house_vertex[i + 2].y, m_house_vertex[i + 2].z);
	}

	glEnd();

	//door
	DrawDoor();

	//triwall
	glBindTexture(GL_TEXTURE_2D, m_housetexture[1]);
	glBegin(GL_TRIANGLES);
	for (int i = 48; i < 54; i += 3)
	{
		glTexCoord2f(m_house_texcoord[i].x, m_house_texcoord[i].y);
		glVertex3f(m_house_vertex[i].x, m_house_vertex[i].y, m_house_vertex[i].z);

		glTexCoord2f(m_house_texcoord[i + 1].x, m_house_texcoord[i + 1].y);
		glVertex3f(m_house_vertex[i + 1].x, m_house_vertex[i + 1].y, m_house_vertex[i + 1].z);

		glTexCoord2f(m_house_texcoord[i + 2].x, m_house_texcoord[i + 2].y);
		glVertex3f(m_house_vertex[i + 2].x, m_house_vertex[i + 2].y, m_house_vertex[i + 2].z);
	}

	glEnd();

	//tile
	glBindTexture(GL_TEXTURE_2D, m_housetexture[2]);
	glBegin(GL_TRIANGLES);
	for (int i = 54; i < 66; i += 3)
	{
		glTexCoord2f(m_house_texcoord[i].x, m_house_texcoord[i].y);
		glVertex3f(m_house_vertex[i].x, m_house_vertex[i].y, m_house_vertex[i].z);

		glTexCoord2f(m_house_texcoord[i + 1].x, m_house_texcoord[i + 1].y);
		glVertex3f(m_house_vertex[i + 1].x, m_house_vertex[i + 1].y, m_house_vertex[i + 1].z);

		glTexCoord2f(m_house_texcoord[i + 2].x, m_house_texcoord[i + 2].y);
		glVertex3f(m_house_vertex[i + 2].x, m_house_vertex[i + 2].y, m_house_vertex[i + 2].z);
	}

	glEnd();

	//eave
	glBindTexture(GL_TEXTURE_2D, m_housetexture[3]);
	glBegin(GL_TRIANGLES);
	for (int i = 66; i < 138; i += 3)
	{
		glTexCoord2f(m_house_texcoord[i].x, m_house_texcoord[i].y);
		glVertex3f(m_house_vertex[i].x, m_house_vertex[i].y, m_house_vertex[i].z);

		glTexCoord2f(m_house_texcoord[i + 1].x, m_house_texcoord[i + 1].y);
		glVertex3f(m_house_vertex[i + 1].x, m_house_vertex[i + 1].y, m_house_vertex[i + 1].z);

		glTexCoord2f(m_house_texcoord[i + 2].x, m_house_texcoord[i + 2].y);
		glVertex3f(m_house_vertex[i + 2].x, m_house_vertex[i + 2].y, m_house_vertex[i + 2].z);
	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, m_housetexture[5]);
	glBegin(GL_TRIANGLES);
	for (int i = 138; i < 144; i += 3)
	{
		glTexCoord2f(m_house_texcoord[i].x, m_house_texcoord[i].y);
		glVertex3f(m_house_vertex[i].x, m_house_vertex[i].y, m_house_vertex[i].z);

		glTexCoord2f(m_house_texcoord[i + 1].x, m_house_texcoord[i + 1].y);
		glVertex3f(m_house_vertex[i + 1].x, m_house_vertex[i + 1].y, m_house_vertex[i + 1].z);

		glTexCoord2f(m_house_texcoord[i + 2].x, m_house_texcoord[i + 2].y);
		glVertex3f(m_house_vertex[i + 2].x, m_house_vertex[i + 2].y, m_house_vertex[i + 2].z);
	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, m_housetexture[6]);
	glBegin(GL_TRIANGLES);
	for (int i = 144; i < 192; i += 3)
	{
		glTexCoord2f(m_house_texcoord[i].x, m_house_texcoord[i].y);
		glVertex3f(m_house_vertex[i].x, m_house_vertex[i].y, m_house_vertex[i].z);

		glTexCoord2f(m_house_texcoord[i + 1].x, m_house_texcoord[i + 1].y);
		glVertex3f(m_house_vertex[i + 1].x, m_house_vertex[i + 1].y, m_house_vertex[i + 1].z);

		glTexCoord2f(m_house_texcoord[i + 2].x, m_house_texcoord[i + 2].y);
		glVertex3f(m_house_vertex[i + 2].x, m_house_vertex[i + 2].y, m_house_vertex[i + 2].z);
	}

	glEnd();
	
	// 在固定的位置以一定的比例显示3D模型
	m_house.Scene(10*50,GL_Terrain::pTerrain->GetFloorHeight()+1,-4*50,1.5f ,180.0f,0.0f,1.0f,0.0f);
	//if (!(GetKeyState('E')))
	//{
	//	m_qiu.Scene(464, GL_Terrain::pTerrain->GetFloorHeight() + 4, -1196, 0.01f, 180.0f, 0.0f, 1.0f, 0.0f);
	//}
	//else
	//{
	//	if (!(GetKeyState('P')) && pick == 0)
	//	{
	//		m_qiu.Scene(464, GL_Terrain::pTerrain->GetFloorHeight() + 4, -1196, 0.01f, 180.0f, 0.0f, 1.0f, 0.0f);
	//	}
	//}
	m_ren.Scene(666,GL_Terrain::pTerrain->GetFloorHeight(),-1349,0.5f ,180.0f,0.0f,1.0f,0.0f);
	glDisable(GL_TEXTURE_2D); 
}


