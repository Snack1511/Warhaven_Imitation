#include "stdafx.h"
#include "CState_Combat_Oxen_Loop_Attack_Warrior.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CState_Combat_Oxen_Loop_Attack_Warrior::CState_Combat_Oxen_Loop_Attack_Warrior()
{
}

CState_Combat_Oxen_Loop_Attack_Warrior::~CState_Combat_Oxen_Loop_Attack_Warrior()
{
}

CState_Combat_Oxen_Loop_Attack_Warrior* CState_Combat_Oxen_Loop_Attack_Warrior::Create()
{
    CState_Combat_Oxen_Loop_Attack_Warrior* pInstance = new CState_Combat_Oxen_Loop_Attack_Warrior();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CState_Combat_Oxen_Loop_Attack_Warrior");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CState_Combat_Oxen_Loop_Attack_Warrior::Initialize()
{

    m_tHitInfo.eHitType = HIT_TYPE::eLEFT;
    m_tHitInfo.fKnockBackPower = 1.f;
    m_tHitInfo.fJumpPower = 6.5f;
    m_tHitInfo.bFly = true;
    m_tHitInfo.iLandKeyFrame = 110;

    m_eAnimType = ANIM_ATTACK;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 23;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = STATE_WARRIOR_OXEN_LOOPATTACK;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    // 선형 보간 시간
    m_fInterPolationTime = 0.f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 2.f;

    // Idle -> 상태(Jump, RUn 등등) -> L, R 비교 -> 상태에서 할 수 있는 거 비교(Attack -> Move) -> 반복

    //enum 에 Idle 에서 마인드맵해서 갈 수 있는 State 를 지정해준다.
    m_iStateChangeKeyFrame = 87;

    Add_KeyFrame(5, 0);
    Add_KeyFrame(47, 1);
    Add_KeyFrame(60, 2);
    Add_KeyFrame(m_iStateChangeKeyFrame, 3);

    return S_OK;
}

void CState_Combat_Oxen_Loop_Attack_Warrior::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData)
{
    m_iRand = random(0, 3);
    m_iDirectionRand = STATE_DIRECTION_N;

    pOwner->On_Use(CUnit::SKILL2);

    /*effect*/
    pOwner->TurnOn_TrailEffect(true);
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SkillLightParticle", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CFunctor::Play_Sound(L"Effect_Oxen_Loop_Attack_Begin", CHANNEL_EFFECTS, pOwner->Get_Transform()->Get_World(WORLD_POS));
    CColorController::COLORDESC tColorDesc;
    ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

    tColorDesc.eFadeStyle = CColorController::KEYFRAME;
    tColorDesc.fFadeInStartTime = 0.f;
    tColorDesc.fFadeInTime = 0.1f;
    tColorDesc.fFadeOutStartTime = 1.f;
    tColorDesc.fFadeOutTime = 0.1f;
    tColorDesc.vTargetColor = RGBA(50, 30, 0, 0.1f);
    tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    tColorDesc.iStartKeyFrame = 2;
    tColorDesc.iEndKeyFrame = 45; // 프레임 맞춰놓음

    GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);


    /* ColorControl*/
    tColorDesc;

    tColorDesc.eFadeStyle = CColorController::KEYFRAME;
    tColorDesc.fFadeInStartTime = 0.f;
    tColorDesc.fFadeInTime = 0.2f;
    tColorDesc.fFadeOutStartTime = 0.f;
    tColorDesc.fFadeOutTime = 0.2f;
    tColorDesc.vTargetColor = _float4(1.f, 1.f, 1.f, 0.5f);

    tColorDesc.iMeshPartType = MODEL_PART_BODY;
    tColorDesc.iStartKeyFrame = 2;
    tColorDesc.iEndKeyFrame = 45; // 프레임 맞춰놓음

    GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);

    tColorDesc.iMeshPartType = MODEL_PART_HEAD;
    GET_COMPONENT_FROM(pOwner, CColorController)->Add_ColorControll(tColorDesc);



    if (ePrevType == AI_STATE_COMBAT_OXEN_LOOP_WARRIOR)
    {
        m_fInterPolationTime = 0.f;
    }
    else
    {
        m_fInterPolationTime = 0.1f;
    }

    pOwner->Set_BounceState(AI_STATE_COMMON_BOUNCE_WARRIOR_R);


    pOwner->CallBack_CollisionEnter += bind(&CState_Combat_Oxen_Loop_Attack_Warrior::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);


    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat_Oxen_Loop_Attack_Warrior::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (pAnimator->Is_CurAnimFinished())
        return AI_STATE_COMBAT_DEFAULT_WARRIOR_R;

    DoMove_AI(pOwner, pAnimator);


    //if (pAnimator->Get_CurAnimFrame() >= m_tHitInfo.iLandKeyFrame && !pOwner->Is_Air())
    //    return STATE_JUMP_LAND_PLAYER_L;
    
    if (m_bHit && m_bKeyInputable)
    {
        switch (m_iRand)
        {
        case 0:
        case 2:
            return AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_L;

        case 1:
        case 3:
            return AI_STATE_COMBAT_HORIZONTALMIDDLE_WARRIOR_R;

        default:
            break;
        }

    }

    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat_Oxen_Loop_Attack_Warrior::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter -= bind(&CState_Combat_Oxen_Loop_Attack_Warrior::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

    pOwner->TurnOn_TrailEffect(false);
	pOwner->Enable_FlyAttackCollider(false);
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;
}

STATE_TYPE CState_Combat_Oxen_Loop_Attack_Warrior::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

void CState_Combat_Oxen_Loop_Attack_Warrior::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{
    // m_iRand
   if (iOtherColType == COL_REDHITBOX_BODY || iOtherColType == COL_REDHITBOX_HEAD ||
       iOtherColType == COL_BLUEHITBOX_BODY || iOtherColType == COL_BLUEHITBOX_HEAD)
    {
        m_bHit = true;
    }






}

void CState_Combat_Oxen_Loop_Attack_Warrior::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
{
    switch (iSequence)
    {
    case 0:
    {
		Physics_Setting(pOwner->Get_Status().fDashAttackSpeed, pOwner, true);

        CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();


        //마찰 조절하기
        //*주의 : 사용하고나면 Exit에서 반드시 1로 되돌려주기
        pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 0.55f;

    }

	break;
       
	case 1:
        Play_Sound(L"Effect_Oxen_Loop_Attack_Warrior", CHANNEL_EFFECTS, m_fAIDeafultVolume);
        m_bAttackTrigger = true;
		pOwner->Enable_FlyAttackCollider(true);
        Play_Voice(pOwner, L"Voice_Oxen", m_fAIDeafultVolume);
		break;


	case 2:
        m_bAttackTrigger = false;
		pOwner->Enable_FlyAttackCollider(false);
		break;

    case 3:
        m_bKeyInputable = true;

        break;
    default:
        break;
    }


}


