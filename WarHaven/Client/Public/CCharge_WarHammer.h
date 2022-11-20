#pragma once
#include "CState_BlendableCharge.h"

BEGIN(Client)
class CColorController;

class CCharge_WarHammer
	: public CState_BlendableCharge
{
	DECLARE_STATE(CCharge_WarHammer);

protected:
	CCharge_WarHammer();
	virtual ~CCharge_WarHammer();


public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	_float m_fOriSpeed = 0.f;

};

END