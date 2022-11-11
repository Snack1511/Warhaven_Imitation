#include "stdafx.h"
#include "CGame_Manager_MJ.h"
#include "MainApp.h"
#include "GameInstance.h"

#include "Texture.h"
#include "GameObject.h"
#include "CShader.h"
#include "Model.h"
#include "HIerarchyNode.h"
#include "Transform.h"
#include "Loading_Manager.h"

#include "Functor.h"
#include "CNavigation.h"
#include "CTerrain.h"
#include "CCell.h"

#include "CWrapping_Easing_Util.h"

IMPLEMENT_SINGLETON(CGame_Manager_MJ);

CGame_Manager_MJ::CGame_Manager_MJ()
{
	m_pWrappedEasing = new CWrapping_Easing_Util();
	Initialize();
	
}
CGame_Manager_MJ::~CGame_Manager_MJ()
{
	m_ArrEasingWrapped.clear();
	delete m_pWrappedEasing;
}

HRESULT CGame_Manager_MJ::Initialize()
{
	SetUp_Wrapping_Easing();
	return S_OK;
}

void CGame_Manager_MJ::Tick()
{

}

void CGame_Manager_MJ::SetUp_Wrapping_Easing()
{
	//function<_float4(CWrapping_Easing_Util*, _float4, _float4, _float, _float)>
//	func = ;
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::Linear));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuadIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuadOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuadInOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::CubicIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::CubicOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::CubicInOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuarticIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuarticOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuarticInOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuinticIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuinticOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::QuinticInOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::SinIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::sinfOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::ExpoIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::ExpoOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::ExpoInOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::CircularIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::CircularOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::CircularInOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::ElasticEaseIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::ElasticEaseOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::ElasticEaseInOut));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::BounceEaseIn));
	Add_Wrapping_Easing(EASINGFUNCS(&CWrapping_Easing_Util::BounceEaseOut));
}

void CGame_Manager_MJ::Add_Wrapping_Easing(EASINGFUNCS func)
{
	m_ArrEasingWrapped.push_back(func);
}

void CGame_Manager_MJ::Easing_Vert(_int Index, _float4* pOutPos, _float4& StartPos, _float4& TargetPos, _float Ratio)
{
	(*pOutPos) = m_ArrEasingWrapped[Index](m_pWrappedEasing, StartPos, TargetPos, Ratio, 1.f);
}
