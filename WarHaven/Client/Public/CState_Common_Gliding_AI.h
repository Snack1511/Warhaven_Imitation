#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Common_Gliding_AI
	: public CState
{
	DECLARE_STATE(CState_Common_Gliding_AI);

private:
	enum Glide_State { GLIDE_JUMP, GLIDE_LOOP, GLIDE_STOP, GLIDE_LAND, GLIDE_NOENTER, GLIDE_END };

private:
	CState_Common_Gliding_AI();
	virtual ~CState_Common_Gliding_AI();

public:
	static CState_Common_Gliding_AI* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

	STATE_TYPE Glide_Land(CUnit* pOwner);
	STATE_TYPE Glide_Cancel(CUnit* pOwner);

private:
	void Move_Gliding(CGameObject* pSourObject, string strTargetPos, _bool bGilde);

private:
	_bool	m_bReturn = false;

private:
	Glide_State m_eGlideState = GLIDE_END;

};

END