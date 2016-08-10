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


//����ӽӿ�
#define VER_IGF_CommandSink INTERFACE_VERSION(1,1)
static const char* IID_IGF_CommandSink = "IID_IGF_CommandSink";
class IGF_CommandSink : public IUnknownEx
{
public:
	virtual ~IGF_CommandSink(){}
    /*
    ������:     ExeCommand
	��������:	UI����ִ��
	����:		void* sender, const char* senderName
    ����˵��:   sender:������� 
                senderName:����������
	����ֵ:     ��	  
	*/
	virtual void *ExeCommand(void* sender, const char* senderName) = 0;
};

//��Ϣ���ӽӿ�
#define VER_IGF_MessageSink INTERFACE_VERSION(1,1)
static const char* IID_IGF_MessageSink= "IID_IGF_MessageSink";

class IGF_MessageSink : public IUnknownEx
{
public:
	virtual ~IGF_MessageSink(){}
     /*
    ������:     OnMessage
	��������:	��Ϣ��Ӧ
	����:		void* sender, long msgID, long wParam, long lParam
    ����˵��:   sender:������� 
                msgID:��ϢID
                wParam:��Ϣ����1
                lParam:��Ϣ����2
	����ֵ:     ��	  
	*/
	virtual void OnMessage(void* sender, long msgID, long wParam, long lParam) = 0;
};

//���UI�ӿ�
#define VER_IGF_ComponentUI INTERFACE_VERSION(1,1)
static const char* IID_IGF_ComponentUI= "IID_IGF_ComponentUI";

class IGF_ComponentUI : public IUnknownEx
{
public:
	virtual ~IGF_ComponentUI(){}
     /*
    ������:     CreateUI
	��������:	���ڴ����û�����
	����:		void* parent, const char* windowName
    ����˵��:   parent:������ָ�� 
                windowName:��������
	����ֵ:     ��	  
	*/
	virtual void* CreateUI(void* parent, const char* windowName) = 0;
};

//����ӿ�
class  IGF_Component : public IUnknownEx
{
public:
	virtual ~IGF_Component(){}
     /*
    ������:     Initialization
	��������:	��ʼ�����
	����:		��
    ����˵��:   
	����ֵ:     ��	  
	*/
	virtual void			Initialization() = 0;
     /*
    ������:     Shutdown
	��������:	ж�����
	����:		��
    ����˵��:   
	����ֵ:     ��	  
	*/
	virtual void			Shutdown() = 0;
     /*
    ������:     GetComponentID
	��������:	��ȡ���ID
	����:		��
    ����˵��:   
	����ֵ:     const char*	  
	*/
	virtual const char*     GetComponentID() = 0;
     /*
    ������:     GetComponentName
	��������:	��ȡ�������
	����:		��
    ����˵��:   
	����ֵ:     const char*	  
	*/
	/*
	������:     SetAllComponent
	��������:	�������е����
	����:		��
	����˵��:   
	����ֵ:     const char*	  
	*/
	virtual void    SetAllComponent(std::map<std::string, IGF_Component*> &, std::map<std::string, std::string> &) = 0;

	virtual const char*     GetComponentName() = 0;
};

//���Ƚӿ�
class IUINotifyCrenter;
class IGF_Attemper
{
public:
	virtual ~IGF_Attemper(){}
    /*
    ������:     LoadInitComponent
	��������:	����ϵͳ��ʼ��ģ��
	����:		��
    ����˵��:   
	����ֵ:     ��
	*/
	virtual void                LoadInitComponent() = 0;
    /*
    ������:     LoadAllComponent
	��������:	����Ӧ�ÿ������
	����:		��
    ����˵��:   
	����ֵ:     ��	  
	*/
	virtual void				LoadAllComponent() = 0;
    /*
    ������:     ShutDownAllComponent
	��������:	ж��Ӧ�ÿ������
	����:		��
    ����˵��:   
	����ֵ:     ��  
	*/
	virtual void				ShutDownAllComponent() = 0;
    /*
    ������:     GetComponentPtr
	��������:	��ȡ����ӿ�ָ��
	����:		const char* componentID
    ����˵��:   componentID:���ID
	����ֵ:     IGF_Component*	  
	*/
	virtual IGF_Component*		GetComponentPtr(const char* componentID) = 0;
    /*
    ������:     GetMainWindow
	��������:	��ȡ������ָ��
	����:		��
    ����˵��:   
	����ֵ:     voidr*	  
	*/
	virtual void*				GetMainWindow() = 0;
    /*
    ������:     PubMessage
	��������:	���������Ϣ
	����:		IGF_Component* IComponent, long msgID, long wParam, long lParam
    ����˵��:   IComponent:��Ϣ������
                msgID:��Ϣ������
                wParam:��Ϣ������
                lParam:��Ϣ������
	����ֵ:     ��	  
	*/
	virtual void				PubMessage(IGF_Component* IComponent, long msgID, long wParam, long lParam) = 0;
    /*
    ������:     SubMessage
	��������:	���������Ϣ
	����:		IGF_Component* IComponent, long msgID
    ����˵��:   IComponent:������
                msgID:������ϢID
	����ֵ:     const char*	  
	*/
	virtual void				SubMessage(IGF_Component* IComponent, long msgID) = 0;
    /*
    ������:     UnSubMessage
	��������:	ȡ��������Ϣ
	����:		long msgID, IGF_Component* IComponent
    ����˵��:   msgID:��ϢID
                IComponent:������
	����ֵ:     const char*	  
	*/
	virtual void				UnSubMessage(long msgID, IGF_Component* IComponent) =0;
    /*
    ������:     QueryInterface
	��������:	��ѯ����ӿ�
	����:		const char* componentID, const char* IID, unsigned int dwQueryVer
    ����˵��:   componentID:���ID
                IID:����ӿ�ID
                dwQueryVer:����汾
	����ֵ:     void*  
	*/
	virtual void*				QueryInterface(const char* componentID, const char* IID, unsigned int dwQueryVer) = 0;
    /*
    ������:     GetAllComponentPtr
	��������:	��ȡ��������ӿ�
	����:		��
    ����˵��:   
	����ֵ:     std::list<IGF_Component*>  
	*/
	virtual std::list<IGF_Component*>   GetAllComponentPtr() = 0;

	virtual IUINotifyCrenter*   GetUINotifyCenter() = 0;

	/*
	������:     GetCurWorkModeID
	��������:	��ȡ��ǰ����ģʽID
	����:		��
	����˵��:   
	����ֵ:     int������ģʽID
	*/
	virtual int GetCurWorkModeID() = 0;

	/*
	������:     GetCurWorkModeName
	��������:	��ȡ��ǰ����ģʽ����
	����:		��
	����˵��:   
	����ֵ:     char *������ģʽ����
	*/
	virtual const char* GetCurWorkModeName() = 0;

	/*
	������:     GetCurWorkModeCfgDirName
	��������:	��ȡ��ǰ����ģʽ�����ļ�����Ŀ¼������
	����:		��
	����˵��:   
	����ֵ:     char *����ǰ����ģʽ�����ļ�����Ŀ¼������
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
	//�ͷŶ���
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
    ������:     Initialization
	��������:	��ʼ��������
	����:		��
    ����˵��:   
	����ֵ:     ��  
	*/
	virtual void	Initialization() = 0;
     /*
    ������:     CloseWindow
	��������:	�ر�������
	����:		��
    ����˵��:   
	����ֵ:     ��  
	*/
    virtual void	CloseWindow() = 0;
     /*
    ������:     ShowFullScreen
	��������:	ȫ����ʾ������
	����:		��
    ����˵��:   
	����ֵ:     �� 
	*/
	virtual void	ShowFullScreen()  = 0;
     /*
    ������:     Resize
	��������:	�ı������ڴ�С
	����:		int w, int h
    ����˵��:   w:��� h:�߶�
	����ֵ:     ��  
	*/
	virtual void    Resize(int w, int h) = 0;
     /*
    ������:     Show
	��������:	��ʾ������
	����:		��
    ����˵��:   
	����ֵ:     ��  
	*/
	virtual void    Show() = 0;
     /*
    ������:     ShowMaximized
	��������:	���������
	����:		��
    ����˵��:   
	����ֵ:     �� 
	*/
	virtual void    ShowMaximized() = 0;
     /*
    ������:     Hide
	��������:	����������
	����:		��
    ����˵��:   
	����ֵ:     ��  
	*/
	virtual void    Hide() = 0;
     /*
    ������:     ShowTop
	��������:	�Ƿ��ö�������
	����:		bool bTop
    ����˵��:   bTop:�Ƿ��ö�
	����ֵ:     ��  
	*/
	virtual void    ShowTop(bool bTop) = 0;
     /*
    ������:     GetUIObject
	��������:	��ȡUI����ָ�룬����������ȡ�˵����ToolBarѡ���ָ��
	����:		const char* objectName
    ����˵��:   objectName:������
	����ֵ:     void*  
	*/
	virtual void*   GetUIObject(const char* objectName) = 0;

	/*
	������:     GetStatusBar
	��������:	��ȡ״̬��
	����:		��
	����˵��:   
	����ֵ:     void*  
	*/
	virtual void*   GetStatusBar() = 0;

     /*
    ������:     ActiveView
	��������:	�л���ͼ
	����:		const char* viewName
    ����˵��:   viewName:��ͼ��
	����ֵ:     �� 
	*/
	virtual void    ActiveView(const char* viewName) = 0;
     /*
    ������:     UpdateSplashInfo
	��������:	�������������Ϣ
	����:		const char* acInfo
    ����˵��:   acInfo:������Ϣ
	����ֵ:     ��  
	*/
	virtual void    UpdateSplashInfo(const char* acInfo) = 0;
     /*
    ������:     CreateComponentUI
	��������:	��ȡ��������ӿ�
	����:		��
    ����˵��:   
	����ֵ:     std::list<IGF_Component*>  
	*/
	virtual void    CreateComponentUI(const char* xmlName, void* pIComponent) = 0;


	/*
	������:     GetMainWndHandle
	��������:	��ȡ������
	����:		��
	����˵��:   
	����ֵ:     std::list<IGF_Component*>  
	*/
	virtual void*    GetMainWndHandle() = 0;
};



//UI�۲�ӿ�
class IUIObserver
{
public:
	virtual ~IUIObserver(){}
	virtual void UpdateUI(int nMsgID, int nDataLen, void* pUIData) = 0;
};

//��Ϣ�ַ���ӿ�
class IUINotifyCrenter
{
public:
	virtual ~IUINotifyCrenter(){}

	/*
	**	�������ƣ�  SuberMessage
	**	��    �ܣ� ������Ϣ
	**	��    �룺 int nMsgID, IUIObserver* pObserver
	**	��    ���� ��
	*/
	virtual void SuberUIMessage(int nMsgID, IUIObserver* pObserver) = 0;

	/*
	**	�������ƣ�  UnSuberMessage
	**	��    �ܣ� ȡ��������Ϣ
	**	��    �룺 int nMsgID, IUIObserver* pObserver
	**	��    ���� ��
	*/
	virtual void UnSuberUIMessage(int nMsgID, IUIObserver* pObserver) = 0;

	/*
	**	�������ƣ�  SendMessasge
	**	��    �ܣ� ������Ϣ
	**	��    �룺 int nMsgID, int nDataLen, void* pUIData
	**	��    ���� ��
	*/
	virtual void NotifyUIMessasge(int nMsgID, int nDataLen, void* pUIData) = 0;
};

//�����������
typedef void * (*COMPONENT_CREATEFUNC)(void* pIAttemper);
#endif