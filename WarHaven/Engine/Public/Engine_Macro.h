#pragma once

#ifdef ENGINE_EXPORTS
#define ENGINE_DLL _declspec(dllexport)
#else
#define ENGINE_DLL _declspec(dllimport)
#endif



#define	SAFE_DELETE(Instance) if (Instance)\
		{\
		delete Instance;\
		Instance = nullptr;\
		}\

#define	SAFE_DELETE_ARRAY(Instance) if (Instance)\
		{\
		delete[] Instance;\
		Instance = nullptr;\
		}\

#define	SAFE_DESTROY(Instance) if (Instance)\
		{\
		Instance->Destroy_Instance();\
		Instance = nullptr;\
		}\

// =========My Macro=========

#define	GAMEINSTANCE CGameInstance::Get_Instance()

#define DEVICE	CGameInstance::Get_Instance()->Get_Device()
#define DEVICE_CONTEXT	CGameInstance::Get_Instance()->Get_DeviceContext()
#define PDEVICE	CGameInstance::Get_Instance()->Get_Device().Get()
#define PDEVICE_CONTEXT	CGameInstance::Get_Instance()->Get_DeviceContext().Get()

#define	HASHCODE(type)	typeid(type).hash_code()

#define DECLARE_PROTOTYPE(type)		public:																\
									virtual type* Clone() {return new type(*this);}						\
									friend class CPrototype_Manager;									\

#define MATRIX_NOSCALE				0x001
#define MATRIX_NOTURN				0x002
#define MATRIX_IDENTITY			    0x004
#define MARTIX_NOTRANS				0x008

#define MATRIX_DEFAULT				0x00f

#define CLONE_GAMEOBJECT(type)				CGameInstance::Get_Instance()->Clone_GameObject<type>()
#define CLONE_COMPONENT(type)					CGameInstance::Get_Instance()->Clone_Component<type>()


// ==========================

#define DT(index)	CGameInstance::Get_Instance()->Get_DT(index)
#define fDT(index)	(float)CGameInstance::Get_Instance()->Get_DT(index)


#define KEY(key, state) CGameInstance::Get_Instance()->Get_KeyState(KEY::key) == KEY_STATE::state
#define MOUSE_MOVE(mousemove) CGameInstance::Get_Instance()->Get_DIMouseMoveState(mousemove)

/* Event_Manager */
#define CREATE_GAMEOBJECT(GameObject, GroupIndex) CGameInstance::Get_Instance()->Create_GameObject(GameObject, GroupIndex)
#define DELETE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Delete_GameObject(GameObject)
#define ENABLE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Enable_GameObject(GameObject)
#define DISABLE_GAMEOBJECT(GameObject) CGameInstance::Get_Instance()->Disable_GameObject(GameObject)
#define CREATE_STATIC(GameObject, keyID) CGameInstance::Get_Instance()->Create_StaticObject(GameObject, keyID)
#define CHANGE_LEVEL(pLevel) CGameInstance::Get_Instance()->Change_Level(pLevel)
#define CREATE_COMPONENT(Component, GameObject) CGameInstance::Get_Instance()->Create_Component(Component, GameObject)
#define DELETE_COMPONENT(Component, GameObject) CGameInstance::Get_Instance()->Delete_Component(Component, GameObject)
#define ENABLE_COMPONENT(Component) CGameInstance::Get_Instance()->Enable_Component(Component)
#define DISABLE_COMPONENT(Component) CGameInstance::Get_Instance()->Disable_Component(Component)

#define		MIN_STR			64
#define		MAX_STR			256

#define		ToRadian(fAngle)	XMConvertToRadians(fAngle)

#define		MAX_VERTEX_ELEMENT			65
#define		ZERO_VECTOR				_float4(0.f, 0.f, 0.f, 1.f)

#define		SATURATE(floatValue) min(max(0.f, floatValue), 1.f)

// ====================== Shader Flag ============================

#define		SH_LIGHT_DEFAULT		_float4(1.f, 0.f, 1.f, 0.f)
#define		SH_LIGHT_BLOOM		_float4(1.f, 1.f, 1.f, 0.f)
#define		SH_LIGHT_NONE		_float4(0.f, 0.f, 0.f, 0.f)
#define		SH_LIGHT_NOSPEC		_float4(0.95f, 0.f, 0.f, 0.f)
#define		SH_LIGHT_NOOUTLINE		_float4(0.75f, 0.f, 0.f, 0.f)
#define		SH_LIGHT_NOCARTOON		_float4(0.65f, 0.f, 0.f, 0.f)
#define		SH_LIGHT_BUILDINGCARTOON		_float4(0.85f, 0.f, 0.f, 0.f)

#define		SH_EFFECT_DEFAULT		_float4(1.f, 1.f, 0.f, 1.f)
#define		SH_EFFECT_NOGLOW		_float4(0.95f, 1.f, 0.f, 1.f)
#define		SH_EFFECT_NOBLOOM		_float4(1.f, 0.95f, 0.f, 1.f)
#define		SH_EFFECT_ONLYBLUR		_float4(0.f, 0.f, 1.f, 1.f)
#define		SH_EFFECT_NONE		_float4(0.f, 0.f, 0.f, 0.f)

#define		SH_UI_BLOOM		_float4(0.f, 1.f, 0.f, 0.f)
#define		SH_UI_HARDBLOOM		_float4(0.f, 0.5f, 0.f, 0.f)
#define		SH_UI_NONE		_float4(0.f, 0.f, 0.f, 0.f)



// ==============================================================

#define BIND_SHADERRESOURCES(classname, variablename) list<CComponent*>	pShdaerlist = m_pOwner->Get_Component<CShader>();\
if (pShdaerlist.empty())\
return;\
static_cast<CShader*>(pShdaerlist.front())->CallBack_SetRawValues +=\
bind(&classname::Set_ShaderResource, this, placeholders::_1, variablename);\

#define REMOVE_SHADERRESOURCES(classname, variablename) list<CComponent*>	pShdaerlist = m_pOwner->Get_Component<CShader>();\
if (pShdaerlist.empty())\
return;\
static_cast<CShader*>(pShdaerlist.front())->CallBack_SetRawValues -=\
bind(&classname::Set_ShaderResource, this, placeholders::_1, variablename);\

#define GET_COMPONENT_FROM(instance, classname) static_cast<classname*>(instance->Get_Component<classname>().front())
#define GET_COMPONENT(classname) static_cast<classname*>(Get_Component<classname>().front())

#define BEGIN(NAMESPACE) namespace NAMESPACE { 
#define END }

#define NO_COPY(ClassName)								\
ClassName(const ClassName&) = delete;					\
ClassName& operator=(const ClassName&) = delete;


#define DECLARE_SINGLETON(ClassName)					\
		NO_COPY(ClassName)								\
public :												\
	static ClassName* Get_Instance();					\
	static void Destroy_Instance();			\
private:												\
	static ClassName* m_pInstance;

#define IMPLEMENT_SINGLETON(ClassName)					\
ClassName* ClassName::m_pInstance = nullptr;			\
ClassName* ClassName::Get_Instance()					\
{														\
	if (nullptr == m_pInstance)							\
		m_pInstance = new ClassName;					\
	return m_pInstance;									\
}														\
void ClassName::Destroy_Instance()						\
{														\
	SAFE_DELETE(m_pInstance);							\
}


//#define DECLARE_SINGLETON(type) public:								\
//									NO_COPY(type)					\
//									static type* Get_Instance() {	\
//										static type mgr;			\
//										return &mgr;				\
//									}								\

									
