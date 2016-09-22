#ifndef JFRAME_FACADE_H
#define JFRAME_FACADE_H

#include <string>
#include <list>

// ��ܲ�Ʒ�汾
#define JFRAME_MAJOR_VERSION 4      /**< ������汾�� */
#define JFRAME_MINOR_VERSION 0      /**< ��ܴΰ汾�� */
#define JFRAME_PATCH_VERSION 0      /**< ��ܲ����汾�� */
#define JFRAME_BUILD_VERSION 0      /**< ��ܱ���汾�� */

/**
 * @brief ��ȡ���汾
 * @param ver : ��ܰ汾��
 * @return int����ֵ
 */
#define J_MAJOR_VERSION(ver) \
    ((((unsigned int)(ver)) & 0xff000000) >> 24)

/**
 * @brief ��ȡ�ΰ汾
 * @param ver : ��ܰ汾��
 * @return int����ֵ
 */
#define J_MINOR_VERSION(ver) \
    ((((unsigned int)(ver)) & 0x00ff0000) >> 16)

/**
 * @brief ��ȡ�����汾
 * @param ver : ��ܰ汾��
 * @return int����ֵ
 */
#define J_PATCH_VERSION(ver) \
    ((((unsigned int)(ver)) & 0x0000ff00) >> 8)

/**
 * @brief ��ȡ�ڲ��汾
 * @param ver : ��ܰ汾��
 * @return int����ֵ
 */
#define J_BUILD_VERSION(ver) \
    (((unsigned int)(ver)) & 0x000000ff)

/**
 * @brief ���ɽӿڰ汾
 * @param major : ���汾��
 * @param minor : �ΰ汾��
 * @return int����ֵ
 */
#define J_INTERFACE_VERSION(major, minor) \
    ((((unsigned int)(JFRAME_MAJOR_VERSION)) << 24) | \
    ((( unsigned int)(major)) << 16) | \
    ((  unsigned int)(minor)  << 8))

/**
 * @brief ���ɽ��̰汾
 * @param major : ���汾��
 * @param minor : �ΰ汾��
 * @param patch : �޶��汾��
 * @return int����ֵ
 */
#define J_PROCESS_VERSION(major, minor, patch) \
    ((((unsigned int)(JFRAME_MAJOR_VERSION)) << 24) | \
    ((( unsigned int)(major)) << 16) | \
    ((  unsigned int)(minor)  << 8) | \
    (   unsigned int)(patch))

#ifndef J_IID_INTERFACE
/**
 * @brief �ӿڱ�ʶ����
 * @param interface : �ӿ�����
 * @return char*����ֵ
 */
#define J_IID_INTERFACE(interface)  "IID" # interface
#endif

/**
 * @brief �ӿڱ�ʶ�ж�
 * @param interface : �ӿ�����
 * @param iid : �ӿڱ�ʶ
 * @param ver : �ӿڰ汾
 * @return bool����ֵ
 */
#define J_IS_INSTANCEOF(interface, iid, ver) \
    (((iid) == IID_ ## interface) \
    && ((ver) == VER_ ## interface))

/**
 * @brief �ӿڶ����ʶ�ж�
 * @param interface : �ӿ�����
 * @param iid : �ӿڱ�ʶ
 * @param ver : �ӿڰ汾
 * @return bool����ֵ
 */
#define J_IS_INTERFACEOF(interface, iid, ver) \
    (((iid) == (interface)->interfaceIdentity()) \
    && ((ver) == (interface)->interfaceVersion()))

/**
 * @brief �ڵ�ǰ�ӿ��Ӷ����в�ѯ�ӿ�
 * @param interface : �ӿ�����
 * @param iid : �ӿڱ�ʶ
 * @param ver : �ӿڰ汾
 */
#define J_QUERY_INTERFACE(interface, iid, ver) \
    do { \
        if (J_IS_INSTANCEOF(interface, iid, ver)) \
        { return static_cast<interface *>(this); } \
    } while(0)

/**
 * @brief �ڸ����ӿ��Ӷ����в�ѯ�ӿ�
 * @param interface : �ӿ�����
 * @param iid : �ӿڱ�ʶ
 * @param ver : �ӿڰ汾
 */
#define J_QUERY_IJUNKNOWN(interface, iid, ver) \
    do { \
        if (J_IS_INSTANCEOF(IJUnknown, iid, ver)) \
        { return static_cast<IJUnknown *>(static_cast<interface *>(this)); } \
    } while(0)

/**
 * @brief ��ѯ��Ա����ʵ��
 * @param interface : �ӿ�����
 * @param iid : �ӿڱ�ʶ
 * @param ver : �ӿڰ汾
 * @param object : ��Ա����
 */
#define J_QUERY_MEMBER_OBJECT(interface, iid, ver, object) \
    do { \
        if ((object) != 0) { \
            if (J_IS_INSTANCEOF(interface, iid, ver)) \
            { return static_cast<interface *>(object); } \
        } \
    } while(0)

/**
 * @brief ��ǰ�ӿڲ�ѯ
 * @param interface : �ӿ�����
 */
#define J_QUERY_THIS_INTERFACE(interface) \
    ((interface *)queryInterface(IID_ ## interface, VER_ ## interface))

/**
 * @brief ���������ò�ѯ
 * @param object : ��ѯ����
 * @param interface : �ӿ�����
 */
#define J_QUERY_OBJECT_INTERFACE(object, interface) \
    ((interface *)object.queryInterface(IID_ ## interface, VER_ ## interface))

/**
 * @brief ��������ָ���ѯ
 * @param object : ��ѯ����
 * @param interface : �ӿ�����
 */
#define J_QUERY_POBJECT_INTERFACE(object, interface) \
    do { \
        if (object != 0) { \
            interface *_t = ((_interface_ *)object->queryInterface( \
            IID_ ## interface, VER_ ## interface)); \
            if (_t) { return _t_; } \
        } \
    } while(0)

//
#ifdef _MSC_VER
#define J_PATH_MAX MAX_PATH
#elif defined(__unix__)
#include <limits.h>
#define J_PATH_MAX PATH_MAX
#else
#define J_PATH_MAX 1024
#endif

//
#ifndef J_ATTR_CDECL
#if defined(_MSC_VER)
#  ifdef __cplusplus
#    define J_EXTERN_C extern "C"
#  else
#    define J_EXTERN_C extern
#  endif
#define J_ATTR_CDECL __cdecl
#define J_ATTR_STDCALL __stdcall
#define J_ATTR_EXPORT __declspec(dllexport)
#define J_EXTERN extern
#elif defined(__APPLE__)
#  ifdef __cplusplus
#    define J_EXTERN_C extern "C"
#  else
#    define J_EXTERN_C extern
#  endif
#define J_ATTR_CDECL
#define J_ATTR_STDCALL
#define J_ATTR_EXPORT
#define J_EXTERN
#elif defined(__unix__)
#  ifdef __cplusplus
#    define J_EXTERN_C extern "C"
#  else
#    define J_EXTERN_C extern
#  endif
#define J_ATTR_CDECL //__attribute__((__cdecl__))
#define J_ATTR_STDCALL __attribute__((__stdcall__))
#define J_ATTR_EXPORT __attribute__((visibility("default")))
#define J_EXTERN
#endif
#endif  // J_ATTR_CDECL

///

//
#ifdef _MSC_VER
typedef unsigned __int64 JWPARAM;
typedef __int64 JLPARAM;
typedef __int64 JLRESULT;
#else
typedef unsigned long long JWPARAM;
typedef long long JLPARAM;
typedef long long JLRESULT;
#endif

//
#ifndef J_TO_STRING
#define J_TO_STRING(_s_) #_s_
#endif

#ifndef J_FRAME_THEME
#define J_FRAME_THEME   /**< J_FRAME_THEME �� */
#define J_FRAME_THEME_OFFICE_2007BLUE       J_TO_STRING(Office2007Blue)     /**< Office2007Blue ��� */
#define J_FRAME_THEME_OFFICE_2007BLACK      J_TO_STRING(Office2007Black)    /**< Office2007Black ��� */
#define J_FRAME_THEME_OFFICE_2007SILVER     J_TO_STRING(Office2007Silver)   /**< Office2007Silver ��� */
#define J_FRAME_THEME_OFFICE_2007AQUA       J_TO_STRING(Office2007Aqua)     /**< Office2007Aqua ��� */
#define J_FRAME_THEME_WINDOWs7_SCENIC       J_TO_STRING(Windows7Scenic)     /**< Windows7Scenic ��� */
#define J_FRAME_THEME_OFFICE_2010SILVER     J_TO_STRING(Office2010Silver)   /**< Office2010Silver ��� */
#define J_FRAME_THEME_OFFICE_2010BLUE       J_TO_STRING(Office2010Blue)     /**< Office2010Blue ��� */
#define J_FRAME_THEME_OFFICE_2010BLACK      J_TO_STRING(Office2010Black)    /**< Office2010Black ��� */
#endif // J_FRAME_THEME

/** IJUnoknown �ӿ����� */
#define VER_IJUnknown J_INTERFACE_VERSION(1, 0)
#define IID_IJUnknown J_IID_INTERFACE(IJUnknown)

/**
 * @brief The IJUnknown class
 */
class IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJUnknown() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJUnknown; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJUnknown; }

    /**
     * @brief ��ѯ�ӿ�
     * @param [in] iid : �ӿڱ�ʶ
     * @param [in] ver : �ӿڰ汾
     * @return �ӿ�ʵ��
     */
    virtual void* queryInterface(const std::string &iid, unsigned int ver)
    { (void)iid; (void)ver; return 0; }

    /**
     * @brief ���ؽӿ�
     * @return true�����سɹ���false������ʧ��
     */
    virtual bool loadInterface() { return true; }

    /**
     * @brief ���½ӿ�
     * @return true�����³ɹ���false������ʧ��
     */
    virtual bool updateInterface() { return true; }

    /**
     * @brief �ͷŽӿ�
     */
    virtual void releaseInterface() {}

    /**
     * @brief ��ѯinvoke�����ڲ�ָ����Ϣ
     * @return �ڲ�ָ�
     */
    virtual std::list<std::string> queryMethod() const
    { return std::list<std::string>(); }

    /**
     * @brief ִ�к�������
     * @param [in] method : �ڲ���������
     * @param [in] argc : �ɱ��������
     * @param [in] ... : �ɱ����
     * @return ���ý����true�����óɹ���false������ʧ��
     */
    virtual bool invokeMethod(const std::string &method, int argc = 0, ...)
    { (void)method; (void)argc; return false; }
};

// �ӿڱ�ʶ
#define VER_IJFrameFacade J_INTERFACE_VERSION(1, 0)
#define IID_IJFrameFacade J_IID_INTERFACE(IJFrameFacade)

/**
 * @brief The IJFrameFacade class
 */
class IJFrameFacade : public IJUnknown
{
public:
    /**
     * @brief ��������
     */
    virtual ~IJFrameFacade() {}

    /**
     * @brief ��ȡ�ӿڱ�ʶ
     * @return �ӿڱ�ʶ
     */
    virtual std::string interfaceIdentity() const { return IID_IJFrameFacade; }

    /**
     * @brief ��ȡ�ӿڰ汾
     * @return �ӿڰ汾
     */
    virtual unsigned int interfaceVersion() const { return VER_IJFrameFacade; }

    /**
     * @brief ��ȡ���ʵ��·��
     * @return ���ʵ��·��
     */
    virtual std::string appDirPath() const = 0;

    /**
     * @brief ��ȡ��������ļ���·��
     * @return ��������ļ���·��
     */
    virtual std::string configDirPath() const = 0;

    /**
     * @brief ��ȡ���ʵ�岿��·����application��ִ���ļ���һ��·����
     * @return ��ܲ���·��
     */
    virtual std::string thisDirPath() const = 0;

    /**
     * @brief ��ȡ���·��
     * @return ���·��
     */
    virtual std::string frameDirPath() const = 0;

    /**
     * @brief ��ȡ���ȫ�������ļ�·��
     * @return ���ȫ�������ļ�·��
     */
    virtual std::string frameGlobalPath() const = 0;

    /**
     * @brief ��ȡ��ܲ��������ļ�·��
     * @return ��ܲ��������ļ�·��
     */
    virtual std::string frameLayoutPath() const = 0;

    /**
     * @brief ��ȡ��ܰ汾
     * @return ��ܰ汾��Ϊ��ʱ��ʾ��ȡʧ��
     */
    virtual std::string frameVersion() const = 0;

    /**
     * @brief ��ȡ��ܰ汾
     * @param [out] major : ���汾��
     * @param [out] minor : �ΰ汾��
     * @param [out] patch : �����汾��
     * @return true����ȡ�ɹ���false����ȡʧ��
     */
    virtual bool frameVersion(int &major, int &minor, int &patch) const = 0;

    /**
     * @brief ���ؿ��
     * @param argc : ��������
     * @param [in] argv : �����б�
     * @param [in] app : �ⲿappʵ�壨��MFC��ܵ�CWinApp*��
     * @return ִ�н����true�����سɹ���false������ʧ��
     */
    virtual bool loadFrame(int argc = 0, char** argv = 0, void *app = 0) = 0;

    /**
     * @brief ��ʾ/���ؿ��������
     * @param show : ��ʾ��־��true����ʾ��false������
     * @param maximized : ��󻯱�־��true����󻯣�false������
     */
    virtual void showFrame(bool show = true, bool maximized = true) = 0;

    /**
     * @brief �˳���ܣ�����ʾ��
     */
    virtual void tryExitFrame() = 0;

    /**
     * @brief �˳����
     */
    virtual void exitFrame() = 0;

    /**
     * @brief �������
     * @param [in] arguments : �����б�
     */
    virtual void restartFrame(const std::list<std::string> &arguments) = 0;

    /**
     * @brief ִ�е�¼������ʾ
     * @return ִ�н����true����¼�ɹ���false����¼ʧ��
     */
    virtual bool loginFrame() = 0;

    /**
     * @brief ע����¼���
     * @return ִ�н����true��ע���ɹ���false��ע��ʧ��
     */
    virtual bool logoutFrame() = 0;

    /**
     * @brief ��ȡ��������ֵ
     * @param [in] name : ������������
     * @return ��������ֵ
     */
    virtual std::string getEnvValue(const std::string &name) const = 0;

    /**
     * @brief ��MFC��theApp���е��ã�theApp���Run������ʵ��Qt��Ϣѭ�����ȣ�
     * @param [in] mfcApp : CWinApp����ʵ������theApp
     * @return Qt�����˳���
     */
    virtual int runQApp(void *mfcApp = 0) = 0;

    /**
     * @brief ��ȡ����ʵ���ľ��
     * @param [in] window : Ŀ�괰��ʵ����CWnd���͡�QWidget���ͣ�
     * @param [in] winType : Ŀ�괰�����͡�1��"QWidget"��2��"CWnd"
     * @return Ŀ�괰�ڴ��ھ����HWND��
     */
    virtual long windowHandle(void *window, const std::string &winType) = 0;

    /**
     * @brief ��ȡ���ϵͳ����
     * @return ���ϵͳ����
     */
    virtual std::string language() const = 0;
};

/** ��ȡ������浥ʵ�������������Ͷ��� */
typedef IJUnknown *(J_ATTR_CDECL *FuncFrameFacadeInst)(void);

///

#ifdef JFRAME_FACADE_LIB
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MINGW32__)
#   ifdef JFRAME_FACADE_BUILD
#       define JFRAME_FACADE_EXPORT __declspec(dllexport)
#   else
#       define JFRAME_FACADE_EXPORT __declspec(dllimport)
#       if defined(DEBUG) || defined(_DEBUG)
#           pragma comment(lib, "jframe_facaded.lib")
#       else
#           pragma comment(lib, "jframe_facade.lib")
#       endif
#   endif // !JFRAME_FACADE_BUILD
#endif // _MSC_VER || ...
#endif // JFRAME_FACADE_LIB

#ifndef JFRAME_FACADE_EXPORT
#define JFRAME_FACADE_EXPORT
#endif

/**
 * @brief jframeFacade : ��ȡ�������ϵͳ��ʵ��
 * @return �������ϵͳ��ʵ��
 */
JFRAME_FACADE_EXPORT IJFrameFacade* jframeFacade();

// ��� - ��־

#ifdef JFRAME_FACADE_LIB

//
#ifndef __FILENAME__
#ifdef _MSC_VER
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)
#endif
#endif

/** �����־��type: char*; msg, char*; where, char*�� */
#define jframeLog(type, msg, where) \
    jframeFacade()->invokeMethod("log", 6, type, msg, where, __FILENAME__, __LINE__, __FUNCTION__)

/** log - emerge - (msg: char*) */
#define jframeLogEmerge(msg) \
    jframeLog("emerge", msg, "all")

/** log - fatal - (msg: char*) */
#define jframeLogFatal(msg) \
    jframeLog("fatal", msg, "all")

/** log - alert - (msg: char*) */
#define jframeLogAlert(msg) \
    jframeLog("alert", msg, "all")

/** log - crit - (msg: char*) */
#define jframeLogCrit(msg) \
    jframeLog("crit", msg, "all")

/** log - error - (msg: char*) */
#define jframeLogError(msg) \
    jframeLog("error", msg, "all")

/** log - warn - (msg: char*) */
#define jframeLogWarning(msg) \
    jframeLog("warn", msg, "all")

/** log - notice - (msg: char*) */
#define jframeLogNotice(msg) \
    jframeLog("notice", msg, "all")

/** log - info - (msg: char*) */
#define jframeLogInfo(msg) \
    jframeLog("info", msg, "all")

/** log - debug - (msg: char*) */
#define jframeLogDebug(msg) \
    jframeLog("debug", msg, "all")

#endif

/////////////////////////////////////////////////////////////

#ifdef JLIBRARY_MODULE

#ifdef _MSC_VER
#include <Windows.h>
#elif defined(__unix__)
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#endif

/**
 * @brief ���ӿ���ع�����
 */
template<typename T> class JLibraryT
{
public:
    /**
     * @brief JLibraryT : ���캯��
     * @param filepath : ���ļ�·��
     * @param autoclose : �Զ����١�true��ʹ�ܣ�false����ֹ
     */
    explicit JLibraryT(const std::basic_string<T> &filepath, bool autoclose = true)
        : _handle(0)
        , _filepath(filepath)
        , _autoclose(autoclose)
    {
        if (!_autoclose) {
            load();
        }
    }

    ~JLibraryT()
    {
        unload();
    }

    /**
     * @brief load : ���ؿ�
     * @return ����״̬��true�����سɹ���false������ʧ��
     */
    bool load()
    {
        if (_handle) {
            return true;
        }
#ifdef _MSC_VER
        _handle = (void *)LoadLibraryA(_filepath.c_str());
#elif defined(__unix__)
        _handle = dlopen(_filepath.c_str(), RTLD_LAZY);
#else
#pragma message("not supported!")
#endif
        if (_handle) {
            return true;
        } else {
            return false;
        }
    }

    /**
     * @brief load : ���ؿ�
     * @param filepath : ���ļ�·��
     * @return ����״̬��true�����سɹ���false������ʧ��
     */
    bool load(const std::basic_string<T> &filepath)
    {
        unload();
        _filepath = filepath;
        return load();
    }

    /**
     * @brief unload : ж�ؿ�
     */
    void unload()
    {
        if (_handle && _autoclose) {
#ifdef _MSC_VER
            CloseHandle((HANDLE)_handle);
#elif defined(__unix__)
            dlclose(_handle);
#else
#pragma message("not supported!")
#endif
            _handle = 0;
        }
    }

    /**
     * @brief resolve : ��ȡ���еķ����ӿ�
     * @param methodname : �����ӿ�����
     * @return �����ӿ�
     */
    void *resolve(const std::basic_string<T> &methodname)
    {
        if (!_handle) {
            return 0;   // failure
        }

        // ��ȡ�����ӿ�
#ifdef _MSC_VER
        return GetProcAddress((HMODULE)_handle, methodname.c_str());
#elif defined(__unix__)
        return dlsym(_handle, methodname.c_str());
#else
#pragma message("not supported!")
        return 0;
#endif
    }

    /**
     * @brief resolve : ��ȡ���еķ����ӿ�
     * @param filepath : ���ļ�·��
     * @param methodname : �����ӿ�����
     * @return �����ӿ�
     */
    static void *resolve(const std::basic_string<T> &filepath, const std::basic_string<T> &methodname)
    {
        JLibraryT<T> library(filepath, false);
        return library.resolve(methodname);
    }

private:
    void *_handle;
    std::basic_string<T> _filepath;
    bool _autoclose;
};

/**
 * @brief ���Ͷ���
 */
typedef JLibraryT<char> JLibrary, JLibraryA;

#endif

#endif // JFRAME_FACADE_H
