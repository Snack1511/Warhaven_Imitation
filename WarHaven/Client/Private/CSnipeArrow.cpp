#include "stdafx.h"
#include "CSnipeArrow.h"

#include "UsefulHeaders.h"

#include "CUnit_Archer.h"
#include "CRectEffects.h"

#include "CColorController.h"

CSnipeArrow::CSnipeArrow()
{
}
CSnipeArrow::~CSnipeArrow()
{
	//m_Test.clear();
}

CSnipeArrow* CSnipeArrow::Create()
{
	CSnipeArrow* pInstance = new CSnipeArrow;

	if (FAILED(pInstance->CSnipeArrow::Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CSnipeArrow");
		return nullptr;
	}

	return pInstance;
}

HRESULT CSnipeArrow::Start()
{
	__super::Start();

	CColorController::COLORDESC tColorDesc;
	ZeroMemory(&tColorDesc, sizeof(CColorController::COLORDESC));

	tColorDesc.eFadeStyle = CColorController::TIME;
	tColorDesc.fFadeInStartTime = 0.f;
	tColorDesc.fFadeInTime = 0.1f;
	tColorDesc.fFadeOutStartTime = 9999.f;
	tColorDesc.fFadeOutTime = 0.1f;
	tColorDesc.vTargetColor = _float4((255.f / 255.f), (140.f / 255.f), (42.f / 255.f), 0.1f);
	//tColorDesc.vTargetColor *= 1.1f;
	tColorDesc.iMeshPartType = MODEL_PART_SKEL;

	GET_COMPONENT(CColorController)->Add_ColorControll(tColorDesc);

	return S_OK;
}

HRESULT CSnipeArrow::Initialize_Prototype()
{
    if (FAILED(SetUp_Projectile(L"../bin/resources/meshes/weapons/longbow/SM_Bolt_Snipe.fbx")))
        return E_FAIL;

#ifdef TESTLEVEL_AI_PROJECTILE


	if (FAILED(SetUp_Colliders(COL_REDATTACK)))
		return E_FAIL;

#else

	if (FAILED(SetUp_Colliders(COL_BLUEATTACK)))
		return E_FAIL;

#endif // TESTLEVEL_AI_PROJECTILE


	m_hcCode = HASHCODE(CSnipeArrow);
	m_vArrowHeadPos = _float4(1.2f, 0.f, 0.f);

	m_fMaxSpeed = 50.f;
	m_fMaxDistance = 100.f;


    return CProjectile::Initialize_Prototype();
}
