#include "CJump_WarHammer_Land_L.h"
#include "CState.h"

#include "UsefulHeaders.h"

CJump_WarHammer_Land_L::CJump_WarHammer_Land_L()
{
}

CJump_WarHammer_Land_L::~CJump_WarHammer_Land_L()
{
}

CJump_WarHammer_Land_L* CJump_WarHammer_Land_L::Create()
{
    CJump_WarHammer_Land_L* pInstance = new CJump_WarHammer_Land_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_WarHammer_Land_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_WarHammer_Land_L::Initialize()
{

    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 10;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_WARHAMMER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

//	m_vecAdjState.push_back(STATE_SWITCH_L_TO_R);

	m_vecAdjState.push_back(STATE_IDLE_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_WALK_WARHAMMER_L);
	m_vecAdjState.push_back(STATE_RUN_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
    m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);
    
    Init_CommonState_Player();

    Add_KeyFrame(10, 0);

    return S_OK;
}

void CJump_WarHammer_Land_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_WarHammer_Land_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_WARHAMMER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_WarHammer_Land_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* 할거없음 */
}

STATE_TYPE CJump_WarHammer_Land_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Walk로 오는 조건
    1. 땅에 닿았을 시 . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}

void CJump_WarHammer_Land_L::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);

    switch (iSequence)
    {
    case 0:
        Play_Sound(L"Env_LandingGround", CHANNEL_ENVIRONMENT, 0.4f);
        break;
    }
}
