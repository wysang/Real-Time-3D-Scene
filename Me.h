


#include "GL_FrameWork.h"										// 包含基本的框架类
#include "GL_Camera.h"
#include "GL_Terrain.h"
#include "GL_Font.h"
#include "GL_Plant.h"
#include "GL_Flag.h"
#include "GL_House.h"
#include "GL_Water.h"
#include "GL_Fountain.h"

class Me : GL_Application								        // 从GL_Application导出一个子类
{
public:


	bool	Initialize();										// 执行所有的初始化工作，如果成功函数返回true
	void	Deinitialize();										// 执行所有的卸载工作
	void	Update(DWORD milliseconds);							// 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位
	void	Draw();												// 执行所有的绘制操作

	void    CaculateFrameRate();                         //计算帧速
	void    PrintText();                                 //输出文字信息
	
private:
	friend class GL_Application;								// 父类为它的一个友类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name)
	Me(LPCWSTR class_name);						        // 构造函数
	float   m_Fps;                                              // 帧速 
    bool    m_PressF1;                                          //切换全屏的
	bool    m_fog;                                              //启用雾效
	bool    m_renderMode;                                       //切换绘制模式
	bool    fp;      //f键是否按下？
	bool    rp;      //r键是否按下

	GL_Terrain  m_Terrain;   //地形类放在其它类前面至关重要，它必须首先初始化
	                         //才能确保下面的类进行正确的初始化
	                         //第一个

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
