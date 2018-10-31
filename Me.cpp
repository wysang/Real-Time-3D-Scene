#include "Me.h"	
#include <time.h>
#pragma comment(lib, "opengl32.lib")		
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")

GL_Application * GL_Application::Create(LPCWSTR class_name)
{
	Me * example = new Me(class_name);
	return reinterpret_cast<GL_Application *>(example);
}

// ���캯��
Me::Me(LPCWSTR class_name) : GL_Application(class_name)
{
	m_Fps = 0;
	m_PressF1= false;                                           //û����f1��
	m_fog    = false;                                           //��ʼû����Ч��
	m_renderMode = false;                                       //��ʼΪ���ģʽ
	fp       = false;                                           //F��
	rp       = false;											//R��
}

bool Me::Initialize()									// ��ʼ��OpenGL
{


	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);				// �ı�OpenGL���ڴ�С��ֱ�ӵ�������ĺ���

    srand((unsigned)time(NULL));  //��ʼ������
    m_Terrain.InitTerrain();    //��ʼ������
	m_House.InitHouse();
    m_Plant.InitPlant();
    m_Flag.InitFlag();
	m_Water.InitWater();

	if(!m_PressF1)
	{
	     /* ��ʼ������ */
	    if(!m_Font.InitFont())
			MessageBoxA(NULL,"��ʼ������ʧ��!","����",MB_OK);
	}

	// ������Ч
	GLuint	fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// ������ʹ洢
    GLuint	fogfilter = 2;								// ���ģ��
    GLfloat	fogColor[4] = {0.5f,0.5f,0.5f,1.0f};        // �����ɫΪ��ɫ

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// �����ɫ��
	glHint(GL_FOG_HINT, GL_DONT_CARE);					// Fog Hint Value
	glFogf(GL_FOG_START, m_TerrainScale);				// Fog Start Depth
	glFogf(GL_FOG_END, m_TerrainScale*12);				// Fog End Depth
	if (m_fog)
		glEnable(GL_FOG);
	else
		glDisable(GL_FOG);
	if(m_renderMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	return true;												// �ɹ�����
}

void Me::Deinitialize()									// �û��Զ����ж�غ���
{
/****************************************************�û��Զ����ж�ع���*******************************************/

	glDisableClientState(GL_VERTEX_ARRAY);          //��������
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	m_Water.m_3ds.Release();  // �ͷ�3dsģ����Դ
	//m_Water.m_3dsxx.Release();
/*******************************************************************************************************************/
}

void Me::Update(DWORD milliseconds)						// ������º���
{
	m_Camera.Update();
    m_Flag.Update();

	if (m_Keys.IsPressed(VK_ESCAPE) == true)					// ��ESC�˳�
	{
		TerminateApplication();									
	}

	//������Ч
	if(m_Keys.IsPressed('F')&&!fp)            
	{
		fp=true;
		m_fog=!m_fog;
		if(m_fog)
			glEnable(GL_FOG);
		else
			glDisable(GL_FOG);

	}
	if(!m_Keys.IsPressed('F'))
		fp=false;

	// �л�����ģʽ
	if(m_Keys.IsPressed('R')&&!rp)        
	{
		rp=true;
		m_renderMode=!m_renderMode;
		if(m_renderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
	if(!m_Keys.IsPressed('R'))
		rp=false;

	
/*******************************************************************************************************************/
}

// ����
void Me::Draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();	
	/* ��������� */	
	m_Camera.Look();

    /* ���ƹ��� */
	pick_x = m_Camera.Position().x;
	pick_z = m_Camera.Position().z;
	printf("%d", pick_x);
	printf("%d", pick_z);
    m_Terrain.CreateSkyBox(0.4f);
    m_Terrain.DrawGround();
	
	m_House.DrawHouse();
	m_Plant.RenderAll();
	m_Flag.DrawFlag();
	m_Water.DrawWater();
	
    /* �����Ļ��Ϣ */ 
	PrintText();

	glFlush();	
}


/* ����֡�� */
void Me::CaculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	     /* ������ʾ֡�� */	
    static float lastTime			= 0.0f;	     /* ��¼�ϴ�ʱ�� */						
    float currentTime = GetTickCount() * 0.001f; /* ��õ�ǰʱ�� */	 			

	framesPerSecond++;                           /* ��ʾ֡������1 */
    /* ���ʱ������1.0�� */
	if( currentTime - lastTime > 1.0f )          
    {
		
	    lastTime = currentTime;                   /* ���浱ǰʱ�� */
		m_Fps = framesPerSecond;                  /* ��ǰ֡������m_Fps */
        framesPerSecond = 0;                      /* ��֡������ */                    
    }

}

/* ���������Ϣ */
void  Me::PrintText()
{
	char string[128];                               /* ���ڱ�������ַ��� */
	glPushAttrib(GL_CURRENT_BIT);                   /* ����������ɫ������Ϣ */
	glColor3f(0.0f,1.0f,1.0f);                      /* ����������ɫΪ��ɫ */
	sprintf_s(string,"X=%3.1f  Z=%3.1f",   
		m_Camera.Position().x,m_Camera.Position().z ,m_Camera.GetSpeed()); /* �ַ�����ֵ */

	m_Font.PrintText(string,-5.5f,3.9f);

	/* ���֡�� */
	glColor3f(0.0f,0.0f,1.0f);
	sprintf(string,"");
    m_Font.PrintText(string, -5.5f,3.4f);

    CaculateFrameRate();                               /* ����֡�� */
    sprintf(string,"FPS:%d",(int)m_Fps);               /* �ַ�����ֵ */
	m_Font.PrintText(string, -5.5f,3.15f);              /* ����ַ��� */

	glPopAttrib();	
}
