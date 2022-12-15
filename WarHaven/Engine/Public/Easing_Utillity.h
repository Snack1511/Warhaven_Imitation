#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CEasing_Utillity final
{
//public:
//	enum EasingTypeID
//	{
//		TYPE_Linear = 0,
//
//		TYPE_QuadIn = 1,
//		TYPE_QuadOut,
//		TYPE_QuadInOut,
//
//		TYPE_CubicIn = 4,
//		TYPE_CubicOut,
//		TYPE_CubicInOut,
//
//		TYPE_QuarticIn = 7,
//		TYPE_QuarticOut,
//		TYPE_QuarticInOut,
//
//		TYPE_QuinticIn = 10,
//		TYPE_QuinticOut,
//		TYPE_QuinticInOut,
//
//		TYPE_SinIn = 13,
//		TYPE_SinOut,
//		TYPE_SinInOut,
//
//		TYPE_ExpoIn = 16,
//		TYPE_ExpoOut,
//		TYPE_ExpoInOut,
//
//		TYPE_CircularIn = 19,
//		TYPE_CircularOut,
//		TYPE_CircularInOut,
//
//		TYPE_ElasticIn = 22,
//		TYPE_ElasticOut,
//		TYPE_ElasticInOut,
//
//		TYPE_BounceIn = 25,
//		TYPE_BounceOut,
//
//
//		TYPE_End
//	};
//

public:
	CEasing_Utillity();
	~CEasing_Utillity();

//public:
//	_float4	Easing(_uint eEasingType, _float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);


public:
	static _float4 Linear(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 QuadIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 QuadOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 QuadInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 CubicIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 CubicOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 CubicInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 QuarticIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 QuarticOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 QuarticInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 QuinticIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 QuinticOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 QuinticInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 SinIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 sinfOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 sinfInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 ExpoIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 ExpoOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 ExpoInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 CircularIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 CircularOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 CircularInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 ElasticEaseIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 ElasticEaseOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 ElasticEaseInOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

	static _float4 BounceEaseIn(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);
	static _float4 BounceEaseOut(_float4 vStartPoint, _float4 vTargetPoint, _float fPassedTime, _float fTotalTime);

public:
	static _float Linear(_float fStartVlue, _float fTargetValue, _float fPassedTime, _float fTotalTime);
	static _float SinIn(_float vStartValue, _float vTargetValue, _float fPassedTime, _float fTotalTime);
	static _float sinfOut(_float vStartValue, _float vTargetValue, _float fPassedTime, _float fTotalTime);
	static _float sinfInOut(_float vStartValue, _float vTargetValue, _float fPassedTime, _float fTotalTime);
	static _float ElasticEaseOut(_float vStartValue, _float vTargetValue, _float fPassedTime, _float fTotalTime);


};

END