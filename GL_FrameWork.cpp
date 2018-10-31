#include "GL_FrameWork.h"

#include <gl/gl.h>
#include <gl/glu.h>														

const UINT GL_Application::WM_TOGGLEFULLSCREEN = (WM_USER + 1);

// ���������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int ret = -1;
	GL_Application * appl = GL_Application::Create(TEXT("minecraft"));			// ����������
	if (appl != 0)
	{
		ret = appl->Main(hInstance, hPrevInstance, lpCmdLine, nCmdShow);// ִ�г�����ѭ��
		delete appl;													// ɾ�������ࣨ�ڼ̳����У�ʹ��Me * example = new Me(class_name);������һ���ڴ棩
	}
	else
	{																	// �����������
		MessageBoxA(HWND_DESKTOP, "�����������", "Error", MB_OK | MB_ICONEXCLAMATION);
	}
	return ret;
}																		

// ��������Ϣ
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LONG user_data = GetWindowLong(hWnd, GWL_USERDATA);					// �����û��Զ����32λ�ĳ��򸽼�ֵ
	if (user_data == 0)
	{	
		// ��������һ������
		if (uMsg == WM_CREATE)											// �����ڴ�����Ϣ
		{																
			// ���ش��ڽṹ��ָ�룬������մ����ĳ���ʵ������
			CREATESTRUCT * creation = reinterpret_cast<CREATESTRUCT *>(lParam);
			// ��ó���ʵ����ָ��
			GL_Application * appl = reinterpret_cast<GL_Application *>(creation->lpCreateParams);
			// �������ʵ����ָ��Ϊ�û��Զ���ĳ��򸽼�ֵ
			SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(appl));
			appl->m_IsVisible = true;									// ���ó���ɼ�
			return 0;													// ����
		}
	}
	else
	{	// ������ǵ�һ�δ��ڣ����س���ʵ����ָ��
		GL_Application * appl = reinterpret_cast<GL_Application *>(user_data);
		return appl->Message(hWnd, uMsg, wParam, lParam);				// ���ó���ʵ���Լ�����Ϣ������
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);					// ����Ĭ�ϵĴ�����Ϣ������
}


// ���캯��
GL_Application::GL_Application(LPCWSTR class_name)					
{
	m_ClassName = (LPCWSTR)class_name;									// ��������
	m_IsProgramLooping = true;											// ���ó���ѭ��Ϊtrue
	m_CreateFullScreen = false;											// ʹ��ȫ��ģʽ
	m_IsVisible = false;												// ���ڲ��ɼ�
	m_ResizeDraw = false;												// �ڴ��ڸı��С��ʱ�򣬲��ɻ���
	m_LastTickCount = 0;
}

void GL_Application::ToggleFullscreen()									// �л� ȫ��/����ģʽ
{
	PostMessage(m_Window, WM_TOGGLEFULLSCREEN, 0, 0);					// �����Զ����л���Ϣ
}

void GL_Application::TerminateApplication()								// ��������
{
	PostMessage(m_Window, WM_QUIT, 0, 0);								// �����˳���Ϣ
	m_IsProgramLooping = false;											// ֹͣ����ѭ��
}

// ��Ϣѭ��
LRESULT GL_Application::Message(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)														// ����ͬ��Windows��Ϣ
	{
		case WM_SYSCOMMAND:												// �ػ�ϵͳ����
			switch (wParam)												
			{
				case SC_SCREENSAVE:										// �ػ���Ļ������������
				case SC_MONITORPOWER:									// �ػ���ʾ��ʡ��ģʽ��������
					return 0;											// ����������������
				break;
			}
		break;															// �˳�

		case WM_CLOSE:													// �رմ���
			TerminateApplication();										// ����TerminateApplication����
			return 0;													
		break;

		case WM_EXITMENULOOP:
		case WM_EXITSIZEMOVE:
			m_LastTickCount = GetTickCount();							// ���¼�������ֵ
			return 0;
		break;

		case WM_MOVE:
			m_Window.SetPosX(LOWORD(lParam));							// ������������
			m_Window.SetPosY(HIWORD(lParam));							
			return 0;
		break;

		case WM_PAINT:
			if (m_ResizeDraw == true)									// �����Ҫ�ػ�
			{
				m_Window.ReshapeGL();									// �������ô��ڵĴ�С
				Draw();													// ���»���
				m_Window.SwapBuffers();									// ����ǰ��֡����
			}
		break;

		case WM_SIZING:													// �������ڸı��С
		{
			RECT * rect = (RECT *)lParam;
			m_Window.SetWidth(rect->right - rect->left);				// ���ô��ڿ��
			m_Window.SetHeight(rect->bottom - rect->top);				// ���ô��ڸ߶�
			return TRUE;
		}
		break;

		case WM_SIZE:													// ���ڸı��С��
			switch (wParam)												// ����ͬ�Ĵ���״̬
			{
				case SIZE_MINIMIZED:									// �Ƿ���С��?
					m_IsVisible = false;								// ����ǣ������ò��ɼ�
					return 0;											
				break;

				case SIZE_MAXIMIZED:									// �����Ƿ����?
				case SIZE_RESTORED:										// ���ڱ���ԭ?
					m_IsVisible = true;									// ����Ϊ�ɼ�
					m_Window.SetWidth(LOWORD(lParam));					// ���ô��ڿ��
					m_Window.SetHeight(HIWORD(lParam));					// ���ô��ڸ߶�
					m_Window.ReshapeGL();								// �ı䴰�ڴ�С
					m_LastTickCount = GetTickCount();					// ���¼�������ֵ
					return 0;											
				break;
			}
		break;															

		case WM_KEYDOWN:												// ���°�����Ϣ
			m_Keys.SetPressed(wParam);									
			return 0;													
		break;

		case WM_KEYUP:													// �����ͷż���Ϣ
			m_Keys.SetReleased(wParam);									
			return 0;													
		break;

		case WM_TOGGLEFULLSCREEN:										// �л� ȫ��/����ģʽ
			m_CreateFullScreen = !m_CreateFullScreen;
			PostMessage(hWnd, WM_QUIT, 0, 0);
		break;															
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);					// ����Ĭ�ϵĴ�����Ϣ������
}

// �������ѭ��
int GL_Application::Main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ע��һ������
	WNDCLASSEX windowClass;												// ������
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));						// ��սṹΪ0
	windowClass.cbSize			= sizeof(WNDCLASSEX);					// ���ڽṹ�Ĵ�С
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// ���ô�������Ϊ���ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// WndProc������Ϣ
	windowClass.hInstance		= hInstance;							// ����ʵ��
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// ���ñ���
	windowClass.hCursor			= LoadCursor(NULL, IDC_CROSS);			// ������
	windowClass.lpszClassName	= m_ClassName;							// ��������
	if (RegisterClassEx(&windowClass) == 0)								// ����ע�ᴰ����
	{																	// NOTE: Failure, Should Never Happen
		MessageBoxA(HWND_DESKTOP, "ע�ᴰ��ʧ��!", "Error", MB_OK | MB_ICONEXCLAMATION);
		return -1;														// �˳�������FALSE
	}

	// ѯ���Ƿ���ȫ��״̬������?
	//if (MessageBoxA(HWND_DESKTOP, "������ȫ��״̬������ô ?", "Start FullScreen ?", MB_YESNO | MB_ICONQUESTION) == IDNO)
	//{
	//	m_CreateFullScreen = false;										// m_CreateFullScreen��¼��ǰ����ʾģʽΪ����
	//}

	while (m_IsProgramLooping)											// ѭ��ֱ��WM_QUIT�˳�����
	{																	
		// ����һ������
		if (m_Window.Create("Minecraft", m_CreateFullScreen, m_ClassName, hInstance, this) == true)
		{	
			// �����ʼ��ʧ�ܣ����˳�
			if (Initialize() == false)									// �����Զ���ĳ�ʼ������
			{															// ʧ��
				TerminateApplication();									// �رմ����˳�����
			}
			else														// �ɹ���ʼ��Ϣѭ��
			{															
				MSG msg;												// Window��Ϣ�ṹ
				bool isMessagePumpActive = true;						// ����Ϣ��Ϊ��ʱ��������Ϣѭ��
				m_LastTickCount = GetTickCount();						// ���ص�ǰ�ļ�ʱ����ֵ
				m_Keys.Clear();											// ������еİ�����Ϣ
				while (isMessagePumpActive == true)						// ����Ϣ��Ϊ��ʱ��������Ϣѭ��
				{														// �ɹ��������ڣ���ⴰ����Ϣ
					if (PeekMessage(&msg, m_Window, 0, 0, PM_REMOVE) != 0)
					{													
						// ����Ƿ�ΪWM_QUIT��Ϣ
						if (msg.message != WM_QUIT)						
						{
							DispatchMessage(&msg);						// ������Ƿ�����Ϣ����Ϣ�ص������д���
						}
						else											
						{
							isMessagePumpActive = false;				// ����ǣ����˳�
						}
					}
					// ���û����Ϣ
					else												
					{
						if (m_IsVisible == false)						// ������ڲ��ɼ�
						{
							WaitMessage();								// ��ͣ���򣬵ȴ���Ϣ
						}
						else											// ������ڿɼ�
						{												// ִ����Ϣѭ��
							DWORD tickCount = GetTickCount();			// ���ؼ�ʱ���ĵ�ǰֵ
							Update(tickCount - m_LastTickCount);		// �����û��Զ���ĸ��º���
							m_LastTickCount = tickCount;				// ����������һ�Σ���������ֵ
							Draw();										// �����û��Զ���Ļ��ƺ���
							m_Window.SwapBuffers();						// ����ǰ��֡����
						}
					}
				}														// ���isMessagePumpActive == true����ѭ��
			}															
																		// �������
			Deinitialize();												// �û��Զ����ж�غ���
			m_Window.Destroy();											// ɾ������
		}
		else															// �����������ʧ��
		{																
			MessageBoxA(HWND_DESKTOP, "����OpenGL���ڴ���", "Error", MB_OK | MB_ICONEXCLAMATION);
			m_IsProgramLooping = false;									// ֹͣ����ѭ��
		}
	}																	

	UnregisterClass(m_ClassName, hInstance);							// ȡ��ע��Ĵ���
	return 0;
}
