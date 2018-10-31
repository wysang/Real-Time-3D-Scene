#ifndef	GL_FRAMEWORK_H
#define	GL_FRAMEWORK_H


#include "GL_Window.h"											// ����GL_Window.hͷ�ļ�


class Keys														// ���������
{
public:
	// ���캯��
	Keys() { Clear(); }
	// ������еİ�����Ϣ
	void Clear() { ZeroMemory(&m_KeyDown, sizeof(m_KeyDown)); }
	// �ж�ĳ�����Ƿ���
	bool IsPressed(unsigned int key)	{ return (key < MAX_KEYS) ? (m_KeyDown[key] == true) : false; }
	// ����ĳ����������
	void SetPressed(unsigned int key)	{ if (key < MAX_KEYS) m_KeyDown[key] = true; }
	// ����ĳ�������ͷ�
	void SetReleased(unsigned int key)	{ if (key < MAX_KEYS) m_KeyDown[key] = false; }

private:
	//static const unsigned int MAX_KEYS = 256;
	enum{MAX_KEYS = 256};
	bool m_KeyDown[MAX_KEYS];									// ����256��������״̬
};


class GL_Application											// �����ĳ����࣬�̳�����������OpenGL����
{
public:
/**************************************************************************************************************/
	// ����һ��ȫ�ֵ�Create����������������뱻�̳���ʵ��
	static GL_Application * Create(LPCWSTR class_name);	// �������Լ�������
/**************************************************************************************************************/
	// �յ���������
	virtual ~GL_Application() {}

protected:
/**************************************************************************************************************/
	// ����ĺ������뱻�̳���ʵ�֣���ɻ�����OpenGL��Ⱦ����
	virtual bool	Initialize() = 0;							// OpenGL�ĳ�ʼ��
	virtual void	Deinitialize() = 0;							// OpenGL��ж��
	virtual void	Update(DWORD milliseconds) = 0;				// ִ��OpenGL����ĸ���
	virtual void	Draw() = 0;									// ����OpenGL����
/**************************************************************************************************************/
	// ͨ�õĺ���
	void	ToggleFullscreen();									// �л� ȫ��/����ģʽ
	void	TerminateApplication();								// ��������
	void	ResizeDraw(bool enable) { m_ResizeDraw = enable; }	// �����ڴ��ڸı��С��ʱ�򣬿��Ի���

	Keys	m_Keys;												// ������ 
	// ���캯��
	GL_Application(LPCWSTR class_name);					

private:
	// �������ѭ��
	friend int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	int		Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
	// ��Ϣ����ص�����
	friend LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT	Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	//static const UINT WM_TOGGLEFULLSCREEN = (WM_USER + 1);		// �Զ�����Ϣ�����л�����ģʽ��ʱ����
    static const UINT WM_TOGGLEFULLSCREEN;
	GL_Window	m_Window;										// Window��
	LPCWSTR     m_ClassName;									// ������
	bool		m_IsProgramLooping;								// ����ѭ����ǣ����Ϊfalse�����˳�����
	bool		m_CreateFullScreen;								// ���ΪTRUE���򴴽�ȫ��ģʽ
	bool		m_IsVisible;									// �����Ƿ�ɼ�
	bool		m_ResizeDraw;									// �Ƿ��ڸı��Сʱ�������˻��ƺ���
	DWORD		m_LastTickCount;								// ��һ�μ�ʱ����ֵ
};

#endif	// GL_FRAMEWORK_H