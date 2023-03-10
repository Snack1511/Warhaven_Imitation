#pragma once
#define RELEASE_IMGUI

#define MOUSE_DPI_X		0.02f
#define MOUSE_DPI_Y		0.02f

#define PI				3.141592f
#define PLAYER			CUser::Get_Instance()->Get_Player()

#define	DEFAULT_TRANS_MATRIX			XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(XMConvertToRadians(270.0f))* XMMatrixRotationZ(XMConvertToRadians(0.0f)) * XMMatrixRotationY(XMConvertToRadians(180.0f))
#define	DEFAULT_MODEL_MATRIX			XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(270.0f))

#define GET_STATE(enumname, classname) static_cast<classname*>(CState_Manager::Get_Instance()->Get_State(enumname))

#define COLOR_RED		_float4(0.8f, 0.2f, 0.2f, 1.0f)
#define COLOR_YELLOW	_float4(1.f, 1.f, 0.6f, 1.0f)
#define COLOR_ORANGE	_float4(0.8f, 0.4f, 0.2f, 1.0f)
#define COLOR_GRAY		_float4(0.5f, 0.5f, 0.5f, 1.0f)
#define COLOR_WHITE		_float4(0.9f, 0.9f, 0.9f, 1.0f)
#define COLOR_BLACK		_float4(0.1f, 0.1f, 0.1f, 1.0f)
#define COLOR_BROWN		_float4(0.4f, 0.2f, 0.1f, 1.0f)

/*Pos*/
#define TEST_POS				_float4(20.f, 3.f, 10.f);
#define TEST_SANDBAG_POS		_float4(20.f, 3.f, 10.f);

#define TRAINING_POS			_float4(20.f, 3.f, -10.f);
#define TRAINING_SANDBAG_POS	_float4(70.f, 3.f, -10.f);

/* Glow*/
#define	GLOW_CHAKRA(power)		_float4(0.f, 0.5f, 1.f, power)
#define RGB(R, G, B)			_float4(R/255.f, G/255.f, B/255.f)
#define RGBA(R, G, B, A)			_float4(R/255.f, G/255.f, B/255.f, A) //Alpha???? 0~1

#define	DECLARE_STATE(classname) public:\
classname* Clone() {return new classname(*this);}

#define SHADER_BINDING(classname, variablename) GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&classname::Set_ShaderResource, this, placeholders::_1, variablename)
#define SHADER_BINDINGOUT(classname, variablename) GET_COMPONENT(CShader)->CallBack_SetRawValues += bind(&classname::Set_ShaderResource, this, placeholders::_1, variablename)
#define CAMERA_LERP(enumname) CUser::Get_Instance()->Get_FollowCam()->Start_LerpType(CScript_FollowCam::enumname)

#define	DECLARE_LEVEL(classname) public:\
static classname* Create() \
{ \
classname* pInstance = new classname(); \
if (FAILED(pInstance->Initialize()))\
{\
	string text = "Failed to Initialize_Prototype : ";\
	text += typeid(classname).name();\
	wstring wText;\
	wText.assign(text.begin(), text.end());\
	Call_MsgBox(wText.c_str());\
	delete pInstance;\
	pInstance = nullptr;\
}\
return pInstance;\
}\

#define	DECLARE_GAMEOBJECT(classname) public:\
static classname* Create() \
{ \
classname* pInstance = new classname(); \
if (FAILED(pInstance->Initialize_Prototype()))\
{\
	string text = "Failed to Initialize_Prototype : ";\
	text += typeid(classname).name();\
	wstring wText;\
	wText.assign(text.begin(), text.end());\
	Call_MsgBox(wText.c_str());\
	delete pInstance;\
	pInstance = nullptr;\
}\
return pInstance;\
}\



