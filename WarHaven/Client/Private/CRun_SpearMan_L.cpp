#include "stdafx.h"
#include "CRun_SpearMan_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_SpearMan_L::CRun_SpearMan_L()
{
}

CRun_SpearMan_L::~CRun_SpearMan_L()
{
}

CRun_SpearMan_L* CRun_SpearMan_L::Create()
{
    CRun_SpearMan_L* pInstance = new CRun_SpearMan_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_SpearMan_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_SpearMan_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 22;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_SPEARMAN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.1f;


    m_fAnimSpeed = 2.5f;

	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_L);
	m_vecAdjState.push_back(STATE_WALK_SPEARMAN_L);



    // 15
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 17;

    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 18;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 19;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 20;

    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 28;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 29;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 30;

    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 21;


    return S_OK;
}

void CRun_SpearMan_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner의 Animator Set Idle로 */
    CColorController::COLORDESC m_tColorDesc;
    ZeroMemory(&m_tColorDesc, sizeof(CColorController::COLORDESC));

    m_tColorDesc.eFadeStyle = CColorController::TIME;
    m_tColorDesc.fFadeInStartTime = 1.f;
    m_tColorDesc.fFadeInTime = 1.f;
    m_tColorDesc.fFadeOutStartTime = 1.f;
    m_tColorDesc.fFadeOutTime = 1.f;

    m_tColorDesc.vTargetColor = _float4(1.f, 0.f, 0.f, 0.f);

    m_tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    m_tColorDesc.iStartKeyFrame = 3;
    m_tColorDesc.iEndKeyFrame = 6;


    GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(m_tColorDesc);



    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CRun_SpearMan_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    { 
        return STATE_STOP_SPEARMAN_L;

    }

    return __super::Tick(pOwner, pAnimator);
}

void CRun_SpearMan_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_SpearMan_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Walk로 오는 조건
    2. WASD 를 누른 상태
    */
	return __super::Check_Condition(pOwner, pAnimator);

}
