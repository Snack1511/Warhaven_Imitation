#pragma once
#include "CState_Combat_Attack.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)
class CState_Combat_Attack_Qanda abstract
	: public CState_Combat_Attack
{

	//이 클래스 상속받아서 쓰면 애니메이션 중에 움직일 수 있음
protected:
	CState_Combat_Attack_Qanda();
	virtual ~CState_Combat_Attack_Qanda();


public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;







protected:
	CHierarchyNode* m_pCoreBone = nullptr;

protected:
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

	/* Attack_Begin 에서만 사용 */
	void				Enter_Attack_Begin(CUnit* pOwner);

	/* Aiming 에서만 사용 */
	void				Enter_Aiming(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, _uint eCamLerpType);
	void				Exit_Aiming(CUnit* pOwner, CAnimator* pAnimator);




private:
	_float4x4 m_AnimWeaponOffsetMatrix;
	_float3 m_fOffSetLerp = _float3(0.f, 0.f, 0.f);
	_bool m_bCharge = true;
	_bool m_bAiming = false;

private:
	_float	m_fMatRotXRadian = 0.f;
	_float	m_fMatRotYRadian = 0.f;

};

END