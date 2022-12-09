#include "stdafx.h"
#include "CGuard_Cancel_Paladin.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CGuard_Cancel_Paladin::CGuard_Cancel_Paladin()
{
}

CGuard_Cancel_Paladin::~CGuard_Cancel_Paladin()
{
}

CGuard_Cancel_Paladin* CGuard_Cancel_Paladin::Create()
{
    CGuard_Cancel_Paladin* pInstance = new CGuard_Cancel_Paladin();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CGuard_Cancel_Paladin");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CGuard_Cancel_Paladin::Initialize()
{
	m_eAnimType = ANIM_BASE_L;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 40;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_GUARD_CANCEL_PALADIN;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;

	m_iStateChangeKeyFrame = 75;

    m_vecAdjState.push_back(STATE_IDLE_PALADIN_R);
    m_vecAdjState.push_back(STATE_WALK_PALADIN_R);
    m_vecAdjState.push_back(STATE_RUN_PALADIN_R);

    //m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_STING_PALADIN_R);
    //m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT_PALADIN);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PALADIN);


    //m_vecAdjState.push_back(STATE_COUNTER_PALADIN);
    //m_vecAdjState.push_back(STATE_SPINATTACK_PALADIN);
    //m_vecAdjState.push_back(STATE_SHIELDATTACK_PALADIN);

    return S_OK;
}

void CGuard_Cancel_Paladin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CGuard_Cancel_Paladin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	// 가드 상태에서도 움직일 수 있도록 설정한다.


    return __super::Tick(pOwner, pAnimator);

}

void CGuard_Cancel_Paladin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CGuard_Cancel_Paladin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Sprint로 오는 조건
	1. Hit 상태.
    */

	if (pAnimator->Is_CurAnimFinished())
		return m_eStateType;
   
    return STATE_END;
}
