#pragma once
#include "CState_Combat.h"


BEGIN(Client)
class CState_Combat_Cure_Loop_Priest 
	: public CState_Combat
{
	DECLARE_STATE(CState_Combat_Cure_Loop_Priest);

	enum class Enum {eWALK, eRUN, eJUMP, eFALL, eLAND, eIDLE};

	//�� Ŭ���� ��ӹ޾Ƽ� ���� �ִϸ��̼� �߿� ������ �� ����
private:
	CState_Combat_Cure_Loop_Priest();
	virtual ~CState_Combat_Cure_Loop_Priest();


public:
	static CState_Combat_Cure_Loop_Priest* Create();

public:
	virtual HRESULT		Initialize() override;
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	CGameObject* m_pTargetObject = nullptr;


};

END