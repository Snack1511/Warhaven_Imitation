#pragma once
#include "CState_PathNavigation_Default_Fiona.h"

BEGIN(Client)
class CState_PathNavigation_Default_Fiona_R
	: public CState_PathNavigation_Default_Fiona
{
	DECLARE_STATE(CState_PathNavigation_Default_Fiona_R);

protected:
	CState_PathNavigation_Default_Fiona_R();
	virtual ~CState_PathNavigation_Default_Fiona_R();

public:
	static CState_PathNavigation_Default_Fiona_R* Create();

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