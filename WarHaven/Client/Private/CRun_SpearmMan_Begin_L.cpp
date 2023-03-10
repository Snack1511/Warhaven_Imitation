#include "stdafx.h"
#include "CRun_SpearmMan_Begin_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_SpearmMan_Begin_L::CRun_SpearmMan_Begin_L()
{
}

CRun_SpearmMan_Begin_L::~CRun_SpearmMan_Begin_L()
{
}

CRun_SpearmMan_Begin_L* CRun_SpearmMan_Begin_L::Create()
{
    CRun_SpearmMan_Begin_L* pInstance = new CRun_SpearmMan_Begin_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_SpearmMan_Begin_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_SpearmMan_Begin_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 17;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_RUNBEGIN_SPEARMAN_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_L);
    m_vecAdjState.push_back(STATE_RUN_SPEARMAN_L);

	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
	//m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);

   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 9;

   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 10;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 11;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 12;

   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 13;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 14;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 15;

   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 16;
    

    return S_OK;
}

void CRun_SpearmMan_Begin_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
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


    GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(m_tColorDesc);



    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CRun_SpearmMan_Begin_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pAnimator->Is_CurAnimFinished())
		return STATE_RUN_SPEARMAN_L;


    return __super::Tick(pOwner, pAnimator);

}

void CRun_SpearmMan_Begin_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CRun_SpearmMan_Begin_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	return __super::Check_Condition(pOwner, pAnimator);
}

void CRun_SpearmMan_Begin_L::On_KeyFrameEvent(CUnit * pOwner, CAnimator * pAnimator, const KEYFRAME_EVENT & tKeyFrameEvent, _uint iSequence)
{
	return __super::On_KeyFrameEvent(pOwner, pAnimator, tKeyFrameEvent, iSequence);
}
