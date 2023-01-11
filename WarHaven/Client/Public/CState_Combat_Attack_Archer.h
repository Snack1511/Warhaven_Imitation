#pragma once
#include "CState_Combat_Attack.h"

BEGIN(Engine)
class CHierarchyNode;
END

BEGIN(Client)
class CState_Combat_Attack_Archer abstract
	: public CState_Combat_Attack
{

	//�� Ŭ���� ��ӹ޾Ƽ� ���� �ִϸ��̼� �߿� ������ �� ����
protected:
	CState_Combat_Attack_Archer();
	virtual ~CState_Combat_Attack_Archer();


public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	_uint	m_iKeyAwayAnimIndex = 0; // �� �� �ִϸ��̼� �ε���
	_uint	m_iMinCancelAnimIndex = 0;  // �ִϸ��̼� �ٲ�� �ּҰ�

	_bool	m_bAiming = false;

	_float	m_fMatRotXRadian = 0.f;
	_float	m_fMatRotYRadian = 0.f;


protected:
	//CHierarchyNode* m_pCoreBone = nullptr;

protected:
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

	/* Attack_Begin ������ ��� */
	void				Enter_Attack_Begin(CUnit* pOwner);

	/* Aiming ������ ��� */
	void				Enter_Aiming(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType);
	void				Exit_Aiming(CUnit* pOwner, CAnimator* pAnimator);


private:
	_bool				m_bHitEffect = false;

	CHierarchyNode* m_pCoreBone = nullptr;

private:
	_bool		m_bAfterEffect = false;
	_bool		m_bBlood = false;
	_float		m_fCreateTimeAcc = 0.f;
	_float		m_fCreateTime = 0.02f;
	_float		m_fCreateTimeSlow = 0.07f;
	void		Create_SoilEffect(); 

private:
	_bool Check_ArrowRay(_float4* pOutPos);


};

END