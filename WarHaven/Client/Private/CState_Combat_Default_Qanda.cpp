#include "stdafx.h"
#include "CState_Combat_Default_Qanda.h"

#include "GameInstance.h"

#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Qanda.h"

#include "CPlayer.h"
#include "CAnimWeapon_Crow.h"

#include "CUser.h"

CState_Combat_Default_Qanda::CState_Combat_Default_Qanda()
{
}

CState_Combat_Default_Qanda::~CState_Combat_Default_Qanda()
{
}

CState_Combat_Default_Qanda* CState_Combat_Default_Qanda::Create()
{
    CState_Combat_Default_Qanda* pInstance = new CState_Combat_Default_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Default_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Default_Qanda::Initialize()
{

    m_eAnimType = ANIM_BASE_R;          // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_DEAFULT_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)
  
    m_iStateChangeKeyFrame = 0;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;
    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 10.f;

    m_iDirectionAnimIndex[STATE_DIRECTION_E] = 8;
    m_iDirectionAnimIndex[STATE_DIRECTION_N] = 9;
    m_iDirectionAnimIndex[STATE_DIRECTION_NE] = 10;
    m_iDirectionAnimIndex[STATE_DIRECTION_NW] = 11;
    m_iDirectionAnimIndex[STATE_DIRECTION_S] = 22;
    m_iDirectionAnimIndex[STATE_DIRECTION_SE] = 23;
    m_iDirectionAnimIndex[STATE_DIRECTION_SW] = 24;
    m_iDirectionAnimIndex[STATE_DIRECTION_W] = 12;

    m_fDirectionAnimSpeed[STATE_DIRECTION_NW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_NE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SW] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_SE] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
    m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    return __super::Initialize();
}

void CState_Combat_Default_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    if (ePrevType == m_eStateType)
        m_fInterPolationTime = 0.f;

    m_iRand = random(0, 7);
    m_iDirectionRand = random(0, 7);

    m_fAIMyLength = 5.f;

    if(Get_TargetLook_Length(pOwner) > m_fAIMyLength)
        Set_Direction_Front_AI(m_iDirectionRand);
        
    else 
        Set_Direction_Back_AI(m_iDirectionRand);


    
    m_fMaxSpeed = pOwner->Get_Status().fRunSpeed * 0.8f;
    m_iAnimIndex = m_iDirectionAnimIndex[m_iDirectionRand];

    m_iStateChangeKeyFrame = 25;

    

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Default_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    STATE_TYPE eAttackState = Shoot_State(pOwner);

    if (eAttackState != STATE_END)
        return eAttackState;

    _uint iFrame = pAnimator->Get_CurAnimFrame() + 1;


    switch (m_iRand)
    {
    case 0:
    case 1:

        if (iFrame == m_iStateChangeKeyFrame)
            return m_eStateType;

        break;

    
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:

    {
        CAnimWeapon_Crow::ePhyxState eCrowState = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->Get_Phase();

        if (eCrowState == CAnimWeapon_Crow::ePhyxState::eSHOOT  ||
            eCrowState == CAnimWeapon_Crow::ePhyxState::eHIT    ||
            eCrowState == CAnimWeapon_Crow::ePhyxState::eSPAWN)
            return m_eStateType;

        else
            return AI_STATE_COMBAT_BEGIN_QANDA;
    }
        break;

    default:
        break;
    }


    DoMove_AI(pOwner, pAnimator);

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Default_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Default_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return __super::Check_Condition(pOwner, pAnimator);
}

STATE_TYPE CState_Combat_Default_Qanda::Shoot_State(CUnit* pOwner)
{
    list<CGameObject*> listFrustumObject = static_cast<CUnit_Qanda*>(pOwner)->Get_MultipleFrustumObject();

    if ((_int)listFrustumObject.size() >= 2)
    {
        if (pOwner->Can_Use(CUnit::SKILL3))
            return AI_STATE_COMBAT_BEGIN_SNIPING_QANDA;
    }

    CAnimWeapon_Crow::ePhyxState eCrowState = static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->Get_Phase();

    if (eCrowState == CAnimWeapon_Crow::ePhyxState::eSHOOT  ||
        eCrowState == CAnimWeapon_Crow::ePhyxState::eHIT    ||
        eCrowState == CAnimWeapon_Crow::ePhyxState::eSPAWN)
        return STATE_END;


    if (Get_TargetLook_Length(pOwner) > m_fAIMyLength)
        return AI_STATE_COMBAT_BEGIN_QANDA;

    return STATE_END;

}
