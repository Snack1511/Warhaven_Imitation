#include "stdafx.h"
#include "CStop_Valkyrie.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CStop_Valkyrie::CStop_Valkyrie()
{
}

CStop_Valkyrie::~CStop_Valkyrie()
{
}

HRESULT CStop_Valkyrie::Initialize()
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;
	m_fInterPolationTime = 0.05f;
	m_fAnimSpeed = 2.f;

	/*���߿� �߰�*/
	m_vecAdjState.push_back(STATE_COUNTER_VALKYRIE);
	//m_vecAdjState.push_back(STATE_SHIELDATTACK_VALKYRIE);
	m_vecAdjState.push_back(STATE_SPINATTACK_VALKYRIE);

	m_iStateChangeKeyFrame = 15;

    return S_OK;
}

void CStop_Valkyrie::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 0.5f;

	if (pData)
	{
		_uint* pDirection = (_uint*)(pData);
		if (*pDirection >= STATE_DIRECTION_END)
			*pDirection = STATE_DIRECTION_N;

		m_iAnimIndex = m_iDirectionAnimIndex[*pDirection];



		delete pDirection;
	}
	

	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CStop_Valkyrie::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	
   
    return __super::Tick(pOwner, pAnimator);
}

void CStop_Valkyrie::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	pOwner->Get_PhysicsCom()->Get_PhysicsDetail().fFrictionRatio = 1.f;
    /* �Ұž��� */
}

STATE_TYPE CStop_Valkyrie::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player�� Walk�� ���� ����
	1.
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
