#pragma once
#include "PhysXCollider.h"

BEGIN(Engine)

class CPhysics;

class ENGINE_DLL CPhysXCharacter
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
	void	Set_Position(_float4 vPos);

public:
	virtual void	Add_Trigger(const TRIGGERDESC& tTriggerDesc);


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual void Start() override;


	virtual void	Tick() override;
	virtual void	Late_Tick() override;
	virtual void	Release() override;

	virtual void	OnEnable() override;
	virtual void	OnDisable() override;

private:
	CPhysics* m_pPhysicsCom = nullptr;


	PHYSXCCDDESC		m_tPhysXCCDDesc;
	PxController* m_pPxController = nullptr;

	PxControllerFilters		m_tControllerFilters;


	


};

END