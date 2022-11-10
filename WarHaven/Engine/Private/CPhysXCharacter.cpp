#include "CPhysXCharacter.h"

CPhysXCharacter::CPhysXCharacter(_uint iGroupID)
	: CPhysXCollider(iGroupID)
{
}

CPhysXCharacter::~CPhysXCharacter()
{
}

CPhysXCharacter* CPhysXCharacter::Create(_uint iGroupID, const PHYSXCOLLIDERDESC& tPhysXColliderDesc)
{
	return nullptr;
}
