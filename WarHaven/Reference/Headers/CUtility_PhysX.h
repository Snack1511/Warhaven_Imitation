#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)
class CUtility_PhysX
{
public:
	static PxVec3			To_PxVec3(_float4 vVector);
	static PxExtendedVec3	To_PxExtendedVec3(_float4 vVector);
};

END