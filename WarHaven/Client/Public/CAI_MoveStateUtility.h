#pragma once
#include "Client_Defines.h"

BEGIN(Client)
class CUnit;
class CAI_MoveStateUtility
{
public:
	static _float4 Get_Dir(CUnit* pUnit, _float4 vDestPosition);

};
END
