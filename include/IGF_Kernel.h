#ifndef _IGF_COMMAND_H__
#define _IGF_COMMAND_H__
#include "IGF_Base.h"
#include <list>
#include<map>
#ifdef WIN32
#include <Windows.h>
#ifdef GF_COMPONENT_LIB
# define GF_COMPONENT_EXPORT __declspec(dllexport)
#else
# define GF_COMPONENT_EXPORT __declspec(dllimport)
#endif
#else
#define GF_COMPONENT_EXPORT
#endif


//命令钩子接口
#define VER_IGF_CommandSink INTERFACE_VERSION(1,1)
static const char* IID_IGF_CommandSink = "IID_IGF_CommandSink";
class IGF_CommandSink : public IUnknownEx
{
public:
	virtual ~IGF_CommandSink(){}
    /*
    函数名:     ExeCommand
	函数功能:	UI命令执行
	参数:		void* sender, const char* senderName
    参数说明:   sender:命令发送者 
                senderName:发送者名称
	返回值:     无	  
	*/
	virtual void *ExeCommand(void* sender, const char* senderName) = 0;
};

//消息钩子接口
#define VER_IGF_MessageSink INTERFACE_VERSION(1,1)
static const char* IID_IGF_MessageSink= "IID_IGF_MessageSink";

class IGF_MessageSink : public IUnknownEx
{
public:
	virtual ~IGF_MessageSink(){}
     /*
    函数名:     OnMessage
	函数功能:	消息响应
	参数:		void* sender, long msgID, long wParam, long lParam
    参数说明:   sender:命令发送者 
                msgID:消息ID
                wParam:消息参数1
                lParam:消息参数2
	返回值:     无	  
	*/
	virtual void OnMessage(void* sender, long msgID, long wParam, long lParam) = 0;
};

//组件UI接口
#define VER_IGF_ComponentUI INTERFACE_VERSION(1,1)
static const char* IID_IGF_ComponentUI= "IID_IGF_ComponentUI";

class IGF_ComponentUI : public IUnknownEx
{
public:
	virtual ~IGF_ComponentUI(){}
     /*
    函数名:     CreateUI
	函数功能:	用于创建用户界面
	参数:		void* parent, const char* windowName
    参数说明:   parent:父窗口指针 
                windowName:窗口名称
	返回值:     无	  
	*/
	virtual void* CreateUI(void* parent, const char* windowName) = 0;
};

//组件接口
class  IGF_Component : public IUnknownEx
{
public:
	virtual ~IGF_Component(){}
     /*
    函数名:     Initialization
	函数功能:	初始化组件
	参数:		无
    参数说明:   
	返回值:     无	  
	*/
	virtual void			Initialization() = 0;
     /*
    函数名:     Shutdown
	函数功能:	卸载组件
	参数:		无
    参数说明:   
	返回值:     无	  
	*/
	virtual void			Shutdown() = 0;
     /*
    函数名:     GetComponentID
	函数功能:	获取组件ID
	参数:		无
    参数说明:   
	返回值:     const char*	  
	*/
	virtual const char*     GetComponentID() = 0;
     /*
    函数名:     GetComponentName
	函数功能:	获取组件名称
	参数:		无
    参数说明:   
	返回值:     const char*	  
	*/
	/*
	函数名:     SetAllComponent
	函数功能:	设置所有的组件
	参数:		无
	参数说明:   
	返回值:     const char*	  
	*/
	virtual void    SetAllComponent(std::map<std::string, IGF_Component*> &, std::map<std::string, std::string> &) = 0;

	virtual const char*     GetComponentName() = 0;
};

//调度接口
class IUINotifyCrenter;
class IGF_Attemper
{
public:
	virtual ~IGF_Attemper(){}
    /*
    函数名:     LoadInitComponent
	函数功能:	加载系统初始化模块
	参数:		无
    参数说明:   
	返回值:     无
	*/
	virtual void                LoadInitComponent() = 0;
    /*
    函数名:     LoadAllComponent
	函数功能:	加载应用开发组件
	参数:		无
    参数说明:   
	返回值:     无	  
	*/
	virtual void				LoadAllComponent() = 0;
    /*
    函数名:     ShutDownAllComponent
	函数功能:	卸载应用开发组件
	参数:		无
    参数说明:   
	返回值:     无  
	*/
	virtual void				ShutDownAllComponent() = 0;
    /*
    函数名:     GetComponentPtr
	函数功能:	获取组件接口指针
	参数:		const char* componentID
    参数说明:   componentID:组件ID
	返回值:     IGF_Component*	  
	*/
	virtual IGF_Component*		GetComponentPtr(const char* componentID) = 0;
    /*
    函数名:     GetMainWindow
	函数功能:	获取主窗口指针
	参数:		无
    参数说明:   
	返回值:     voidr*	  
	*/
	virtual void*				GetMainWindow() = 0;
    /*
    函数名:     PubMessage
	函数功能:	发送组件消息
	参数:		IGF_Component* IComponent, long msgID, long wParam, long lParam
    参数说明:   IComponent:消息接收者
                msgID:消息接收者
                wParam:消息接收者
                lParam:消息接收者
	返回值:     无	  
	*/
	virtual void				PubMessage(IGF_Component* IComponent, long msgID, long wParam, long lParam) = 0;
    /*
    函数名:     SubMessage
	函数功能:	订阅组件消息
	参数:		IGF_Component* IComponent, long msgID
    参数说明:   IComponent:订阅者
                msgID:订阅消息ID
	返回值:     const char*	  
	*/
	virtual void				SubMessage(IGF_Component* IComponent, long msgID) = 0;
    /*
    函数名:     UnSubMessage
	函数功能:	取消订阅消息
	参数:		long msgID, IGF_Component* IComponent
    参数说明:   msgID:消息ID
                IComponent:订阅者
	返回值:     const char*	  
	*/
	virtual void				UnSubMessage(long msgID, IGF_Component* IComponent) =0;
    /*
    函数名:     QueryInterface
	函数功能:	查询组件接口
	参数:		const char* componentID, const char* IID, unsigned int dwQueryVer
    参数说明:   componentID:组件ID
                IID:组件接口ID
                dwQueryVer:组件版本
	返回值:     void*  
	*/
	virtual void*				QueryInterface(const char* componentID, const char* IID, unsigned int dwQueryVer) = 0;
    /*
    函数名:     GetAllComponentPtr
	函数功能:	获取所有组件接口
	参数:		无
    参数说明:   
	返回值:     std::list<IGF_Component*>  
	*/
	virtual std::list<IGF_Component*>   GetAllComponentPtr() = 0;

	virtual IUINotifyCrenter*   GetUINotifyCenter() = 0;

	/*
	函数名:     GetCurWorkModeID
	函数功能:	获取当前工作模式ID
	参数:		无
	参数说明:   
	返回值:     int，工作模式ID
	*/
	virtual int GetCurWorkModeID() = 0;

	/*
	函数名:     GetCurWorkModeName
	函数功能:	获取当前工作模式名称
	参数:		无
	参数说明:   
	返回值:     char *：工作模式名称
	*/
	virtual const char* GetCurWorkModeName() = 0;

	/*
	函数名:     GetCurWorkModeCfgDirName
	函数功能:	获取当前工作模式配置文件所在目录的名称
	参数:		无
	参数说明:   
	返回值:     char *：当前工作模式配置文件所在目录的名称
	*/
	virtual const char* GetCurWorkModeCfgDirName() = 0;
};


enum WINDOWTYPE
{
	E_MFC_SUBVIEW = 0,
	E_QT_SUBVIEW,
	E_MFC_POPWINDOW_NOMODAL,
	E_MFC_POPWINDOW_MODAL,
	E_QT_POPWINDOW,
	E_QT_STATUSBAR
};

class IUIPlugin
{
public:
	IUIPlugin(){}
	virtual ~IUIPlugin(){}

public:
	virtual long  Shutdown() = 0;
	//释放对象
	virtual void  Release()=0;
	virtual long  CreatePluginWindow(long parentHanle = 0) = 0;
	virtual const char* GetWindowName() = 0;
	virtual WINDOWTYPE  GetWindowType() = 0;
	virtual void    ExeCommand(const char* command) = 0;
};

class  IGF_MainWindow
{
public:
	virtual ~IGF_MainWindow(){}
     /*
    函数名:     Initialization
	函数功能:	初始化主界面
	参数:		无
    参数说明:   
	返回值:     无  
	*/
	virtual void	Initialization() = 0;
     /*
    函数名:     CloseWindow
	函数功能:	关闭主窗口
	参数:		无
    参数说明:   
	返回值:     无  
	*/
    virtual void	CloseWindow() = 0;
     /*
    函数名:     ShowFullScreen
	函数功能:	全屏显示主窗口
	参数:		无
    参数说明:   
	返回值:     无 
	*/
	virtual void	ShowFullScreen()  = 0;
     /*
    函数名:     Resize
	函数功能:	改变主窗口大小
	参数:		int w, int h
    参数说明:   w:宽度 h:高度
	返回值:     无  
	*/
	virtual void    Resize(int w, int h) = 0;
     /*
    函数名:     Show
	函数功能:	显示主窗口
	参数:		无
    参数说明:   
	返回值:     无  
	*/
	virtual void    Show() = 0;
     /*
    函数名:     ShowMaximized
	函数功能:	主窗口最大化
	参数:		无
    参数说明:   
	返回值:     无 
	*/
	virtual void    ShowMaximized() = 0;
     /*
    函数名:     Hide
	函数功能:	隐藏主界面
	参数:		无
    参数说明:   
	返回值:     无  
	*/
	virtual void    Hide() = 0;
     /*
    函数名:     ShowTop
	函数功能:	是否置顶主窗口
	参数:		bool bTop
    参数说明:   bTop:是否置顶
	返回值:     无  
	*/
	virtual void    ShowTop(bool bTop) = 0;
     /*
    函数名:     GetUIObject
	函数功能:	获取UI对象指针，可以用来获取菜单项或ToolBar选项的指针
	参数:		const char* objectName
    参数说明:   objectName:对象名
	返回值:     void*  
	*/
	virtual void*   GetUIObject(const char* objectName) = 0;

	/*
	函数名:     GetStatusBar
	函数功能:	获取状态栏
	参数:		无
	参数说明:   
	返回值:     void*  
	*/
	virtual void*   GetStatusBar() = 0;

     /*
    函数名:     ActiveView
	函数功能:	切换视图
	参数:		const char* viewName
    参数说明:   viewName:视图名
	返回值:     无 
	*/
	virtual void    ActiveView(const char* viewName) = 0;
     /*
    函数名:     UpdateSplashInfo
	函数功能:	更新组件加载信息
	参数:		const char* acInfo
    参数说明:   acInfo:加载信息
	返回值:     无  
	*/
	virtual void    UpdateSplashInfo(const char* acInfo) = 0;
     /*
    函数名:     CreateComponentUI
	函数功能:	获取所有组件接口
	参数:		无
    参数说明:   
	返回值:     std::list<IGF_Component*>  
	*/
	virtual void    CreateComponentUI(const char* xmlName, void* pIComponent) = 0;


	/*
	函数名:     GetMainWndHandle
	函数功能:	获取主窗口
	参数:		无
	参数说明:   
	返回值:     std::list<IGF_Component*>  
	*/
	virtual void*    GetMainWndHandle() = 0;
};



//UI观察接口
class IUIObserver
{
public:
	virtual ~IUIObserver(){}
	virtual void UpdateUI(int nMsgID, int nDataLen, void* pUIData) = 0;
};

//消息分发类接口
class IUINotifyCrenter
{
public:
	virtual ~IUINotifyCrenter(){}

	/*
	**	函数名称：  SuberMessage
	**	功    能： 订阅消息
	**	输    入： int nMsgID, IUIObserver* pObserver
	**	输    出： 无
	*/
	virtual void SuberUIMessage(int nMsgID, IUIObserver* pObserver) = 0;

	/*
	**	函数名称：  UnSuberMessage
	**	功    能： 取消订阅消息
	**	输    入： int nMsgID, IUIObserver* pObserver
	**	输    出： 无
	*/
	virtual void UnSuberUIMessage(int nMsgID, IUIObserver* pObserver) = 0;

	/*
	**	函数名称：  SendMessasge
	**	功    能： 发送消息
	**	输    入： int nMsgID, int nDataLen, void* pUIData
	**	输    出： 无
	*/
	virtual void NotifyUIMessasge(int nMsgID, int nDataLen, void* pUIData) = 0;
};

//组件创建函数
typedef void * (*COMPONENT_CREATEFUNC)(void* pIAttemper);
#endif