#include "GL_Font.h"

GL_Font::GL_Font()
{
}
/* 析构函数 */
GL_Font::~GL_Font()
{
	if(m_hFont)
		DeleteObject(m_hFont); /* 删除字体句柄 */
}
/* 初始化字体 */
bool GL_Font::InitFont()
{
    //字体重量是一个很重要的参数，你可以设置一个0–1000之间的值或使用一个已定义的值。
	//FW_DONTCARE是0, FW_NORMAL是400, FW_BOLD是700 and FW_BLACK是900。
   m_hFont = CreateFont(-20,					/* 字体高度 */
						0,						/* 字体宽度 */
						0,						/* 字体的旋转角度 Angle Of Escapement */
						0,						/* 字体底线的旋转角度Orientation Angle */
						FW_NORMAL,				/* 字体的重量 */
						FALSE,					/* 是否使用斜体 */
						FALSE,					/* 是否使用下划线 */
						FALSE,					/* 是否使用删除线 */
						GB2312_CHARSET,			/* 设置字符集 */
						OUT_TT_PRECIS,			/* 输出精度 */
						CLIP_DEFAULT_PRECIS,	/* 裁剪精度 */
						ANTIALIASED_QUALITY,	/* 输出质量 */
				FF_DONTCARE|DEFAULT_PITCH,		/* Family And Pitch */
						TEXT("新宋体"));				/* 字体名称 */
 if(!m_hFont)
		return false;  /* 创建字体失败则返回 */
  /* 初始化成功则返回true */
   return true;
}

/* 在指定位置输出字符串 */
void GL_Font::PrintText(char *string, float x, float y)
{
	HBITMAP hBitmap,hOldBmp; /* 定义两个位图句柄 */
	BITMAP bm;               /* 位图结构变量 */
	SIZE size;               /* 位图尺寸 */
	GLboolean lp,tp,fp;
	HDC hDC = ::CreateCompatibleDC(0); /* 暂存设备场景 */
	glGetBooleanv(GL_LIGHTING,&lp);  /* 查看场景中是否有光照 */
	glGetBooleanv(GL_TEXTURE_2D,&tp);/* 查看场景中是否启用纹理 */
	glGetBooleanv(GL_FOG,&fp);       //是否有雾效
	/* 保存和设置一些属性 */
	glLoadIdentity();
	glPushMatrix();
	//将坐标系向屏幕里移动几个单位。如果不移动的话无法显示出文字。
	//你会注意到如果把坐标系在屏幕里放的更深远，字体并不会想你想象的那样缩小，
	//只是你可以在控制文字位置时有更多的选择。如果你将坐标系移入屏幕一个单位，
	//你就可以在X轴上-0.5到+0.5的范围内调整文字的位置。如果深入10个单位的话，
	//移动范围就从-5到+5。
	glTranslatef(0,0,-10.0f);  
	glDisable(GL_FOG);          //关闭雾效
	glDisable(GL_LIGHTING);    /* 关闭光照 */
	glDisable(GL_TEXTURE_2D);  /* 关闭纹理 */
	glDisable(GL_DEPTH_TEST);  /* 关闭深度测试 */
	SelectObject(hDC, m_hFont); /* 选择字体 */
	::GetTextExtentPoint32A(hDC, string, strlen(string), &size);/* 获取字符位图大小 */
	hBitmap = CreateBitmap(size.cx, size.cy,1, 1, NULL); /* 创建与hDC相关单色位图 */
	hOldBmp = (HBITMAP)SelectObject(hDC,hBitmap); /* 选择位图 */
  
	SetBkColor(hDC, RGB(0, 0, 0));              /* 背景色为黑色 */
	SetTextColor(hDC, RGB(255, 255, 255));      /* 字体颜色白色 */
	SetBkMode(hDC, OPAQUE);                     /* 用当前的背景颜色填充背景 */
	TextOutA(hDC, 0, 0, string, strlen(string)); /* 输出文字到暂存hDC */
	/* 获得相关位图数据结构 */
	GetObject(hBitmap, sizeof(bm), &bm);
	size.cx = (bm.bmWidth + 31) & (~31); /* 边缘对齐 */
	size.cy = bm.bmHeight;
	int bufsize = size.cx * size.cy/8;  /* 图形数据长度 */
    /*　定义单色位图结构 */  
	struct {
	BITMAPINFOHEADER bih;
	RGBQUAD col[2];
	}bic;
    /* 获取单色位图结构信息 */
	BITMAPINFO *binf = (BITMAPINFO *)&bic;
	binf->bmiHeader.biSize = sizeof(binf->bmiHeader); /* 修改结构信息 */
	binf->bmiHeader.biWidth = bm.bmWidth;
	binf->bmiHeader.biHeight = bm.bmHeight;
	binf->bmiHeader.biPlanes = 1;
	binf->bmiHeader.biBitCount = 1;       /* 单色 */
	binf->bmiHeader.biCompression = BI_RGB;  /* 颜色方式 */
	binf->bmiHeader.biSizeImage = bufsize;
	binf->bmiHeader.biXPelsPerMeter = 1;
	binf->bmiHeader.biYPelsPerMeter = 1;
	binf->bmiHeader.biClrUsed = 0;
	binf->bmiHeader.biClrImportant = 0;
	/* 定义图形数据块 */ 
	UCHAR* pBmpBits = new UCHAR[bufsize];	
	memset(pBmpBits, 0, sizeof(UCHAR)*bufsize);
	/* 将设备无关数据保存在pBmpBits指向的数据块中 */
	::GetDIBits(hDC, hBitmap, 0, bm.bmHeight, pBmpBits, binf,DIB_RGB_COLORS);
	 
	/* 显示字符串 */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); /* 指定像素存储模式 */
	//屏幕的中心是（0，0），注意，这里没有Z轴位置。
	//位图字体只使用X轴（左/右）和Y轴（上/下）。
	glRasterPos2f(x,y);                  /* 定位 */
	glBitmap(size.cx, size.cy, 0.0, 0.0, 0.0, 0.0, pBmpBits); /* 位图显示 */
	delete pBmpBits;                       /* 删除指针 */
	SelectObject(hDC, hOldBmp);           /* 恢复原来位图信息 */
	DeleteObject(hBitmap);
	::DeleteDC(hDC);
	/* 恢复一些属性 */
	if(lp)
         glEnable(GL_LIGHTING);       /* 启用光照 */
	if(tp)
		glEnable(GL_TEXTURE_2D);      /* 启用纹理 */
	if(fp)
		glEnable(GL_FOG);             //启用雾效
	glEnable(GL_DEPTH_TEST);          /* 启用深度测试 */
	glPopMatrix();


}