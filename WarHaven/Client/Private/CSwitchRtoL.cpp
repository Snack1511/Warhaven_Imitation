#include "stdafx.h"
#include "CSwitchRtoL.h"

#include "GameInstance.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

CSwitchRtoL::CSwitchRtoL()
{
}

CSwitchRtoL::~CSwitchRtoL()
{
}

CSwitchRtoL* CSwitchRtoL::Create()
{
    CSwitchRtoL* pInstance = new CSwitchRtoL();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSwitchRtoL");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSwitchRtoL::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 53;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWITCH_R_TO_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 10;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 3.f;

    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);

	m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);


    return S_OK;
}

void CSwitchRtoL::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSwitchRtoL::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
		return STATE_ATTACK_HORIZONTALMIDDLE_L;

    return __super::Tick(pOwner, pAnimator);
}

void CSwitchRtoL::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSwitchRtoL::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::R)
        return m_eStateType;

    return STATE_END;
}
