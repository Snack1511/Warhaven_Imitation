#pragma once
#include "CState.h"

BEGIN(Client)
class CSprint_End_WarHammer
	: public CState
{
	DECLARE_STATE(CSprint_End_WarHammer);

private:
	CSprint_End_WarHammer();
	virtual ~CSprint_End_WarHammer();

public:
	static CSprint_End_WarHammer* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);


};

END