#include "CWrapping_Easing_Util.h"
#include "Easing_Utillity.h"
CWrapping_Easing_Util::CWrapping_Easing_Util()
{
}

CWrapping_Easing_Util::~CWrapping_Easing_Util()
{
}

_float4 CWrapping_Easing_Util::Default(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
    return vTargetPoint;
}

_float4 CWrapping_Easing_Util::Linear(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
    return CEasing_Utillity::Linear(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuadIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
    return CEasing_Utillity::QuadIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuadOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
    return CEasing_Utillity::QuadOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuadInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
    return CEasing_Utillity::QuadInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::CubicIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
    return CEasing_Utillity::CubicIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::CubicOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::CubicOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::CubicInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::CubicInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuarticIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::QuarticIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuarticOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::QuarticOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuarticInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::QuarticInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuinticIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::QuinticIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuinticOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::QuinticOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::QuinticInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::QuinticInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::SinIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::SinIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::sinfOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::sinfOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::sinfInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::sinfInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::ExpoIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::ExpoIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::ExpoOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::ExpoOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::ExpoInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::ExpoInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::CircularIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::CircularIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::CircularOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::CircularOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::CircularInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::CircularInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::ElasticEaseIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::ElasticEaseIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::ElasticEaseOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::ElasticEaseOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::ElasticEaseInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::ElasticEaseInOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::BounceEaseIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::BounceEaseIn(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}

_float4 CWrapping_Easing_Util::BounceEaseOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime)
{
        return CEasing_Utillity::BounceEaseOut(vStartPoint, vTargetPoint, fPassedTime, fTotalTime);
}
