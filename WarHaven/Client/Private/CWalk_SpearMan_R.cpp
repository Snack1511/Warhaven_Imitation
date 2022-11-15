#include "stdafx.h"
#include "CWalk_SpearMan_R.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_SpearMan_R::CWalk_SpearMan_R()
{
}

CWalk_SpearMan_R::~CWalk_SpearMan_R()
{
}

CWalk_SpearMan_R* CWalk_SpearMan_R::Create()
{
    CWalk_SpearMan_R* pInstance = new CWalk_SpearMan_R();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWalk_SpearMan_R");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWalk_SpearMan_R::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 30;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WALK_SPEARMAN_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.1f;

    m_fAnimSpeed = 2.5f;


	/*Base*/
    m_vecAdjState.push_back(STATE_IDLE_SPEARMAN_R);
	m_vecAdjState.push_back(STATE_JUMP_SPEARMAN_R);
    m_vecAdjState.push_back(STATE_RUN_SPEARMAN_R);




	// 알파벳 순 애니메이션 정렬
	m_iDirectionAnimIndex[STATE_DIRECTION_E] = 37;
	m_iDirectionAnimIndex[STATE_DIRECTION_N] = 38;
	m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 39;
	m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 40;
	m_iDirectionAnimIndex[STATE_DIRECTION_S] = 41;
	m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 42;
	m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 43;
	m_iDirectionAnimIndex[STATE_DIRECTION_W] = 44;
                            
    
    
    
	



    return S_OK;
}

void CWalk_SpearMan_R::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner의 Animator Set Idle로 */

    


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWalk_SpearMan_R::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	if (pOwner->Is_Air())
		return STATE_JUMPFALL_SPEARMAN_R;

    if (
        KEY(W, NONE) &&
        KEY(A, NONE) &&
        KEY(S, NONE) &&
        KEY(D, NONE)
        )
    {
        return STATE_IDLE_SPEARMAN_R;
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_SpearMan_R::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CWalk_SpearMan_R::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* SPEARMAN가 Walk로 오는 조건
    1. 
    */

	return __super::Check_Condition(pOwner, pAnimator);
}
