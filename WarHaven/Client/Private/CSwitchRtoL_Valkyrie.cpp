#include "stdafx.h"
#include "CSwitchRtoL_Valkyrie.h"

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

CSwitchRtoL_Valkyrie::CSwitchRtoL_Valkyrie()
{
}

CSwitchRtoL_Valkyrie::~CSwitchRtoL_Valkyrie()
{
}

CSwitchRtoL_Valkyrie* CSwitchRtoL_Valkyrie::Create()
{
    CSwitchRtoL_Valkyrie* pInstance = new CSwitchRtoL_Valkyrie();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CSwitchRtoL_Valkyrie");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CSwitchRtoL_Valkyrie::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 44;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_SWITCH_R_TO_L_VALKYRIE;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 20;

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    m_vecAdjState.push_back(STATE_IDLE_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_RUN_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_WALK_VALKYRIE_L);
    m_vecAdjState.push_back(STATE_JUMP_VALKYRIE_L);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_VALKYRIE);


    /*나중에 추가*/
    m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
    m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
    m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);


    return S_OK;
}

void CSwitchRtoL_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CSwitchRtoL_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	/*if (CUser::Get_Instance()->Get_LastKey() == KEY::LBUTTON)
		return STATE_ATTACK_HORIZONTALMIDDLE_VALKYRIE_L;

    if (CUser::Get_Instance()->Get_LastKey() == KEY::V)
        return STATE_CHARGE_FRONT_VALKYRIE_L;


    if(MOUSE_MOVE(MMS_WHEEL) > 0)
        return STATE_VERTICALATTACK_VALKYRIE_L;*/

    return __super::Tick(pOwner, pAnimator);
}

void CSwitchRtoL_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CSwitchRtoL_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* VALKYRIE가 Idle로 오는 조건
    1. 현재 진행중인 애니메이션이 끝났을 때
    */

    if (CUser::Get_Instance()->Get_LastKey() == KEY::TAB)
        return m_eStateType;

    return STATE_END;
}
