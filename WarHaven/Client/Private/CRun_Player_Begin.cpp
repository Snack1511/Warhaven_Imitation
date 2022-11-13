#include "stdafx.h"
#include "CRun_Player_Begin.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CRun_Player_Begin::CRun_Player_Begin()
{
}

CRun_Player_Begin::~CRun_Player_Begin()
{
}

HRESULT CRun_Player_Begin::Initialize()
{
    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);
	m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

    m_iStateChangeKeyFrame = 0;

    m_fInterPolationTime = 0.f;
	m_fMyAccel = 10.f;

	// �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
	


    return S_OK;
}

void CRun_Player_Begin::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
	m_fMyMaxLerp = 0.4f;

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
	pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fRunBeginSpeed);

	bAnimTrigger = false;

	Add_KeyFrame(5, 0);

	m_fAnimSpeed = 10.f;
    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CRun_Player_Begin::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (CUser::Get_Instance()->Get_LastKey() == KEY::LSHIFT)
        return STATE_SPRINT_BEGIN_PLAYER;

	_uint iCurAnimIndex = m_iAnimIndex;

	_uint iDirection = Get_Direction();

	Changing_Location(iDirection, pAnimator);


	Move(iDirection, pOwner);

    //if (KEY(W, HOLD))
    //{
    //    // Key(CTRL + W + A)
    //    if (KEY(A, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_iDirectionAnimIndex[STATE_DIRECTION_NW])
    //        {
    //            ;
    //        }
    //    }

    //    // Key(CTRL + W + D)
    //    else if (KEY(D, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_iDirectionAnimIndex[STATE_DIRECTION_NE])
    //        {
    //            Changing_Location(STATE_DIRECTION_NE, pAnimator);
    //        }
    //    }

    //    // Key(CTRL + W)
    //    else
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_iDirectionAnimIndex[STATE_DIRECTION_N])
    //        {
    //            Changing_Location(STATE_DIRECTION_N, pAnimator);
    //        }
    //    }


    //}

    //// Key(CTRL + S)
    //else if (KEY(S, HOLD))
    //{

    //    // Key(CTRL + S + A)
    //    if (KEY(A, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_iDirectionAnimIndex[STATE_DIRECTION_SW])
    //        {
    //            Changing_Location(STATE_DIRECTION_SW, pAnimator);
    //        }
    //    }

    //    // Key(CTRL + S + D)
    //    else if (KEY(D, HOLD))
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_iDirectionAnimIndex[STATE_DIRECTION_SE])
    //        {
    //            Changing_Location(STATE_DIRECTION_SE, pAnimator);
    //        }
    //    }

    //    // Key(CTRL + S)
    //    else
    //    {
    //        // ����ó��
    //        if (m_iAnimIndex != m_iDirectionAnimIndex[STATE_DIRECTION_S])
    //        {
    //            Changing_Location(STATE_DIRECTION_S, pAnimator);
    //        }
    //    }
    //}

    //// Key(CTRL + A)
    //else if (KEY(A, HOLD))
    //{
    //    // ����ó��
    //    if (m_iAnimIndex != m_iDirectionAnimIndex[STATE_DIRECTION_W])
    //    {
    //        Changing_Location(STATE_DIRECTION_W, pAnimator);
    //    }
    //}

    //// Key(CTRL + D)
    //else if (KEY(D, HOLD))
    //{
    //    // ����ó��
    //    if (m_iAnimIndex != m_iDirectionAnimIndex[STATE_DIRECTION_E])
    //    {
    //        Changing_Location(STATE_DIRECTION_E, pAnimator);
    //    }
    //}
    //

    return __super::Tick(pOwner, pAnimator);
}

void CRun_Player_Begin::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
	bAnimTrigger = false;
}

STATE_TYPE CRun_Player_Begin::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{

    // �ɾ��.
    if (KEY(W, HOLD) ||
        KEY(A, HOLD) ||
        KEY(S, HOLD) ||
        KEY(D, HOLD))
    {

        return m_eStateType;
    }


    return STATE_END;
}


void CRun_Player_Begin::Changing_Location(_uint iDirection, CAnimator* pAnimator)
{
	if (!bAnimTrigger)
	{
		_int iFrame = pAnimator->Get_CurAnimFrame();

		m_iAnimIndex = m_iDirectionAnimIndex[iDirection];

		pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
		pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);

		pAnimator->Set_CurFrame(iFrame);
	}


	bAnimTrigger = true;
}

void CRun_Player_Begin::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
	switch (iSequence)
	{
	case 0:
	{
		CTransform* pMyTransform = pOwner->Get_Transform();
		CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

		//�ӽ�
		pMyPhysicsCom->Get_Physics().bAir = false;

		_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
		vCamLook.y = 0.f;

		//1���� �� (�ȿ��� Normalize ��), 2���� ������ �ɸ��� �ִ�ð�
		pMyTransform->Set_LerpLook(vCamLook, m_fMyMaxLerp);

		//���� �����̴� ����
		pMyPhysicsCom->Set_Dir(vCamLook);

		//�ִ�ӵ� ����
		pMyPhysicsCom->Set_MaxSpeed(pOwner->Get_Status().fRunBeginSpeed);

	}


	default:
		break;
	}


}