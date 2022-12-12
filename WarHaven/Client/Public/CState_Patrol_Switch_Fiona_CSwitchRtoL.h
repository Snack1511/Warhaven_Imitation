#pragma once
#include "CState_Patrol_Switch_Fiona.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CState_Patrol_Switch_Fiona_CSwitchRtoL
	: public CState_Patrol_Switch_Fiona
{
	DECLARE_STATE(CState_Patrol_Switch_Fiona_CSwitchRtoL);

private:
	CState_Patrol_Switch_Fiona_CSwitchRtoL();
	virtual ~CState_Patrol_Switch_Fiona_CSwitchRtoL();

public:
	static CState_Patrol_Switch_Fiona_CSwitchRtoL* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;

private:
	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
};

END