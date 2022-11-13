#include "stdafx.h"
#include "CRun_Player_Begin_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_Player_Begin_R::CRun_Player_Begin_R()
{
}

CRun_Player_Begin_R::~CRun_Player_Begin_R()
{
}

CRun_Player_Begin_R* CRun_Player_Begin_R::Create()
{
    CRun_Player_Begin_R* pInstance = new CRun_Player_Begin_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_Player_Begin_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_Player_Begin_R::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 25;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUN_BEGIN_PLAYER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


	m_fAnimSpeed = 10.f;

    m_iStateChangeKeyFrame = 21;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_JUMP_PLAYER_R);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_R);


   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;

   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;

   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 22;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 23;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 24;

   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 25;
    


    return S_OK;
}

void CRun_Player_Begin_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
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

STATE_TYPE CRun_Player_Begin_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_RUN_PLAYER_R;


    return __super::Tick(pOwner, pAnimator);

}

void CRun_Player_Begin_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CRun_Player_Begin_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    if (KEY(LSHIFT, NONE))
    {
        if (
            KEY(W, HOLD) ||
            KEY(A, HOLD) ||
            KEY(S, HOLD) ||
            KEY(D, HOLD)
            )
        {
            return m_eStateType;
        }
    }

   
    return STATE_END;
}