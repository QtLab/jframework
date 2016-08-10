#ifndef _IGF_BASE_H__
#define _IGF_BASE_H__

//////////////////////////////////////////////////////////////////////////
//模块接口

#define VER_IUnknownEx INTERFACE_VERSION(1,1)
static const char* IID_IUnknownEx = "IID_IUnknownEx";

//基础接口
class IUnknownEx
{
public:
	virtual ~IUnknownEx(){}
	//释放对象
	virtual void  Release()=0;
	//接口查询
	virtual void*  QueryInterface(const char* IID, unsigned int dwQueryVer)=0;
};

//////////////////////////////////////////////////////////////////////////
//版本比较宏

//产品版本号
#define PRODUCT_VER					8									//产品版本

//接口版本
#define INTERFACE_VERSION(cbMainVer,cbSubVer)							\
		(unsigned int)(														\
		(((unsigned char)(PRODUCT_VER))<<24)+									\
		(((unsigned char)(cbMainVer))<<16)+										\
		((unsigned char)(cbSubVer)<<8))

//模块版本
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)					\
		(unsigned int)(														\
		(((unsigned char)(PRODUCT_VER))<<24)+									\
		(((unsigned char)(cbMainVer))<<16)+										\
		((unsigned char)(cbSubVer)<<8)+											\
		(unsigned char)(cbBuildVer))

//产品版本
inline unsigned char GetProductVer(unsigned int dwVersion)
{
	return ((unsigned char *)&dwVersion)[3];
}

//主要版本
inline unsigned char GetMainVer(unsigned int dwVersion)
{
	return ((unsigned char *)&dwVersion)[2];
}

//次要版本
inline unsigned char GetSubVer(unsigned int dwVersion)
{
	return ((unsigned char *)&dwVersion)[1];
}

//编译版本
inline unsigned char GetBuildVer(unsigned int dwVersion)
{
	return ((unsigned char *)&dwVersion)[0];
}

//版本比较
inline bool InterfaceVersionCompare(unsigned int dwQueryVer, unsigned int dwInterfaceVer)
{
	if (GetProductVer(dwQueryVer)!=GetProductVer(dwInterfaceVer)) return false;
	if (GetMainVer(dwQueryVer)!=GetMainVer(dwInterfaceVer)) return false;
	if (GetSubVer(dwQueryVer)>GetSubVer(dwInterfaceVer)) return false;
	return true;
};

//////////////////////////////////////////////////////////////////////////
//内部接口查询

//查询接口
#define QUERYINTERFACE(Interface,Guid,dwQueryVer)															\
	if ((strcmp(Guid,IID_##Interface) == 0)&&(InterfaceVersionCompare(dwQueryVer,VER_##Interface)))						\
		return static_cast<Interface *>(this);											

//查询接口
#define QUERYINTERFACE_IUNKNOWNEX(BaseInterface,Guid,dwQueryVer)											\
	if ((strcmp(Guid,IID_IUnknownEx)==0)&&(InterfaceVersionCompare(dwQueryVer,VER_IUnknownEx)))						\
		return static_cast<IUnknownEx *>(static_cast<BaseInterface *>(this));			

//////////////////////////////////////////////////////////////////////////
//外部接口查询

//查询接口
#define QUERY_ME_INTERFACE(Interface)																		\
	((Interface *)QueryInterface(IID_##Interface,VER_##Interface))

//查询接口
#define QUERY_OBJECT_INTERFACE(Object,Interface)															\
	((Interface *)Object.QueryInterface(IID_##Interface,VER_##Interface))

//查询接口
#define QUERY_OBJECT_PTR_INTERFACE(pObject,Interface)														\
	((pObject==NULL)?NULL:((Interface *)pObject->QueryInterface(IID_##Interface,VER_##Interface)))

//////////////////////////////////////////////////////////////////////////

#endif