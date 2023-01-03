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
#include "CUI_Popup.h"
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
#include "CUnit_Qanda.h"
#include "CUnit_Priest.h"
#include "CUnit_Lancer.h"

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

#include "CGameSystem.h"

#include "CPath.h"

#include "CUI_Trail.h"

#pragma region AI 추가용
#include "CAIController.h"
#include "CAIPersonality.h"
#include "CBehavior.h"
#pragma endregion AI 추가용




CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	m_DeadLights.clear();
	SAFE_DELETE(m_pCurPath);
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
		L"../bin/resources/meshes/Characters/Priest/Priest.fbx",
		L"../bin/resources/meshes/Characters/WarHammer/WarHammer.fbx",
		L"../bin/resources/meshes/Characters/Valkyrie/Valkyrie.fbx",
		L"../bin/resources/meshes/Characters/Qanda/Qanda.fbx",
		L"",
		L"../bin/resources/meshes/Characters/Lancer/Lancer.fbx",
	};

	wstring wstrModeFace[CLASS_END] =
	{
		L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00_50.fbx", // WARRIOR
		L"",
		L"../bin/resources/meshes/characters/Archer/Head/SK_Archer001_Face_A00_50.fbx",
		L"../bin/resources/meshes/characters/Paladin/Head/SK_Paladin0001_Face_A00_50.fbx",
		L"../bin/resources/meshes/Characters/Priest/Head/SK_Priest0000_Face_A00_30.fbx",
		L"../bin/resources/meshes/Characters/WarHammer/Head/SK_Engineer0001_Face_A00_50.fbx",
		L"../bin/resources/meshes/Characters/Valkyrie/Head/SK_Fiona0001_Face_A00_50.fbx",
		L"../bin/resources/meshes/Characters/Qanda/Head/SK_Qanda0001_Face_A00_50.fbx",
		L"",
		L"../bin/resources/meshes/Characters/Lancer/Head/SK_Lancer0000_Face_A00_20.fbx",
	};

	//L"../bin/resources/meshes/characters/Warrior/Head/SK_Warrior0001_Face_A00_50.fbx", // WARRIOR

	wstring wstrModeWeapon_L[CLASS_END] =
	{
	L"",
	L"",
	L"",
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



#ifdef WARRIOR_TH
	m_pAllUnitClass[WARRIOR] = CUnit_Warrior::Create(tModelData[WARRIOR]);
#endif

#ifdef ENGINEER_TH
	m_pAllUnitClass[ENGINEER] = CUnit_WarHammer::Create(tModelData[ENGINEER]);
#endif


#ifdef ARCHER_TH
	m_pAllUnitClass[ARCHER] = CUnit_Archer::Create(tModelData[ARCHER]);
#endif


#ifdef PALADIN_TH
	m_pAllUnitClass[PALADIN] = CUnit_Paladin::Create(tModelData[PALADIN]);
#endif	

#ifdef FIONA_TH
	m_pAllUnitClass[FIONA] = CUnit_Valkyrie::Create(tModelData[FIONA]);
#endif	

#ifdef QANDA_TH
	m_pAllUnitClass[QANDA] = CUnit_Qanda::Create(tModelData[QANDA]);
#endif	


#ifdef PRIEST_TH
	m_pAllUnitClass[PRIEST] = CUnit_Priest::Create(tModelData[PRIEST]);
#endif	

#ifdef LANCER_TH
	m_pAllUnitClass[LANCER] = CUnit_Lancer::Create(tModelData[LANCER]);
#endif	


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
			m_pAllUnitClass[i]->Set_OwnerPlayer(this);

			if (FAILED(m_pAllUnitClass[i]->Initialize()))
			{
				Call_MsgBox_Index(L"Failed to Initialize Unit / Index : ", i);
				return;

			}


			if (!m_pFollowCam)
			{
				Call_MsgBox(L"카메라 생성 안됐음");
				return;
			}

			m_pAllUnitClass[i]->Set_FollowCam(m_pFollowCam);
		}

	}



	/* 변신 Index */
	m_iChangeHeroAnimIndex[WARRIOR] = 62;
	m_iChangeHeroAnimIndex[PALADIN] = 41;
	m_iChangeHeroAnimIndex[ARCHER] = 53;
	m_iChangeHeroAnimIndex[ENGINEER] = 62;
	m_iChangeHeroAnimIndex[PRIEST] = 49;
}

void CPlayer::Player_CollisionEnter(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType, _float4 vHitPos)
{
	if (eMyColType == COL_REVIVE)
		return;

	if (!pOtherObj->Is_Valid() || m_pCurrentUnit == pOtherObj)
		return;

	m_pAIController->m_NearObjectList.push_back(pOtherObj);

}
void CPlayer::Player_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	if (eMyColType == COL_REVIVE)
		return;

	if (!pOtherObj->Is_Valid() || m_pCurrentUnit == pOtherObj)
		return;

	m_pAIController->m_NearObjectList.push_back(pOtherObj);
}
void CPlayer::Player_CollisionExit(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{

	if (eMyColType == COL_REVIVE)
		return;
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
		//m_pCurrentUnit->On_ChangeClass();
		DISABLE_GAMEOBJECT(m_pCurrentUnit);
	}

	if (m_eCurrentClass >= FIONA)
	{
		m_pCurrentUnit->Get_Status().fHP = m_pCurrentUnit->Get_Status().fMaxHP;
	}

	m_pCurrentUnit = m_pAllUnitClass[eClassType];
	ENABLE_GAMEOBJECT(m_pCurrentUnit);

	m_pFollowCam->Set_FollowTarget(m_pCurrentUnit);
	Set_Postion(vPos);
	m_pCurrentUnit->Get_Transform()->Set_Look(vLook);
	m_pCurrentUnit->Get_Transform()->Make_WorldMatrix();


	m_pCurrentUnit->Enter_State((STATE_TYPE)m_iReserveStateDefault[eClassType]);


	if (m_bIsMainPlayer)
	{
		GAMEINSTANCE->Stop_GrayScale();

		CUser::Get_Instance()->Set_HUD(m_eCurrentClass);
		CUser::Get_Instance()->Transform_SkillUI(m_eCurrentClass);

		if (m_eCurrentClass > ENGINEER)
		{
			CUser::Get_Instance()->Set_HeroPort(1);
		}
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
	m_bAbleRevival = false;
	m_fRevivalAcc = 0.f;
	Change_UnitClass(eClass);

	Set_Postion(vPos);

	m_pCurrentUnit->On_Respawn();

	if (!m_bIsMainPlayer)
	{
		ENABLE_GAMEOBJECT(m_pUnitHUD);

		if (Get_Team()->IsMainPlayerTeam())
		{
			m_pUnitHUD->Disable_RevivalUI();
		}

		//Path 갱신 + 캐릭터 재선택
		if (!m_bReborn)
		{
			m_pMyPlayerInfo->Choose_Character();

			if (m_bIsLeaderPlayer)
			{
				/*Set_NewPath(CGameSystem::Get_Instance()->Clone_RandomStartPath(m_pAIController, m_pMyTeam->Get_TeamType()));
				m_strStartPath = m_pCurPath->m_strName;*/

				m_strStartPath = (m_pMyTeam->Get_TeamType() == eTEAM_TYPE::eBLUE) ? ("Paden_BlueTeam_MainPath_0") : ("Paden_RedTeam_MainPath_0");
				Set_NewPath(CGameSystem::Get_Instance()->Clone_Path(m_strStartPath, m_pAIController));

			}
			else
			{
				Set_NewPath(CGameSystem::Get_Instance()->Clone_Path(m_pMySquad->Get_LeaderPlayer()->m_strStartPath, m_pAIController));
			}
		}
	}
	else
	{
		//메인플레이어 path에다가 트레일 UI 깔아보기
		if (m_pUI_Trail)
		{

			CPath* pPath = CGameSystem::Get_Instance()->Find_Path(m_strStartPath);


			if (pPath)
			{
				m_pUI_Trail->Clear_Nodes();
				vPos.y += 0.1f;
				m_pUI_Trail->Add_Node(vPos);


				vector<_float4>& vecPositions = pPath->Get_vecPositions();

				_float4 vLook = vecPositions.front() - vPos;

				m_pCurrentUnit->Get_Transform()->Set_Look(vLook);
				m_pCurrentUnit->Get_Transform()->Make_WorldMatrix();
				m_pFollowCam->Get_Transform()->Set_Look(vLook);
				m_pFollowCam->Get_Transform()->Make_WorldMatrix();

				for (auto& elem : vecPositions)
				{
					_float4 vNode = elem;
					vNode.y += 0.1f;
					m_pUI_Trail->Add_Node(vNode);

				}

				ENABLE_GAMEOBJECT(m_pUI_Trail);
				m_pUI_Trail->ReMap_TrailBuffers();
			}

		}
	}


	m_bReborn = false;

	for (auto& elem : m_DeadLights)
	{
		static_cast<CRectEffects*>(elem)->Set_LoopControlfalse();
	}

	if (m_pReviveCollider)
		DISABLE_COMPONENT(m_pReviveCollider);
}

_float4 CPlayer::Get_WorldPos()
{
	return m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
}

void CPlayer::Reserve_State(_uint eState)
{
	m_pCurrentUnit->Reserve_State(STATE_TYPE(eState));

}

void CPlayer::Set_Unit_ReserveState(_uint eClassType, _uint eState)
{
	m_iReserveStateDefault[eClassType] = eState;
}

void CPlayer::SetUp_ReserveState()
{

	CUnit::UNIT_TYPE m_eType = CUnit::UNIT_TYPE(m_iUnitType);

	for (int i = 0; i < CLASS_END; ++i)
	{
		if (m_pAllUnitClass[i] == nullptr)
			continue;

		m_pAllUnitClass[i]->SetUp_ReserveState(m_eType);
	}


	switch (m_eType)
	{

	case CUnit::UNIT_TYPE::ePlayer:

		m_iReserveStateDefault[WARRIOR] = STATE_IDLE_PLAYER_R;
		m_iReserveStateDefault[ARCHER] = STATE_IDLE_ARCHER_R;
		m_iReserveStateDefault[ENGINEER] = STATE_IDLE_WARHAMMER_R;
		m_iReserveStateDefault[PALADIN] = STATE_IDLE_PALADIN_R;
		m_iReserveStateDefault[FIONA] = STATE_IDLE_VALKYRIE_R;
		m_iReserveStateDefault[QANDA] = STATE_IDLE_QANDA;
		m_iReserveStateDefault[LANCER] = STATE_IDLE_LANCER;
		m_iReserveStateDefault[PRIEST] = STATE_IDLE_PRIEST;
		break;

	case CUnit::UNIT_TYPE::eAI_Default:

		m_iReserveStateDefault[WARRIOR] = AI_STATE_PATROL_DEFAULT_WARRIOR_R;
		//	m_iReserveStateDefault[ARCHER] = AI_STATE_PATROL_DEFAULT_ARCHER_R;
		m_iReserveStateDefault[ENGINEER] = AI_STATE_PATROL_DEFAULT_ENGINEER_R;
		m_iReserveStateDefault[FIONA] = AI_STATE_PATROL_DEFAULT_FIONA_R;
		m_iReserveStateDefault[PALADIN] = AI_STATE_PATROL_DEFAULT_PALADIN_R;

		break;

	case CUnit::UNIT_TYPE::eAI_idiot:

		m_iReserveStateDefault[WARRIOR] = AI_STATE_COMMON_HIT_WARRIOR;
		//	m_iReserveStateDefault[ARCHER] = AI_STATE_PATROL_DEFAULT_ARCHER_R;
		m_iReserveStateDefault[ENGINEER] = AI_STATE_COMMON_HIT_WARRIOR;
		m_iReserveStateDefault[FIONA] = AI_STATE_COMMON_HIT_WARRIOR;
		m_iReserveStateDefault[PALADIN] = AI_STATE_COMMON_HIT_WARRIOR;

		break;


	default:
		break;
	}

}

void CPlayer::SetUp_UnitColliders(_bool bBlueTeam)
{
	for (int i = 0; i < CLASS_END; ++i)
	{
		if (m_pAllUnitClass[i] == nullptr)
			continue;

		m_pAllUnitClass[i]->SetUp_Colliders(bBlueTeam);
	}

	SetUp_ReserveState();
}


void CPlayer::SetUp_UnitHitStates()
{
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

	m_bIsMainPlayer = m_pMyPlayerInfo->m_bIsMainPlayer;

#pragma region AI컴포넌트 추가용 구문
	if (nullptr != m_pMyPlayerInfo->m_pPersonality)
	{
		if (m_pMyPlayerInfo->m_bIsMainPlayer)
			assert(0);//메인플레이어는 Personality가 할당되면 안됩니다

		CAIController* pAIComponent = CAIController::Create(m_pMyPlayerInfo->m_pPersonality);

		if (!pAIComponent)
			return E_FAIL;

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
		m_eCurrentClass = ARCHER;
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

	if (FAILED(SetUp_Collider()))
		return E_FAIL;

	/* UI_TRAIL */
	if (m_bIsMainPlayer)
	{
		CUI_Trail* pUI_Trail = CUI_Trail::Create(CP_BEFORE_RENDERER, 100, 0.35f, -0.05f, 100.f, ZERO_VECTOR, _float4(0.2f, 1.f, 0.2f, 1.f),
			L"../bin/resources/textures/effects/warhaven/texture/T_ArrowUI_01_FX.dds",
			L"../bin/resources/textures/White.png"
		);

		if (!pUI_Trail)
			return E_FAIL;

		m_pUI_Trail = pUI_Trail;
	}

	return S_OK;
}


HRESULT CPlayer::Initialize()
{

	return S_OK;
}

HRESULT CPlayer::Start()
{
	__super::Start();


	SetUp_ReserveState();
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

	// 미니맵에 트랜스폼 할당
	CUser::Get_Instance()->Set_MiniMapPlayer(this);
	CUser::Get_Instance()->Set_OperPlayer(this);

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


	if (m_pUI_Trail)
	{
		CREATE_GAMEOBJECT(m_pUI_Trail, GROUP_EFFECT);
		DISABLE_GAMEOBJECT(m_pUI_Trail);
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

HRESULT CPlayer::SetUp_Collider()
{
	m_pReviveCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM,
		1.5f,
		COL_REVIVE, _float4(0.f, 0.5f, 0.f), DEFAULT_TRANS_MATRIX);

	if (!m_pReviveCollider)
		return E_FAIL;

	Add_Component(m_pReviveCollider);

	if (m_bIsMainPlayer)
		return S_OK;

	if (!m_pMyPlayerInfo->m_pPersonality)
		return S_OK;

	m_pSightRangeCollider = CCollider_Sphere::Create(CP_AFTER_TRANSFORM,
		m_pMyPlayerInfo->m_pPersonality->m_tPersonalDesc.tPersonalityData.fSIghtRadius,
		COL_SIGHTRANGE, ZERO_VECTOR, DEFAULT_TRANS_MATRIX);

	if (!m_pSightRangeCollider)
		return E_FAIL;

	Add_Component(m_pSightRangeCollider);



	return S_OK;
}

CAIPersonality* CPlayer::Get_Personality()
{
	if (nullptr == m_pMyPlayerInfo)
		return nullptr;
	if (nullptr == m_pMyPlayerInfo->m_pPersonality)
		return nullptr;
	return m_pMyPlayerInfo->m_pPersonality;
}

void CPlayer::Set_Personality(CAIPersonality* pPersonality)
{
	if (nullptr != m_pAIController)
	{
		if (nullptr != pPersonality)
		{
			m_pMyPlayerInfo->Set_Personality(pPersonality);
			m_pAIController->Set_Personality(pPersonality);
		}
	}//Personality가 필요한 애들만.. --> AIController가 null이다 == AI가 아니거나 State에 의존하는 AI다
}

CBehavior* CPlayer::Get_CurBehavior()
{
	if (nullptr == m_pAIController)
		return nullptr;
	CBehavior* pBehavior = m_pAIController->Get_CurBehavior();
	return pBehavior;


}

void CPlayer::On_Die()
{
	//m_bDie = true;
	DISABLE_GAMEOBJECT(m_pUnitHUD);
	m_bDieDelay = true;

	if (!m_bIsMainPlayer)
	{
		if (!Get_Team())
			return;
	}

	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->SetActive_HUD(false);
	}
}

_bool CPlayer::Is_EndRevivalTime()
{
	return (m_fRevivalAcc >= m_fMaxRevivalTime);
}

_bool	CPlayer::Is_Died()
{
	return (m_bDie || m_bDieDelay);
}

_bool CPlayer::Is_AbleRevival()
{
	return m_bAbleRevival;
}

void CPlayer::On_RealDie()
{
	/* 이 함수가 소생 이펙트 켜지는 곳임 */
	if (m_pReviveCollider)
		ENABLE_COMPONENT(m_pReviveCollider);

	//소생 
	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Toggle_DeadUI(true, true);
	}
	else
	{
		if (!Get_Team())
			return;

		if (Get_Team()->IsMainPlayerTeam())
		{
			m_pUnitHUD->Enable_RevivalUI();
		}
	}

	m_bDieDelay = false;
	m_fDieDelayAcc = 0.f;
	m_bDie = true;
	m_bAbleRevival = true;

	m_DeadLights.clear();

	if (m_pMyTeam)
	{
		if (m_pMyTeam->IsMainPlayerTeam())
		{
			m_DeadLights = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"DeadLight", m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS));
			m_pCurrentUnit->Create_Light(m_DeadLights.back(), _float4(0.f, 0.5f, 0.f), 5.f, 0.f, 0.5f, 0.f, 0.5f, RGB(255, 160, 50), true);
		}
	}
	else if (m_bIsMainPlayer)
	{
		m_DeadLights = CEffects_Factory::Get_Instance()->Create_MultiEffects(L"DeadLight", m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS));
		m_pCurrentUnit->Create_Light(m_DeadLights.back(), _float4(0.f, 0.5f, 0.f), 5.f, 0.f, 0.5f, 0.f, 0.5f, RGB(255, 160, 50), true);
	}
}

void CPlayer::On_Reborn()
{
	if (m_bIsMainPlayer)
		GAMEINSTANCE->Stop_GrayScale();

	m_bAbleRevival = false;
	m_fRevivalAcc = 0.f;

	for (auto& elem : m_DeadLights)
	{
		static_cast<CRectEffects*>(elem)->Set_LoopControlfalse();
	}
	m_DeadLights.clear();
	m_pCurrentUnit->Enter_State((STATE_TYPE)m_iReserveStateDefault[m_eCurrentClass]);
	m_bDie = false;

	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Toggle_DeadUI(false, false);
		CUser::Get_Instance()->SetActive_HUD(true);
		CUser::Get_Instance()->SetActive_SquardInfo(true);
	}
	else
	{
		if (Get_Team()->IsMainPlayerTeam())
		{
			m_pUnitHUD->Disable_RevivalUI();
		}
	}

	m_bReborn = true;
}

void CPlayer::Start_Reborn()
{
	m_bAbleRevival = false;
	m_fRevivalAcc = 0.f;
}

void CPlayer::On_PlusGauge(_float fGauge)
{
	m_fGauge += fGauge;

	if (m_fGauge >= m_fMaxGauge)
		m_fGauge = m_fMaxGauge;
}

void CPlayer::SetActive_UnitHUD(_bool value)
{
	m_pUnitHUD->SetActive(value);
}

void CPlayer::On_RealChangeBehavior()
{
	m_pCurBehaviorDesc = m_pReserveBehaviorDesc;

	switch (m_pCurBehaviorDesc->eCurType)
	{
	case eBehaviorType::eAttack:
		m_pTargetPlayer = m_pCurBehaviorDesc->pEnemyPlayer;
		break;
	default:
		break;
	}
}

void CPlayer::On_FinishGame(CTeamConnector* pLoseTeam)
{
	/* 상태 접근 */
	m_pCurrentUnit->On_FinishGame((m_pMyTeam == pLoseTeam) ? false : true);

	if (m_pAIController)
		DISABLE_COMPONENT(m_pAIController);

}

void CPlayer::On_ScoreKDA_Kill(CPlayer* pOtherPlayer)
{
	m_fKillStreakTimeAcc = 0.f;

	m_tKdaStat.iCurKillCount++;
	m_tKdaStat.iKillStreak++;
	m_tKdaStat.iTotalKillCount++;

	if (m_bIsMainPlayer)
	{
		if (pOtherPlayer->Get_PlayerName() == L"Jusin_Burger")
		{
			CUser::Get_Instance()->Enable_Popup(CUI_Popup::eBURGERKING);
		}
		else if (m_tKdaStat.iHeadShotKillCount == 3)
		{
			CUser::Get_Instance()->Enable_Popup(CUI_Popup::eHEADHUNTER);
		}
		else if (pOtherPlayer->Get_CurClass() >= CLASS_TYPE::FIONA)
		{
			CUser::Get_Instance()->Enable_Popup(CUI_Popup::eKILLELITE);
		}
		else if (m_tKdaStat.iKillStreak == 2)
		{
			CUser::Get_Instance()->Enable_Popup(CUI_Popup::eKILL);
		}
		else if (m_tKdaStat.iKillStreak == 3)
		{
			CUser::Get_Instance()->Enable_Popup(CUI_Popup::eKILL2);
		}
		else if (m_tKdaStat.iKillStreak == 4)
		{
			CUser::Get_Instance()->Enable_Popup(CUI_Popup::eKILL3);
		}
		else if (m_tKdaStat.iKillStreak >= 5)
		{
			CUser::Get_Instance()->Enable_Popup(CUI_Popup::eKILL4);
		}
	}
}

void CPlayer::Change_NearPath()
{
	_float4 vUnitPosition = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
	CPath* pPath = CGameSystem::Get_Instance()->Get_NearPath(vUnitPosition);
	if (nullptr == pPath)
		return;
	Set_NewPath(pPath->Clone());
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
	if (!m_bIsMainPlayer)
	{
		if (KEY(J, TAP))
		{
			m_pCurrentUnit->Get_Status().fHP = 0.f;
			m_pCurrentUnit->Teleport_Unit(_float4(0.f, 20.f, 0.f));
		}
	}

	if (m_bIsBattle)
	{
		m_fBattlAccTime += fDT(0);

		if (m_fBattlAccTime > m_fMaxBattlTime)
		{
			m_fBattlAccTime = 0.f;
			m_bIsBattle = false;
		}
	}

	//공통으로 업데이트 되어야 하는것

	m_pUnitHUD->Set_UnitStatus(m_pCurrentUnit->Get_Status());

	Update_HeroGauge();

	Update_DieDelay();

	Check_AbleRevival();

	Update_KDA();

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

}

void CPlayer::Update_HP()
{
	CUser::Get_Instance()->Set_HP(m_pCurrentUnit->Get_Status().fHP, m_pCurrentUnit->Get_Status().fMaxHP);
}

void CPlayer::On_ChangeBehavior(BEHAVIOR_DESC* pBehaviorDesc)
{
	m_pCurrentUnit->On_ChangeBehavior(pBehaviorDesc);
}

void CPlayer::Set_NewPath(CPath* pPath)
{
	SAFE_DELETE(m_pCurPath);
	m_pCurPath = pPath;
	if (m_pCurPath)
		m_pCurPath->Init_Indices();

	m_pCurPath->m_vPrevPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
}


_float4 CPlayer::Get_LookDir()
{
	return m_pCurrentUnit->Get_Transform()->Get_World(WORLD_LOOK);
}

void CPlayer::Set_MainPlayerStartPath(_uint iTriggerType)
{
	switch (iTriggerType)
	{
	case 0:
		m_strStartPath = (m_pMyTeam->Get_TeamType() == eTEAM_TYPE::eBLUE) ? ("Paden_BlueTeam_MainPath_0") : ("Paden_RedTeam_MainPath_0");
		break;
	case 1:
		m_strStartPath = (m_pMyTeam->Get_TeamType() == eTEAM_TYPE::eBLUE) ? ("Paden_BlueTeam_Respawn_1") : ("Paden_RedTeam_Respawn_1");
		break;
	case 2:
		m_strStartPath = (m_pMyTeam->Get_TeamType() == eTEAM_TYPE::eBLUE) ? ("Paden_BlueTeam_Cannon_0") : ("Paden_RedTeam_Cannon_0");
		break;
	default:
		break;
	}

}

_float4 CPlayer::Get_SquadDir()
{
	_float4 vDir = m_pMySquad->Get_LeaderPlayer()->Get_LookDir();
	//스쿼드 위치를 가져와서 현재 월드 포지션이랑 뺸 뒤의 방향을 던져주는 식으로..
	return vDir;
}

CPath* CPlayer::Get_CurPath()
{
	return m_pCurPath;
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

		_float fGaugeSpeed = fDT(0) * 0.1f;

		if (m_bIsMainPlayer)
		{
			if (CUser::Get_Instance()->Get_CurLevel() == LEVEL_TEST)
				fGaugeSpeed *= 200.f;
			else
				fGaugeSpeed *= 5.f;

		}
		if (!m_bIsHero) //CChangeHero_Player
		{
			if (m_bAlive)
				m_fGauge += fGaugeSpeed;

			if (m_fGauge > m_fMaxGauge)
			{
				m_fGauge = m_fMaxGauge;

				On_AbleHero();
			}

		}
		else //변신 중일때 
		{
			m_fGauge -= fDT(0) * 2.f; // 인게임속도2.f 

			if (m_bIsMainPlayer)
			{
				On_FinishHero_KeyInput();
			}
			else if (0 >= m_fGauge)
			{
				On_FinishHero();
			}
		}
	}
}

void CPlayer::Update_KDA()
{
	if (m_tKdaStat.iKillStreak > 0)
	{
		m_fKillStreakTimeAcc += fDT(0);

		if (m_fKillStreakTimeAcc >= m_fKillStreakTime)
		{
			m_fKillStreakTimeAcc = 0.f;
			m_tKdaStat.iKillStreak = 0;
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
		CUser::Get_Instance()->Turn_AbleHeroFire();
		CUser::Get_Instance()->SetActive_AbleHeroText(true);
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

	else if (0 >= m_fGauge)
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

void CPlayer::Check_AbleRevival()
{
	if (!m_pAIController && !m_bIsMainPlayer)
	{
		return;
	}

	if (m_bAbleRevival)
	{
		m_fRevivalAcc += fDT(0);
		if (m_fRevivalAcc >= m_fMaxRevivalTime)
		{
			/*if (m_bIsLeaderPlayer)
				Set_NewPath(CGameSystem::Get_Instance()->Clone_RandomStartPath(m_pAIController, m_pMyTeam->Get_TeamType()));
			else
			{
				CPath* pPath = m_pMySquad->Get_LeaderPlayer()->Get_CurPath();

				if (nullptr == pPath)
				{
					pPath = CGameSystem::Get_Instance()->Clone_RandomStartPath(m_pAIController, m_pMyTeam->Get_TeamType());
					Set_NewPath(pPath);
				}
				else
					Set_NewPath(pPath->Clone());


			}*/
			if (!m_bIsMainPlayer)
			{
				_float4 vStartPos = m_pMyTeam->Find_RespawnPosition_Start();
				Respawn_Unit(vStartPos, m_eCurrentClass);
			}



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


