#include "stdafx.h"
#include "CJump_WarHammer_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CJump_WarHammer_R::CJump_WarHammer_R()
{
}

CJump_WarHammer_R::~CJump_WarHammer_R()
{
}

CJump_WarHammer_R* CJump_WarHammer_R::Create()
{
    CJump_WarHammer_R* pInstance = new CJump_WarHammer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CJump_WarHammer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}

HRESULT CJump_WarHammer_R::Initialize()
{
	__super::Initialize();


    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 15;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_JUMP_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.05f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;



    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    //m_vecAdjState.push_back(STATE_IDLE_WARHAMMER);


    m_vecAdjState.push_back(STATE_JUMPFALL_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_JUMP_LAND_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);


	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 16;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 19;

    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 17;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 18;


	iPlaceJumpAnimIndex = 15;


    return S_OK;
}

void CJump_WarHammer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CJump_WarHammer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
   
    return __super::Tick(pOwner, pAnimator);
    
}

void CJump_WarHammer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CJump_WarHammer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Check_Condition(pOwner, pAnimator);

}
