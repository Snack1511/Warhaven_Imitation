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
#include "CUnit_Paladin.h"
#include "CUnit_Archer.h"

#include "CUser.h"

#include "CEffects_Factory.h"
#include "CRectEffects.h"

#include "CSquad.h"
#include "CUI_UnitHUD.h"
#include "CUtility_Transform.h"

#include "CUI_HUD.h"
#include "CUI_Portrait.h"
#include "CUI_HeroGauge.h"
#include "CUI_Skill.h"

#pragma region AI 추가용
#include "CAIController.h"
#include "CAIPersonality.h"
#pragma endregion AI 추가용
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

void CPlayer::Create_Class(CPlayerInfo::PLAYER_SETUP_DATA tSetUpData)
{
	wstring wstrModeSkel[CLASS_END] =
	{
		L"../bin/resources/meshes/characters/Warrior/Warrior.fbx", // WARRIOR
		L"",
		L"../bin/resources/meshes/characters/Archer/Archer.fbx",
		L"../bin/resources/meshes/characters/Paladin/Paladin.fbx",
		L"",
		L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx",
		L"../bin/resources/meshes/Characters/Valkyrie/Valkyrie.fbx",
		L"",
		L"",
		L"",
	};

	wstring wstrModeFace[CLASS_END] =
	{
		L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00_50.fbx", // WARRIOR
		L"",
		L"../bin/resources/meshes/characters/Archer/Head/SK_Archer001_Face_A00_50.fbx",
		L"../bin/resources/meshes/characters/Paladin/Head/SK_Paladin0001_Face_A00_50.fbx",
		L"",
		L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Face_A00_50.fbx",
		L"../bin/resources/meshes/Characters/Valkyrie/Head/SK_Fiona0001_Face_A00_50.fbx",
		L"",
		L"",
		L"",
	};

	
	wstring wstrModeWeapon_L[CLASS_END] =
	{
	L"",
	L"",
	L"../bin/resources/meshes/weapons/longBow/SK_WP_LongBow0005_A00.fbx",
	L"../bin/resources/meshes/Weapons/KiteShield/SM_WP_KiteShield0001_A00.fbx",
	L"",
	L"",	// ENGINEER
	L"../bin/resources/meshes/Weapons/Valkyrie_Shield/SK_WP_HeaterShield0001_A00_30.fbx",
	L"",
	L"",
	L"", 
	};

	wstring wstrModeBody[CLASS_END];

	for (_uint i = 0; i < CLASS_END; ++i)
		wstrModeBody[i] = tSetUpData.wstrBodyMeshPath[i];

	wstring wstrModeHead[CLASS_END];

	for (_uint i = 0; i < CLASS_END; ++i)
		wstrModeHead[i] = tSetUpData.wstrHelmetMeshPath[i];

	wstring wstrModeWeapon_R[CLASS_END];
	for (_uint i = 0; i < CLASS_END; ++i)
		wstrModeWeapon_R[i] = tSetUpData.wstrWeaponMeshPath[i];


	CUnit::UNIT_MODEL_DATA  tModelData[CLASS_END];


	for (int i = 0; i < CLASS_END; ++i)
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

	m_pAllUnitClass[WARRIOR] = CUnit_Warrior::Create(tModelData[WARRIOR]);
	m_pAllUnitClass[ENGINEER] = CUnit_WarHammer::Create(tModelData[ENGINEER]);
	m_pAllUnitClass[FIONA] = CUnit_Valkyrie::Create(tModelData[FIONA]);
	//m_pAllUnitClass[CLASS_DEFAULT_SPEAR] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_SPEAR]);
	//m_pAllUnitClass[ARCHER] = CUnit_Archer::Create(tModelData[ARCHER]);
	m_pAllUnitClass[PALADIN] = CUnit_Paladin::Create(tModelData[PALADIN]);
	//m_pAllUnitClass[CLASS_DEFAULT_PRIEST] = CUnit_Warrior::Create(tModelData[CLASS_DEFAULT_PRIEST]);


	for (int i = 0; i < CLASS_END; ++i)
	{
		if (nullptr == m_pAllUnitClass[i])
		{
			continue;
			Call_MsgBox(L"클래스 생성 안됐음");
			return;
		}
		else
		{
			if (FAILED(m_pAllUnitClass[i]->Initialize()))
			{
				Call_MsgBox_Index(L"Failed to Initialize Unit / Index : ", i);
				return;

			}

			m_pAllUnitClass[i]->Set_OwnerPlayer(this);

			if (!m_pFollowCam)
			{
				Call_MsgBox(L"카메라 생성 안됐음");
				return;
			}

			m_pAllUnitClass[i]->Set_FollowCam(m_pFollowCam);
		}

	}

	/* 변신 후 예약된 클래스 */
	m_iReserveStateDefault[WARRIOR] = STATE_IDLE_PLAYER_R;
	m_iReserveStateDefault[ENGINEER] = STATE_IDLE_WARHAMMER_R;
	m_iReserveStateDefault[PALADIN] = STATE_IDLE_PALADIN_R;
	m_iReserveStateDefault[FIONA] = STATE_IDLE_VALKYRIE_R;


	/* ���� �ִϸ��̼� Index */
	m_iChangeHeroAnimIndex[WARRIOR] = 62;
	m_iChangeHeroAnimIndex[PALADIN] = 41;
	m_iChangeHeroAnimIndex[ENGINEER] = 62;
}

void CPlayer::Player_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	m_pAIController->m_NearObjectList.push_back(pOtherObj);
}
void CPlayer::Player_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	m_pAIController->m_NearObjectList.push_back(pOtherObj);
}
void CPlayer::Player_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType) 
{

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

HRESULT CPlayer::Change_UnitClass(CLASS_TYPE eClassType)
{
	if (eClassType >= CLASS_END)
		return E_FAIL;

	if (eClassType >= CT_DEFAULT_END)
		m_bIsHero = true;

	m_ePrevClass = m_eCurrentClass;

	m_eCurrentClass = eClassType;

	_float4 vPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
	_float4	vLook = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_LOOK);


	if (m_pCurrentUnit)
	{
		DISABLE_GAMEOBJECT(m_pCurrentUnit);
	}


	m_pCurrentUnit = m_pAllUnitClass[eClassType];
	ENABLE_GAMEOBJECT(m_pCurrentUnit);

	m_pFollowCam->Set_FollowTarget(m_pCurrentUnit);
	Set_Postion(vPos);
	m_pCurrentUnit->Get_Transform()->Set_Look(vLook);

	m_pCurrentUnit->Enter_State((STATE_TYPE)m_iReserveStateDefault[eClassType]);

	GAMEINSTANCE->Stop_GrayScale();

	CUser::Get_Instance()->Set_HUD(m_eCurrentClass);

	if (m_eCurrentClass > ENGINEER)
	{
		CUser::Get_Instance()->Set_HeroPort(1);
		CUser::Get_Instance()->Transform_SkillUI(m_eCurrentClass);
	}
	
	return S_OK;
}


void CPlayer::Respawn_Unit(_float4 vPos, CLASS_TYPE eClass)
{
	if (eClass >= CT_DEFAULT_END)
	{
		Call_MsgBox(L"Respawn to Wrong Unit : CPlayer");
		return;
	}

	m_bDie = false;
	Change_UnitClass(eClass);

	Set_Postion(vPos);

	m_pCurrentUnit->On_Respawn();

	if (!m_bIsMainPlayer)
		ENABLE_GAMEOBJECT(m_pUnitHUD);

	for (auto& elem : m_DeadLights)
	{
		static_cast<CRectEffects*>(elem)->Set_LoopControlfalse();
	}
}

void CPlayer::Reserve_State(_uint eState)
{
	m_pCurrentUnit->Reserve_State(STATE_TYPE(eState));

}

void CPlayer::Set_Unit_ReserveState(_uint eClassType, _uint eState)
{
	m_iReserveStateDefault[eClassType] = eState;
}

void CPlayer::SetUp_UnitColliders(_bool bBlueTeam)
{
	for (int i = 0; i < CLASS_END; ++i)
	{
		if (m_pAllUnitClass[i] == nullptr)
			continue;

		m_pAllUnitClass[i]->SetUp_Colliders(bBlueTeam);
	}

}


void CPlayer::SetUp_UnitHitStates()
{

	if (m_bIsMainPlayer)
		m_iUnitType = 0;
	else
	{
		if (m_iUnitType != (_uint)CUnit::UNIT_TYPE::eSandbag)
			m_iUnitType = (_uint)CUnit::UNIT_TYPE::eAI_TG;
	}

	for (int i = 0; i < CLASS_END; ++i)
	{
		if (m_pAllUnitClass[i] == nullptr)
			continue;

		m_pAllUnitClass[i]->SetUp_HitStates((CUnit::UNIT_TYPE)m_iUnitType);
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

	for (int i = 0; i < CLASS_END; ++i)
	{
		if (m_pAllUnitClass[i] == nullptr)
			continue;

		m_pAllUnitClass[i]->Set_MainPlayer();
	}
}



HRESULT CPlayer::Initialize_Prototype()
{
	if (!m_pMyPlayerInfo)
		return E_FAIL;

#pragma region AI컴포넌트 추가용 구문
	if (nullptr != m_pMyPlayerInfo->m_pPersonality)
	{
		if (m_pMyPlayerInfo->m_bIsMainPlayer)
			assert(0);//메인플레이어는 Personality가 할당되면 안됩니다

		CAIController* pAIComponent = CAIController::Create(m_pMyPlayerInfo->m_pPersonality);
		m_pAIController = pAIComponent;
		Add_Component(pAIComponent);
	}
#pragma endregion AI컴포넌트 추가용 구문


	Set_FollowCam(m_pMyPlayerInfo->m_tPlayerInfo.wstrCamName);

	Create_Class(m_pMyPlayerInfo->m_tPlayerSetUpData);


	_uint iCharacter = m_pMyPlayerInfo->Choose_Character();

	m_pCurrentUnit = m_pAllUnitClass[iCharacter];

	switch (iCharacter)
	{
	case Client::WARRIOR:
		m_eCurrentClass = WARRIOR;
		break;
	case Client::SPEAR:
		break;
	case Client::ARCHER:
		//m_eCurrentClass = ENGINEER;
		break;
	case Client::PALADIN:
		//Set_CustomWeapon_Paladin(eWeaponEnum);
		break;
	case Client::PRIEST:
		break;
	case Client::ENGINEER:
		m_eCurrentClass = ENGINEER;
		break;
	case Client::FIONA:
		//Set_CustomWeapon_Fiona(eWeaponEnum);
		break;
	case Client::QANDA:
		break;
	case Client::HOEDT:
		break;
	case Client::LANCER:
		break;
	case Client::CLASS_END:
		break;
	default:
		break;

	}


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

	SetUp_UnitHitStates();

	for (int i = 0; i < CLASS_END; ++i)
	{
		if (nullptr == m_pAllUnitClass[i])
		{
			continue;
			Call_MsgBox(L"CPlayer : 기본 영웅 추가 안됐음");
			return E_FAIL;
		}

		CREATE_GAMEOBJECT(m_pAllUnitClass[i], GROUP_PLAYER);
		DISABLE_GAMEOBJECT(m_pAllUnitClass[i]);

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


	CallBack_CollisionEnter += bind(&CPlayer::Player_CollisionEnter, this, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
	CallBack_CollisionStay += bind(&CPlayer::Player_CollisionStay, this, placeholders::_1, placeholders::_2, placeholders::_3);
	CallBack_CollisionExit += bind(&CPlayer::Player_CollisionExit, this, placeholders::_1, placeholders::_2, placeholders::_3);


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

HRESULT CPlayer::SetUp_Collider()
{
	m_pSightRangeCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM, 
		m_pMyPlayerInfo->m_pPersonality->m_tPersonalDesc.fSIghtRadius, 
		COL_SIGHTRANGE, ZERO_VECTOR, DEFAULT_TRANS_MATRIX);

	if (!m_pSightRangeCollider)
		return E_FAIL;

	Add_Component(m_pSightRangeCollider);

	return S_OK;
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
	m_DeadLights.clear();
	m_pCurrentUnit->Enter_State((STATE_TYPE)m_iReserveStateDefault[m_eCurrentClass]);
	m_bDie = false;

	CUser::Get_Instance()->SetActive_HUD(true);
}

void CPlayer::Set_TeamType(eTEAM_TYPE eTeamType)
{
	m_eTeamType = eTeamType;
}

void CPlayer::Set_OutlineType(OUTLINETYPE eOutlineType)
{
	m_eOutlineType = eOutlineType;

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

	for (_uint i = 0; i < CLASS_END; ++i)
	{
		if (m_pAllUnitClass[i])
		{
			GET_COMPONENT_FROM(m_pAllUnitClass[i], CModel)->Set_OutlineFlag(vOutlineFlag);

		}
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

	if (nullptr != m_pCurrentUnit)
	{
		_float4 vUnitPosition = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
		m_pTransform->Set_World(WORLD_POS, vUnitPosition);
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
		for (_uint i = 0; i < CLASS_END; ++i)
		{
			if (m_pAllUnitClass[i])
			{
				GET_COMPONENT_FROM(m_pAllUnitClass[i], CModel)->Set_RimLightFlag(vRimLightFlag);

			}
		}
		cout << vRimLightFlag.w << endl;

	}

	if (KEY(DOWN, TAP))
	{
		vRimLightFlag.w -= 0.01f;
		for (_uint i = 0; i < CLASS_END; ++i)
		{
			if (m_pAllUnitClass[i])
			{
				GET_COMPONENT_FROM(m_pAllUnitClass[i], CModel)->Set_RimLightFlag(vRimLightFlag);

			}
		}
		cout << vRimLightFlag.w << endl;

	}
}

void CPlayer::Update_HP()
{
	CUser::Get_Instance()->Set_HP(m_pCurrentUnit->Get_Status().fHP, m_pCurrentUnit->Get_Status().fMaxHP);
}
void CPlayer::Update_HeroGauge()
{
	if (!CUser::Get_Instance()->Get_HUD(CUI_HUD::HUD_HeroGauge)->Is_Valid())
		return;

	if (m_bIsMainPlayer)
	{
		/*_bool IsHeroGaugeEnable = CUser::Get_Instance()->Is_OnHeroGauge();
		if (!IsHeroGaugeEnable)
			return;*/

		CUser::Get_Instance()->Set_HeroGauge(m_fGauge, m_fMaxGauge);
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
		CUser::Get_Instance()->Set_HeroPort(0);
		CUser::Get_Instance()->Turn_HeroGaugeFire(true);
	}
}

void CPlayer::On_FinishHero()
{
	m_fGauge = 0.f;
	m_bIsHero = false;

	Change_UnitClass(m_ePrevClass);
	CEffects_Factory::Get_Instance()->Create_MultiEffects(L"UnHenshin", m_pCurrentUnit, m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS));

	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Set_HUD(m_eCurrentClass);
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
	if (!m_pCurrentUnit->Is_Valid())
		return;

	_float fDis = CUtility_Transform::Get_FromCameraDistance(m_pCurrentUnit);
	if (fDis < m_fEnable_UnitHUDis)
	{
		m_pUnitHUD->Set_UnitDis(fDis);

		_float4 vPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
		vPos.y += 2.f;

		if (GAMEINSTANCE->isIn_Frustum_InWorldSpace(vPos.XMLoad(), 0.1f))
		{
			if (!m_pUnitHUD->Is_Valid())
			{
				ENABLE_GAMEOBJECT(m_pUnitHUD);
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


