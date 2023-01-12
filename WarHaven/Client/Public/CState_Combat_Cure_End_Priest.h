#pragma once
#include "CState_Combat.h"

BEGIN(Client)
class CColorController;

class CState_Combat_Cure_End_Priest
	: public CState_Combat
{
	DECLARE_STATE(CState_Combat_Cure_End_Priest);

private:
	CState_Combat_Cure_End_Priest();
	virtual ~CState_Combat_Cure_End_Priest();

public:
	static CState_Combat_Cure_End_Priest* Create();

public:
	// CState��(��) ���� ��ӵ�
	virtual HRESULT Initialize()	override;
	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;


};

END