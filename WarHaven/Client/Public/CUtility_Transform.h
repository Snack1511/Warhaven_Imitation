#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CTransform;
END

BEGIN(Client)

class CUtility_Transform
{
public:
	static	void	Turn_ByAngle(CTransform* pTransform, _float4 vAxis, _float fAngle);
	static	void	Turn_ByAngle(_float4x4& Matrix, _float4 vAxis, _float fAngle);
	static	void	Turn_ByAngle_Up(_float4x4& Matrix, _float fAngle);
	static  void	Turn(CTransform* pTransform, _float4 vAxis, _float fSpeed);
	static	void	Rotation(CTransform* pTransform, _float4 vAxis, _float fAngle);
	static _float4  LookAt(CTransform* pTransform, _float4 vPos, _bool bNoY = false);

	static _float4	Get_ProjPos(CTransform* pTransform, _float4 vOffsetPos = ZERO_VECTOR);
	static _float4	Get_ProjPos(_float4 vPosition);

	static _float4 Get_Dir_2D(CTransform* pFrom, CTransform* pTo);

	static	_float	Get_LookRotateAngle(_float4 vLook);
	//static _float4x4 Matrix_ByLook(_float4 vLook);

	static _float Get_FromCameraDistance(CGameObject* pObject);
	//static _float4 Get_FromCameraDistance(CGameObject* pObject);

public:
	static	_float4	Turn_ByAngle(_float4 vLook, _float4 vAxis, _float fAngle);
	static _float4	Get_Quaternion(_float4x4 matWorld);
	
	static _float4x4 Get_MatrixbyLook(_float4 vLook, _float4 vPos);
	static _float4x4 Get_MatrixbyQuat(_float4 vQuat, _float4 vPos);

};
END
