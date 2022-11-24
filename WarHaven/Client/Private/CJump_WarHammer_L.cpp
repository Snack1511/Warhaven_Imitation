#include "stdafx.h"
#include "CJump_WarHammer_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_WarHammer_L::CJump_WarHammer_L()
{
}

CJump_WarHammer_L::~CJump_WarHammer_L()
{
}

CJump_WarHammer_L* CJump_WarHammer_L::Create()
{
    CJump_WarHammer_L* pInstance = new CJump_WarHammer_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_WarHammer_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_WarHammer_L::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 5;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_WARHAMMER_L; // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;



    m_vecAdjState.push_back(STATE_JUMPFALL_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_JUMP_LAND_WARHAMMER_L);
    
    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_L);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_L);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_L);

 
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 8;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 9;
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 6;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 8;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 8;

	iPlaceJumpAnimIndex = 5;


    return S_OK;
}

void CJump_WarHammer_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_WarHammer_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CJump_WarHammer_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_WarHammer_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
