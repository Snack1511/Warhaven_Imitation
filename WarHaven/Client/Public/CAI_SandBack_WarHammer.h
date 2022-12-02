#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CAI_SandBack_WarHammer
	: public CState
{
	DECLARE_STATE(CAI_SandBack_WarHammer);

private:
	CAI_SandBack_WarHammer();
	virtual ~CAI_SandBack_WarHammer();

public:
	static CAI_SandBack_WarHammer* Create();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos);

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

};

END