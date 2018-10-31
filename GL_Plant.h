#ifndef GL_PLANT_H
#define GL_PLANT_H
#include "GL_Texture.h"

class GL_Plant
{
public:
	GL_Plant();
	~GL_Plant();

	//��������
	bool   InitPlant();
	void   RenderAll();

    static GL_Plant *pPlant;
	int    GetNumber(){return numtrees;}
	float  *GetHeight(){return height;}
	float  *GetXpos  (){return xpos;}
	float  *GetZpos  (){return zpos;}

private:
	//��һ���� (x,z)Ϊ����λ��
	void DrawTree(int i,float x,float z,int j=0);
    void RenderPineBranch(float width);
	
	//������֦
	void RenderBranch(int i,int j);
	//��������
	void RenderTreebody();
	//����ֲ�ﾥ�İ뾶
	void SetRadius(float size)	{m_radiusTree=size;}
	//����ֲ��ĸ߶�
	void SetHeight(float h)		{m_height=h;m_radiusTree=h/80.0f;}
	//����֦Ҷ�Ŀ��
	void SetBranchWidth(float w){m_branchWidth=w;}
	//��������ͷ��
	void DrawTop(float width,float height);

	void InitPlantData();                 //��ʼ��ֲ������
private:
	GLuint	m_treeBody[3];		// ���ɵ����� ��pine  poplar
	GLuint	m_treeBranch[3];	// ��֦������ ��pine  poplar  maple
	float		m_radiusTree;	// ���İ뾶
	float		m_branchWidth;	// ֦Ҷ�Ŀ��
	float		m_height;		// ���ɸ߶�
	GL_Texture  m_texture;

	//����� �Ǵ��ļ��ж�ȡֲ�����Ϣ��ŵĵط�
	int        numtrees;        //���ĸ���
	float      *height;         //���ĸ߶�
	int        *type;           //��������
	float      *xpos;           //λ�� 
	float      *zpos;     
	int        *detail;         //ϸ��  

};

#endif   //GL_PLANT_H