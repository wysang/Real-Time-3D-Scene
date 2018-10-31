#ifndef	GL_FONT_H
#define	GL_FONT_H
#include<windows.h>
#include <gl\gl.h>				 
#include <gl\glu.h>
#include <glaux.h>
#include "GL_Load3DS.h"
#include "GL_Vector3.h"
#include "GL_Terrain.h"
/* 定义字体类 */
class GL_Font															
{
public:
    /* 构造函数和析构函数 */
	GL_Font();
	~GL_Font();
    //成员方法
	bool InitFont();  /* 初始化字体 */
	void PrintText(char *string, float x, float y); /* 在(x,y)处输出string内容 */
	C3DSModel	m_bed0;
	
protected:
	HFONT m_hFont;  /* 字体句柄 */
		
};

#endif	// GL_FONT_H