


#include "GL_FrameWork.h"										// ���������Ŀ����
#include "GL_Camera.h"
#include "GL_Terrain.h"
#include "GL_Font.h"
#include "GL_Plant.h"
#include "GL_Flag.h"
#include "GL_House.h"
#include "GL_Water.h"
#include "GL_Fountain.h"

class Me : GL_Application								        // ��GL_Application����һ������
{
public:


	bool	Initialize();										// ִ�����еĳ�ʼ������������ɹ���������true
	void	Deinitialize();										// ִ�����е�ж�ع���
	void	Update(DWORD milliseconds);							// ִ�����еĸ��²���������Ĳ���Ϊ���β���������ʱ�䣬�Ժ���Ϊ��λ
	void	Draw();												// ִ�����еĻ��Ʋ���

	void    CaculateFrameRate();                         //����֡��
	void    PrintText();                                 //���������Ϣ
	
private:
	friend class GL_Application;								// ����Ϊ����һ�����࣬�����������������ʵ����������GL_Application * GL_Application::Create(const char * class_name)
	Me(LPCWSTR class_name);						        // ���캯��
	float   m_Fps;                                              // ֡�� 
    bool    m_PressF1;                                          //�л�ȫ����
	bool    m_fog;                                              //������Ч
	bool    m_renderMode;                                       //�л�����ģʽ
	bool    fp;      //f���Ƿ��£�
	bool    rp;      //r���Ƿ���

	GL_Terrain  m_Terrain;   //���������������ǰ��������Ҫ�����������ȳ�ʼ��
	                         //����ȷ��������������ȷ�ĳ�ʼ��
	                         //��һ��

	GL_House    m_House;
	GL_Plant    m_Plant;
	GL_Flag     m_Flag;

    
	GL_Camera   m_Camera;
	GL_Water    m_Water;
	GL_Font     m_Font;
};

static bool pick=0;
static float pick_x;
static float pick_z;
