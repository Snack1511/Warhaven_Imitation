#pragma once
#include "CState_Blendable.h"

BEGIN(Engine)
END

BEGIN(Client)
class CState_AI_Warrior_Blendable abstract
	: public CState_Blendable
{
	enum class Enum {eWALK, eRUN, eJUMP, eFALL, eLAND, eIDLE};

	//이 클래스 상속받아서 쓰면 애니메이션 중에 움직일 수 있음
protected:
	CState_AI_Warrior_Blendable();
	virtual ~CState_AI_Warrior_Blendable();

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos) override;
	virtual void	OnCollisionStay(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType) override;


public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;


protected:
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);
	
protected:
	_bool				m_bAIAttack = false;
	_bool				m_bAIJump = false;
	_bool				m_bAIMove = false;

private:
	/*사전 설정 아니고, 안에서 쓰는 것*/
	Enum				m_eEnum = Enum::eIDLE;
	_bool				m_bBlendable = true;
	_bool				m_bCycling = false;
	_bool				m_bHitEffect = false;

private:
	_bool		m_bAfterEffect = false;
	_bool		m_bBlood = false;
	_float		m_fCreateTimeAcc = 0.f;
	_float		m_fCreateTime = 0.02f;
	_float		m_fCreateTimeSlow = 0.07f;
	void		Create_SwordAfterEffect();
	void		Create_SoilEffect(); 
	void		Create_BloodEffect();

private:
	void				Move_Cycle(CAnimator* pAnimator, _uint* arrDirectionAnimIndices, ANIM_TYPE eAnimType);
	void				On_EnumChange(Enum eEnum, CAnimator* pAnimator);

private:
	STATE_TYPE	Update_Walk(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Run(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Jump(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Fall(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Land(CUnit* pOwner, CAnimator* pAnimator);
	STATE_TYPE	Update_Idle(CUnit* pOwner, CAnimator* pAnimator);
};

END