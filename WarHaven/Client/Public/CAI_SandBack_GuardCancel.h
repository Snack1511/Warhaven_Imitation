//#pragma once
//#include "CState_Hit.h"
//
//BEGIN(Engine)
//END
//
//
//BEGIN(Client)
//class CAI_SandBack_GuardCancel
//	: public CState_Hit
//{
//	DECLARE_STATE(CAI_SandBack_GuardCancel);
//
//private:
//	CAI_SandBack_GuardCancel();
//	virtual ~CAI_SandBack_GuardCancel();
//
//public:
//	static CAI_SandBack_GuardCancel* Create();
//
//public:
//	// CState을(를) 통해 상속됨
//	virtual HRESULT Initialize()	override;
//	virtual void Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData = nullptr) override;
//	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
//	virtual void Exit(CUnit* pOwner, CAnimator* pAnimator) override;
//
//private:
//	virtual STATE_TYPE Check_Condition(CUnit* pOwner, CAnimator* pAnimator) override;
//
//private:
//	_bool	bTest = false; 
//
//};
//
//END