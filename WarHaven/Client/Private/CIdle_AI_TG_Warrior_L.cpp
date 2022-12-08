#include "stdafx.h"
#include "CIdle_AI_TG_Warrior_L.h"

#include "GameInstance.h"
#include "Functor.h"
#include "CAnimator.h"
#include "CUnit.h"
#include "Transform.h"
#include "CUser.h"
#include "CEffects_Factory.h"
#include "CSword_Effect.h"
#include "Animation.h"
#include "Model.h"
#include "CColorController.h"

CIdle_AI_TG_Warrior_L::CIdle_AI_TG_Warrior_L()
{
}

CIdle_AI_TG_Warrior_L::~CIdle_AI_TG_Warrior_L()
{
}

CIdle_AI_TG_Warrior_L* CIdle_AI_TG_Warrior_L::Create()
{
    CIdle_AI_TG_Warrior_L* pInstance = new CIdle_AI_TG_Warrior_L();

    if (FAILED(pInstance->Initialize()))
    {
        Call_MsgBox(L"Failed to Initialize : CIdle_AI_TG_Warrior_L");
        SAFE_DELETE(pInstance);
    }

    return pInstance;
}
HRESULT CIdle_AI_TG_Warrior_L::Initialize()
{
 //   __super::Initialize();

    // 상하체 분리.
    // 


    m_eAnimType = ANIM_BASE_L;            // 애니메이션의 메쉬타입
    m_iAnimIndex = 3;                   // 현재 내가 사용하고 있는 애니메이션 순서(0 : IDLE, 1 : Run)
    m_eStateType = AI_STATE_IDLE_WARRIOR_L;   // 나의 행동 타입(Init 이면 내가 시작할 타입)

    m_fMyMaxLerp = 0.45f;
    
    // 선형 보간 시간
    m_fInterPolationTime = 0.1f;

    // 애니메이션의 전체 속도를 올려준다.
    m_fAnimSpeed = 1.f;

    m_vecAdjState.push_back(AI_STATE_RUN_WARRIOR_L);
    m_vecAdjState.push_back(AI_STATE_ATTACK_HORIZONTALMIDDLE_L);
    m_vecAdjState.push_back(AI_STATE_GUARD_BEGIN_WARRIOR);

    return S_OK;
}

void CIdle_AI_TG_Warrior_L::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    if (ePrevType == AI_STATE_TG_HIT_WARRIOR ||
        ePrevType == AI_STATE_TG_GUARDHIT_WARRIOR ||
        ePrevType == AI_STATE_TG_GROGGYHIT_WARRIOR ||
        ePrevType == AI_STATE_TG_STINGHIT_WARRIOR ||
        ePrevType == AI_STATE_TG_FLYHIT_WARRIOR
        )
    {
        m_iRand = random(0, 2);
        fwaitCurTime = 20.f;
    }

    if (ePrevType == AI_STATE_RUNBEGIN_WARRIOR_L ||
        ePrevType == AI_STATE_RUNBEGIN_WARRIOR_R
        )
        fwaitCurTime = 0.4f;

    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CIdle_AI_TG_Warrior_L::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
    fwaitCurTime += fDT(0);

    if (fwaitCurTime < fwaitCoolTime + 1.f)
    {
        if (fwaitCurTime > fwaitCoolTime)
            return AI_STATE_WALK_WARRIOR_L;
    }

    CUnit* pUnit = pOwner->Get_TargetUnit();


    CTransform* pMyTransform = pOwner->Get_Transform();

    _float4 vLook = pUnit->Get_Transform()->Get_World(WORLD_POS) - pOwner->Get_Transform()->Get_World(WORLD_POS);

    pMyTransform->Set_LerpLook(vLook.Normalize(), m_fMyMaxLerp);

    _float fLength = vLook.Length();


    if (fLength < 3.f)
    {
        return AI_STATE_RUNBEGIN_WARRIOR_L;
    }
    else if (fLength < 2.f)
    {
        if ((_uint)fwaitCurTime == 20)
        {
            if (m_iRand == 0)
                return AI_STATE_ATTACK_HORIZONTALMIDDLE_L;
            else if (m_iRand == 1)
                return AI_STATE_GUARD_BEGIN_WARRIOR;
        }
    }

    return __super::Tick(pOwner, pAnimator);

}

void CIdle_AI_TG_Warrior_L::Exit(CUnit* pOwner, CAnimator* pAnimator)
{
    /* 할거없음 */
}

STATE_TYPE CIdle_AI_TG_Warrior_L::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
	/* Player가 Idle로 오는 조건
	1. 현재 진행중인 애니메이션이 끝났을 때
	*/

	{		
	
		if (pAnimator->Is_CurAnimFinished())
			return m_eStateType;
	}

    return STATE_END;
}
