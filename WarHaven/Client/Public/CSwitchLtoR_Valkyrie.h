#pragma once
#include "CState.h"

BEGIN(Engine)
class CAnimator;
END


BEGIN(Client)
class CSwitchLtoR_Valkyrie
	: public CState
{
	DECLARE_STATE(CSwitchLtoR_Valkyrie);

private:
	CSwitchLtoR_Valkyrie();
	virtual ~CSwitchLtoR_Valkyrie();

public:
	static CSwitchLtoR_Valkyrie* Create();

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