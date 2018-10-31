#include "GL_Terrain.h"
#define GL_CLAMP_TO_EDGE 0x812F

GL_Terrain *GL_Terrain::pTerrain=NULL;

//Ϊʲô��ȡ����Ĳ��ֺͳ�ʼ�����β��ܷ��ڹ��캯�����棿
//��Ϊ���Ǻ���һЩopengl��״̬�����ã��������ö�������������������ȵ�
//�������Ƿŵ����캯�������ʱ����䣺Me * example = new Me(class_name);
//��m_Terrain��Me��һ����Ա��������ô��ʼ��m_Terrain������m_Terrain��Ĭ�Ϲ��캯��
//���opengl��״̬�����������ö�������������������ȵ�ֻ����GL_Terrain������������
//*********����Me��û��Ч������������Me�����ܹ����õ�����opengl״̬�����õĺ����ſ���*****
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
	
	//����ĵȼ���glDeleteTextures(6,m_Skybox)
	for(int i=0;i<6;i++) 
		glDeleteTextures(1, &m_Skybox[i]);   //ɾ���������
	glDeleteTextures(2, m_Groundtexture);
}

void GL_Terrain::InitTerrainData()
{
	//��ȡͼ��Ҷ���Ϊ���θ߶�
	m_TerrainImage = auxDIBImageLoadA("data/terrain/Terrain32.bmp");
	//��ʼ��������������
	int vertex;
	for(int z=0; z<m_TerrainSize;z++)
	{
		for(int x=0; x<m_TerrainSize;x++)
		{ 
			vertex=z*m_TerrainSize+x;
			m_TerrainCoord [vertex].x = float(x)*m_TerrainScale;
			//�����:ÿ��������r g b��������������3������ֻȡ��һ������ 
			//�߶���0~255,̫��,���Գ���һ��������С
			//ע��Ϊ��ȷ��ֵ������֮�ڣ�ͨ���趨Ҫ����ͼ�Ĵ�СΪ��m_TerrainSize*m_TerrainSize
            m_TerrainCoord [vertex].y = float(m_TerrainImage->data[vertex*3]/1.0f);
			m_TerrainCoord [vertex].z = -float(z)*m_TerrainScale;
		}
	}
}

//��ʼ������,��ʼ��m_TerrainVertex��m_TerrainCoord��m_TerrainTexCoord
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
    glEnable(GL_CULL_FACE);//���ö�����޳�
	//glFrontFace(GL_CCW);   //Ĭ�ϵ���ʱ�뷽��Ϊǰ�棬������   
	glCullFace(GL_FRONT);  //�������޳�����Ϊ���ǵ��ζ����˳��Ϊ˳ʱ�룬�����泯��
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

/* ����˫���Բ�ֵ�õ�����߶� */
float GL_Terrain::GetHeight(float x,float z)
{
 //����õ���С������м䣬��ô���ĸ߶�yֵ��ͨ����С������ĸ������ֵ���õ�
 //�������ͨ���õ㻭�����ֱ�ƽ����x���z����ߣ���С�����Ϊ�ĸ�����
 //Ȼ��ͨ���ĸ����ε�����Ĵ�С��Ϊ���أ�ϵ��������ֵ�õ��õ�ĸ߶�ֵ
	float CameraX = x/m_TerrainScale;
	float CameraZ =-z/m_TerrainScale;  
	int Col0 = int(CameraX);  //�õ�������
	int Row0 = int(CameraZ);  //������
	int Col1 = Col0 + 1;
	int Row1 = Row0 + 1; 
	if (Col1 > m_TerrainSize)	Col1 = 0;  //����������Ҫ�����߽�
	if (Row1 > m_TerrainSize)	Row1 = 0;
	float h00=m_TerrainCoord[Col0 + Row0*m_TerrainSize].y;  //�߶�ֵ
	float h01=m_TerrainCoord[Col1 + Row0*m_TerrainSize].y;
	float h11=m_TerrainCoord[Col1 + Row1*m_TerrainSize].y;
	float h10=m_TerrainCoord[Col0 + Row1*m_TerrainSize].y;
	float tx =CameraX - int(CameraX);  //�õ�С������
	float ty =CameraZ - int(CameraZ);
	float txty = tx * ty;      //������Ǹ����ĸ�С���εı�������ֵ�õ��߶�
	//���һ����Ҫע����Ƿ��ȹ�ϵ���õ�߶Ⱥ������Ǹ������С�ľ��εĹ�ϵ����
	return (h00*(1.0f-ty-tx+txty)+h01*(tx-txty)+h11*txty+h10*(ty-txty)); 
}


//������պ�
//ratio Ϊ�����µĲ���������ϵĲ��ֵı�ֵ,һ��ȡΪ0.2����
void GL_Terrain::CreateSkyBox(float ratio)
{
	float width =float(m_TerrainSize*m_TerrainScale*2);
	float height=float(m_TerrainSize*m_TerrainScale*(ratio+0.6));
	float length=float(m_TerrainSize*m_TerrainScale*2);

	//��պе�ԭ�㣨x,y,z��
	float x = -m_TerrainSize*m_TerrainScale*0.5f;        
	float y = -m_TerrainSize*m_TerrainScale*ratio;
	float z = -m_TerrainSize*m_TerrainScale*1.5f;
	
	// ʹ������
	glEnable(GL_TEXTURE_2D);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[0]);  //Z���������
	//������������պ����޷����ӵĹؼ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z+length);
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z+length);
	glEnd();
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[1]);  //right
	//������������պ����޷����ӵĹؼ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z+length);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z+length); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x+width,y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x+width,y,		 z);
	glEnd();
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[2]);  //Z�Ḻ�������
	//������������պ����޷����ӵĹؼ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
		glTexCoord2f(1.0f,0.0f); glVertex3f(x+width,y,		 z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x+width,y+height,z); 
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z);
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z);
	glEnd();
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[3]);  //left
	//������������պ����޷����ӵĹؼ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBegin(GL_QUADS);
	    glTexCoord2f(1.0f,0.0f); glVertex3f(x,		y,		 z);
		glTexCoord2f(1.0f,1.0f); glVertex3f(x,		y+height,z);	
		glTexCoord2f(0.0f,1.0f); glVertex3f(x,		y+height,z+length); 
		glTexCoord2f(0.0f,0.0f); glVertex3f(x,		y,		 z+length);
	glEnd();
	glBindTexture  (GL_TEXTURE_2D, m_Skybox[4]);  //top
	//������������պ����޷����ӵĹؼ�����
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


