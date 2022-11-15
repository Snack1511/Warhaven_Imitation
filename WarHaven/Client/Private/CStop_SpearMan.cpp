#include "stdafx.h"
#include "CStop_SpearMan.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CStop_SpearMan::CStop_SpearMan()
{
}

CStop_SpearMan::~CStop_SpearMan()
{
}

HRESULT CStop_SpearMan::Initialize()
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;
	m_fInterPolationTime = 0.05f;
	m_fAnimSpeed = 2.f;

	m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
	m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);

	m_vecAdjState.push_back(STATE_SLIDE_BEGIN_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


	m_iStateChangeKeyFrame = 15;
	
	//m_fMaxSpeed = 4.f;


    return S_OK;
}

void CStop_SpearMan::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	_uint* pDirection = (_uint*)(pData);
	if (*pDirection >= STATE_DIRECTION_END)
		*pDirection = STATE_DIRECTION_N;

	m_iAnimIndex = m_iDirectionAnimIndex[*pDirection];



	delete pDirection;

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_SpearMan::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
        
   
    return __super::Tick(pOwner, pAnimator);
}

void CStop_SpearMan::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* 할거없음 */
}

STATE_TYPE CStop_SpearMan::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Walk로 오는 조건
	1.
	*/

	// m_eStateType 이 End 에 가지 않으면 Enter 를 호출한다.

	// W 랑 A 를 누르면 왼쪽 앞으로 이동한다.
	// W 랑 D 를 누르면 왼쪽 옆으로 이동한다.

	// 만약 WASD 를 눌렀다면
	if (
		KEY(W, NONE) &&
		KEY(A, NONE) &&
		KEY(S, NONE) &&
		KEY(D, NONE)
		)
	{
		return m_eStateType;
	}




    return STATE_END;
}
