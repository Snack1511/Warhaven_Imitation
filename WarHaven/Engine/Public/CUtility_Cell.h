#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)
class ENGINE_DLL CUtility_Cell
{
public:
	//Dot Product
	static _float Dot(_float2 Lhs, _float2 Rhs);
	//ccw알고리즘용
	static _int ccw(_float2 a, _float2 b, _float2 c);
	// u, v, a가 같은 직선 위 인지만 판단
	static _bool Is_IncludeLine(_float2 u, _float2 v, _float2 a);
	// a가 u와 v 사이에 있는지 판단(같은 선일때도 포함)
	static _bool Is_BetweenLine(_float2 u, _float2 v, _float2 a, _bool bSameCheck = false);
	static _bool Is_SameDir(_float2 u, _float2 v, _float2 a);
	static _bool Is_SamePoint(_float2 a, _float2 b);
	// 선분 uv와 선분 ab가 겹치는가? (단, 겹치는 점이 u 또는 v인 경우는 제외)
	static bool Is_Cross(_float2 u, _float2 v, _float2 a, _float2 b, _bool bSameCheck = false);
	static bool Is_NearPoint(_float2 u, _float2 v, _float2 a, _float fRange = 1.f);

	static _float Length(_float2 Point);
};
END

