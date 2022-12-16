#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END

BEGIN(Client)
class CColorController;

class CPaladin_ShieldWall_End
	: public CState
{
	DECLARE_STATE(CPaladin_ShieldWall_End);

private:
	CPaladin_ShieldWall_End();
	virtual ~CPaladin_ShieldWall_End();

public:
	static CPaladin_ShieldWall_End* Create();

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