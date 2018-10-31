#ifndef GL_PLANT_H
#define GL_PLANT_H
#include "GL_Texture.h"

class GL_Plant
{
public:
	GL_Plant();
	~GL_Plant();

	//载入纹理
	bool   InitPlant();
	void   RenderAll();

    static GL_Plant *pPlant;
	int    GetNumber(){return numtrees;}
	float  *GetHeight(){return height;}
	float  *GetXpos  (){return xpos;}
	float  *GetZpos  (){return zpos;}

private:
	//画一棵树 (x,z)为树的位置
	void DrawTree(int i,float x,float z,int j=0);
    void RenderPineBranch(float width);
	
	//绘制树枝
	void RenderBranch(int i,int j);
	//绘制树身
	void RenderTreebody();
	//设置植物茎的半径
	void SetRadius(float size)	{m_radiusTree=size;}
	//设置植物的高度
	void SetHeight(float h)		{m_height=h;m_radiusTree=h/80.0f;}
	//设置枝叶的宽度
	void SetBranchWidth(float w){m_branchWidth=w;}
	//绘制树的头部
	void DrawTop(float width,float height);

	void InitPlantData();                 //初始化植物数据
private:
	GLuint	m_treeBody[3];		// 树干的纹理 有pine  poplar
	GLuint	m_treeBranch[3];	// 树枝的纹理 有pine  poplar  maple
	float		m_radiusTree;	// 树的半径
	float		m_branchWidth;	// 枝叶的宽度
	float		m_height;		// 树干高度
	GL_Texture  m_texture;

	//下面的 是从文件中读取植物的信息存放的地方
	int        numtrees;        //树的个数
	float      *height;         //树的高度
	int        *type;           //树的类型
	float      *xpos;           //位置 
	float      *zpos;     
	int        *detail;         //细节  

};

#endif   //GL_PLANT_H