#include "..\Public\Easing_Utillity.h"

#define PI 3.141592f

CEasing_Utillity::CEasing_Utillity()
{
}

CEasing_Utillity::~CEasing_Utillity()
{
}
//
//_float4 CEasing_Utillity::Easing(_uint eEasingType, _float4 vStartPoint, _float4 vTargetPoint, _float dPassedTime, _float dTotalTime)
//{
//	if (eEasingType >= TYPE_End)
//		return _float4();
//
//	switch (eEasingType)
//	{
//	case Engine::CEasing_Utillity::TYPE_Linear:
//		return Linear(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuadIn:
//		return QuadIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuadOut:
//		return  QuadOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuadInOut:
//		return  QuadInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CubicIn:
//		return  CubicIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CubicOut:
//		return  CubicOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CubicInOut:
//		return  CubicInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuarticIn:
//		return  QuarticIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuarticOut:
//		return  QuarticOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuarticInOut:
//		return QuarticInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuinticIn:
//		return  QuinticIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuinticOut:
//		return QuinticOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_QuinticInOut:
//		return QuinticInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_SinIn:
//		return SinIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_SinOut:
//		return  sinfOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_SinInOut:
//		return sinfInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ExpoIn:
//		return  ExpoIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ExpoOut:
//		return  ExpoOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ExpoInOut:
//		return  ExpoInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CircularIn:
//		return  CircularIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CircularOut:
//		return  CircularOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_CircularInOut:
//		return  CircularInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ElasticIn:
//		return  ElasticEaseIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ElasticOut:
//		return  ElasticEaseOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_ElasticInOut:
//		return  ElasticEaseInOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_BounceIn:
//		return  BounceEaseIn(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	case Engine::CEasing_Utillity::TYPE_BounceOut:
//		return  BounceEaseOut(vStartPoint, vTargetPoint, dPassedTime, dTotalTime);
//
//	default:
//		break;
//	}
//
//	return _float4();
//}


_float4 CEasing_Utillity::Linear(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float4((vTargetPoint - vStartPoint) * fPassedTime / fTotalTime + vStartPoint);
}

_float4 CEasing_Utillity::QuadIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float4((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime + vStartPoint);
}

_float4 CEasing_Utillity::QuadOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;



	return _float4((vTargetPoint - vStartPoint) * -1.f * fPassedTime * (fPassedTime - 2) + vStartPoint);
}

_float4 CEasing_Utillity::QuadInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float4((vTargetPoint - vStartPoint) / 2 * fPassedTime*fPassedTime + vStartPoint);
	fPassedTime--;
	return _float4((vTargetPoint - vStartPoint) / -2 * (fPassedTime*(fPassedTime - 2) - 1) + vStartPoint);
}

_float4 CEasing_Utillity::CubicIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float4((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_float4 CEasing_Utillity::CubicOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _float4((vTargetPoint - vStartPoint) * (fPassedTime*fPassedTime*fPassedTime + 1) + vStartPoint);
}

_float4 CEasing_Utillity::CubicInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return (vTargetPoint - vStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime + vStartPoint;
	fPassedTime -= 2;
	return _float4((vTargetPoint - vStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime + 2) + vStartPoint);
}

_float4 CEasing_Utillity::QuarticIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float4((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_float4 CEasing_Utillity::QuarticOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _float4((vTargetPoint - vStartPoint) * -(fPassedTime*fPassedTime*fPassedTime*fPassedTime - 1) + vStartPoint);
}

_float4 CEasing_Utillity::QuarticInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float4((vTargetPoint - vStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
	fPassedTime -= 2;
	return _float4((vTargetPoint - vStartPoint) / 2 * -(fPassedTime*fPassedTime*fPassedTime*fPassedTime - 2) + vStartPoint);
}

_float4 CEasing_Utillity::QuinticIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float4((vTargetPoint - vStartPoint) * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
}

_float4 CEasing_Utillity::QuinticOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _float4((vTargetPoint - vStartPoint) * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 1) + vStartPoint);
}

_float4 CEasing_Utillity::QuinticInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float4((vTargetPoint - vStartPoint) / 2 * fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + vStartPoint);
	fPassedTime -= 2;
	return _float4((vTargetPoint - vStartPoint) / 2 * (fPassedTime*fPassedTime*fPassedTime*fPassedTime*fPassedTime + 2) + vStartPoint);
}

_float4 CEasing_Utillity::SinIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float4((vTargetPoint - vStartPoint) * -cosf(fPassedTime / fTotalTime * (PI / 2)) + (vTargetPoint - vStartPoint) + vStartPoint);
}

_float4 CEasing_Utillity::sinfOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float4((vTargetPoint - vStartPoint) * sinf(fPassedTime / fTotalTime * (PI / 2)) + vStartPoint);
}

_float4 CEasing_Utillity::sinfInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float4((vTargetPoint - vStartPoint) / -2 * (cosf(PI*fPassedTime / fTotalTime) - 1) + vStartPoint);
}

_float4 CEasing_Utillity::ExpoIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float4((vTargetPoint - vStartPoint) * pow(2, 10.0f * (fPassedTime / fTotalTime - 1)) + vStartPoint);
}

_float4 CEasing_Utillity::ExpoOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float4((vTargetPoint - vStartPoint) * (-pow(2, -10 * fPassedTime / fTotalTime) + 1) + vStartPoint);
}

_float4 CEasing_Utillity::ExpoInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float4((vTargetPoint - vStartPoint) / 2 * pow(2, 10 * (fPassedTime - 1)) + vStartPoint);
	fPassedTime--;
	return _float4((vTargetPoint - vStartPoint) / 2 * (-pow(2, -10 * fPassedTime) + 2) + vStartPoint);
}

_float4 CEasing_Utillity::CircularIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	return _float4((vTargetPoint - vStartPoint) * -(sqrtf(1 - fPassedTime * fPassedTime) - 1) + vStartPoint);
}

_float4 CEasing_Utillity::CircularOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime;
	fPassedTime--;
	return _float4((vTargetPoint - vStartPoint) * sqrtf(1 - fPassedTime * fPassedTime) + vStartPoint);
}

_float4 CEasing_Utillity::CircularInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	fPassedTime /= fTotalTime / 2;
	if (fPassedTime < 1) return _float4((vTargetPoint - vStartPoint) / -2 * (sqrtf(1 - fPassedTime * fPassedTime) - 1) + vStartPoint);
	fPassedTime -= 2;
	return _float4((vTargetPoint - vStartPoint) / 2 * (sqrtf(1 - fPassedTime * fPassedTime) + 1) + vStartPoint);
}

_float4 CEasing_Utillity::ElasticEaseOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{

	if ((fPassedTime /= fTotalTime) == 1)
		return vStartPoint + (vTargetPoint - vStartPoint);

	_float p = fTotalTime * .3f;
	_float s = p / 4;

	return _float4((vTargetPoint - vStartPoint) * pow(2, -10 * fPassedTime) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) + (vTargetPoint - vStartPoint) + vStartPoint);
}

_float4 CEasing_Utillity::ElasticEaseIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime) == 1)
		return vStartPoint + (vTargetPoint - vStartPoint);

	_float p = fTotalTime * .3f;
	_float s = p / 4.f;

	return _float4(((vTargetPoint - vStartPoint) * -pow(2, 10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p)) + vStartPoint);
}

_float4 CEasing_Utillity::ElasticEaseInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime / 2) == 2)
		return _float4(vStartPoint + (vTargetPoint - vStartPoint));

	_float p = fTotalTime * (.3f * 1.5f);
	_float s = p / 4;

	if (fPassedTime < 1)
		return _float4(((vTargetPoint - vStartPoint) * pow(2, 10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p)) + vStartPoint) * -0.5f;

	return _float4((vTargetPoint - vStartPoint) * pow(2, -10 * (fPassedTime -= 1)) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) * .5f + (vTargetPoint - vStartPoint) + vStartPoint);
}

_float4 CEasing_Utillity::BounceEaseOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime) < (1 / 2.75f))
		return _float4((vTargetPoint - vStartPoint) * (7.5625f * fPassedTime * fPassedTime) + vStartPoint);
	else if (fPassedTime < (2 / 2.75f))
		return _float4((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (1.5f / 2.75f)) * fPassedTime + .75f) + vStartPoint);
	else if (fPassedTime < (2.5f / 2.75f))
		return _float4((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (2.25f / 2.75f)) * fPassedTime + .9375f) + vStartPoint);
	else
		return _float4((vTargetPoint - vStartPoint) * (7.5625f * (fPassedTime -= (2.625f / 2.75f)) * fPassedTime + .984375f) + vStartPoint);
}

_float CEasing_Utillity::SinIn(_float vStartValue, _float vTargetValue, _float fPassedTime, _float fTotalTime)
{
	return ((vTargetValue - vStartValue) * -cosf(fPassedTime / fTotalTime * (PI / 2)) + (vTargetValue - vStartValue) + vStartValue);
}


_float CEasing_Utillity::sinfOut(_float vStartValue, _float vTargetValue, _float fPassedTime, _float fTotalTime)
{
	return ((vTargetValue - vStartValue) * sinf(fPassedTime / fTotalTime * (PI / 2)) + vStartValue);
}

_float CEasing_Utillity::sinfInOut(_float vStartValue, _float vTargetValue, _float fPassedTime, _float fTotalTime)
{
	return ((vTargetValue - vStartValue) / -2 * (cosf(PI * fPassedTime / fTotalTime) - 1) + vStartValue);
}

_float CEasing_Utillity::ElasticEaseOut(_float vStartValue, _float vTargetValue, _float fPassedTime, _float fTotalTime)
{
	if ((fPassedTime /= fTotalTime) == 1)
		return vStartValue + (vTargetValue - vStartValue);

	_float p = fTotalTime * .3f;
	_float s = p / 4;

	return ((vTargetValue - vStartValue) * pow(2, -10 * fPassedTime) * sinf((fPassedTime * fTotalTime - s) * (2 * PI) / p) + (vTargetValue - vStartValue) + vStartValue);
}


_float4 CEasing_Utillity::BounceEaseIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
	return _float4(BounceEaseOut(vTargetPoint, vStartPoint, fTotalTime - fPassedTime, fTotalTime));
}
//_float CEasing_Utillity::BounceEaseInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
//{
//	if (dPassedTime < dTotalTime * 0.5f)
//		return BounceEaseIn(vStartPoint, vTargetPoint * 0.5f, dPassedTime , dTotalTime * 0.5f);
//	else
//		return BounceEaseOut(vTargetPoint * 0.5f, vTargetPoint , dPassedTime-(dTotalTime * 0.5f) , dTotalTime * 0.5f);
//}
//
//_float CEasing_Utillity::BounceEaseOutIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
//{
//
//	if (dPassedTime < dTotalTime * 0.5f )
//		return BounceEaseOut(vStartPoint, vTargetPoint * 0.5f, dPassedTime * 0.5f, dTotalTime * 0.5f);
//	return BounceEaseIn(vTargetPoint * 0.5f, vTargetPoint, dPassedTime * 0.5f, dTotalTime * 0.5f);
//}

