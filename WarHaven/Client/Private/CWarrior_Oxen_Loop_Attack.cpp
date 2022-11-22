#include "stdafx.h"
#include "CWarrior_Oxen_Loop_Attack.h"

#include "UsefulHeaders.h"
#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Transform.h"
#include "CColorController.h"

CWarrior_Oxen_Loop_Attack::CWarrior_Oxen_Loop_Attack()
{
}

CWarrior_Oxen_Loop_Attack::~CWarrior_Oxen_Loop_Attack()
{
}

CWarrior_Oxen_Loop_Attack* CWarrior_Oxen_Loop_Attack::Create()
{
    CWarrior_Oxen_Loop_Attack* pInstance = new CWarrior_Oxen_Loop_Attack();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CWarrior_Oxen_Loop_Attack");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CWarrior_Oxen_Loop_Attack::Initialize()
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



    m_vecAdjState.push_back(STATE_WALK_PLAYER_L);
    m_vecAdjState.push_back(STATE_RUN_PLAYER_L);
    m_vecAdjState.push_back(STATE_IDLE_PLAYER_L);

    m_vecAdjState.push_back(STATE_ATTACK_STING_PLAYER_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALUP_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALMIDDLE_L);
    m_vecAdjState.push_back(STATE_ATTACK_HORIZONTALDOWN_L);
    m_vecAdjState.push_back(STATE_SWITCH_L_TO_R);

    m_vecAdjState.push_back(STATE_SPRINT_BEGIN_PLAYER);

    m_vecAdjState.push_back(STATE_WARRIOR_GUARDBREAK);
    m_vecAdjState.push_back(STATE_WARRIOR_OXEN_BEGIN);

    m_vecAdjState.push_back(STATE_SLIDE_BEGIN_PLAYER);
    m_vecAdjState.push_back(STATE_GUARD_BEGIN_PLAYER);

    m_vecAdjState.push_back(STATE_ATTACK_VERTICALCUT);


    Add_KeyFrame(5, 0);
	Add_KeyFrame(40, 3);
	Add_KeyFrame(47, 1);
	Add_KeyFrame(60, 2);


    return S_OK;
}

void CWarrior_Oxen_Loop_Attack::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    pOwner->On_Use(CUnit::SKILL1);

    /*effect*/
    pOwner->TurnOn_TrailEffect(true);
    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"SkillLightParticle", pOwner, pOwner->Get_Transform()->Get_World(WORLD_POS));

    CColorController::COLORDESC tColorDesc;
    ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

    tColorDesc.eFadeStyle = CColorController::KEYFRAME;
    tColorDesc.fFadeInStartTime = 0.f;
    tColorDesc.fFadeInTime = 0.1f;
    tColorDesc.fFadeOutStartTime = 1.f;
    tColorDesc.fFadeOutTime = 0.1f;
    tColorDesc.vTargetColor = _float4((230.f / 255.f), (150.f / 255.f), (40.f / 255.f), 0.f);
    tColorDesc.iMeshPartType = MODEL_PART_WEAPON;
    tColorDesc.iStartKeyFrame = 2;
    tColorDesc.iEndKeyFrame = 45; // 프레임 맞춰놓음

    GET_COMPONENT_FROM(pOwner, CColorController)->Set_ColorControll(tColorDesc);

    if (ePrevType == STATE_WARRIOR_OXEN_LOOPATTACK)
    {
        m_fInterPolationTime = 0.f;
    }
    else
    {
        m_fInterPolationTime = 0.1f;
    }


    pOwner->CallBack_CollisionEnter += bind(&CWarrior_Oxen_Loop_Attack::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);


    /* Owner의 Animator Set Idle로 */
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CWarrior_Oxen_Loop_Attack::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    if (m_bParringed)
        return STATE_BOUNCE_PLAYER_R;

    if (pAnimator->Get_CurAnimFrame() >= m_tHitInfo.iLandKeyFrame && !pOwner->Is_Air())
        return STATE_JUMP_LAND_PLAYER_L;
    
    if (m_bKeyInputable)
    {
        if (KEY(SPACE, TAP))
            m_bKeyInput = true;
    }

    if (m_bHit && m_bKeyInput)
    {
            return STATE_JUMP_PLAYER_L;
    }


    Follow_MouseLook_Turn(pOwner);
    pOwner->Set_DirAsLook();

    /*dust*/
    if (m_bAttackTrigger)
    {
        // 공격 진입
        if (pOwner->Is_Weapon_R_Collision())
        {
            _float4 vHitPos = pOwner->Get_HitPos();
            _float4 vPos = pOwner->Get_Transform()->Get_World(WORLD_POS);

            //HitPos가 발 보다 아래, 즉 땅을 때린 경우에는 튕겨나는게 아니라 작은 파티클과 진동만.
            if (vHitPos.y <= vPos.y + 0.1f)
            {
                //CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", pOwner->Get_HitMatrix());
                CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), pOwner->Get_HitMatrix());
                CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), pOwner->Get_HitMatrix());
                CEffects_Factory::Get_Instance()->Create_MultiEffects(L"GroundHitParticle", vHitPos);
            }

            m_bAttackTrigger = false;
        }

    }

    return __super::Tick(pOwner, pAnimator);
}

void CWarrior_Oxen_Loop_Attack::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    pOwner->CallBack_CollisionEnter -= bind(&CWarrior_Oxen_Loop_Attack::OnCollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);

    pOwner->TurnOn_TrailEffect(false);
	pOwner->Enable_FlyAttackCollider(false);
    CPhysics* pMyPhysicsCom = pOwner->Get_PhysicsCom();
    pMyPhysicsCom->Get_PhysicsDetail().fFrictionRatio = 1.f;

    // 끄기
}

STATE_TYPE CWarrior_Oxen_Loop_Attack::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    /* Player가 Oxen 으로 오는 조건
    1.  Q 을 이용해 공격한다.
        */
    
        
    if (KEY(Q, NONE))
        return m_eStateType;

    return STATE_END;
}

void CWarrior_Oxen_Loop_Attack::OnCollisionEnter(CGameObject* pOtherObject, const _uint& iOtherColType, const _uint& iMyColType, _float4 vHitPos)
{


    if (iOtherColType == COL_ENEMYGUARD)
    {
        CEffects_Factory::Get_Instance()->Create_MultiEffects(L"BigSparkParticle", vHitPos);
        CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"SmallSparkParticle_0"), vHitPos);
        CEffects_Factory::Get_Instance()->Create_Effects(Convert_ToHash(L"HItSmokeParticle_0"), vHitPos);
        m_bParringed = true;
    }
    else if (iOtherColType == COL_ENEMYHITBOX_BODY || iOtherColType == COL_ENEMYHITBOX_HEAD)
    {

        //case STATE_ATTACK_HORIZONTALUP_L:
        //    break;
        //case STATE_ATTACK_HORIZONTALMIDDLE_L:
        //    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Left", pOwner, vHitPos);
        //    break;
        //case STATE_ATTACK_HORIZONTALDOWN_L:
        //    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_LD", pOwner, vHitPos);
        //    break;
        //case STATE_ATTACK_HORIZONTALUP_R:
        //    break;
        //case STATE_ATTACK_HORIZONTALMIDDLE_R:
        //    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_Right", pOwner, vHitPos);
        //    break;
        //case STATE_ATTACK_HORIZONTALDOWN_R:
            CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_RD", m_pOwner, vHitPos);
        //case STATE_ATTACK_VERTICALCUT:
        //    CEffects_Factory::Get_Instance()->Create_MultiEffects(L"HitSlash_D", pOwner, vHitPos);
        //    break;
        m_bHit = true;
    }






}

void CWarrior_Oxen_Loop_Attack::On_KeyFrameEvent(CUnit* pOwner, CAnimator* pAnimator, const KEYFRAME_EVENT& tKeyFrameEvent, _uint iSequence)
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
        m_bAttackTrigger = true;
		pOwner->Enable_FlyAttackCollider(true);
		break;


	case 2:
        m_bAttackTrigger = false;
		pOwner->Enable_FlyAttackCollider(false);
		break;

    case 3:
        m_bKeyInputable = true;

        // 활성화
        break;
    default:
        break;
    }


}


