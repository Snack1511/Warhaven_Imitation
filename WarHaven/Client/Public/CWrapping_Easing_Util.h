#pragma once
#include "Client_Defines.h"

BEGIN(Client)
class CWrapping_Easing_Util
{
private:
	CWrapping_Easing_Util();
	~CWrapping_Easing_Util();

	friend class CGame_Manager_MJ;

private:
	_float4 Default(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 Linear(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 QuadIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 QuadOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 QuadInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 CubicIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 CubicOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 CubicInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 QuarticIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 QuarticOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 QuarticInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 QuinticIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 QuinticOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 QuinticInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 SinIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 sinfOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 sinfInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 ExpoIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 ExpoOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 ExpoInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 CircularIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 CircularOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 CircularInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 ElasticEaseIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 ElasticEaseOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 ElasticEaseInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	_float4 BounceEaseIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	_float4 BounceEaseOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

};

END