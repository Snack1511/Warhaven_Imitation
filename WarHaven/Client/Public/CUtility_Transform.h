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
	static _float4  LookAt(CTransform* pTransform, _float4 vPos, _bool bNoY = false);

	static _float4	Get_ProjPos(CTransform* pTransform);

};
END
