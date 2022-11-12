#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CGameObject;
END

BEGIN(Client)
class CWrapping_Easing_Util;
class CGame_Manager_MJ
{
	DECLARE_SINGLETON(CGame_Manager_MJ);

private:
	CGame_Manager_MJ();
	~CGame_Manager_MJ();
	typedef function<_float4(CWrapping_Easing_Util*, _float4, _float4, _float, _float)> EASINGFUNCS;
public:
	HRESULT	Initialize();
	void	Tick();
public:
	void SetUp_Wrapping_Easing();
	void Add_Wrapping_Easing(EASINGFUNCS func);
public:
	void Easing_Vert(_int Index, _float4* pOutPos, _float4& StartPos, _float4& TargetPos, _float Ratio);

private:
	vector<function<_float4(CWrapping_Easing_Util*, _float4, _float4, _float, _float)>> m_ArrEasingWrapped;
	CWrapping_Easing_Util* m_pWrappedEasing = nullptr;
};
END

