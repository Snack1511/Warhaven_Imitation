#include "CJump_Archer_Land_L.h"
#include "CState.h"

#include "UsefulHeaders.h"

CJump_Archer_Land_L::CJump_Archer_Land_L()
{
}

CJump_Archer_Land_L::~CJump_Archer_Land_L()
{
}

CJump_Archer_Land_L* CJump_Archer_Land_L::Create()
{
    CJump_Archer_Land_L* pInstance = new CJump_Archer_Land_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Archer_Land_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Archer_Land_L::Initialize()
{

    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 8;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_LAND_ARCHER_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.15f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;


	m_vecAdjState.push_back(STATE_IDLE_ARCHER_L);
	m_vecAdjState.push_back(STATE_WALK_ARCHER_L);
	m_vecAdjState.push_back(STATE_RUN_ARCHER_L);

    m_vecAdjState.push_back(STATE_ATTACK_BEGIN_ARCHER);

    


    return S_OK;
}

void CJump_Archer_Land_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fMaxSpeed = pOwner->Get_Status().fWalkSpeed;
    Physics_Setting(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
    pAnimator->Set_CurFrame(10);
}

STATE_TYPE CJump_Archer_Land_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return STATE_IDLE_ARCHER_L;

    return __super::Tick(pOwner, pAnimator);
}

void CJump_Archer_Land_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* 할거없음 */
}

STATE_TYPE CJump_Archer_Land_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* ARCHER가 Walk로 오는 조건
    1. 땅에 닿았을 시 . 
    */

	if (!pOwner->Is_Air())
		return m_eStateType;

    return STATE_END;
}
