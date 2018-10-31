#ifndef	GL_FONT_H
#define	GL_FONT_H
#include<windows.h>
#include <gl\gl.h>				 
#include <gl\glu.h>
#include <glaux.h>
#include "GL_Load3DS.h"
#include "GL_Vector3.h"
#include "GL_Terrain.h"
/* ���������� */
class GL_Font															
{
public:
    /* ���캯������������ */
	GL_Font();
	~GL_Font();
    //��Ա����
	bool InitFont();  /* ��ʼ������ */
	void PrintText(char *string, float x, float y); /* ��(x,y)�����string���� */
	C3DSModel	m_bed0;
	
protected:
	HFONT m_hFont;  /* ������ */
		
};

#endif	// GL_FONT_H