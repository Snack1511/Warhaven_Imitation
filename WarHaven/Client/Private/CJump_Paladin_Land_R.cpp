#include "stdafx.h"
#include "CJump_Paladin_Land_R.h"


#include "UsefulHeaders.h"


CJump_Paladin_Land_R::CJump_Paladin_Land_R()
{
}

CJump_Paladin_Land_R::~CJump_Paladin_Land_R()
{
}

CJump_Paladin_Land_R* CJump_Paladin_Land_R::Create()
{
    CJump_Paladin_Land_R* pInstance = new CJump_Paladin_Land_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Paladin_Land_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Paladin_Land_R::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_PALADIN_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    
	//m_vecAdjState.push_back(STATE_SWITCH_R_TO_L);

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PALADIN);

	m_vecAdjState.push_back(STATE_IDLE_PALADIN_R);
	m_vecAdjState.push_back(STATE_WALK_PALADIN_R);

	m_vecAdjState.push_back(STATE_RUN_PALADIN_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);

    m_vecAdjState.push_back(STATE_SHIELDWALL_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_RUSH_BEGIN_PALADIN);
    m_vecAdjState.push_back(STATE_SHIELDSLAM_PALADIN);

    Init_CommonState_Player();

    return S_OK;
}

void CJump_Paladin_Land_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;


    switch (ePrevType)
    {
    case Client::STATE_SPRINT_JUMP_PALADIN:
    case Client::STATE_SPRINT_JUMPFALL_PALADIN:
        pOwner->Enter_State(STATE_SPRINT_END_PALADIN);
        return;

        break;
    default:
        break;
    }


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Paladin_Land_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PALADIN_R;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Paladin_Land_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_Paladin_Land_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* PALADIN가 Walk로 오는 조건
    1. 땅에 닿았을 시 . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
