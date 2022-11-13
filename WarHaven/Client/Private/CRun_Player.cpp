#include "stdafx.h"
#include "CRun_Player.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Player::CRun_Player()
{
}

CRun_Player::~CRun_Player()
{
}

HRESULT CRun_Player::Initialize()
{
    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);

    m_vecAdjState.push_back(STATE_SLIDE_BEGIN_PLAYER);
	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);

	m_vecAdjState.push_back(STATE_ATTACK_VERTICAL_CUT);


	m_iDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
	m_iDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
	m_iDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
	m_iDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
	m_iDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
	m_iDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
	m_iDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
	m_iDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    m_iStateChangeKeyFrame = 0;

	m_fInterPolationTime = 0.1f;


    return S_OK;
}

void CRun_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	m_fMyMaxLerp = 0.4f;
	m_fMyAccel = 20.f;


	CTransform* pMyTransform = pOwner->Get_Transform();
	CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
	
	pMyPhysicsCom->Get_Physics().bAir = false;

	_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
	vCamLook.y = 0.f;

	//1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
	pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

	//���� �����̴� ����
	pMyPhysicsCom->Set_Dir(vCamLook);

	//�ִ�ӵ� ����
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);
	pMyPhysicsCom->Set_SpeedasMax();

    
	if (ePrevType == STATE_RUN_PLAYER_R || ePrevType == STATE_RUN_PLAYER_L)
	{

		m_fInterPolationTime = 0.f;
		pAnimator->Set_CurFrame(22);
	}


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CRun_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

	Move_Direction_Loop(pOwner, pAnimator, 0.1f);


    return __super::Tick(pOwner, pAnimator);

}

void CRun_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. 
    */

    if (KEY(SPACE, NONE))
    {
        // õõ�� 
        if (KEY(CTRL, NONE))
        {
            // �ɾ��.
            if (KEY(W, HOLD) ||
                KEY(A, HOLD) ||
                KEY(S, HOLD) ||
                KEY(D, HOLD))
            {

                return m_eStateType;
            }

        }
    }
   
        //CTRL �� �ٲټ�.
      
      

   


    return STATE_END;
}
