#define MJDEVELOP

#include "stdafx.h"
#include "CLevel_Stage.h"

#include "GameInstance.h"

#include "CUser.h"

#include "ImGui_Manager.h"

#include "Transform.h"
#include "CSkyBox.h"
#include "CTerrain.h"

#include "CPlayer.h"

#include "CUnit_Warrior.h"
#include "CUnit_Spearman.h"
#include "CUnit_WarHammer.h"
#include "CUnit_Valkyrie.h"

#include "CBarricade.h"

#include "CCamera_Free.h"
#include "CCamera_Follow.h"

#include "CSword_Effect.h"
#include "CEffects_Factory.h"
#include "CBloodOverlay.h"

#include "Model.h"

// MJ_INCLUDE
#include "CDrawable_Terrain.h"
#include "CStructure_Instance.h"
#include "CUtility_Transform.h"
#include "CMap_Loader.h"
// YJ
#include "CDebugObject.h"

#include "CPhysXCharacter.h"
#include "CStructure.h"

#include "CMapColliders.h"

#include "CGameSystem.h"

//JJ
#include "Loading_Manager.h"

CLevel_Stage::CLevel_Stage()
{
}

CLevel_Stage::~CLevel_Stage()
{
}

HRESULT CLevel_Stage::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Stage::SetUp_Prototypes()
{
	GAMEINSTANCE->Create_Scene(CPhysX_Manager::Scene::SCENE_CURRENT);
	

	m_fLoadingFinish = 0.2f;
	
	CCamera* pFreeCam = GAMEINSTANCE->Find_Camera(L"FreeCam");
	DISABLE_GAMEOBJECT(pFreeCam);

	return S_OK;
}

HRESULT CLevel_Stage::Enter()
{
	__super::Enter();

	CUser::Get_Instance()->On_EnterStageLevel();
	LEVEL_TYPE_CLIENT eLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	if (eLevel != LEVEL_BOOTCAMP)
	{
		CUser::Get_Instance()->On_EnterLevel();
	}

	CEffects_Factory::Get_Instance()->On_EnterLevel();

	GAMEINSTANCE->Begin_PhysScene();

	return S_OK;
}

void CLevel_Stage::Tick()
{
	if (!m_bStaticShadowBake)
	{
		m_fDealyAcc += fDT(0);

		if (m_fDealyAcc >= m_fDelayTime)
		{
			list<CGameObject*> ObjList = GAMEINSTANCE->Get_ObjGroup(GROUP_DECORATION);

			for (auto& elem : ObjList)
			{
				/*if (dynamic_cast<CDrawable_Terrain*>(elem))
					continue;*/

				m_StaticShadowObjects.push_back(elem);

			}
			m_vSunLook.w = 0.f;

			GAMEINSTANCE->Bake_StaticShadow(m_StaticShadowObjects, m_vCenterPos, m_fDistance, m_vSunLook, m_bLensFlare);
			/*_float4 vLook = _float4(-1.f, -2.f, -1.f, 0.f).Normalize();

			_float4 vSunPos = m_vCenterPos + (vLook * -1.f * 500.f);
			CREATE_GAMEOBJECT(CDebugObject::Create(vSunPos, _float4(10.f, 10.f, 10.f)), GROUP_DEBUG);*/

			m_bStaticShadowBake = true;
			GAMEINSTANCE->Save_Memory();

		}
	}


#ifdef _DEBUG
	CImGui_Manager::Get_Instance()->Tick();
#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	CImGui_Manager::Get_Instance()->Tick();

#endif // !_DEBUG
#endif

	CUser::Get_Instance()->Fix_CursorPosToCenter();
	CUser::Get_Instance()->KeyInput_FPSSetter();
	CUser::Get_Instance()->Update_KeyCommands();

}

void CLevel_Stage::Late_Tick()
{
}

HRESULT CLevel_Stage::Render()
{

#ifdef _DEBUG
	if (FAILED(CImGui_Manager::Get_Instance()->Render()))
		return E_FAIL;

#endif

#ifdef RELEASE_IMGUI

#ifndef _DEBUG
	if (FAILED(CImGui_Manager::Get_Instance()->Render()))
		return E_FAIL;

#endif // !_DEBUG
#endif

	return S_OK;
}

HRESULT CLevel_Stage::OnExit()
{
	__super::OnExit();

	CEffects_Factory::Get_Instance()->On_ExitLevel();
	GAMEINSTANCE->Delete_Scene(CPhysX_Manager::Scene::SCENE_CURRENT);
	GAMEINSTANCE->End_PhysScene();


	return S_OK;
}

HRESULT CLevel_Stage::Exit()
{
	__super::Exit();

	CUser::Get_Instance()->On_ExitStageLevel();
	GAMEINSTANCE->Reset_Camera(L"DefaultCam");
	CGameSystem::Get_Instance()->On_ExitLevel();
	


	return S_OK;
}
