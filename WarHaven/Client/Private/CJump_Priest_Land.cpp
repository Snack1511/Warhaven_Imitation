#include "stdafx.h"
#include "CJump_Priest_Land.h"


#include "UsefulHeaders.h"


CJump_Priest_Land::CJump_Priest_Land()
{
}

CJump_Priest_Land::~CJump_Priest_Land()
{
}

CJump_Priest_Land* CJump_Priest_Land::Create()
{
    CJump_Priest_Land* pInstance = new CJump_Priest_Land();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Priest_Land");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Priest_Land::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 16;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_PRIEST;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;

    
	m_vecAdjState.push_back(STATE_IDLE_PRIEST);
	m_vecAdjState.push_back(STATE_WALK_PRIEST);
	m_vecAdjState.push_back(STATE_RUN_PRIEST);


    Init_AttackState_Priest();
    Init_CommonState_Player();

    return S_OK;
}

void CJump_Priest_Land::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

    if (ePrevType == STATE_GLIDING)
        m_fInterPolationTime = 0.2f;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Priest_Land::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_PRIEST;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Priest_Land::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CJump_Priest_Land::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER가 Walk로 오는 조건
    1. 땅에 닿았을 시 . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
