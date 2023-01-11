#include "stdafx.h"
#include "CState_PathNavigation_Sprint_Archer_Loop.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_PathNavigation_Sprint_Archer_Loop::CState_PathNavigation_Sprint_Archer_Loop()
{
}

CState_PathNavigation_Sprint_Archer_Loop::~CState_PathNavigation_Sprint_Archer_Loop()
{
}

CState_PathNavigation_Sprint_Archer_Loop* CState_PathNavigation_Sprint_Archer_Loop::Create()
{
    CState_PathNavigation_Sprint_Archer_Loop* pInstance = new CState_PathNavigation_Sprint_Archer_Loop();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Sprint_Archer_Loop");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Sprint_Archer_Loop::Initialize()
{

    __super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // �ִϸ��̼��� �޽�Ÿ��
    m_iAnimIndex = 46;                   // ���� ���� ����ϰ� �ִ� �ִϸ��̼� ����(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_SPRINTLOOP_ARCHER;   // ���� �ൿ Ÿ��(Init �̸� ���� ������ Ÿ��)

    m_iStateChangeKeyFrame = 99;

    m_fAnimSpeed = 2.5f;

    m_fInterPolationTime = 0.f;

    m_eWalkState = AI_STATE_PATHNAVIGATION_WALK_ARCHER_R;

    return S_OK;
}

void CState_PathNavigation_Sprint_Archer_Loop::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_fRand = frandom(2.1f, 8.f);
    m_iRand = random(0, 5);

    

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Sprint_Archer_Loop::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_fAIDelayTime > m_fRand)
    {
        switch (m_iRand)
        {
        case 0:
        case 1:
        case 2:
           
            m_iRand = random(0, 5);

            break;

        case 3:

            return AI_STATE_PATHNAVIGATION_SPRINTJUMP_ARCHER;


        case 4:

            return AI_STATE_PATHNAVIGATION_SPRINTEND_ARCHER;

        default:

            m_iRand = random(0, 5);

            break;
        }

            
    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Sprint_Archer_Loop::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Sprint_Archer_Loop::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}
