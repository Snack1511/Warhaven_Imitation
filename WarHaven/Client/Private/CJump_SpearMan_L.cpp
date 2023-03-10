#include "stdafx.h"
#include "CJump_SpearMan_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_SpearMan_L::CJump_SpearMan_L()
{
}

CJump_SpearMan_L::~CJump_SpearMan_L()
{
}

CJump_SpearMan_L* CJump_SpearMan_L::Create()
{
    CJump_SpearMan_L* pInstance = new CJump_SpearMan_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_SpearMan_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CJump_SpearMan_L::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 4;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_SPEARMAN_L; // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;


    m_vecAdjState.push_back(STATE_JUMPFALL_SPEARMAN_L);
    m_vecAdjState.push_back(STATE_JUMP_LAND_SPEARMAN_L);

	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 4;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 5;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 6;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 7;
    

	iPlaceJumpAnimIndex = 3;


    return S_OK;
}

void CJump_SpearMan_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{

    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CJump_SpearMan_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);

}

void CJump_SpearMan_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_SpearMan_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
