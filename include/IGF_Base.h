#ifndef _IGF_BASE_H__
#define _IGF_BASE_H__

//////////////////////////////////////////////////////////////////////////
//ģ��ӿ�

#define VER_IUnknownEx INTERFACE_VERSION(1,1)
static const char* IID_IUnknownEx = "IID_IUnknownEx";

//�����ӿ�
class IUnknownEx
{
public:
	virtual ~IUnknownEx(){}
	//�ͷŶ���
	virtual void  Release()=0;
	//�ӿڲ�ѯ
	virtual void*  QueryInterface(const char* IID, unsigned int dwQueryVer)=0;
};

//////////////////////////////////////////////////////////////////////////
//�汾�ȽϺ�

//��Ʒ�汾��
#define PRODUCT_VER					8									//��Ʒ�汾

//�ӿڰ汾
#define INTERFACE_VERSION(cbMainVer,cbSubVer)							\
		(unsigned int)(														\
		(((unsigned char)(PRODUCT_VER))<<24)+									\
		(((unsigned char)(cbMainVer))<<16)+										\
		((unsigned char)(cbSubVer)<<8))

//ģ��汾
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)					\
		(unsigned int)(														\
		(((unsigned char)(PRODUCT_VER))<<24)+									\
		(((unsigned char)(cbMainVer))<<16)+										\
		((unsigned char)(cbSubVer)<<8)+											\
		(unsigned char)(cbBuildVer))

//��Ʒ�汾
inline unsigned char GetProductVer(unsigned int dwVersion)
{
	return ((unsigned char *)&dwVersion)[3];
}

//��Ҫ�汾
inline unsigned char GetMainVer(unsigned int dwVersion)
{
	return ((unsigned char *)&dwVersion)[2];
}

//��Ҫ�汾
inline unsigned char GetSubVer(unsigned int dwVersion)
{
	return ((unsigned char *)&dwVersion)[1];
}

//����汾
inline unsigned char GetBuildVer(unsigned int dwVersion)
{
	return ((unsigned char *)&dwVersion)[0];
}

//�汾�Ƚ�
inline bool InterfaceVersionCompare(unsigned int dwQueryVer, unsigned int dwInterfaceVer)
{
	if (GetProductVer(dwQueryVer)!=GetProductVer(dwInterfaceVer)) return false;
	if (GetMainVer(dwQueryVer)!=GetMainVer(dwInterfaceVer)) return false;
	if (GetSubVer(dwQueryVer)>GetSubVer(dwInterfaceVer)) return false;
	return true;
};

//////////////////////////////////////////////////////////////////////////
//�ڲ��ӿڲ�ѯ

//��ѯ�ӿ�
#define QUERYINTERFACE(Interface,Guid,dwQueryVer)															\
	if ((strcmp(Guid,IID_##Interface) == 0)&&(InterfaceVersionCompare(dwQueryVer,VER_##Interface)))						\
		return static_cast<Interface *>(this);											

//��ѯ�ӿ�
#define QUERYINTERFACE_IUNKNOWNEX(BaseInterface,Guid,dwQueryVer)											\
	if ((strcmp(Guid,IID_IUnknownEx)==0)&&(InterfaceVersionCompare(dwQueryVer,VER_IUnknownEx)))						\
		return static_cast<IUnknownEx *>(static_cast<BaseInterface *>(this));			

//////////////////////////////////////////////////////////////////////////
//�ⲿ�ӿڲ�ѯ

//��ѯ�ӿ�
#define QUERY_ME_INTERFACE(Interface)																		\
	((Interface *)QueryInterface(IID_##Interface,VER_##Interface))

//��ѯ�ӿ�
#define QUERY_OBJECT_INTERFACE(Object,Interface)															\
	((Interface *)Object.QueryInterface(IID_##Interface,VER_##Interface))

//��ѯ�ӿ�
#define QUERY_OBJECT_PTR_INTERFACE(pObject,Interface)														\
	((pObject==NULL)?NULL:((Interface *)pObject->QueryInterface(IID_##Interface,VER_##Interface)))

//////////////////////////////////////////////////////////////////////////

#endif