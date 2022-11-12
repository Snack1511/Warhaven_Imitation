#include "CUtility_PhysX.h"

PxVec3 CUtility_PhysX::To_PxVec3(_float4 vVector)
{
	return PxVec3(vVector.x, vVector.y, vVector.z);
}

PxExtendedVec3 CUtility_PhysX::To_PxExtendedVec3(_float4 vVector)
{
	return PxExtendedVec3(vVector.x, vVector.y, vVector.z);
}
