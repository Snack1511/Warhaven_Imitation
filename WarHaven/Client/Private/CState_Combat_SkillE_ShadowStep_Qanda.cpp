#include "stdafx.h"
#include "CState_Combat_SkillE_ShadowStep_Qanda.h"

#include "GameInstance.h"
#include "Physics.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "CUnit_Qanda.h"
#include "CPlayer.h"

#include "Model.h"
#include "Transform.h"

#include "CUser.h"
#include "Renderer.h"
#include "CColorController.h"

#include "CEffects_Factory.h"
#include "CRectEffects.h"
#include "CAnimWeapon.h"

CState_Combat_SkillE_ShadowStep_Qanda::CState_Combat_SkillE_ShadowStep_Qanda()
{
}

CState_Combat_SkillE_ShadowStep_Qanda::~CState_Combat_SkillE_ShadowStep_Qanda()
{
}

CState_Combat_SkillE_ShadowStep_Qanda* CState_Combat_SkillE_ShadowStep_Qanda::Create()
{
    CState_Combat_SkillE_ShadowStep_Qanda* pInstance = new CState_Combat_SkillE_ShadowStep_Qanda();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_SkillE_ShadowStep_Qanda");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_SkillE_ShadowStep_Qanda::Initialize()
{
    m_eAnimType = ANIM_BASE_R;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 9;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMBAT_SHADOWSTEP_QANDA;   // 나의 행동 타입(Init 이면 내가 시작할 타입)


    m_iStateChangeKeyFrame = 99;

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.5f;
    m_fMyMaxLerp = 0.4f;
    m_fMyAccel = 30.f;


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
   m_fDirectionAnimSpeed[STATE_DIRECTION_N] = 2.5f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_S] = 2.f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_W] = 1.8f;
   m_fDirectionAnimSpeed[STATE_DIRECTION_E] = 1.8f;

    return S_OK;
}

void CState_Combat_SkillE_ShadowStep_Qanda::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
   
    DISABLE_COMPONENT(GET_COMPONENT_FROM(pOwner->Get_AnimWeapon(), CRenderer));
    DISABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));
    
    pOwner->Enable_HitBoxColliders(false);

    m_EffectsList = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Dodge", pOwner, ZERO_VECTOR);
    static_cast<CUnit_Qanda*>(pOwner)->Turn_TransformParticle(false);
    static_cast<CUnit_Qanda*>(pOwner)->TurnOn_Trail(false);

    m_fMaxSpeed = pOwner->Get_Status().fSprintSpeed * 1.2f;
    m_iDirectionRand = random(0, 7);

    Set_Direction_Back_AI(m_iDirectionRand);

    Physics_Setting_AI(m_fMaxSpeed, pOwner);

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_SkillE_ShadowStep_Qanda::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_iStack == 5)
        return AI_STATE_COMBAT_DEAFULT_QANDA;

    _float4 vLook = ZERO_VECTOR;

    if (m_pCurrentTargetUnit)
    {
        vLook =  pOwner->Get_Transform()->Get_World(WORLD_POS) - m_pCurrentTargetUnit->Get_Transform()->Get_World(WORLD_POS);
    }
    else if(pOwner->Get_TargetUnit())
    {
        
        vLook = pOwner->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_TargetUnit()->Get_Transform()->Get_World(WORLD_POS);
    }

    vLook.Normalize();

    CTransform* pMyTransform = pOwner->Get_Transform();
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();

    pMyTransform->Set_LerpLook(vLook, m_fMyMaxLerp);

    pMyPhysicsCom->Set_MaxSpeed(m_fMaxSpeed);
    pMyPhysicsCom->Set_Dir(vLook);
    pMyPhysicsCom->Set_Accel(m_fMyAccel);

    m_fTimeAcc += fDT(0);


    if (m_fTimeAcc > 0.5f)
    {
        if (m_iDirectionRand % 2 == 0 && !m_bJump)
        {
            pOwner->Get_PhysicsCom()->Set_Jump(pOwner->Get_Status().fJumpPower);
            m_bJump = true;
        }
        
        //m_iDirectionRand = random(0, 8);
        //Set_Direction_Back_AI(m_iDirectionRand);
        m_fTimeAcc = 0.f;
        ++m_iStack;
    }

  /*  m_fCurGuageMinusTime += fDT(0);

    if (m_fCurGuageMinusTime > m_fMaxGuageMinusTime)
    {
        --pOwner->Get_OwnerPlayer()->Get_Gauge();
        m_fCurGuageMinusTime = 0.f;
    }*/

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_SkillE_ShadowStep_Qanda::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
   pOwner->Enable_HitBoxColliders(true);
   ENABLE_COMPONENT(GET_COMPONENT_FROM(pOwner, CRenderer));
   ENABLE_COMPONENT(GET_COMPONENT_FROM(pOwner->Get_AnimWeapon(), CRenderer));
  
   TurnOff_DodgeEffect(pOwner);
   static_cast<CUnit_Qanda*>(pOwner)->Turn_TransformParticle(true);
   static_cast<CUnit_Qanda*>(pOwner)->TurnOn_Trail(true);
}

STATE_TYPE CState_Combat_SkillE_ShadowStep_Qanda::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_SkillE_ShadowStep_Qanda::TurnOff_DodgeEffect(CUnit* pOwner)
{
    for (auto& elem : m_EffectsList)
    {
        static_cast<CRectEffects*>(elem)->Set_AllFadeOut(0.3f);
    }
    CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"Dodge_0"), pOwner, ZERO_VECTOR);
}
