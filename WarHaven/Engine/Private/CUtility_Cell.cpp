#include "CUtility_Cell.h"


_float CUtility_Cell::Dot(_float2 Lhs, _float2 Rhs)
{
	return Lhs.x * Rhs.x + Lhs.y * Rhs.y;
};

_int CUtility_Cell::ccw(_float2 a, _float2 b, _float2 c)

{
	_float2 Lhs = _float2(b.x - a.x, b.y - a.y);//ab
	_float2 Rhs = _float2(c.x - b.x, c.y - b.y);//bc
	auto cross = [](_float2 Lhs, _float2 Rhs)
	{
		_float Value = (Lhs.x * Rhs.y - Lhs.y * Rhs.x);
		if (Value == 0)
			return 0.f;
		return Value / fabsf(Value);
	};// 양수 : 같은 방향, 음수 다른방향, 0 : 평행

	return cross(Lhs, Rhs);
}

_bool CUtility_Cell::Is_IncludeLine(_float2 u, _float2 v, _float2 a)

{
	return ccw(u, v, a) == 0;
}

_bool CUtility_Cell::Is_BetweenLine(_float2 u, _float2 v, _float2 a, _bool bSameCheck)
{
	_float2 Lhs = _float2(a.x - u.x, a.y - u.y);//a-u
	_float2 Rhs = _float2(v.x - u.x, v.y - u.y);//v-u
	//lhs.x* rhs.x + lhs.y * rhs.y
	if (bSameCheck) {
		return Dot(Lhs, Rhs) >= 0
			&& Dot(Lhs, Rhs) <= Dot(Rhs, Rhs);
	}
	else
	{
		return Dot(Lhs, Rhs) >= 0
			&& Dot(Lhs, Rhs) < Dot(Rhs, Rhs);
	}
}

_bool CUtility_Cell::Is_SameDir(_float2 u, _float2 v, _float2 a)
{
	_float2 Lhs = _float2(a.x - u.x, a.y - u.y);//a-u
	_float2 Rhs = _float2(v.x - u.x, v.y - u.y);//v-u
	//lhs.x* rhs.x + lhs.y * rhs.y
	return Dot(Lhs, Rhs) >= 0;
}

_bool CUtility_Cell::Is_SamePoint(_float2 a, _float2 b)
{
	_float2 Rhs = _float2(b.x - a.x, b.y - a.y);//b-a
	//lhs.x* rhs.x + lhs.y * rhs.y
	return fabsf(Dot(Rhs, Rhs)) - XMVectorGetX(g_XMEpsilon) <= 0;
}

bool CUtility_Cell::Is_Cross(_float2 u, _float2 v, _float2 a, _float2 b, _bool bSameCheck)
{
	if (bSameCheck)
	{
		
		return ccw(u, v, a) * ccw(u, v, b) <= 0
			&& ccw(a, b, u) * ccw(a, b, v) <= 0;
	}
	else
	{
		return ccw(u, v, a) * ccw(u, v, b) < 0
			&& ccw(a, b, u) * ccw(a, b, v) < 0;
	}
}

bool CUtility_Cell::Is_NearPoint(_float2 u, _float2 v, _float2 a, _float fRange)
{

	_float2 uvLine = _float2(v.x - u.x, v.y - u.y);
	_float2 uaLine = _float2(a.x - u.x, a.y - u.y);

	if (0 == Length(uvLine))
		return false;

	_float uaCos = Dot(uvLine, uaLine) / Length(uvLine);
	_float uaLength = Length(uaLine);
	_bool bNear = (uaLength * uaLength) - (uaCos * uaCos) <= fRange;

	return bNear;
}

_float CUtility_Cell::Length(_float2 Point)
{
	return sqrtf(Point.x * Point.x + Point.y * Point.y);

}
