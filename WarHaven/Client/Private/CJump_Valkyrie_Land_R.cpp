#include "stdafx.h"
#include "CJump_Valkyrie_Land_R.h"


#include "UsefulHeaders.h"


CJump_Valkyrie_Land_R::CJump_Valkyrie_Land_R()
{
}

CJump_Valkyrie_Land_R::~CJump_Valkyrie_Land_R()
{
}

CJump_Valkyrie_Land_R* CJump_Valkyrie_Land_R::Create()
{
    CJump_Valkyrie_Land_R* pInstance = new CJump_Valkyrie_Land_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Valkyrie_Land_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Valkyrie_Land_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_VALKYRIE_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    
	//m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_VALKYRIE);

	m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_R);
	m_vecAdjState.push_back(STATE_WALK_VALKYRIE_R);

	m_vecAdjState.push_back(STATE_RUN_VALKYRIE_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_VALKYRIE_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_VALKYRIE_R);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);

    Init_CommonState_Hero_Player();

    Add_KeyFrame(9, 0);

    return S_OK;
}

void CJump_Valkyrie_Land_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;


    switch (ePrevType)
    {
    case Client::STATE_SPRINT_JUMP_VALKYRIE:
    case Client::STATE_SPRINT_JUMPFALL_VALKYRIE:
        pOwner->Enter_State(STATE_SPRINT_END_VALKYRIE);
        return;

        break;
    default:
        break;
    }

    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Valkyrie_Land_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_VALKYRIE_L;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Valkyrie_Land_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_Valkyrie_Land_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* VALKYRIE가 Walk로 오는 조건
    1. 땅에 닿았을 시 . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}

void CJump_Valkyrie_Land_R::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_LandingGround", CHANNEL_ENVIRONMENT);
        break;
    }
}