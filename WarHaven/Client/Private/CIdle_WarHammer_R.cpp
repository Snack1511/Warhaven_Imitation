#include "stdafx.h"
#include "CIdle_WarHammer_R.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CIdle_WarHammer_R::CIdle_WarHammer_R()
{
}

CIdle_WarHammer_R::~CIdle_WarHammer_R()
{
}

CIdle_WarHammer_R* CIdle_WarHammer_R::Create()
{
    CIdle_WarHammer_R* pInstance = new CIdle_WarHammer_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_WarHammer_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_WarHammer_R::Initialize()
{


    // 상하체 분리.
    // 

  //  __super::Initialize();

    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 14;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_IDLE_WARHAMMER_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    
    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;


    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);
    
    m_vecAdjState.push_back(STATE_SWITCH_R_TO_L_WARHAMMER);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_WALK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_JUMP_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_RUNBEGIN_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_CHARGE_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_GROGGYATTACK_WARHAMMER);
    m_vecAdjState.push_back(STATE_AIRSPIKE_BEGIN_WARHAMMER);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_VERTICALATTACK_WARHAMMER_R);
    m_vecAdjState.push_back(STATE_ATTACK_STING_WARHAMMER_R);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_WARHAMMER);

    m_vecAdjState.push_back(STATE_GUARD_BEGIN_WARHAMMER);

    m_vecAdjState.push_back(STATE_INSTALL_BEIGN_WARHAMMER);

    Init_CommonState_Player();

    
    return S_OK;
}

void CIdle_WarHammer_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    /* Owner의 Animator Set Idle로 */
    //GET_COMPONENT_FROM(pOwner, CModel)->Set_ShaderColor(MODEL_PART_WEAPON, _float4(1, 0.3, 0, 0));

    if (ePrevType == STATE_SWITCH_L_TO_R    ||
        ePrevType == STATE_SPRINT_END_WARHAMMER
        )
        m_fInterPolationTime = 0.f;


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_WarHammer_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    return __super::Tick(pOwner, pAnimator);
}

void CIdle_WarHammer_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CIdle_WarHammer_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* WARHAMMER가 Idle로 오는 조건 
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
