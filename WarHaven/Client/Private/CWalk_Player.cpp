#include "stdafx.h"
#include "CWalk_Player.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CWalk_Player::CWalk_Player()
{
}

CWalk_Player::~CWalk_Player()
{
}

HRESULT CWalk_Player::Initialize()
{
    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);

	m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);


    return S_OK;
}

void CWalk_Player::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType)
{
    /* Owner�� Animator Set Idle�� */

    


    __super::Enter(pOwner, pAnimator, ePrevType);
}

STATE_TYPE CWalk_Player::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    //_float4 vCamLook = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    //vCamLook.y = 0.f;

    ////Dir : ���� �̵�����
    //_float4 vDir;
    //_float4 vRight = pOwner->Get_Transform()->Get_World(WORLD_RIGHT);
    //_float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);

    //if (KEY(W, HOLD))
    //{
    //    _float4 vDir = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    //    vDir.y = 0.f;
    //    m_pTransform->Set_LerpLook(vCamLook, 0.4f);
    //    m_pPhysics->Set_Dir(vDir);
    //    m_pPhysics->Set_MaxSpeed(m_tUnitStatus.fRunSpeed);
    //    m_pPhysics->Set_Accel(100.f);
    //}
    //else if (KEY(A, HOLD))
    //{
    //    _float4 vDir = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);
    //    vDir *= -1.f;
    //    vDir.y = 0.f;
    //    m_pTransform->Set_LerpLook(vCamLook, 0.4f);
    //    m_pPhysics->Set_Dir(vDir);
    //    m_pPhysics->Set_MaxSpeed(m_tUnitStatus.fRunSpeed);
    //    m_pPhysics->Set_Accel(100.f);
    //}
    //else if (KEY(S, HOLD))
    //{
    //    _float4 vDir = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_LOOK);
    //    vDir *= -1.f;
    //    vDir.y = 0.f;
    //    m_pTransform->Set_LerpLook(vCamLook, 0.4f);
    //    m_pPhysics->Set_Dir(vDir);
    //    m_pPhysics->Set_MaxSpeed(m_tUnitStatus.fWalkSpeed);
    //    m_pPhysics->Set_Accel(100.f);
    //}
    //else if (KEY(D, HOLD))
    //{
    //    _float4 vDir = GAMEINSTANCE->Get_CurCam()->Get_Transform()->Get_World(WORLD_RIGHT);
    //    vDir.y = 0.f;
    //    m_pTransform->Set_LerpLook(vCamLook, 0.4f);
    //    m_pPhysics->Set_Dir(vDir);
    //    m_pPhysics->Set_MaxSpeed(m_tUnitStatus.fRunSpeed);
    //    m_pPhysics->Set_Accel(100.f);
    //}
    //else
    //{
    //    m_pPhysics->Set_Speed(0.f);
    //}

    //if (KEY(SPACE, TAP))
    //{
    //    m_pPhysics->Set_Jump(6.f);
    //}






    if (KEY(W, HOLD))
    {
        // Key(CTRL + W + A)
        if (KEY(A, HOLD))
        {
            // ����ó��
            //vDir = vLook - vRight;

            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NW])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_NW];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // Key(CTRL + W + D)
        else if (KEY(D, HOLD))
        {
            //vDir = vLook + vRight;

            // ����ó��
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_NE])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_NE];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // Key(CTRL + W)
        else
        {
            // ����ó��
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_N])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_N];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
            }
        }


    }

    // Key(CTRL + S)
    else if (KEY(S, HOLD))
    {

        // Key(CTRL + S + A)
        if (KEY(A, HOLD))
        {
            // ����ó��
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SW])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_SW];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // Key(CTRL + S + D)
        else if (KEY(D, HOLD))
        {
            // ����ó��
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_SE])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_SE];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }

        // Key(CTRL + S)
        else
        {
            // ����ó��
            if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_S])
            {
                m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_S];

                pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
                pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
            }
        }
    }

    // Key(CTRL + A)
    else if (KEY(A, HOLD))
    {
        // ����ó��
        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_W])
        {
            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_W];

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.2f);
        }
    }

    // Key(CTRL + D)
    else if (KEY(D, HOLD))
    {
        // ����ó��
        if (m_iAnimIndex != m_VecDirectionAnimIndex[STATE_DIRECTION_E])
        {
            m_iAnimIndex = m_VecDirectionAnimIndex[STATE_DIRECTION_E];

            pAnimator->Set_CurAnimIndex(m_eAnimType, m_iAnimIndex);
            pAnimator->Set_AnimSpeed(m_eAnimType, m_iAnimIndex, 2.5f);
        }
    }

    return __super::Tick(pOwner, pAnimator);
}

void CWalk_Player::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* �Ұž��� */
}

STATE_TYPE CWalk_Player::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player�� Walk�� ���� ����
    1. 
    */

    // m_eStateType �� End �� ���� ������ Enter �� ȣ���Ѵ�.

    // W �� A �� ������ ���� ������ �̵��Ѵ�.
    // W �� D �� ������ ���� ������ �̵��Ѵ�.

    // ���� WASD �� �����ٸ�

        // õõ�� 
        if (KEY(CTRL, HOLD))
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
        //CTRL �� �ٲټ�.
      
      

   


    return STATE_END;
}
