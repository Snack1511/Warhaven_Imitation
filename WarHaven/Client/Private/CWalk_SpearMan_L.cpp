#include "stdafx.h"
#include "CWalk_SpearMan_L.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_SpearMan_L::CWalk_SpearMan_L()
{
}

CWalk_SpearMan_L::~CWalk_SpearMan_L()
{
}

CWalk_SpearMan_L* CWalk_SpearMan_L::Create()
{
    CWalk_SpearMan_L* pInstance = new CWalk_SpearMan_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_SpearMan_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_SpearMan_L::Initialize()
{
    __super::Initialize();


    m_eAnimType = ANIM_BASE_L;        
    m_iAnimIndex = 30;                   
    m_eStateType = STATE_WALK_SPEARMAN_L;   


    m_iStateChangeKeyFrame = 0;


    m_fInterPolationTime = 0.1f;

    m_fAnimSpeed = 2.5f;


	m_vecAdjState.push_back(STATE_IDLE_SPEARMAN_L);
	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_L);
	m_vecAdjState.push_back(STATE_RUN_SPEARMAN_L);
	

	
	// 애니메이션 알파벳 순 정렬
	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 28;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 29;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 30;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 31;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 32;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 33;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 34;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 35;
    
    


    return S_OK;
}

void CWalk_SpearMan_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner의 Animator Set Idle로 */

    


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWalk_SpearMan_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	if (pOwner->Is_Air())
		return STATE_JUMPFALL_SPEARMAN_L;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_SPEARMAN_L;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CWalk_SpearMan_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_SpearMan_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN가 Walk로 오는 조건
    1. 
    */

    return __super::Check_Condition(pOwner, pAnimator);
}
