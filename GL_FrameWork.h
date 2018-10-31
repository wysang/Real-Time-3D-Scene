#ifndef	GL_FRAMEWORK_H
#define	GL_FRAMEWORK_H


#include "GL_Window.h"											// 包含GL_Window.h头文件


class Keys														// 定义键盘类
{
public:
	// 构造函数
	Keys() { Clear(); }
	// 清空所有的按键信息
	void Clear() { ZeroMemory(&m_KeyDown, sizeof(m_KeyDown)); }
	// 判断某个键是否按下
	bool IsPressed(unsigned int key)	{ return (key < MAX_KEYS) ? (m_KeyDown[key] == true) : false; }
	// 设置某个键被按下
	void SetPressed(unsigned int key)	{ if (key < MAX_KEYS) m_KeyDown[key] = true; }
	// 设置某个键被释放
	void SetReleased(unsigned int key)	{ if (key < MAX_KEYS) m_KeyDown[key] = false; }

private:
	//static const unsigned int MAX_KEYS = 256;
	enum{MAX_KEYS = 256};
	bool m_KeyDown[MAX_KEYS];									// 保存256个按键的状态
};


class GL_Application											// 基本的程序类，继承它用来创建OpenGL程序
{
public:
/**************************************************************************************************************/
	// 创建一个全局的Create函数，这个函数必须被继承类实现
	static GL_Application * Create(LPCWSTR class_name);	// 创建你自己的子类
/**************************************************************************************************************/
	// 空的析构函数
	virtual ~GL_Application() {}

protected:
/**************************************************************************************************************/
	// 下面的函数必须被继承类实现，完成基本的OpenGL渲染过程
	virtual bool	Initialize() = 0;							// OpenGL的初始化
	virtual void	Deinitialize() = 0;							// OpenGL的卸载
	virtual void	Update(DWORD milliseconds) = 0;				// 执行OpenGL程序的更新
	virtual void	Draw() = 0;									// 绘制OpenGL场景
/**************************************************************************************************************/
	// 通用的函数
	void	ToggleFullscreen();									// 切换 全屏/窗口模式
	void	TerminateApplication();								// 结束程序
	void	ResizeDraw(bool enable) { m_ResizeDraw = enable; }	// 设置在窗口改变大小的时候，可以绘制

	Keys	m_Keys;												// 按键类 
	// 构造函数
	GL_Application(LPCWSTR class_name);					

private:
	// 程序的主循环
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	int		Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	// 消息处理回调函数
	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT	Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//static const UINT WM_TOGGLEFULLSCREEN = (WM_USER + 1);		// 自定义消息，在切换窗口模式的时候发送
    static const UINT WM_TOGGLEFULLSCREEN;
	GL_Window	m_Window;										// Window类
	LPCWSTR     m_ClassName;									// 程序名
	bool		m_IsProgramLooping;								// 程序循环标记，如果为false，则退出程序
	bool		m_CreateFullScreen;								// 如果为TRUE，则创建全屏模式
	bool		m_IsVisible;									// 窗口是否可见
	bool		m_ResizeDraw;									// 是否在改变大小时，调用了绘制函数
	DWORD		m_LastTickCount;								// 上一次计时器的值
};

#endif	// GL_FRAMEWORK_H