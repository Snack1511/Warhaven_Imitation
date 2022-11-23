#pragma once
#include "CState.h"

BEGIN(Engine)
END

BEGIN(Client)
class CState_BlendableCharge abstract
	: public CState
{
	enum class Enum { eWALK, eRUN, eJUMP, eFALL, eLAND, eIDLE };

	//�� Ŭ���� ��ӹ޾Ƽ� ���� �ִϸ��̼� �߿� ������ �� ����
protected:
	CState_BlendableCharge();
	virtual ~CState_BlendableCharge();

public:
	virtual HRESULT		Initialize();
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) override;

protected:
	//L, R ����
	ANIM_TYPE	m_eAnimLeftorRight = ANIM_BASE_R;


	/* Idle �ε��� ������ ��*/
	_uint		m_iIdle_Index = 9999;

	/*���⺰ �ش� �ִϸ��̼� �ε��� �־��ָ� ��*/
	_uint				m_iRunLeftAnimIndex[STATE_DIRECTION_END] = {};
	/*���⺰ �ش� �ִϸ��̼� �ε��� �־��ָ� ��*/
	_uint				m_iRunRightAnimIndex[STATE_DIRECTION_END] = {};
	/*���⺰ �ش� �ִϸ��̼� �ε��� �־��ָ� ��*/
	_uint				m_iWalkLeftAnimIndex[STATE_DIRECTION_END] = {};
	/*���⺰ �ش� �ִϸ��̼� �ε��� �־��ָ� ��*/
	_uint				m_iWalkRightAnimIndex[STATE_DIRECTION_END] = {};

	/*���⺰ �ش� �ִϸ��̼� �ε��� �־��ָ� ��*/
	_uint				m_iJumpLeftAnimIndex[STATE_DIRECTION_END] = {};
	/*���⺰ �ش� �ִϸ��̼� �ε��� �־��ָ� ��*/
	_uint				m_iJumpRightAnimIndex[STATE_DIRECTION_END] = {};

	/*�ش� �ִϸ��̼� �ε���*/
	_uint				m_iJumpFallLeftIndex = 9999;
	/*�ش� �ִϸ��̼� �ε���*/
	_uint				m_iJumpFallRightIndex = 9999;

	/*�ش� �ִϸ��̼� �ε���*/
	_uint				m_iLandLeftIndex = 9999;
	/*�ش� �ִϸ��̼� �ε���*/
	_uint				m_iLandRightIndex = 9999;

	/*�ִϸ��̼� ������ ���ư� ���� ENUM ��*/
	STATE_TYPE			m_eWalkState = STATE_END;
	/*�ִϸ��̼� ������ ���ư� ���� ENUM ��*/
	STATE_TYPE			m_eRunState = STATE_END;
	/*�ִϸ��̼� ������ ���ư� ���� ENUM ��*/
	STATE_TYPE			m_eJumpState = STATE_END;
	/*�ִϸ��̼� ������ ���ư� ���� ENUM ��*/
	STATE_TYPE			m_eFallState = STATE_END;
	/*�ִϸ��̼� ������ ���ư� ���� ENUM ��*/
	STATE_TYPE			m_eLandState = STATE_END;
	/*�ִϸ��̼� ������ ���ư� ���� ENUM ��*/
	STATE_TYPE			m_eIdleState = STATE_END;
	/*�ִϸ��̼� ������ ���ư� ���� ENUM ��*/
	STATE_TYPE			m_eBounceState = STATE_END;

protected:
	/* �ִ� ��¡ �ð� */
	_uint				m_iMaxChargeFrame = 100;

	/* m_iMaxChargeFrame�� ���� �� �� ���·� �Ѿ. */
	STATE_TYPE			m_eChargeAttackState = STATE_END;

	/* �ش� Ű�� None�� �Ǹ� ���°� Ǯ�� */
	KEY					m_eCurrentChargeKey = KEY::LAST;


	// �ش� Ű�� ���� �� �����Ӹ�ŭ ������ ���°� Ǯ�� 
	_uint				m_iChargeChangeKeyFrame = 0;



	virtual void	On_MaxCharge(CUnit* pOwner, CAnimator* pAnimator);

protected:
	virtual void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence);

private:
	/*���� ���� �ƴϰ�, �ȿ��� ���� ��*/
	Enum				m_eEnum = Enum::eIDLE;
	_bool				m_bBlendable = true;
	_bool				m_bCycling = false;
	_bool				m_bChargeFinish = false;

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