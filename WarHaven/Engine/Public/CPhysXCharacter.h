#pragma once
#include "PhysXCollider.h"

BEGIN(Engine)
class CPhysXCharacter
	: public CPhysXCollider
{
	DECLARE_PROTOTYPE(CPhysXCharacter);

private:
	CPhysXCharacter(_uint iGroupID);
	virtual ~CPhysXCharacter();

public:
	static CPhysXCharacter* Create(_uint iGroupID, const PHYSXCOLLIDERDESC& tPhysXColliderDesc);

	


};

END