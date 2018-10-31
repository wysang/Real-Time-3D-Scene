#ifndef	GL_TERRAIN_H
#define	GL_TERRAIN_H
#include "GL_Texture.h"
#include "GL_Vector3.h"
//地形大小，必须取为图片的大小，这里图片的大小为32x32的，所以取为32
//因为我们使用的是依据这个图片的灰度值设定地面的高度
enum{m_TerrainSize=32};
const float m_TerrainScale=50.0f;   //缩放地形，即每个小网格的边长为m_TerrainScale

class GL_Terrain
{
public:
    //constructor
	GL_Terrain();
	~GL_Terrain();
	static GL_Terrain *pTerrain;
	//该地形上的房子的地板高度，房子范围：X： 3~18  Z：-25~-29
	//                             参数对应：  15          27  
	float  GetFloorHeight(){return m_TerrainCoord[27*m_TerrainSize+15].y;}
    void         CreateSkyBox(float ratio);               //天空包围盒
    void	     DrawGround();                            //绘制地面并纹理贴图
	//初始化地形
	void         InitTerrain();
	float        GetHeight(float x,float z);      //获得地面高度,这个用内联函数
public:
	GL_Vector3  m_TerrainCoord[m_TerrainSize*m_TerrainSize];   //网格坐标数据
private:
	void         InitTerrainData();               //初始化地形数据
    void         DrawGrid(int minrow,int maxrow,int mincol,int maxcol);
private:

	GLuint   m_Skybox[6];                      //天空盒纹理索引，6个面
	GLuint   m_Groundtexture[2]; 
	//地形顶点索引数组，地图大小为：m_TerrainSize*m_TerrainSize
	//就是依据这个顶点索引调用glDrawElements来绘值地面并贴上纹理的
	//GLuint m_TerrainVertex    [m_TerrainSize*m_TerrainSize*2];
	//float  m_TerrainCoord     [m_TerrainSize*m_TerrainSize][3];   //地形顶点的坐标
	//float  m_TerrainTexCoord  [m_TerrainSize*m_TerrainSize][2];   //地形纹理坐标

    AUX_RGBImageRec *m_TerrainImage;
	GL_Texture       m_Texture;
};

#endif	// GL_TERRAIN_H