#include "stdafx.h"
#include "CPlayer.h"

#include "CEffects_Factory.h"

#include "GameInstance.h"

#include "CCamera_Default.h"
#include "CCollider_Sphere.h"
#include "Texture.h"
#include "CShader.h"
#include "CModel_Renderer.h"
#include "Transform.h"
#include "Functor.h"
#include "CFader.h"
#include "Physics.h"
#include "CMesh_Rect.h"
#include "CMesh_Terrain.h"
#include "Model.h"
#include "CAnimator.h"
#include "CNavigation.h"
#include "CCell.h"
#include "CTrailEffect.h"
#include "CTrailBuffer.h"

#include "CTeamConnector.h"

#include "CScript_FollowCam.h"

#include "MeshContainer.h"

#include "CState.h"
#include "CState_Manager.h"

#include "CPhysXCharacter.h"

#include "CBoneCollider.h"

#include "CCamera_Follow.h"
#include "CState_Hit.h"

#include "CUnit_Warrior.h"
#include "CUnit_Spearman.h"
#include "CUnit_WarHammer.h"
#include "CUnit_Valkyrie.h"

#include "CUser.h"

#include "CEffects_Factory.h"
#include "CRectEffects.h"

#include "CSquad.h"
#include "CUI_UnitHUD.h"
#include "CUtility_Transform.h"



#define ERR_MSG_TH


CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{

}

CPlayer* CPlayer::Create(CPlayerInfo* pPlayerInfo)
{
	CPlayer* pInstance = new CPlayer;

	pInstance->m_pMyPlayerInfo = pPlayerInfo;

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CPlayer");
		return nullptr;
	}

	if (FAILED(pInstance->Initialize()))
	{
		SAFE_DELETE(pInstance);
		Call_MsgBox(L"Failed to Initialize_Prototype : CPlayer");
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Create_DefaultClass(CPlayerInfo::PLAYER_SETUP_DATA tSetUpData)
{
	wstring wstrModeSkel[CLASS_DEFAULT_END] = {
		L"../bin/resources/meshes/characters/Warrior/Warrior.fbx", // WARRIOR
		L"",
		L"",
		L"",
		L"",
		L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx"
	};

	wstring wstrModeFace[CLASS_DEFAULT_END] = {
		L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00_50.fbx", // WARRIOR
		L"",
		L"",
		L"",
		L"",
		L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Face_A00_50.fbx"
	};

	wstring wstrModeWeapon_L[CLASS_DEFAULT_END] = {
	L"", // WARRIOR
	L"",
	L"",
	L"",
	L"",
	L"" // 
	};

	wstring wstrModeBody[CLASS_DEFAULT_END];

	for (_uint i = 0; i < CLASS_DEFAULT_END; ++i)
		wstrModeBody[i] = tSetUpData.wstrBodyMeshPath[i];

	wstring wstrModeHead[CLASS_DEFAULT_END];

	for (_uint i = 0; i < CLASS_DEFAULT_END; ++i)
		wstrModeHead[i] = tSetUpData.wstrHelmetMeshPath[i];

	wstring wstrModeWeapon_R[CLASS_DEFAULT_END];
	for (_uint i = 0; i < CLASS_DEFAULT_END; ++i)
		wstrModeWeapon_R[i] = tSetUpData.wstrWeaponMeshPath[i];


	CUnit::UNIT_MODEL_DATA  tModelData[CLASS_DEFAULT_END];


	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (wstrModeSkel[i].empty())
			continue;

		tModelData[i].strModelPaths[MODEL_PART_SKEL] = wstrModeSkel[i];

		tModelData[i].strModelPaths[MODEL_PART_BODY] = wstrModeBody[i];
		tModelData[i].strModelPaths[MODEL_PART_FACE] = wstrModeFace[i];
		tModelData[i].strModelPaths[MODEL_PART_HEAD] = wstrModeHead[i];


		if (wstrModeWeapon_R[i] != L"")
		{
			tModelData[i].strModelPaths[MODEL_PART_WEAPON] = wstrModeWeapon_R[i];
			tModelData[i].strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";
		}

		if (wstrModeWeapon_L[i] != L"")
		{
			tModelData[i].strModelPaths[MODEL_PART_WEAPON_L] = wstrModeWeapon_L[i];
			tModelData[i].strRefBoneName[MODEL_PART_WEAPON_L] = "0B_L_WP1";
		}
	}

	m_pDefaultClass[CLASS_DEFAULT_WARRIOR] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_WARRIOR]);
	m_pDefaultClass[CLASS_DEFAULT_ENGINEER] = CUnit_WarHammer::Create(tModelData[CLASS_DEFAULT_ENGINEER]);
	/*_pDefaultClass[CLASS_DEFAULT_SPEAR] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_SPEAR]);
	m_pDefaultClass[CLASS_DEFAULT_ARCHER] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_ARCHER]);
	m_pDefaultClass[CLASS_DEFAULT_PALADIN] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_PALADIN]);
	m_pDefaultClass[CLASS_DEFAULT_PRIEST] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_PRIEST]);
	*/

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (nullptr == m_pDefaultClass[i])
		{
			continue;
			Call_MsgBox(L"유닛 생성 안됐음.");
			return;
		}
		else
		{
			if (FAILED(m_pDefaultClass[i]->Initialize()))
			{
				Call_MsgBox_Index(L"Failed to Initialize Unit / Index : ", i);
				return;

			}

			m_pDefaultClass[i]->Set_OwnerPlayer(this);

			if (!m_pFollowCam)
			{
				Call_MsgBox(L"카메라 생성 안됐음.");
				return;
			}

			m_pDefaultClass[i]->Set_FollowCam(m_pFollowCam);
		}

	}

	/* 변신 후 예약된 클래스 */
	m_iReserveStateDefault[CLASS_DEFAULT_WARRIOR] = STATE_IDLE_PLAYER_R;
	m_iReserveStateDefault[CLASS_DEFAULT_ENGINEER] = STATE_IDLE_WARHAMMER_R;


	/* 변신 애니메이션 Index */
	m_iChangeHeroAnimIndex[CLASS_DEFAULT_WARRIOR] = 62;
	m_iChangeHeroAnimIndex[CLASS_DEFAULT_ENGINEER] = 62;
}

void CPlayer::Create_HeroClass(CPlayerInfo::PLAYER_SETUP_DATA tSetUpData)
{

	wstring wstrModeSkel[HERO_END] = {
		L"../bin/resources/meshes/characters/Valkyrie/Valkyrie.fbx" // FIONA
		L"",
		L"",
		L"",
	};

	wstring wstrModeFace[HERO_END] = {
	L"../bin/resources/meshes/characters/Valkyrie/Head/SK_Fiona0001_Face_A00_50.fbx" // FIONA
	L"",
	L"",
	L"",
	};

	wstring wstrModeWeapon_L[HERO_END] = {
		L"../bin/resources/meshes/weapons/Valkyrie_Shield/SK_WP_HeaterShield0001_A00.fbx" // FIONA 
		L"",
		L"",
		L"",
	};

	wstring wstrModeBody[HERO_END];

	for (_uint i = 0; i < HERO_END; ++i)
		wstrModeBody[i] = tSetUpData.wstrBodyMeshPath[i + FIONA];

	wstring wstrModeHead[HERO_END];

	for (_uint i = 0; i < HERO_END; ++i)
		wstrModeHead[i] = tSetUpData.wstrHelmetMeshPath[i + FIONA];

	wstring wstrModeWeapon_R[HERO_END];
	for (_uint i = 0; i < HERO_END; ++i)
		wstrModeWeapon_R[i] = tSetUpData.wstrWeaponMeshPath[i + FIONA];



	CUnit::UNIT_MODEL_DATA  tModelData[HERO_END];

	for (int i = 0; i < HERO_END; ++i)
	{
		if (wstrModeSkel[i].empty())
			continue;

		tModelData[i].strModelPaths[MODEL_PART_SKEL] = wstrModeSkel[i];

		tModelData[i].strModelPaths[MODEL_PART_BODY] = wstrModeBody[i];
		tModelData[i].strModelPaths[MODEL_PART_FACE] = wstrModeFace[i];
		tModelData[i].strModelPaths[MODEL_PART_HEAD] = wstrModeHead[i];


		if (wstrModeWeapon_R[i] != L"")
		{
			tModelData[i].strModelPaths[MODEL_PART_WEAPON] = wstrModeWeapon_R[i];
			tModelData[i].strRefBoneName[MODEL_PART_WEAPON] = "0B_R_WP1";
		}

		if (wstrModeWeapon_L[i] != L"")
		{
			tModelData[i].strModelPaths[MODEL_PART_WEAPON_L] = wstrModeWeapon_L[i];
			tModelData[i].strRefBoneName[MODEL_PART_WEAPON_L] = "0B_L_WP1";
		}
	}


	m_pHeroClass[CLASS_HREO_FIONA - CLASS_HREO_FIONA] = CUnit_Valkyrie::Create(tModelData[CLASS_HREO_FIONA - CLASS_HREO_FIONA]);
	//m_pHeroClass[CLASS_HREO_QANDA - CLASS_HREO_FIONA] = CUnit_Warrior::Create(tModelData[CLASS_HREO_QANDA]);
	//m_pHeroClass[CLASS_HREO_HOEDT - CLASS_HREO_FIONA] = CUnit_Warrior::Create(tModelData[CLASS_HREO_HOEDT]);
	//m_pHeroClass[CLASS_HREO_LANCER - CLASS_HREO_FIONA] = CUnit_Warrior::Create(tModelData[CLASS_HREO_LANCER]);


	for (int i = 0; i < HERO_END; ++i)
	{
		if (nullptr == m_pHeroClass[i])
		{
			continue;
			Call_MsgBox(L"유닛 생성 안됐음.");
			return;
		}
		else
		{
			if (FAILED(m_pHeroClass[i]->Initialize()))
			{
				Call_MsgBox_Index(L"Failed to Initialize UnitHero / Index : ", i);
				return;

			}

			m_pHeroClass[i]->Set_OwnerPlayer(this);

			if (!m_pFollowCam)
			{
				Call_MsgBox(L"카메라 생성 안됐음.");
				return;
			}

			m_pHeroClass[i]->Set_FollowCam(m_pFollowCam);
		}
	}

	m_iReserveStateHero[CLASS_HREO_FIONA - CLASS_HREO_FIONA] = STATE_IDLE_VALKYRIE_R;
}

HRESULT CPlayer::Set_FollowCam(wstring wstrCamKey)
{
	m_pFollowCam = CCamera_Follow::Create(nullptr, nullptr);
	m_pFollowCam->Initialize();
	m_pFollowCam->Get_Transform()->Set_World(WORLD_POS, ZERO_VECTOR);
	m_pFollowCam->Get_Transform()->Make_WorldMatrix();
	CREATE_GAMEOBJECT(m_pFollowCam, GROUP_CAMERA);
	GAMEINSTANCE->Add_Camera_Level(wstrCamKey, m_pFollowCam);
	DISABLE_GAMEOBJECT(m_pFollowCam);

	return S_OK;
}

HRESULT CPlayer::Change_DefaultUnit(CLASS_DEFAULT eClass)
{
	if (eClass >= CLASS_DEFAULT_END)
		return E_FAIL;

	_float4 vPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);

	if (m_pCurrentUnit)
	{
		DISABLE_GAMEOBJECT(m_pCurrentUnit);
	}

	m_pCurrentUnit = m_pDefaultClass[eClass];
	ENABLE_GAMEOBJECT(m_pCurrentUnit);

	m_pFollowCam->Set_FollowTarget(m_pCurrentUnit);
	Set_Postion(vPos);

	m_pCurrentUnit->Enter_State((STATE_TYPE)m_iReserveStateDefault[eClass]);


	m_eCurrentDefaultClass = eClass;

	GAMEINSTANCE->Stop_GrayScale();


	return S_OK;
}

HRESULT CPlayer::Change_HeroUnit(CLASS_HREO eClass)
{
	if (eClass >= CLASS_HERO_END)
		return E_FAIL;

	_float4 vPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
	_float4 vLook = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_LOOK);

	if (m_pCurrentUnit)
	{
		DISABLE_GAMEOBJECT(m_pCurrentUnit);
	}

	m_pCurrentUnit = m_pHeroClass[eClass - CPlayer::CLASS_HREO_FIONA];
	ENABLE_GAMEOBJECT(m_pCurrentUnit);

	m_pCurrentUnit->Get_Transform()->Set_Look(vLook);
	m_pCurrentUnit->Set_MainPlayer();
	m_pCurrentUnit->Enter_State((STATE_TYPE)m_iReserveStateHero[eClass - CPlayer::CLASS_HREO_FIONA]);
	//m_pCurrentUnit->Reserve_State((STATE_TYPE)m_iReserveStateHero[eClass]);
	m_pFollowCam->Set_FollowTarget(m_pCurrentUnit);

	Set_Postion(vPos);

	return S_OK;
}

void CPlayer::Respawn_Unit(_float4 vPos, CLASS_DEFAULT eClass)
{
	m_bDie = false;
	Change_DefaultUnit(eClass);

	Set_Postion(vPos);

	m_pCurrentUnit->On_Respawn();

	if (!m_bIsMainPlayer)
		ENABLE_GAMEOBJECT(m_pUnitHUD);
}

void CPlayer::Reserve_State(_uint eState)
{
	m_pCurrentUnit->Reserve_State(STATE_TYPE(eState));
	
}

void CPlayer::Set_Default_ReserveState(_uint eClass, _uint eState)
{
	m_iReserveStateDefault[eClass] = eState;
}

void CPlayer::SetUp_UnitColliders(_bool bPlayer)
{
	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (m_pDefaultClass[i] == nullptr)
			continue;

		m_pDefaultClass[i]->SetUp_Colliders(bPlayer);
	}

	for (int i = 0; i < HERO_END; ++i)
	{
		if (m_pHeroClass[i] == nullptr)
			continue;

		m_pHeroClass[i]->SetUp_Colliders(bPlayer);
	}

}

void CPlayer::SetUp_UnitHitStates(_bool bPlayer)
{

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (m_pDefaultClass[i] == nullptr)
			continue;

		m_pDefaultClass[i]->SetUp_HitStates(bPlayer);
	}

	for (int i = 0; i < HERO_END; ++i)
	{
		if (m_pHeroClass[i] == nullptr)
			continue;

		m_pHeroClass[i]->SetUp_HitStates(bPlayer);
	}
}

void CPlayer::Set_Postion(_float4 vPos)
{
	if (m_pCurrentUnit)
	{
		m_pCurrentUnit->Teleport_Unit(vPos);
		m_pCurrentUnit->Synchronize_CamPos();
	}
}

void CPlayer::Set_LookToTarget()
{
	if (m_pCurrentUnit)
	{
		m_pCurrentUnit->Set_LookToTarget();
	}
}

void CPlayer::Set_MainPlayer()
{
	m_bIsMainPlayer = true;

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (m_pDefaultClass[i] == nullptr)
			continue;

		m_pDefaultClass[i]->Set_MainPlayer();
	}

	for (int i = 0; i < CLASS_HERO_END - CLASS_HREO_FIONA; ++i)
	{
		if (m_pHeroClass[i] == nullptr)
			continue;

		m_pHeroClass[i]->Set_MainPlayer();
	}
}



HRESULT CPlayer::Initialize_Prototype()
{
	if (!m_pMyPlayerInfo)
		return E_FAIL;


	Set_FollowCam(m_pMyPlayerInfo->m_tPlayerInfo.wstrCamName);


	Create_DefaultClass(m_pMyPlayerInfo->m_tPlayerSetUpData);
	Create_HeroClass(m_pMyPlayerInfo->m_tPlayerSetUpData);

	m_pCurrentUnit = m_pDefaultClass[WARRIOR];
	m_eCurrentDefaultClass = CLASS_DEFAULT_WARRIOR;
	m_pFollowCam->Set_FollowTarget(m_pCurrentUnit);

	Create_UnitHUD();

	return S_OK;
}


HRESULT CPlayer::Initialize()
{

	return S_OK;
}

HRESULT CPlayer::Start()
{
	__super::Start();

	SetUp_UnitHitStates(m_bIsMainPlayer);

	for (int i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (nullptr == m_pDefaultClass[i])
		{
			continue;
			Call_MsgBox(L"CPlayer : 기본 영웅 추가 안됐음");
			return E_FAIL;
		}

		CREATE_GAMEOBJECT(m_pDefaultClass[i], GROUP_PLAYER);
		DISABLE_GAMEOBJECT(m_pDefaultClass[i]);

	}

	for (int i = 0; i < HERO_END; ++i)
	{
		if (nullptr == m_pHeroClass[i])
		{
			continue;
			Call_MsgBox(L"CPlayer : 영웅 추가 안됐음");
			return E_FAIL;
		}

		CREATE_GAMEOBJECT(m_pHeroClass[i], GROUP_PLAYER);
		DISABLE_GAMEOBJECT(m_pHeroClass[i]);

	}
	/* hud 생성 */
	Enable_UnitHUD();

	if (m_pCurrentUnit)
	{
		if (m_bEnableOnStart)
		{
			m_pCurrentUnit->On_Respawn();

			ENABLE_GAMEOBJECT(m_pCurrentUnit);
			if (!m_bIsMainPlayer)
				ENABLE_GAMEOBJECT(m_pUnitHUD);
		}
	}
	else
	{
		Call_MsgBox(L"CPlayer : 현재 지정된 유닛이 없음.");
		return E_FAIL;
	}

	if (m_bIsMainPlayer)
	{
		if (m_pMySquad)
		{
			m_pMySquad->SetUp_OutlineType_SquadMember();
		}
	}
	


	return S_OK;
}

void CPlayer::OnEnable()
{
	__super::OnEnable();
}

void CPlayer::OnDisable()
{
	__super::OnDisable();
}

void CPlayer::On_Die()
{
	//m_bDie = true;
	DISABLE_GAMEOBJECT(m_pUnitHUD);
	m_bDieDelay = true;

	if (m_bIsMainPlayer)
	{

	}

	

}

void CPlayer::On_RealDie()
{
	m_bDieDelay = false;
	m_fDieDelayAcc = 0.f;
	m_bDie = true;

	m_DeadLights.clear();

	if (m_pMyTeam)
	{
		if (m_pMyTeam->IsMainPlayerTeam())
			m_DeadLights = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"DeadLight", m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS));

	}
	else if (m_bIsMainPlayer)
		m_DeadLights = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"DeadLight", m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS));


}

void CPlayer::On_Reborn()
{
	GAMEINSTANCE->Stop_GrayScale();

	for (auto& elem : m_DeadLights)
	{
		static_cast<CRectEffects*>(elem)->Set_LoopControlfalse();
	}

	m_pCurrentUnit->Enter_State((STATE_TYPE)m_iReserveStateDefault[m_eCurrentDefaultClass]);
	m_bDie = false;
}

void CPlayer::Set_TeamType(eTEAM_TYPE eTeamType)
{
	m_eTeamType = eTeamType;
}

void CPlayer::Set_OutlineType(OUTLINETYPE eOutlineType)
{
	_float4 vOutlineFlag = ZERO_VECTOR;

	switch (eOutlineType)
	{
	case Client::CPlayer::eENEMY:
		vOutlineFlag = _float4(0.9f, 0.1f, 0.1f);
		break;
	case Client::CPlayer::eSQUADMEMBER:
		vOutlineFlag = _float4(0.709f, 0.901f, 0.113f);
		break;
	default:
		break;
	}

	for (_uint i = 0; i < CLASS_DEFAULT_END; ++i)
	{
		if (m_pDefaultClass[i])
		{
			GET_COMPONENT_FROM(m_pDefaultClass[i], CModel)->Set_OutlineFlag(vOutlineFlag);

		}
	}

	for (_uint i = 0; i < CLASS_HERO_END; ++i)
	{
		if (m_pHeroClass[i])
			GET_COMPONENT_FROM(m_pHeroClass[i], CModel)->Set_OutlineFlag(vOutlineFlag);
	}
}

void CPlayer::My_Tick()
{
	//공통으로 업데이트 되어야 하는것

	m_pUnitHUD->Set_UnitStatus(m_pCurrentUnit->Get_Status());

	Update_HeroGauge();

	Update_DieDelay();

	if (!m_bIsMainPlayer)
		return;

	Update_HP();
}

void CPlayer::My_LateTick()
{
	//공통으로 업데이트 되어야 하는것

	if (m_pCurrentUnit->Get_Status().fHP > 0.f)
	{
		Frustum_UnitHUD();
		TransformProjection();
	}

	if (!m_bIsMainPlayer)
		return;

	if (m_bDie && KEY(ENTER, TAP))
	{
		m_pCurrentUnit->Start_Reborn();

	}


	static _float4 vRimLightFlag = _float4(0.f, 0.f, 1.f, 0.01f);

	if (KEY(UP, TAP))
	{
		vRimLightFlag.w += 0.01f;
		for (_uint i = 0; i < CLASS_DEFAULT_END; ++i)
		{
			if (m_pDefaultClass[i])
			{
				GET_COMPONENT_FROM(m_pDefaultClass[i], CModel)->Set_RimLightFlag(vRimLightFlag);

			}
		}
		cout << vRimLightFlag.w << endl;

	}

	if (KEY(DOWN, TAP))
	{
		vRimLightFlag.w -= 0.01f;
		for (_uint i = 0; i < CLASS_DEFAULT_END; ++i)
		{
			if (m_pDefaultClass[i])
			{
				GET_COMPONENT_FROM(m_pDefaultClass[i], CModel)->Set_RimLightFlag(vRimLightFlag);

			}
		}
		cout << vRimLightFlag.w << endl;

	}
}

void CPlayer::Update_HP()
{
	CUser::Get_Instance()->Set_HP(m_pCurrentUnit->Get_Status().fMaxHP, m_pCurrentUnit->Get_Status().fHP);
}

void CPlayer::Update_HeroGauge()
{
	if (m_bIsMainPlayer)
	{
		_bool IsHeroGaugeEnable = CUser::Get_Instance()->Is_OnHeroGauge();
		if (!IsHeroGaugeEnable)
			return;

		CUser::Get_Instance()->Set_HeroGauge(m_fMaxGauge, m_fGauge);
	}

	if (!m_bAbleHero) //CChangeHero_Player, HUD
	{
		_float fGaugeSpeed = fDT(0);

		if (!m_bIsHero) //CChangeHero_Player
		{
			m_fGauge += fGaugeSpeed * 20.f;
			if (m_fGauge > m_fMaxGauge)
			{
				On_AbleHero();
			}
		}
		else //변신 중일때 
		{
			m_fGauge -= fGaugeSpeed * 2.f; // 인게임속도2.f 

			if (m_bIsMainPlayer)
			{
				On_FinishHero_KeyInput();
			}

			if (m_fGauge <= 0.f)
			{
				On_FinishHero();
			}
		}
	}


}

void CPlayer::On_AbleHero()
{
	m_bAbleHero = true;
	m_fGauge = m_fMaxGauge;

	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->SetActive_HeroPortrait(true);
		CUser::Get_Instance()->Turn_HeroGaugeFire(true);
	}
}

void CPlayer::On_FinishHero()
{
	m_fGauge = 0.f;
	m_bIsHero = false;

	Change_DefaultUnit(m_eCurrentDefaultClass);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"UnHenshin", m_pCurrentUnit, m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS));

	if (m_bIsMainPlayer)
	{

		CUser::Get_Instance()->Set_HUD((CLASS_TYPE)m_pCurrentUnit->Get_OwnerPlayer()->Get_CurrentDefaultClass());
		CUser::Get_Instance()->Turn_HeroGaugeFire(false);
	}

}

void CPlayer::On_FinishHero_KeyInput()
{
	//if(KEY(CTRL, HOLD)) //1번 자주눌러서 막음
	if (KEY(NUM1, TAP))
	{
		On_FinishHero();
	}
}

void CPlayer::Update_DieDelay()
{
	if (m_bDieDelay)
	{
		m_fDieDelayAcc += fDT(0);
		if (m_fDieDelayAcc >= m_fDieCoolTime)
		{
			On_RealDie();
		}
	}
}

void CPlayer::Create_UnitHUD()
{
	m_pUnitHUD = CUI_UnitHUD::Create();
	m_pUnitHUD->Set_OwnerPlayer(this);
}

void CPlayer::Enable_UnitHUD()
{
	CREATE_GAMEOBJECT(m_pUnitHUD, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pUnitHUD);
}


void CPlayer::Frustum_UnitHUD()
{
	_float fDis = CUtility_Transform::Get_FromCameraDistance(m_pCurrentUnit);

	if (fDis < 30.f)
	{
		m_pUnitHUD->Set_UnitDis(fDis);

		_float4 vPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
		vPos.y += 2.f;

		if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(vPos.XMLoad(), 0.1f))
		{
			if (!m_pUnitHUD->Is_Valid())
			{
				if (!m_bIsMainPlayer)
				{
					ENABLE_GAMEOBJECT(m_pUnitHUD);
				}
			}
		}
		else
		{
			if (m_pUnitHUD->Is_Valid())
			{
				DISABLE_GAMEOBJECT(m_pUnitHUD);
			}
		}
	}
	else
	{
		if (m_pUnitHUD->Is_Valid())
		{
			DISABLE_GAMEOBJECT(m_pUnitHUD);
		}
	}
}

void CPlayer::TransformProjection()
{
	if (m_pCurrentUnit)
	{
		m_pUnitHUD->Set_ProjPos(m_pCurrentUnit->Get_Transform());
	}
}


