#include "stdafx.h"
#include "CState_Hit.h"

#include "UsefulHeaders.h"

#include "HIerarchyNode.h"
#include "CUnit_Priest.h"
#include "CUnit_Paladin.h"
#include "CUnit_Qanda.h"
#include "CUnit_Lancer.h"
#include "CUnit_Archer.h"
#include "Loading_Manager.h"
#include "CCamera_Follow.h"
#include "CAnimWeapon_Crow.h"

#include "CLancerNeedle.h"
#include "CSnipeArrow.h"

CState_Hit::CState_Hit()
{
}

CState_Hit::~CState_Hit()
{
}

HRESULT CState_Hit::Initialize()
{
    m_fInterPolationTime = 0.1f;

    m_iFallHitIndex = 0;
    m_iFlyHitIndex = 1;

    m_iGroggyIndex[GROGGY_STATE_E] = 3;
    m_iGroggyIndex[GROGGY_STATE_N] = 4;
    m_iGroggyIndex[GROGGY_STATE_S] = 5;
    m_iGroggyIndex[GROGGY_STATE_W] = 6;
    m_iGroggyIndex[GROGGY_STATE_PLACE] = 7;

    m_iGuardIndex[GUARD_STATE_F] = 8;
    m_iGuardIndex[GUARD_STATE_L] = 9;
    m_iGuardIndex[GUARD_STATE_R] = 10;
    m_iGuardIndex[GUARD_STATE_TOP] = 11;

    m_iHitIndex[HIT_STATE_E] = 12;
    m_iHitIndex[HIT_STATE_N] = 13;
    m_iHitIndex[HIT_STATE_S] = 14;
    m_iHitIndex[HIT_STATE_W] = 15;

    m_iHitStingIndex[HIT_STATE_E] = 16;
    m_iHitStingIndex[HIT_STATE_N] = 17;
    m_iHitStingIndex[HIT_STATE_S] = 18;
    m_iHitStingIndex[HIT_STATE_W] = 19;


    return S_OK;
}

void CState_Hit::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevStateType, void* pData)
{
    STATE_TYPE eCurStateType = pOwner->Get_CurState();

    if (pOwner->Get_HitType().eStingHitState == eCurStateType)
    {
        if (ePrevStateType == eCurStateType)
        {
            m_bStingDead = true;
        }

    }

    if (!m_tHitInfo.vDir.Is_Zero())
        pOwner->Get_PhysicsCom()->Set_Dir(m_tHitInfo.vDir);

    if (!m_tHitInfo.vLook.Is_Zero())
        pOwner->Get_Transform()->Set_LerpLook(m_tHitInfo.vLook, 0.3f);

    if (m_tHitInfo.fJumpPower > 0.f)
        pOwner->Get_PhysicsCom()->Set_Jump(m_tHitInfo.fJumpPower);

    if (m_tHitInfo.fKnockBackPower > 0.f)
        pOwner->Get_PhysicsCom()->Set_Speed(m_tHitInfo.fKnockBackPower);


    CLASS_TYPE eClass = pOwner->Get_OwnerPlayer()->Get_CurClass();

    CSnipeArrow* pArrow = nullptr;
    
    switch (eClass)
    {
    case Client::WARRIOR:
        break;
    case Client::ARCHER:
        pArrow = dynamic_cast<CSnipeArrow*>(static_cast<CUnit_Archer*>(pOwner)->Get_CurArrow());

        if (pArrow)
            pArrow->Turn_Effect(false);

        break;
    case Client::PALADIN:
        static_cast<CUnit_Paladin*>(pOwner)->Turn_RushEffect(false);
        break;
    case Client::PRIEST:
        static_cast<CUnit_Priest*>(pOwner)->TurnOff_AllEffect();
        break;
    case Client::FIONA:
        break;
    case Client::QANDA:
        static_cast<CUnit_Qanda*>(pOwner)->Turn_ChargeEffect(false);
        static_cast<CUnit_Qanda*>(pOwner)->Turn_SteamEffect(false);
        static_cast<CUnit_Qanda*>(pOwner)->Get_Crow()->On_ChangePhase(CAnimWeapon_Crow::eIDLE);
        break;
    case Client::LANCER:

        for (_int i = 0; i < CUnit_Lancer::eNeedle::eNeedle_Max; ++i)
        {
            CLancerNeedle* pNeedle = static_cast<CUnit_Lancer*>(pOwner)->Get_Needle(i);

            if (!pNeedle)
                continue;

            pNeedle->On_ChangePhase(CLancerNeedle::LANCERNEEDLE_STOP);
        }
        break;
    default:
        break;
    }


    if (pOwner->Is_MainPlayer())
    {
        pOwner->Lerp_Camera(0);
        GAMEINSTANCE->Stop_RadialBlur();
        GAMEINSTANCE->Stop_ChromaticAberration();
    }

    __super::Enter(pOwner, pAnimator, ePrevStateType);
}

STATE_TYPE CState_Hit::Tick(CUnit* pOwner, CAnimator* pAnimator)
{

    if (m_tHitInfo.bSting)
    {
        if (!m_bStingDead)
        {

            CTransform* pMyTransform = pOwner->Get_Transform();
            _float4 vMyPos = pMyTransform->Get_World(WORLD_POS);

            // ?????? ????????	
            if (CLoading_Manager::Get_Instance()->Get_LoadLevel() >= LEVEL_PADEN)
            {
                // ???? ???? ???? ???? ???????? ????????, ???? ???? ???? ?????? ???? ??????.
                m_tHitInfo.pOtherUnit->Get_OwnerPlayer()->On_ScoreKDA_Kill(pOwner->Get_OwnerPlayer());
                pOwner->Get_OwnerPlayer()->On_ScoreKDA_Death();
            }


            CUser::Get_Instance()->Add_KillLog(m_tHitInfo.pOtherUnit->Get_OwnerPlayer(), pOwner->Get_OwnerPlayer());

            if (pOwner->Get_OwnerPlayer()->IsMainPlayer())
            {
                CUser::Get_Instance()->Turn_HeroGaugeFire(false);
                CUser::Get_Instance()->SetActive_SquardInfo(false);
                CUser::Get_Instance()->SetActive_HUD(false);
                CUser::Get_Instance()->Set_TargetInfo(m_tHitInfo.pOtherUnit->Get_OwnerPlayer()->Get_PlayerInfo());
                CUser::Get_Instance()->Toggle_DeadUI(true);

                // Other(????) ?????? ?????? ???? ?????? ?????? ?? ?????? ????
                pOwner->Get_FollowCam()->Set_FollowTarget(m_tHitInfo.pOtherUnit);
            }
            else
            {
                // ???????? ???? ???????? ??????? ???? ????
                if (m_tHitInfo.pOtherUnit->Get_OwnerPlayer()->IsMainPlayer())
                {
                    wstring wstrEnemyName = pOwner->Get_OwnerPlayer()->Get_PlayerName();
                    CUser::Get_Instance()->Add_KillName(wstrEnemyName);
                }
            }
        }

        m_tHitInfo.bSting = false;
    }

    if (m_pStingBone)
    {
        m_fTimeAcc += fDT(0);

        if (m_fTimeAcc > 1.5f)
        {
            CFunctor::Play_Sound(L"Effect_Die", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
            pOwner->On_Die();

        }
    }

    return __super::Tick(pOwner, pAnimator);
}



void CState_Hit::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

void CState_Hit::Face_Check(CUnit* pOwner)
{
    if (m_tHitInfo.bFace)
    {
        if (m_tHitInfo.eHitType == HIT_TYPE::eLEFT)
            m_tHitInfo.eHitType = HIT_TYPE::eRIGHT;

        else  if (m_tHitInfo.eHitType == HIT_TYPE::eRIGHT)
            m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
    } 
}

void CState_Hit::Fly_State()
{
    if (m_tHitInfo.bFly)
    {
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iFlyHitIndex;
    }
    else
    {
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_N];
    }
        
}

void CState_Hit::Hit_State(CUnit* pOwner)
{
    //if (m_tHitInfo.bSting)
    //{
    //    if (pOwner->Get_Status().fHP <= 0.f)
    //    { // strStingBoneName
    //        m_pStingBone = GET_COMPONENT_FROM(m_tHitInfo.pOtherUnit, CModel)->Find_HierarchyNode("0B_R_WP1");
    //        m_eAnimType = ANIM_HIT;
    //        m_iAnimIndex = m_iHitStingIndex[HIT_STATE_N];
    //        return;
    //    }
    //}
    


    Face_Check(pOwner);

    /* Hit ???? */
    switch (m_tHitInfo.eHitType)
    {
        /* ???? ?????????? ???????? ?????????? ???? */
    case HIT_TYPE::eLEFT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_W];

        break;

    case HIT_TYPE::eRIGHT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_E];

        break;

    case HIT_TYPE::eUP:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_N];

        break;

    case HIT_TYPE::eDOWN:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitIndex[HIT_STATE_S];

        break;

    default:
        break;
    }
}


void CState_Hit::Guard_State(CUnit* pOwner)
{
    Face_Check(pOwner);

    m_tHitInfo.fJumpPower = 0.f;
    m_tHitInfo.fKnockBackPower = 0.f;

    /* ???? Hit ???? */
    switch (m_tHitInfo.eHitType)
    {
    case HIT_TYPE::eLEFT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iGuardIndex[GUARD_STATE_R];

        break;

    case HIT_TYPE::eRIGHT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iGuardIndex[GUARD_STATE_L];

        break;

    case HIT_TYPE::eUP:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iGuardIndex[GUARD_STATE_TOP];

        break;

    case HIT_TYPE::eDOWN:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iGuardIndex[GUARD_STATE_F];

        break;

    default:
        break;
    }
}

void CState_Hit::Groggy_State(CUnit* pOwner)
{
    Face_Check(pOwner);

    /* ??????(????) ???? */
    switch (m_tHitInfo.eHitType)
    {
        /* ???? ?????????? ???????? ?????????? ???? */
    case HIT_TYPE::eLEFT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iGroggyIndex[HIT_STATE_W];

        break;

    case HIT_TYPE::eRIGHT:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iGroggyIndex[HIT_STATE_E];

        break;

    case HIT_TYPE::eUP:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iGroggyIndex[HIT_STATE_N];

        break;

    case HIT_TYPE::eDOWN:
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iGroggyIndex[HIT_STATE_S];

        break;

    default:
        break;
    }
}


void CState_Hit::Sting_State(CUnit* pOwner)
{
 
    Face_Check(pOwner);

    if (pOwner->Get_Status().fHP <= 0.f)
    {
        m_pStingBone = GET_COMPONENT_FROM(m_tHitInfo.pOtherUnit, CModel)->Find_HierarchyNode(m_tHitInfo.strStingBoneName.c_str());
        m_eAnimType = ANIM_HIT;
        m_iAnimIndex = m_iHitStingIndex[HIT_STATE_N];

        switch (m_tHitInfo.eHitType)
        {
            /* ???? ?????????? ???????? ?????????? ???? */
        case HIT_TYPE::eLEFT:
            m_eAnimType = ANIM_HIT;
            m_iAnimIndex = m_iHitStingIndex[HIT_STATE_W];

            break;

        case HIT_TYPE::eRIGHT:
            m_eAnimType = ANIM_HIT;
            m_iAnimIndex = m_iHitStingIndex[HIT_STATE_E];

            break;

        case HIT_TYPE::eUP:
            m_eAnimType = ANIM_HIT;
            m_iAnimIndex = m_iHitStingIndex[HIT_STATE_N];

            break;

        case HIT_TYPE::eDOWN:
            m_eAnimType = ANIM_HIT;
            m_iAnimIndex = m_iHitStingIndex[HIT_STATE_S];

            break;

        default:
            break;
        }

    }
    else
    {
        Groggy_State(pOwner);
    }


    ///* ??????(????) ???? */
    //switch (m_tHitInfo.eHitType)
    //{
    //    /* ???? ?????????? ???????? ?????????? ???? */
    //case HIT_TYPE::eLEFT:
    //    m_eAnimType = ANIM_HIT;
    //    m_iAnimIndex = m_iHitStingIndex[HIT_STATE_W];

    //    break;

    //case HIT_TYPE::eRIGHT:
    //    m_eAnimType = ANIM_HIT;
    //    m_iAnimIndex = m_iHitStingIndex[HIT_STATE_E];

    //    break;

    //case HIT_TYPE::eUP:
    //    m_eAnimType = ANIM_HIT;
    //    m_iAnimIndex = m_iHitStingIndex[HIT_STATE_N];

    //    break;

    //case HIT_TYPE::eDOWN:
    //    m_eAnimType = ANIM_HIT;
    //    m_iAnimIndex = m_iHitStingIndex[HIT_STATE_S];

    //    break;

    //default:
    //    break;
    //}

    /* Hit ???? */
    //switch (m_tHitInfo.eHitType)
    //{
    //    /* ???? ?????????? ???????? ?????????? ???? */
    //case HIT_TYPE::eLEFT:
    //    m_eAnimType = ANIM_HIT;
    //    m_iAnimIndex = m_iHitIndex[HIT_STATE_W];

    //    break;

    //case HIT_TYPE::eRIGHT:
    //    m_eAnimType = ANIM_HIT;
    //    m_iAnimIndex = m_iHitIndex[HIT_STATE_E];

    //    break;

    //case HIT_TYPE::eUP:
    //    m_eAnimType = ANIM_HIT;
    //    m_iAnimIndex = m_iHitIndex[HIT_STATE_N];

    //    break;

    //case HIT_TYPE::eDOWN:
    //    m_eAnimType = ANIM_HIT;
    //    m_iAnimIndex = m_iHitIndex[HIT_STATE_S];

    //    break;

    //default:
    //    break;
    //}   

}
