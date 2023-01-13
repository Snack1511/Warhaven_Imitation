#include "stdafx.h"
#include "CState_Combat.h"

#include "UsefulHeaders.h"


#include "CAnimator.h"
#include "CUnit.h"

#include "CUser.h"

CState_Combat::CState_Combat()
{
}

CState_Combat::~CState_Combat()
{
}

HRESULT CState_Combat::Initialize()
{

    return S_OK;
}

void CState_Combat::Enter(CUnit* pOwner, CAnimator* pAnimator, STATE_TYPE ePrevType, void* pData )
{
    __super::Enter(pOwner, pAnimator, ePrevType, pData);
}

STATE_TYPE CState_Combat::Tick(CUnit* pOwner, CAnimator* pAnimator)
{
	//높이 차 별로 안나면 ray 쏴야함
	if (!pOwner->Get_OwnerPlayer()->Get_TargetObject())
		return STATE_END;

	_float4 vOutPos;
	_float fOutDist;

	_float4 vCurPos = pOwner->Get_Transform()->Get_World(WORLD_POS);
	_float4 vTargetPos = pOwner->Get_OwnerPlayer()->Get_TargetObjPos();

	//높이 차 별로 안나면 ray 쏴야함
	_float4 vOutPos;
	_float fOutDist;

	_float4 vRayStartPos = vCurPos;
	vRayStartPos.y += 0.5f;
	_float4 vRayEndPos = vTargetPos;
	vRayEndPos.y += 0.5f;

	_float4 vRayDir = vRayEndPos - vRayStartPos;

	if (GAMEINSTANCE->Shoot_RaytoStaticActors(&vOutPos, &fOutDist, vCurPos, vRayDir, vRayDir.Length()))
	{
		//ray 맞았으면 PathFind로 가야지 (common)


		//여기다 return pathfind로 ㅇㅋ?


	}


    return __super::Tick(pOwner, pAnimator);
}

void CState_Combat::Exit(CUnit* pOwner, CAnimator* pAnimator)
{

}

STATE_TYPE CState_Combat::Check_Condition(CUnit* pOwner, CAnimator* pAnimator)
{
    return STATE_END;
}

