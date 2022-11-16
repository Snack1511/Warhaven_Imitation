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


	m_iStateChangeKeyFrame = 0;
	
	m_vecAdjState.push_back(STATE_SPEARMAN_GUARDBREAK);
	m_vecAdjState.push_back(STATE_ATTACK_STING_SPEARMAN);
	m_vecAdjState.push_back(STATE_SPEARMAN_SPECIALGUARD);

	//m_fMaxSpeed = 4.f;


    return S_OK;
}

void CStop_SpearMan::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	//_uint* pDirection = (_uint*)(pData);
	//if (*pDirection >= STATE_DIRECTION_END)
	//	*pDirection = STATE_DIRECTION_N;

	//m_iAnimIndex = m_iDirectionAnimIndex[*pDirection];

	//delete pDirection;

	m_iAnimIndex = m_iDirectionAnimIndex[STATE_DIRECTION_N];



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
}

STATE_TYPE CStop_SpearMan::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player�� Stop�� ���� ����
	1. WASD ���� ���� ���� �� 
	*/

	// m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

	// W �� A �� ������ ���� ������ �̵��Ѵ�.
	// W �� D �� ������ ���� ������ �̵��Ѵ�.

	// ���� WASD �� �����ٸ�
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
