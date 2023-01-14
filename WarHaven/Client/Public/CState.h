#pragma once
#include "Client_Defines.h"

BEGIN(Engine)
class CAnimator;
class CGameObject;
END

BEGIN(Client)

class CUnit;

class CState
{
public:
	enum class HIT_TYPE { eLEFT, eRIGHT, eUP, eDOWN, eGROGGY, eEND };
	struct HIT_INFO
	{
		/* 맞고나서 밀려나는 힘 */
		_float	fKnockBackPower = 0.f;
		/* 맞고나서 띄워지는 힘*/
		_float	fJumpPower = 0.f;
		/* 상대 위치에서 내 위치로의 방향, or 밀려날 방향 */
		_float4 vDir = ZERO_VECTOR;
		/* 내가 바라볼 방향 */
		_float4 vLook = ZERO_VECTOR;

		string  strStingBoneName = "0B_R_WP1";

		_bool	bHeadShot = false;
		_bool	bNoneHeadAttack = false;

		_bool	bFace = false;
		_bool	bSting = false;

		_bool	bGuardBreak = false;
		_bool	bFly = false;

		_uint	iLandKeyFrame = 0;

		HIT_TYPE	eHitType = HIT_TYPE::eEND;

		CUnit* pOtherUnit = nullptr;


	};

public:
	struct KEYFRAME_EVENT
	{
		//enum EVENT_TYPE {EVENT_MOVE, EVENT_ATTACK, EVENT_ATTACKEND, EVENT_END};
		_uint	iKeyFrame = 0;
		//EVENT_TYPE	eEventType = EVENT_END;
		_uint	iSequence = 0;
		_bool	bExecuted = false;

	};

public:
	enum STATE_DIRECTION {
		STATE_DIRECTION_NW, STATE_DIRECTION_NE, STATE_DIRECTION_N, 
		STATE_DIRECTION_SW, STATE_DIRECTION_SE, STATE_DIRECTION_S, 
		STATE_DIRECTION_W, 
		STATE_DIRECTION_E, 
		STATE_DIRECTION_END};

protected:
	CState();
	virtual ~CState();

	friend class CState_Manager;
	friend class CUnit;

public:
	virtual CState* Clone() PURE;

public:
	virtual void	OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos);
	virtual void	OnCollisionStay(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType) {};

public:
	_uint Get_StateChangeKeyFrame() {
		return m_iStateChangeKeyFrame;
	}

public:
	void	Set_AnimType(ANIM_TYPE eAnimType) { m_eAnimType = eAnimType; }
	void	Set_AnimIndex(_uint iAnimIndex) { m_iAnimIndex = iAnimIndex; }

	_float	Get_DamagePumping() { return m_fDamagePumping; }
	HIT_INFO& Get_HitInfo() { return m_tHitInfo; }

public:
	virtual void Play_Voice(CUnit* pOwner, wstring strName, _float fVol = 1.f, _int iRand = 0);

public:
	virtual HRESULT		Initialize()	PURE;
	virtual void		Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData = nullptr);
	virtual STATE_TYPE	Tick(CUnit* pOwner, CAnimator* pAnimator);
	virtual void		Exit(CUnit* pOwner, CAnimator* pAnimator) PURE;

protected:
	void Re_Enter(CUnit* pOwner, CAnimator* pAnimator, _float fInterpolationTime = -1.f, _float fAnimSpeed = -1.f);

protected:
	void Init_CommonState_Player();
	void Init_CommonState_Hero_Player();
	void Init_CommonState_AI();
	void Init_CommonState_Hero_AI();

	void Init_AttackState_Priest();



protected:
	virtual void Hit_GroundEffect(CUnit* pOwner);

protected:
	CUnit* m_pOwner = nullptr;

protected:
	vector<STATE_TYPE>		m_vecAdjState;
	STATE_TYPE			m_eStateType = STATE_END;
	ANIM_DIVIDE			m_eAnimDivide = ANIM_DIVIDE::eDEFAULT;
	ANIM_TYPE			m_eAnimType = ANIM_BASE_R;
	_uint				m_iAnimIndex = 0;
	_float				m_fAnimSpeed = 1.f;
	_float				m_fInterPolationTime = 0.1f;

	_float				m_fTimeAcc = 0.f;
	_float				m_fMaxTime = 0.f;

	_bool				m_bExecuted = false;

	_uint				m_iStateChangeKeyFrame = 0;

	_float				m_fETCCurTime = 0.f; // 건설, 점령 수리 등에 사용 
	_float				m_fETCCoolTime = 0.f; // 건설, 점령 수리 등에 사용 

	_float				m_fMyMaxLerp = 0.f; 
	_float				m_fMyAccel = 0.f;
	_float				m_fMaxSpeed = 0.f;

	_bool				m_bMoveTrigger = true;
	_bool				m_bAttackTrigger = false;

	_bool				m_bSkillQ_Trigger = false;
	_bool				m_bSkillE_Trigger = false;
	_bool				m_bSkillR_Trigger = false;

	_bool				m_bAIMove = false;
	_bool				m_bAIAttack = false;
	
	_float				m_fRand = 0.f;
	_uint				m_iRand = 0;
	_int				m_iDirectionRand = 0;


	_bool				m_bHit = false;
	_bool				m_bKeyInput = false;
	_bool				m_bKeyInputable = false;

	_int				m_iFinishedFrame = 0;

	_int				m_iDirectionAnimIndex[STATE_DIRECTION_END];
	_float				m_fDirectionAnimSpeed[STATE_DIRECTION_END];

	_int				m_iStateForcePushIndex = 0;

protected:
	_bool				m_bStraight = false;
	_bool				m_bStopDamagePumping = false;

protected:
	_float				m_fAIDelayTime = 0.f;
	_float				m_fAIMyLength = 0.f; // AI -> Player Length 비교 시 사용
	STATE_TYPE			m_iAINextState = STATE_END; // AI return 사용 용이하게 변수로 만듦

	_float4				m_vAIRandLook;


protected:
		/*애니메이션 끝나고 돌아갈 상태 ENUM 값*/
	STATE_TYPE			m_eBounceState = STATE_END;

	STATE_TYPE			m_ePreStateType = STATE_END;
	STATE_TYPE			m_eJumpFallStateType = STATE_END;


protected:
	/* 공격, 피격 정보
	1. 공격하는 상태가 들고있는 hit Info를
	맞는 상태한테 넘겨줘서 쓰는 방식.
	
	*/
	HIT_INFO			m_tHitInfo;
	_float				m_fDamagePumping = 1.f;

protected:
	vector<KEYFRAME_EVENT>	m_vecKeyFrameEvent;



	/* 플레이어 용도*/
protected:
	_uint	Get_Direction(); // 8방향
	_uint	Get_Direction_Four(); // 4방향
	
	_uint	Move_Direction_Loop(CUnit* pOwner, CAnimator* pAnimator, _float fInterPolationTime);

	void	Change_Location_Loop(_uint iDirection, CAnimator* pAnimator, _float fInterPolationTime);

	_uint	DoMove(_uint iDirection, CUnit* pOwner);
	void	Follow_MouseLook(CUnit* pOwner);
	void	Follow_MouseLook_Turn(CUnit* pOwner);

	void	Prevent_Oneframe(CUnit* pOwner);

	/* AI 용도*/
protected:
	void	Set_Direction_Front_AI(_int& iDirectionRand);
	void	Set_Direction_Back_AI(_int& iDirectionRand);

	_float	Get_Length(CUnit* pOwner);

	_float	Move_Direction_Loop_AI(CUnit* pOwner);

	_float	Get_TargetLook_Length(CUnit* pOwner);

	void	DoMove_AI(CUnit* pOwner, CAnimator* pAnimator);
	void	DoMove_AI_NoTarget(CUnit* pOwner, CAnimator* pAnimator);

protected:
	void	Physics_Setting(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax = true, _bool bBackStep = false);
	void	Physics_Setting_Right(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax = true, _bool bRight = true);

	void	Physics_Setting_AI(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax = true, _bool bBackStep = false);
	void	Physics_Setting_Right_AI(_float fSpeed, CUnit* pOwner, _bool bSpeedasMax = true, _bool bRight = true);

protected:
	void	Enable_ModelParts(CUnit* pOwner, _uint iPartType, _bool bEnable);


protected:
	void					Add_KeyFrame(_uint iKeyFrameIndex, _uint iSequence);
	void					Check_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator);
	virtual		void		On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence) {};

protected:
	CHANNEL_CLIENT	m_eChannel = CHANNEL_EFFECTS;
	wstring	m_wstrSoundKey;
	_float	m_fLoopSoundAcc = 0.f;
	_float	m_fSoundLoopTime = 0.1f;

	_bool	m_bRight = true;

	_float	m_fDelayTime = 0.f;


protected:
	virtual STATE_TYPE		Check_Condition(CUnit* pOwner, CAnimator* pAnimator) PURE;
	virtual void		OnExecute(CUnit* pOwner, CAnimator* pAnimator) {};
	virtual void		TickExecute(CUnit* pOwner, CAnimator* pAnimator) {};


private:

	
};

END