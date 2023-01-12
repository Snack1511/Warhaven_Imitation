#include "CAI_MoveStateUtility.h"
#include "CUnit.h"
#include "Transform.h"
_float4 CAI_MoveStateUtility::Get_Dir(CUnit* pUnit, _float4 vDestPosition)
{
	_float4 vCurUnitPos = pUnit->Get_Transform()->Get_World(WORLD_POS);

	_float4 vDir = vDestPosition - vCurUnitPos;
	vDir.Normalize();
	return vDir;
}
