#pragma once
#include "CState_Common.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Common_Revive_AI
	: public CState_Common
{
	DECLARE_STATE(CState_Common_Revive_AI);

private:
	CState_Common_Revive_AI();
	virtual ~CState_Common_Revive_AI();

	enum PHASE {BEGIN, LOOP, PHASE_END, DANCE, PHASE_NONE};

public:
	static CState_Common_Revive_AI* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	PHASE	m_eCurPhase = BEGIN;
	_float4 m_vPos;
	CGameObject* m_pAbjPlayer = nullptr;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;

};

END