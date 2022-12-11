#include "stdafx.h"
#include "CState_PathNavigation_Test.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

#include "CPath.h"

#include "UsefulHeaders.h"

CState_PathNavigation_Test::CState_PathNavigation_Test()
{
}

CState_PathNavigation_Test::~CState_PathNavigation_Test()
{
}

CState_PathNavigation_Test* CState_PathNavigation_Test::Create()
{
    CState_PathNavigation_Test* pInstance = new CState_PathNavigation_Test();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_PathNavigation_Test");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_PathNavigation_Test::Initialize()
{

    //__super::Initialize();

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 39;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_PATHNAVIGATION_DEFAULT_WARRIOR_R;   // 나의 행동 타입(Init 이면 내가 시작할 타입)
    m_iStateChangeKeyFrame = 0;
    m_fAnimSpeed = 2.5f;


    // 알파벳 순 애니메이션 정렬
    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 38;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 39;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 40;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 41;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 42;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 43;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 44;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 45;


    return S_OK;
}

void CState_PathNavigation_Test::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fWalkSpeed);
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_PathNavigation_Test::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    CPath* pCurPath = pOwner->Get_CurPath();

    if (!pCurPath)
    {
        assert(0);
        return STATE_END;
    }

    /* 따라가면 대 */
    _float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

    _float4 vDir = pCurPath->Get_CurDir(pOwner->Get_Transform()->Get_World(WORLD_POS));

    pOwner->Get_Transform()->Set_LerpLook(vDir, 0.4f);
    pOwner->Get_PhysicsCom()->Set_Dir(vDir);
    pOwner->Get_PhysicsCom()->Set_Accel(100.f);

    
    pCurPath->Update_CurrentIndex(vCurPos);

    return __super::Tick(pOwner, pAnimator);
}

void CState_PathNavigation_Test::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
   // __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_PathNavigation_Test::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
