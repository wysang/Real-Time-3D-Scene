#ifndef	GL_TERRAIN_H
#define	GL_TERRAIN_H
#include "GL_Texture.h"
#include "GL_Vector3.h"
//���δ�С������ȡΪͼƬ�Ĵ�С������ͼƬ�Ĵ�СΪ32x32�ģ�����ȡΪ32
//��Ϊ����ʹ�õ����������ͼƬ�ĻҶ�ֵ�趨����ĸ߶�
enum{m_TerrainSize=32};
const float m_TerrainScale=50.0f;   //���ŵ��Σ���ÿ��С����ı߳�Ϊm_TerrainScale

class GL_Terrain
{
public:
    //constructor
	GL_Terrain();
	~GL_Terrain();
	static GL_Terrain *pTerrain;
	//�õ����ϵķ��ӵĵذ�߶ȣ����ӷ�Χ��X�� 3~18  Z��-25~-29
	//                             ������Ӧ��  15          27  
	float  GetFloorHeight(){return m_TerrainCoord[27*m_TerrainSize+15].y;}
    void         CreateSkyBox(float ratio);               //��հ�Χ��
    void	     DrawGround();                            //���Ƶ��沢������ͼ
	//��ʼ������
	void         InitTerrain();
	float        GetHeight(float x,float z);      //��õ���߶�,�������������
public:
	GL_Vector3  m_TerrainCoord[m_TerrainSize*m_TerrainSize];   //������������
private:
	void         InitTerrainData();               //��ʼ����������
    void         DrawGrid(int minrow,int maxrow,int mincol,int maxcol);
private:

	GLuint   m_Skybox[6];                      //��պ�����������6����
	GLuint   m_Groundtexture[2]; 
	//���ζ����������飬��ͼ��СΪ��m_TerrainSize*m_TerrainSize
	//�����������������������glDrawElements����ֵ���沢���������
	//GLuint m_TerrainVertex    [m_TerrainSize*m_TerrainSize*2];
	//float  m_TerrainCoord     [m_TerrainSize*m_TerrainSize][3];   //���ζ��������
	//float  m_TerrainTexCoord  [m_TerrainSize*m_TerrainSize][2];   //������������

    AUX_RGBImageRec *m_TerrainImage;
	GL_Texture       m_Texture;
};

#endif	// GL_TERRAIN_H