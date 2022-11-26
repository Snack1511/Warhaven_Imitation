#include "stdafx.h"
#include "CRun_AI_TG_Warrior_L.h"

#include "UsefulHeaders.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CColorController.h"

CRun_AI_TG_Warrior_L::CRun_AI_TG_Warrior_L()
{
}

CRun_AI_TG_Warrior_L::~CRun_AI_TG_Warrior_L()
{
}

CRun_AI_TG_Warrior_L* CRun_AI_TG_Warrior_L::Create()
{
    CRun_AI_TG_Warrior_L* pInstance = new CRun_AI_TG_Warrior_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CRun_AI_TG_Warrior_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CRun_AI_TG_Warrior_L::Initialize()
{
    __super::Initialize();

    m_eAnimType = ANIM_BASE_L;            // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 22;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_RUN_WARRIOR_L;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)


    m_iStateChangeKeyFrame = 0;

    // ���� ���� �ð�
    m_fInterPolationTime = 0.1f;

    // �ִϸ��̼��� ��ü �ӵ��� �÷��ش�.
    m_fAnimSpeed = 2.5f;

    // Idle -> ����(Jump, RUn ���) -> L, R �� -> ���¿��� �� �� �ִ� �� ��(Attack -> Move) -> �ݺ�

    //enum �� Idle ���� ���ε���ؼ� �� �� �ִ� State �� �������ش�.
//    m_vecAdjState.push_back(STATE_ATTACK_WARRIOR);

	//m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_L);

    // 15

   m_iDirectionAnimIndex[STATE_DIRECTION_E] = 18;
   m_iDirectionAnimIndex[STATE_DIRECTION_N] = 19;
   m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 20;
   m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 21;
   m_iDirectionAnimIndex[STATE_DIRECTION_S] = 34;
   m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 35;
   m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 36;
   m_iDirectionAnimIndex[STATE_DIRECTION_W] = 22;


    return S_OK;
}

void CRun_AI_TG_Warrior_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    /* Owner�� Animator Set Idle�� */
  /*  CColorController::COLORDESC m_tColorDesc;
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


    GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(m_tColorDesc);*/



    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CRun_AI_TG_Warrior_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iRand == 0)
        return AI_STATE_WALK_WARRIOR_L;

    if (m_bAIMove)
        return AI_STATE_RUNBEGIN_WARRIOR_R;

    CUnit* pUnit = pOwner->Get_TargetUnit();

    pOwner->Set_LookToTarget();



    _float4 vLook = (pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS));
    _float4 vRight = (pUnit->Get_Transform()->Get_World(WORLD_LOOK) - pOwner->Get_Transform()->Get_World(WORLD_LOOK));

    _float fLength = vLook.Length();

    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    vLook.Normalize();
    vRight.Normalize();

    pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);


    pMyPhysicsCom->Set_Accel(m_fMyAccel);

    if (fLength < 2.f)
    {
        _int iRand = rand() % 20;

        if (iRand < 15)
            return AI_STATE_ATTACK_HORIZONTALMIDDLE_R;

        else
            return AI_STATE_GUARD_BEGIN_WARRIOR;

        //switch (iRand)
        //{

        //case 0:
        //case 1:

        //    return AI_STATE_ATTACK_HORIZONTALMIDDLE_L;

        //case 2:

        //    return AI_STATE_GUARD_BEGIN_WARRIOR;

        //default:
        //    break;
        //}
    }


    //if (fLength > 6.f)
    //{
        if (!vLook.Is_Zero())
            pMyPhysicsCom->Set_Dir(vLook);


        Change_Location_Loop(STATE_DIRECTION_N, pAnimator, 0.1f);

    //}
    //else
    //{

    //    _float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    //    _float4 vCamRight = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);


    //    if (vRight.z > 0.9f)
    //    {
    //        if ((!vLook.Is_Zero()))
    //            pMyPhysicsCom->Set_Dir(vLook);

    //        Change_Location_Loop(STATE_DIRECTION_N, pAnimator, 0.1f);
    //    }
    //}



    return __super::Tick(pOwner, pAnimator);
}

void CRun_AI_TG_Warrior_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CRun_AI_TG_Warrior_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    2. WASD �� ���� ����
    */

	return __super::Check_Condition(pOwner, pAnimator);

	//if (KEY(CTRL, NONE))
	//{
	//	if (KEY(W, HOLD) ||
	//		KEY(A, HOLD) ||
	//		KEY(S, HOLD) ||
	//		KEY(D, HOLD))
	//	{
	//		return m_eStateType;
	//	}
	//}


    return STATE_END;
}
