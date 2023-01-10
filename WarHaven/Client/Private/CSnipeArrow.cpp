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


	SetUp_TrailEffect(
		_float4(0.f, -0.2f, 0.f, 1.f),	//Weapon Low
		_float4(0.f, 0.2f, 0.f, 1.f),	//Weapon High
		_float4(-0.2f, 0.f, 0.f, 1.f), //Left
		_float4(0.2f, 0.f, 0.f, 1.f), //Right
		_float4(1.f, 1.f, 0.f, 0.05f), // GlowFlag
		_float4(1.f, 0.8f, 0.1f, 0.4f), //vColor
		0.f,
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_Glow_04.dds",
		L"../bin/resources/Textures/Effects/WarHaven/Texture/T_SmokeShadow_01.dds",
		100
	);

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

void CSnipeArrow::My_Tick()
{
	__super::My_Tick();

}

void CSnipeArrow::OnEnable()
{
	__super::OnEnable();

	//Turn_Effect(true);
}

void CSnipeArrow::OnDisable()
{
	if (m_bCollect)
		static_cast<CUnit_Archer*>(m_pOwnerUnit)->Collect_Arrow(m_hcCode, this);
	
	//Turn_Effect(false);

	__super::OnDisable();
}

void CSnipeArrow::Turn_Effect(_bool bOnOff)
{
	if (bOnOff)
	{
		if (m_Test.empty())
		{
			m_Test = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"Qanda_Sniping", this, ZERO_VECTOR); //스나이핑 이펙트 추가할것
		}
	}
	else
	{
		if (!m_Test.empty())
		{
			for (auto& elem : m_Test)
			{
				static_cast<CRectEffects*>(elem)->Set_AllFadeOut();
			}
			m_Test.clear();
		}
	}
}
