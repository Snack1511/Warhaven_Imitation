#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)
class ENGINE_DLL CUtility_Cell
{
public:
	//Dot Product
	static _float Dot(_float2 Lhs, _float2 Rhs);
	//ccw�˰����
	static _int ccw(_float2 a, _float2 b, _float2 c);
	// u, v, a�� ���� ���� �� ������ �Ǵ�
	static _bool Is_IncludeLine(_float2 u, _float2 v, _float2 a);
	// a�� u�� v ���̿� �ִ��� �Ǵ�(���� ���϶��� ����)
	static _bool Is_BetweenLine(_float2 u, _float2 v, _float2 a, _bool bSameCheck = false);
	static _bool Is_SameDir(_float2 u, _float2 v, _float2 a);
	static _bool Is_SamePoint(_float2 a, _float2 b);
	// ���� uv�� ���� ab�� ��ġ�°�? (��, ��ġ�� ���� u �Ǵ� v�� ���� ����)
	static bool Is_Cross(_float2 u, _float2 v, _float2 a, _float2 b, _bool bSameCheck = false);
	static bool Is_NearPoint(_float2 u, _float2 v, _float2 a, _float fRange = 1.f);

	static _float Length(_float2 Point);
};
END

