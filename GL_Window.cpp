#include "GL_Window.h"									

#include <gl/gl.h>
#include <gl/glu.h>														


GL_Window::GL_Window()													
{
	m_WindowPosX	= 0;												// ���ڵ����Ͻǵ�Xλ��
	m_WindowPosY	= 0;												// ���ڵ����Ͻǵ�Yλ��
	m_WindowWidth	= 800;												// ���ڵĿ��
	m_WindowHeight	= 600;												// ���ڵĸ߶�
	m_ScreenWidth	= 1024;												// ȫ���Ŀ��
	m_ScreenHeight	= 768;												// ȫ���ĸ߶�
	m_BitsPerPixel	= 32;												// ��ɫλ��
	m_IsFullScreen	= false;											// ��ȫ��

	m_hWnd = 0;
	m_hDC = 0;
	m_hRC = 0;
}

// ���ش��ڵĴ�С
int GL_Window::GetWidth()
{
	if (m_IsFullScreen == true)
	{
		return m_ScreenWidth;
	}
	else
	{
		return m_WindowWidth;
	}
}
int GL_Window::GetHeight()
{
	if (m_IsFullScreen == true)
	{
		return m_ScreenHeight;
	}
	else
	{
		return m_WindowHeight;
	}
}

// ���ô��ڵĴ�С
void GL_Window::SetWidth(int width)
{
	if (m_IsFullScreen == true)
	{
		m_ScreenWidth = width;
	}
	else
	{
		m_WindowWidth = width;
	}
}
void GL_Window::SetHeight(int height)
{
	if (m_IsFullScreen == true)
	{
		m_ScreenHeight = height;
	}
	else
	{
		m_WindowHeight = height;
	}
}

// ���ش������Ͻǵ�λ��
int GL_Window::GetPosX()
{
	if (m_IsFullScreen == false)
	{
		return m_WindowPosX;
	}
	return 0;
}
int GL_Window::GetPosY()
{
	if (m_IsFullScreen == false)
	{
		return m_WindowPosY;
	}
	return 0;
}

// ���ô������Ͻǵ�λ��
void GL_Window::SetPosX(int x)
{
	if (m_IsFullScreen == false)
	{
		m_WindowPosX = x;
	}
}
void GL_Window::SetPosY(int y)
{
	if (m_IsFullScreen == false)
	{
		m_WindowPosY = y;
	}
}

// �����ڴ�С�ı�ʱ��֪ͨOpenGL������С
void GL_Window::ReshapeGL()
{
	GLsizei width = GetWidth();
	GLsizei height = GetHeight();
	glViewport(0, 0, width, height);									// ���������ӿ�
	glMatrixMode(GL_PROJECTION);									
	glLoadIdentity();			

	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 1.0f, 20000.0f);	
	glMatrixMode(GL_MODELVIEW);											
	glLoadIdentity();													
}

// �ı䴰�ڵ���ʾ����
bool GL_Window::ChangeScreenResolution()
{
	DEVMODE dmScreenSettings;											// �豸ģʽ
	ZeroMemory(&dmScreenSettings, sizeof(DEVMODE));						// ����ṹ
	dmScreenSettings.dmSize			= sizeof(DEVMODE);					// �ṹ��С
	dmScreenSettings.dmPelsWidth	= GetWidth();						// ���ÿ��
	dmScreenSettings.dmPelsHeight	= GetHeight();						// ���ø߶�
	dmScreenSettings.dmBitsPerPel	= m_BitsPerPixel;					// ����λ���
	dmScreenSettings.dmFields		= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

	// �ı���ʾģʽ
	if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
	{
		return false;													// ���ʧ�ܣ�����false
	}
	return true;														// �ɹ�����
}

// ����windows����
bool GL_Window::Create(const char * window_title, bool full_screen, LPCWSTR class_name, HINSTANCE h_instance, LPVOID lpParam)
{
	m_IsFullScreen = full_screen;										// ���õ�ǰ����ʾģʽ ����/ȫ��

	PIXELFORMATDESCRIPTOR pfd =											// �������������ṹ
	{
		sizeof(PIXELFORMATDESCRIPTOR),									// ���������ṹ�Ĵ�С
		1,																// �汾��
		PFD_DRAW_TO_WINDOW	|											// �������������ʾ��һ��������
		PFD_SUPPORT_OPENGL	|											// ������֧��OpenGL��ͼ
		PFD_STEREO			|											// ��ɫ�����������建��
		PFD_DOUBLEBUFFER,												// ��ɫ��������˫����
		PFD_TYPE_RGBA,													// ʹ��RGBA��ɫ��ʽ
		m_BitsPerPixel,													// ��ɫ����������ɫֵ��ռ��λ��
		0, 0, 0, 0, 0, 0,												// ʹ��Ĭ�ϵ���ɫ����
		1,																// ��Alpha����
		0,																// ��ɫ��������alpha�ɷֵ���λ����
		0,																// ���ۼƻ�����
		0, 0, 0, 0,														// �ۼƻ���������λ
		32,																// 32λ��Ȼ���
		1,																// ���ɰ滺��
		0,																// �޸���������
		PFD_MAIN_PLANE,													// ����ΪPFD_MAIN_PLANE������Ϊ����ͼ��
		0,																// ��ʾOpenGLʵ����֧�ֵ��ϲ���²�ƽ���������
		0, 0, 0															// ��ʱ���Բ���ʹ��
	};

	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// �������Ǵ������ͣ�ʹ�ó����趨
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						

	if (m_IsFullScreen == true)											// ���Ϊȫ��ģʽ������ת��Ϊȫ��ģʽ
	{
		if (ChangeScreenResolution() == false)
		{												
			MessageBoxA(HWND_DESKTOP, "ģʽת��ʧ��.\n�ڴ���ģʽ������.", "Error", MB_OK | MB_ICONEXCLAMATION);
			m_IsFullScreen = false;										// ����Ϊ����ģʽ
		}
		else															// ���Ϊ����ģʽ
		{
			windowStyle = WS_POPUP;										// ���ô���ģʽΪ���㴰��
			windowExtendedStyle |= WS_EX_TOPMOST;						
		}																
	}

	// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
	RECT windowRect = {GetPosX(), GetPosY(), GetPosX() + GetWidth(), GetPosY() + GetHeight()};
	if (m_IsFullScreen == false)										// �ڴ���ģʽ��ʹ��
	{	
		// �������Ǵ��ڵĴ�С��ʹ��ͻ����Ĵ�СΪ�������õĴ�С
		AdjustWindowRectEx(&windowRect, windowStyle, 0, windowExtendedStyle);
		// �жϴ��ڵ����Ͻ��Ƿ�������������
		if (windowRect.left < 0)										// �������X����Ϊ�����ƶ����굽0�������������ڵ�λ��
		{
			windowRect.right -= windowRect.left;						
			windowRect.left = 0;										
		}
		if (windowRect.top < 0)											// �������Y����Ϊ�����ƶ����굽0�������������ڵ�λ��
		{
			windowRect.bottom -= windowRect.top;						
			windowRect.top = 0;											
		}
	}

	// ��������
	m_hWnd = CreateWindowEx(windowExtendedStyle,						// ���ڵ���չ���
							class_name,								    // ���ڵ�����
							(LPCWSTR)window_title,								// ���ڱ���
							windowStyle,								// ���ڵķ��
							windowRect.left, windowRect.top,			// ���ڵ����Ͻ�λ��
							windowRect.right - windowRect.left,			// ���ڵĿ��
							windowRect.bottom - windowRect.top,			// ���ڵĸ߶�
							HWND_DESKTOP,								// ���ڵĸ�����Ϊ����
							0,											// �޲˵�
							h_instance,									// ���봰�ڵ�ʵ�����
							lpParam);									// ������������

	while (m_hWnd != 0)													// �����Ƿ񴴽��ɹ�
	{
		m_hDC = GetDC(m_hWnd);											// ���ش��ڵ��豸������
		if (m_hDC == 0)													// ���Ϊ��
		{																// ʧ��
			break;														
		}

		GLuint PixelFormat = ChoosePixelFormat(m_hDC, &pfd);			// ����һ�����ݵ����ظ�ʽ
		if (PixelFormat == 0)											// ���û�ҵ�
		{																// ʧ��
			break;														
		}
		if (SetPixelFormat(m_hDC, PixelFormat, &pfd) == false)			// �������ظ�ʽ
		{																// ʧ��
			break;														
		}
		m_hRC = wglCreateContext(m_hDC);								// ����OpenGL����Ⱦ������
		if (m_hRC == 0)													// ���Ϊ��
		{																// ʧ��
			break;														
		}
		if (wglMakeCurrent(m_hDC, m_hRC) == false)						// ���õ�ǰ��OpenGL����Ⱦ����Ϊ��ǰ�Ĵ���
		{																// ʧ��
			break;														
		}


		ShowWindow(m_hWnd, SW_NORMAL);									// ��ʾ����
		ShowCursor(false);											    // �������
		ReshapeGL();													// ����OpenGL�������ڴ�С
		return true;													// �ɹ�����
	}																	

	Destroy();															// �ͷ���Դ
	return false;														// ����ʧ��
}

// ɾ��OpenGL����
void GL_Window::Destroy()									
{
	if (m_hWnd != 0)										// ���ھ���Ƿ����
	{
		if (m_hDC != 0)										// �����豸�������Ƿ����
		{
			wglMakeCurrent(m_hDC, 0);						// ���õ�ǰ��OpenGL����Ⱦ����Ϊ��NULL
			if (m_hRC != 0)									// OpenGL����Ⱦ�������Ƿ����
			{
				wglDeleteContext(m_hRC);					// �ͷ�OpenGL����Ⱦ������
				m_hRC = 0;									// ����OpenGL����Ⱦ������Ϊ0
			}
			ReleaseDC(m_hWnd, m_hDC);						// �ͷŴ��ڵ��豸������
			m_hDC = 0;										// ���ô��ڵ��豸������Ϊ0
		}
		DestroyWindow(m_hWnd);								// ɾ������
		m_hWnd = 0;											// ���ô��ھ��Ϊ0
	}

	if (m_IsFullScreen)										// ���Ϊȫ��ģʽ���ڳ�������󣬱任������ģʽ������ʾ���
	{
		ChangeDisplaySettings(NULL, 0);						// �任������ģʽ
		//ShowCursor(true);									// ��ʾ���
	}
}
