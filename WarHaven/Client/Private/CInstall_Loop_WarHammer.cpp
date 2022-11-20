#include "stdafx.h"
#include "CInstall_Loop_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit_WarHammer.h"

#include "CUser.h"

CInstall_Loop_WarHammer::CInstall_Loop_WarHammer()
{
}

CInstall_Loop_WarHammer::~CInstall_Loop_WarHammer()
{
}

CInstall_Loop_WarHammer* CInstall_Loop_WarHammer::Create()
{
    CInstall_Loop_WarHammer* pInstance = new CInstall_Loop_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CInstall_Loop_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CInstall_Loop_WarHammer::Initialize()
{
	m_eAnimType = ANIM_ETC;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 5;                // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_INSTALL_LOOP_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.f;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 2.f;

	m_vecAdjState.push_back(STATE_INSTALL_END_WARHAMMER);

    return S_OK;
}

void CInstall_Loop_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
	CUnit_WarHammer* PWarHammer = static_cast<CUnit_WarHammer*>(pOwner);

	pOwner->On_Use(CUnit::SKILL3);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CInstall_Loop_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{


    return __super::Tick(pOwner, pAnimator);
}

void CInstall_Loop_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CInstall_Loop_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	if (pAnimator->Is_CurAnimFinished())
	{
		return m_eStateType;
	}

   
    return STATE_END;
}
