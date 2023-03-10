#pragma once
#include "CAttack_Qanda.h"


BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)
class CColorController;

class CQanda_Attack_Begin
	: public CAttack_Qanda
{
	DECLARE_STATE(CQanda_Attack_Begin);

private:
	CQanda_Attack_Begin();
	virtual ~CQanda_Attack_Begin();

public:
	static CQanda_Attack_Begin* Create();

public:
	// CState을(를) 통해 상속됨
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);


private:
	_float3	  m_fOffSetLerp = _float3(0.f, 0.f, 0.f);
	_float4x4 m_AnimWeaponOffsetMatrix;

private:
	_uint	m_iAnimFrame = 0;
	_float m_fSndTime = 0.f;
};

END