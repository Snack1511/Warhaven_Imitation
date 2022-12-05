#include "stdafx.h"
#include "CJump_Paladin_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_Paladin_L::CJump_Paladin_L()
{
}

CJump_Paladin_L::~CJump_Paladin_L()
{
}

CJump_Paladin_L* CJump_Paladin_L::Create()
{
    CJump_Paladin_L* pInstance = new CJump_Paladin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_Paladin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_Paladin_L::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_PALADIN_L; // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;


    m_vecAdjState.push_back(STATE_JUMPFALL_PALADIN_L);
    m_vecAdjState.push_back(STATE_JUMP_LAND_PALADIN_L);
    
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_PALADIN_L);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_PALADIN_L);
 
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 8;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 7;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 7;

	iPlaceJumpAnimIndex = 4;


    return S_OK;
}

void CJump_Paladin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_Paladin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CJump_Paladin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_Paladin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
