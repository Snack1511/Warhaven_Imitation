#include "stdafx.h"
#include "CSpearMan_Attack_Sting.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"


CSpearMan_Attack_Sting::CSpearMan_Attack_Sting()
{
}

CSpearMan_Attack_Sting::~CSpearMan_Attack_Sting()
{
}

CSpearMan_Attack_Sting* CSpearMan_Attack_Sting::Create()
{
    CSpearMan_Attack_Sting* pInstance = new CSpearMan_Attack_Sting();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSpearMan_Attack_Sting");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSpearMan_Attack_Sting::Initialize()
{

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 0;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_ATTACK_STING_SPEARMAN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 80;
    
    m_vecAdjState.push_back(STATE_IDLE_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_WALK_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);

	m_vecAdjState.push_back(STATE_SPEARMAN_GUARDBREAK);

	Add_KeyFrame(27, 0);
	Add_KeyFrame(52, 1);

    return S_OK;
}

void CSpearMan_Attack_Sting::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSpearMan_Attack_Sting::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_IDLE_SPEARMAN_R;

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_SPEARMAN_R;

	if (m_bAttackTrigger)
	{
		if (pOwner->Is_Weapon_R_Collision())
			return STATE_BOUNCE_SPEARMAN;
	}


    return __super::Tick(pOwner, pAnimator);
}

void CSpearMan_Attack_Sting::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
}

STATE_TYPE CSpearMan_Attack_Sting::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN가 Attack 으로 오는 조건
    1. CTRL + LBuutton 을 이용해 공격한다.
    */

   
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
        return m_eStateType;

    return STATE_END;
}


void CSpearMan_Attack_Sting::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{

	case 0:
		m_bAttackTrigger = true;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, true);
		break;

	case 1:
		m_bAttackTrigger = false;
		pOwner->Enable_UnitCollider(CUnit::WEAPON_R, false);
		break;

	default:
		break;
	}
}
