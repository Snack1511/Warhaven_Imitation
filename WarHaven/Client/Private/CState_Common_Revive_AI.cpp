#include "stdafx.h"
#include "CState_Common_Revive_AI.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"

#include "CColorController.h"

#include "UsefulHeaders.h"

CState_Common_Revive_AI::CState_Common_Revive_AI()
{
}

CState_Common_Revive_AI::~CState_Common_Revive_AI()
{
}

CState_Common_Revive_AI* CState_Common_Revive_AI::Create()
{
    CState_Common_Revive_AI* pInstance = new CState_Common_Revive_AI();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Common_Revive_AI");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Common_Revive_AI::Initialize()
{
    m_eAnimType = ANIM_ETC;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 28;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_COMMON_REVIVE_AI;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.5f;

    m_iStateChangeKeyFrame = 99;

    m_fAIMyLength = 1.5f;

    return S_OK;
}

void CState_Common_Revive_AI::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_eAnimType = ANIM_BASE_R;

    switch (pOwner->Get_OwnerPlayer()->Get_CurClass())
    {
    case WARRIOR:
        m_iAnimIndex = 27;
        break;
    case ENGINEER:
        m_iAnimIndex = 30;
        break;
    case ARCHER:
        m_iAnimIndex = 27;
        break;
    case PALADIN:
        m_iAnimIndex = 19;
        break;
    case PRIEST:
        m_iAnimIndex = 19;
        break;
    default:
        break;
    }

    pOwner->Get_PhysicsCom()->Set_MaxSpeed(pOwner->Get_Status().fRunSpeed);


    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Common_Revive_AI::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    m_fTimeAcc += fDT(0);

    STATE_TYPE eDefaultState = pOwner->Get_DefaultState();

    CUnit* pTargetUnit = pOwner->Get_TargetUnit();

  

    switch (m_eCurPhase)
    {
    case Client::CState_Common_Revive_AI::eFOLLOW:
    {
        //목표까지 가야함
        if (pOwner->Get_RevivalPlayer())
        {
            m_eCurPhase = BEGIN;
            m_eAnimType = ANIM_ETC;

            if (pOwner->Get_OwnerPlayer()->Get_CurClass() >= FIONA)
                m_iAnimIndex = 22;
            else
                m_iAnimIndex = 30;


            _float4 vMyPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
            _float4 vLook = pOwner->Get_RevivalPlayer()->Get_Transform()->Get_World(WORLD_POS) - vMyPos;

            vLook.y = 0.f;

            pOwner->Get_Transform()->Set_LerpLook(vLook.Normalize(), 0.5f);
            pOwner->Get_RevivalPlayer()->Get_CurrentUnit()->Start_Reborn();

            CState::Enter(pOwner, pAnimator, (STATE_TYPE)m_eCurPhase);
        }
        else
        {
            //방향으로 가기 
            if (!pTargetUnit)
                return __super::Tick(pOwner, pAnimator);

            pOwner->Set_LookToTarget();
            _float4 vLook = pOwner->Get_Transform()->Get_World(WORLD_LOOK);
            pOwner->Get_PhysicsCom()->Set_Dir(vLook);
            pOwner->Get_PhysicsCom()->Set_Accel(50.f);


        }


    }

    break;

    case Client::CState_Common_Revive_AI::PHASE_NONE:

        
        return eDefaultState;
        break;


    case Client::CState_Common_Revive_AI::DANCE:

        if (pAnimator->Is_CurAnimFinished())
        {
            return eDefaultState;
        }


        break;

    case Client::CState_Common_Revive_AI::BEGIN:

        if (pAnimator->Is_CurAnimFinished())
        {
            m_eCurPhase = LOOP;

            if (pOwner->Get_OwnerPlayer()->Get_CurClass() >= FIONA)
                m_iAnimIndex = 22;
            else
                m_iAnimIndex = 30;
            
            CState::Enter(pOwner, pAnimator, (STATE_TYPE)m_eCurPhase);
        }

        break;
    case Client::CState_Common_Revive_AI::LOOP:
        if (m_fTimeAcc >= 4.f)
        {
            m_eCurPhase = PHASE_END;

            if (pOwner->Get_OwnerPlayer()->Get_CurClass() >= FIONA)
                m_iAnimIndex = 21;
            else
                m_iAnimIndex = 29;
            
            // 만약 부활이 되지 않는다면 이 코드의 주석을 해제하세요..
            //if(m_pAbjPlayer)
            //    static_cast<CPlayer*>(m_pAbjPlayer)->Respawn_Unit(m_vPos, static_cast<CPlayer*>(m_pAbjPlayer)->Get_CurClass());

            CState::Enter(pOwner, pAnimator, m_ePreStateType);
        }
        else
        {
            if (!m_pAbjPlayer)
            {
                m_eCurPhase = PHASE_END;
                m_iAnimIndex = 29;
                CState::Enter(pOwner, pAnimator, m_ePreStateType);
            }
        }

        break;
    case Client::CState_Common_Revive_AI::PHASE_END:
        if (pAnimator->Is_CurAnimFinished())
        {
            CUser::Get_Instance()->Disable_RevivalUI();

            STATE_TYPE eDefaultState = pOwner->Get_DefaultState();
            return eDefaultState;
        }
        break;
    default:
        break;
    }


    return __super::Tick(pOwner, pAnimator);
}

void CState_Common_Revive_AI::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    __super::Exit(pOwner, pAnimator);
}

STATE_TYPE CState_Common_Revive_AI::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}
