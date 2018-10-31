#ifndef	GL_WINDOW_H
#define	GL_WINDOW_H

#include <windows.h>													// 包含windows.h头文件

class GL_Window															// windows类
{
public:
/**********************************************重要的窗口函数****************************************************/
	// 构造函数
	GL_Window();

	// 创建windows窗口
	bool	Create(const char * window_title, bool full_screen, LPCWSTR class_name, HINSTANCE h_instance, LPVOID lpParam);
	// 删除OpenGL窗口
	void	Destroy();
	// 改变窗口的显示设置
	bool	ChangeScreenResolution();
	// 当窗口大小改变时，通知OpenGL调整大小
	void	ReshapeGL();
	// Swap Buffers (Double Buffering)
	void	SwapBuffers() { ::SwapBuffers(m_hDC); }

/**************************************下面函数为设置/返回窗口类的属性********************************************/
	// 设置窗口左上角的位置
	void	SetPosX(int x);
	void	SetPosX(unsigned short x) { SetPosX((signed short)x); }		
	void	SetPosY(int y);
	void	SetPosY(unsigned short y) { SetPosY((signed short)y); }		
	// 返回窗口的大小
	int		GetWidth();
	int		GetHeight();
	// 设置窗口的大小
	void	SetWidth(int width);
	void	SetHeight(int height);
	// 返回窗口左上角的位置
	int		GetPosX();
	int		GetPosY();
	// 设置窗口的颜色位深
	void	SetHiColor()	{ m_BitsPerPixel = 16; }
	void	SetTrueColor()	{ m_BitsPerPixel = 32; }
	// 运算符转换，可以让GL_Window m_Window声明后的m_Window作为窗口句柄使用
	operator HWND() { return m_hWnd; }

private:
	HWND	m_hWnd;														// 窗口句柄
	HDC		m_hDC;														// 设备描述表
	HGLRC	m_hRC;														// OpenGL渲染描述表

	int		m_WindowPosX;												// 窗口的左上角的X位置
	int		m_WindowPosY;												// 窗口的左上角的Y位置
	int		m_WindowWidth;												// 窗口的宽度
	int		m_WindowHeight;												// 窗口的高度
	int		m_ScreenWidth;												// 全屏的宽度
	int		m_ScreenHeight;												// 全屏的高度
	int		m_BitsPerPixel;												// 颜色位深
	bool	m_IsFullScreen;												// 是否全屏
};

#endif	// GL_WINDOW_H