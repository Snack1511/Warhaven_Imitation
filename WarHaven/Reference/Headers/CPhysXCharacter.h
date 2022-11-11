#pragma once
#include "PhysXCollider.h"

BEGIN(Engine)
class CPhysXCharacter
	: public CPhysXCollider
{
public:
	typedef struct PhysXCCDDesc
	{
		_float		fRadius = 0.5f;
		_float		fHeight = 1.f;
	}PHYSXCCDDESC;

	DECLARE_PROTOTYPE(CPhysXCharacter);


private:
	CPhysXCharacter(_uint iGroupID);
	virtual ~CPhysXCharacter();

public:
	static CPhysXCharacter* Create(_uint iGroupID, const PHYSXCCDDESC& tPhysXCCDDesc);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void	Tick() override;
	virtual void	Late_Tick() override;
	virtual void	Release() override;

	virtual void	OnEnable() override;
	virtual void	OnDisable() override;

private:
	PHYSXCCDDESC		m_tPhysXCCDDesc;
	PxController* m_pPxController = nullptr;


	


};

END