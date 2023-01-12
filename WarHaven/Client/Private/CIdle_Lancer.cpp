#include "stdafx.h"
#include "CIdle_Lancer.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"
#include "CScript_FollowCam.h"

CIdle_Lancer::CIdle_Lancer()
{
}

CIdle_Lancer::~CIdle_Lancer()
{
}

CIdle_Lancer* CIdle_Lancer::Create()
{
    CIdle_Lancer* pInstance = new CIdle_Lancer();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_Lancer");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_Lancer::Initialize()
{


    // 상하체 분리.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_LANCER;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    
    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;


    m_vecAdjState.push_back(STATE_JUMP_LANCER);
    m_vecAdjState.push_back(STATE_RUN_LANCER);

    m_vecAdjState.push_back(STATE_ATTACK_STING_LANCER);
    m_vecAdjState.push_back(STATE_ATTACK_DEAFULT_LANCER);
    m_vecAdjState.push_back(STATE_ATTACK_BREEZE_BEGIN_LANCER);


    
    return S_OK;
}

void CIdle_Lancer::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{    
    pOwner->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_LANCER);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_Lancer::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Tick(pOwner, pAnimator);
}

void CIdle_Lancer::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    
}

STATE_TYPE CIdle_Lancer::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* LANCER가 Idle로 오는 조건 
    1. 현재 진행중인 애니메이션이 끝났을 때
    */
	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE))
	{
		if (pAnimator->Is_CurAnimFinished())
			return m_eStateType;
	}

    return STATE_END;
}
