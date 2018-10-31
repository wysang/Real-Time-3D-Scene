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

// 构造函数
Me::Me(LPCWSTR class_name) : GL_Application(class_name)
{
	m_Fps = 0;
	m_PressF1= false;                                           //没按过f1键
	m_fog    = false;                                           //开始没有雾效的
	m_renderMode = false;                                       //开始为填充模式
	fp       = false;                                           //F键
	rp       = false;											//R键
}

bool Me::Initialize()									// 初始化OpenGL
{


	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);				// 改变OpenGL窗口大小，直接调用子类的函数

    srand((unsigned)time(NULL));  //初始化种子
    m_Terrain.InitTerrain();    //初始化地形
	m_House.InitHouse();
    m_Plant.InitPlant();
    m_Flag.InitFlag();
	m_Water.InitWater();

	if(!m_PressF1)
	{
	     /* 初始化字体 */
	    if(!m_Font.InitFont())
			MessageBoxA(NULL,"初始化字体失败!","错误",MB_OK);
	}

	// 加入雾效
	GLuint	fogMode[]= { GL_EXP, GL_EXP2, GL_LINEAR };	// 雾的类型存储
    GLuint	fogfilter = 2;								// 雾的模型
    GLfloat	fogColor[4] = {0.5f,0.5f,0.5f,1.0f};        // 雾的颜色为白色

	glFogi(GL_FOG_MODE, fogMode[fogfilter]);			// Fog Mode
	glFogfv(GL_FOG_COLOR, fogColor);					// 雾的颜色绑定
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

	return true;												// 成功返回
}

void Me::Deinitialize()									// 用户自定义的卸载函数
{
/****************************************************用户自定义的卸载过程*******************************************/

	glDisableClientState(GL_VERTEX_ARRAY);          //禁用数组
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	
	m_Water.m_3ds.Release();  // 释放3ds模型资源
	//m_Water.m_3dsxx.Release();
/*******************************************************************************************************************/
}

void Me::Update(DWORD milliseconds)						// 程序更新函数
{
	m_Camera.Update();
    m_Flag.Update();

	if (m_Keys.IsPressed(VK_ESCAPE) == true)					// 按ESC退出
	{
		TerminateApplication();									
	}

	//启用雾效
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

	// 切换绘制模式
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

// 绘制
void Me::Draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();	
	/* 放置摄像机 */	
	m_Camera.Look();

    /* 绘制过程 */
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
	
    /* 输出屏幕信息 */ 
	PrintText();

	glFlush();	
}


/* 计算帧速 */
void Me::CaculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	     /* 保存显示帧数 */	
    static float lastTime			= 0.0f;	     /* 记录上次时间 */						
    float currentTime = GetTickCount() * 0.001f; /* 获得当前时间 */	 			

	framesPerSecond++;                           /* 显示帧数递增1 */
    /* 如果时间差大于1.0秒 */
	if( currentTime - lastTime > 1.0f )          
    {
		
	    lastTime = currentTime;                   /* 保存当前时间 */
		m_Fps = framesPerSecond;                  /* 当前帧数传给m_Fps */
        framesPerSecond = 0;                      /* 将帧数置零 */                    
    }

}

/* 输出文字信息 */
void  Me::PrintText()
{
	char string[128];                               /* 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                   /* 保存现有颜色属性信息 */
	glColor3f(0.0f,1.0f,1.0f);                      /* 设置文字颜色为青色 */
	sprintf_s(string,"X=%3.1f  Z=%3.1f",   
		m_Camera.Position().x,m_Camera.Position().z ,m_Camera.GetSpeed()); /* 字符串赋值 */

	m_Font.PrintText(string,-5.5f,3.9f);

	/* 输出帧速 */
	glColor3f(0.0f,0.0f,1.0f);
	sprintf(string,"");
    m_Font.PrintText(string, -5.5f,3.4f);

    CaculateFrameRate();                               /* 计算帧速 */
    sprintf(string,"FPS:%d",(int)m_Fps);               /* 字符串赋值 */
	m_Font.PrintText(string, -5.5f,3.15f);              /* 输出字符串 */

	glPopAttrib();	
}
