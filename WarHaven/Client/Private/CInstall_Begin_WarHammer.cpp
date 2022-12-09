#include "stdafx.h"
#include "CInstall_Begin_WarHammer.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit_WarHammer.h"

#include "CUser.h"


CInstall_Begin_WarHammer::CInstall_Begin_WarHammer()
{
}

CInstall_Begin_WarHammer::~CInstall_Begin_WarHammer()
{
}

CInstall_Begin_WarHammer* CInstall_Begin_WarHammer::Create()
{
    CInstall_Begin_WarHammer* pInstance = new CInstall_Begin_WarHammer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CInstall_Begin_WarHammer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CInstall_Begin_WarHammer::Initialize()
{
	m_eAnimType = ANIM_ETC;          // 애니메이션의 메쉬타입
	m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
	m_eStateType = STATE_INSTALL_BEIGN_WARHAMMER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

	// 선형 보간 시간
	m_fInterPolationTime = 0.1f;

	m_iStateChangeKeyFrame = 20;

	// 애니메이션의 전체 속도를 올려준다.
	m_fAnimSpeed = 3.f;

	
	m_vecAdjState.push_back(STATE_INSTALL_LOOP_WARHAMMER);
	m_vecAdjState.push_back(STATE_INSTALL_END_WARHAMMER);


    return S_OK;
}

void CInstall_Begin_WarHammer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	CUnit_WarHammer* PWarHammer = static_cast<CUnit_WarHammer*>(pOwner);

	PWarHammer->Set_BarricadeMatrix();

	__super::Enable_ModelParts(PWarHammer, 4, false);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CInstall_Begin_WarHammer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CInstall_Begin_WarHammer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	__super::Enable_ModelParts(pOwner, 4, true);
}

STATE_TYPE CInstall_Begin_WarHammer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Sprint로 오는 조건
	1. 스프린트 공격 준비가 끝나면
    */

	if (!pOwner->Can_Use(CUnit::SKILL3))
		return STATE_END;

	CUnit_WarHammer* PWarHammer = static_cast<CUnit_WarHammer*>(pOwner);

	// R을 누르면 임시로 보일 수 있게 설정
	if (KEY(R, TAP))
		pOwner->Get_SkillTrigger().bSkillRTrigger = !pOwner->Get_SkillTrigger().bSkillRTrigger;

	// R을 눌렀던 상태에서 LButtion 을 누르면 만약 바리게이트 설치할 개수가 남아 있다면 바리게이트 위치를 설정하고 상태를 바꾼다.
	if (pOwner->Get_SkillTrigger().bSkillRTrigger && KEY(LBUTTON, TAP) && PWarHammer->Get_Size() > 0)
	{
		PWarHammer->Set_BarricadeMatrix();
		pOwner->Get_SkillTrigger().bSkillRTrigger = false;
		return m_eStateType;
	}
   

    return STATE_END;
}
