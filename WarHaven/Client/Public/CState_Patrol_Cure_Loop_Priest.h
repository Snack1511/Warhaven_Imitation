#pragma once
#include "CState_Patrol.h"


BEGIN(Client)
class CState_Patrol_Cure_Loop_Priest 
	: public CState_Patrol
{
	DECLARE_STATE(CState_Patrol_Cure_Loop_Priest);

	enum class Enum {eWALK, eRUN, eJUMP, eFALL, eLAND, eIDLE};

	//�� Ŭ���� ��ӹ޾Ƽ� ���� �ִϸ��̼� �߿� ������ �� ����
private:
	CState_Patrol_Cure_Loop_Priest();
	virtual ~CState_Patrol_Cure_Loop_Priest();


public:
	static CState_Patrol_Cure_Loop_Priest* Create();

public:
	virtual HRESULT		Initialize() override;
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;


private:
	CGameObject* m_pTargetObject = nullptr;

	_float m_fSndTime = 0.f;
	_uint m_iSndIdx = 0;
};

END