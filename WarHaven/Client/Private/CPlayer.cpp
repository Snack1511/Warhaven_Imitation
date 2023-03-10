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
#include "Loading_Manager.h"

#include "CCannon.h"

#include "CTeamConnector.h"
#include "CUI_Popup.h"
#include "CScript_FollowCam.h"

#include "MeshContainer.h"
#include "CUI_UnitHP.h"
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

#include "CUI_HUD.h"
#include "CUI_Skill.h"
#include "CCannonBall.h"
#include "CUI_ScoreInfo.h"

#pragma region AI 추가용
#include "CAIController.h"
#include "CAIPersonality.h"
#include "CBehavior.h"
#pragma endregion AI 추가용
#include "CDebugObject.h"

#include "CTrigger.h"



CPlayer::CPlayer()
{
}

CPlayer::~CPlayer()
{
	m_DeadLights.clear();
	if (m_pCurPath == m_pStartMainPath)
	{
		SAFE_DELETE(m_pCurPath);
		m_pStartMainPath = nullptr;
	}
	SAFE_DELETE(m_pCurPath);
	SAFE_DELETE(m_pStartMainPath);

#ifdef _DEBUG
	Clear_DebugObject();
#endif
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
		L""
	};


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

	if (!pOtherObj->Is_Valid() || m_pCurrentUnit == pOtherObj || pOtherObj == this)
		return;

	m_pAIController->m_NearObjectList.push_back(pOtherObj);

}
void CPlayer::Player_CollisionStay(CGameObject* pOtherObj, const _uint& eOtherColType, const _uint& eMyColType)
{
	if (eMyColType == COL_REVIVE)
		return;

	if (!pOtherObj->Is_Valid() || m_pCurrentUnit == pOtherObj || pOtherObj == this)
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
	CUnit* pUnit = m_pAllUnitClass[eClassType];

	if (nullptr != pUnit)
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
		m_pCurrentUnit = pUnit;

	


		ENABLE_GAMEOBJECT(pUnit);

		m_pFollowCam->Set_FollowTarget(pUnit);
		Set_Postion(vPos);
		pUnit->Get_Transform()->Set_Look(vLook);
		pUnit->Get_Transform()->Make_WorldMatrix();

		if (m_bIsMainPlayer)
			m_pCurrentUnit->Lerp_Camera(CScript_FollowCam::CAMERA_LERP_DEFAULT);

		pUnit->Enter_State((STATE_TYPE)m_iReserveStateDefault[eClassType]);


		if (m_eCurrentClass >= FIONA)
		{
			m_pCurrentUnit->Get_Status().fHP = m_pCurrentUnit->Get_Status().fMaxHP;
			CFunctor::Play_Sound(L"Effect_ChangeHeroBoom", CHANNEL_EFFECTS, Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS), 1.f);
			CFunctor::Play_Sound(L"Effect_ChangeHero", CHANNEL_EFFECTS, Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS), 1.f);
		}
		else
		{
			CFunctor::Play_Sound(L"Effect_UnchangeHero", CHANNEL_EFFECTS, Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS), 1.f);
		}

	}
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
			//m_pMyPlayerInfo->Choose_Character();

			if (m_bIsLeaderPlayer)
			{
				CPath* pNewPath = nullptr;

				if (!m_bFirstPath && (CUser::Get_Instance()->Get_CurLevel() == LEVEL_PADEN))
				{
					m_strStartPath = (m_pMyTeam->Get_TeamType() == eTEAM_TYPE::eBLUE) ? ("Paden_BlueTeam_Cannon_0") : ("Paden_RedTeam_Cannon_0");
					pNewPath = CGameSystem::Get_Instance()->Clone_Path(m_strStartPath, m_pAIController);
				}
				else	// 1. respawn 거점 먹었으면 respawn에서
				if (m_pMyTeam->Has_RespawnTrigger())
				{
					pNewPath = CGameSystem::Get_Instance()->Clone_RandomRespawnPath(m_pAIController, m_pMyTeam->Get_TeamType());
				}
				/*else if (m_pMyTeam->Has_CenterTrigger())
				{
					pNewPath = CGameSystem::Get_Instance()->Clone_CenterPath(m_pAIController, m_pMyTeam->Get_TeamType());
				}*/
				else
				{
					pNewPath = CGameSystem::Get_Instance()->Clone_RandomStartPath(m_pAIController, m_pMyTeam->Get_TeamType());
				}

				Set_NewMainPath(pNewPath);
				if (m_pStartMainPath)
					m_strStartPath = m_pStartMainPath->m_strName;

				/*무조건 중앙으로 모이게 하는 코드*/
				//m_strStartPath = (m_pMyTeam->Get_TeamType() == eTEAM_TYPE::eBLUE) ? ("Paden_BlueTeam_MainPath_0") : ("Paden_RedTeam_MainPath_0");
				//Set_NewPath(CGameSystem::Get_Instance()->Clone_Path(m_strStartPath, m_pAIController));
				m_bFirstPath = true;

			}
			else
			{
				CPath* pNewPath = nullptr;
				if (m_pMyTeam->Has_RespawnTrigger())
				{
					pNewPath = CGameSystem::Get_Instance()->Clone_RandomRespawnPath(m_pAIController, m_pMyTeam->Get_TeamType());
				}
				else
				{
					pNewPath = CGameSystem::Get_Instance()->Clone_Path(m_pMySquad->Get_LeaderPlayer()->m_strStartPath, m_pAIController);
				}
				Set_NewMainPath(pNewPath);
			}
		}
	}
	else
	{
		//메인플레이어 path에다가 트레일 UI 깔아보기
		if (m_pUI_Trail)
		{
			if (m_strStartPath.empty())
			{
				DISABLE_GAMEOBJECT(m_pUI_Trail);
			}
			else
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
	if (!m_pCurrentUnit)
		return ZERO_VECTOR;

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
		m_iReserveStateDefault[PALADIN] = STATE_IDLE_PALADIN_R;
		m_iReserveStateDefault[PRIEST] = STATE_IDLE_PRIEST;
		m_iReserveStateDefault[ENGINEER] = STATE_IDLE_WARHAMMER_R;
		m_iReserveStateDefault[FIONA] = STATE_IDLE_VALKYRIE_R;
		m_iReserveStateDefault[QANDA] = STATE_IDLE_QANDA;
		m_iReserveStateDefault[LANCER] = STATE_IDLE_LANCER;
		break;

	case CUnit::UNIT_TYPE::eAI_Default:

		m_iReserveStateDefault[WARRIOR] = AI_STATE_PATROL_DEFAULT_WARRIOR_R;
		m_iReserveStateDefault[ARCHER] = AI_STATE_PATROL_DEFAULT_ARCHER_R;
		m_iReserveStateDefault[PALADIN] = AI_STATE_PATROL_DEFAULT_PALADIN_R;
		m_iReserveStateDefault[PRIEST] = AI_STATE_PATROL_DEFAULT_PRIEST;
		m_iReserveStateDefault[ENGINEER] = AI_STATE_PATROL_DEFAULT_ENGINEER_R;
		m_iReserveStateDefault[FIONA] = AI_STATE_COMBAT_DEFAULT_FIONA_R;
		m_iReserveStateDefault[QANDA] = AI_STATE_PATROL_DEFAULT_QANDA; // 공격상태로 바꿔야함.
		m_iReserveStateDefault[LANCER] = AI_STATE_PATROL_DEFAULT_LANCER;

		break;

	case CUnit::UNIT_TYPE::eAI_idiot:

		m_iReserveStateDefault[WARRIOR] = AI_STATE_COMMON_HIT_WARRIOR;
		m_iReserveStateDefault[ARCHER] = AI_STATE_COMMON_HIT_WARRIOR;
		m_iReserveStateDefault[ENGINEER] = AI_STATE_COMMON_HIT_WARRIOR;
		m_iReserveStateDefault[FIONA] = AI_STATE_COMMON_HIT_WARRIOR;
		m_iReserveStateDefault[PALADIN] = AI_STATE_COMMON_HIT_WARRIOR;
		m_iReserveStateDefault[PRIEST] = AI_STATE_COMMON_HIT_WARRIOR;

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

	m_pFollowCam->Set_MainCam();

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
		m_eCurrentClass = PALADIN;
		break;
	case Client::PRIEST:
		m_eCurrentClass = PRIEST;
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

	if (!m_bIsMainPlayer)
		Create_UnitHUD();

	if (FAILED(SetUp_Collider()))
		return E_FAIL;

	/* UI_TRAIL */
	if (m_bIsMainPlayer)
	{


		CUI_Trail* pUI_Trail = CUI_Trail::Create(CP_BEFORE_RENDERER, 100, 0.35f, -0.035f, 70.f, ZERO_VECTOR, _float4(0.2f, 1.f, 0.2f, 1.f),
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

	if (m_bIsMainPlayer)
	{
		CUser::Get_Instance()->Set_ScoreBoardPlayer(this);
	}

	if (!m_pScoreInfo)
	{
		LEVEL_TYPE_CLIENT eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();
		if (eLoadLevel >= LEVEL_PADEN)
		{
			m_pScoreInfo = CUI_ScoreInfo::Create();
			m_pScoreInfo->Set_Player(this);

			CUser::Get_Instance()->Get_ScoreInfo(this);

			CREATE_GAMEOBJECT(m_pScoreInfo, GROUP_UI);
			DISABLE_GAMEOBJECT(m_pScoreInfo);
		}
	}

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
	if (m_pUnitHUD)
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

CPlayer* CPlayer::Get_TargetPlayer(eTargetPlayerType eType)
{
	if (!m_pAIController)
		return m_pTargetPlayer;

	if (nullptr == m_pCurBehaviorDesc)
		return nullptr;


	/*switch (eType)
	{
	case eTargetPlayerType::eEnemy:
		return m_pCurBehaviorDesc->pEnemyPlayer;
	case eTargetPlayerType::eAllies:
		return m_pCurBehaviorDesc->pAlliesPlayer;
	}*/

	return m_pTargetPlayer;

}

_float4 CPlayer::Get_TargetObjPos()
{
	if (!m_pTargetObj)
		return ZERO_VECTOR;

	if (dynamic_cast<CPlayer*>(m_pTargetObj))
		return static_cast<CPlayer*>(m_pTargetObj)->Get_WorldPos();

	return m_pTargetObj->Get_Transform()->Get_World(WORLD_POS);
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
			m_pUnitHUD->Enable_RevivalUI();
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

	CUser::Get_Instance()->SetActive_HUD_RevivalUI(false);


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
			m_pUnitHUD->Disable_RevivalUI();
	}

	m_bReborn = true;
}

void CPlayer::Start_Reborn()
{
	
	CFunctor::Play_Sound(L"Effect_Revive", CHANNEL_EFFECTS, 1.f);
	m_bAbleRevival = false;
	m_fRevivalAcc = 0.f;
}

void CPlayer::On_PlusGauge(_float fGauge)
{
	if (m_bIsHero)
		return;

	m_fGauge += fGauge;

	if (m_fGauge >= m_fMaxGauge)
		m_fGauge = m_fMaxGauge;
}

void CPlayer::SetActive_UnitHUD(_bool value)
{
	if (m_pUnitHUD)
		m_pUnitHUD->SetActive(value);
}

void CPlayer::On_RealChangeBehavior()
{
	m_pCurBehaviorDesc = m_pReserveBehaviorDesc;

	if (!m_pCurBehaviorDesc)
		return;

	switch (m_pCurBehaviorDesc->eCurType)
	{
	case eBehaviorType::eCombat:
		m_pTargetPlayer = m_pCurBehaviorDesc->pEnemyPlayer;
		m_bKeepRay = true;

		break;
	case eBehaviorType::eRevive:
		m_pTargetPlayer = m_pCurBehaviorDesc->pAlliesPlayer;

		break;

	case eBehaviorType::eGliding:
		//m_pTargetObj = m_pCurBehaviorDesc->pTriggerPtr;

		break;
	case eBehaviorType::ePadenCannonInteract:
		m_pTargetObj = m_pCurBehaviorDesc->pNearCannon;
		break;

	case eBehaviorType::ePathFinding:
		m_bKeepRay = false;
		break;

	case eBehaviorType::eCatchCannon:
		m_bKeepRay = false;
		m_pTargetObj = m_pCurBehaviorDesc->pCannonBall;

		break;

	default:
		m_bKeepRay = false;

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

	if (CLoading_Manager::Get_Instance()->Get_LoadLevel() >= LEVEL_PADEN)
	{
		m_pScoreInfo->Update_KillCnt(m_tKdaStat.iTotalKillCount);
		CUser::Get_Instance()->Sort_ScoreInfo();
	}

	if (m_bIsMainPlayer)
	{
		GAMEINSTANCE->Stop_Sound((CHANNEL_GROUP)CHANNEL_VOICE);
		m_pCurrentUnit->Play_Voice(L"Voice_Kill", 1.f, false);

		if (pOtherPlayer->Get_PlayerInfo()->Is_Burger())
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

void CPlayer::On_ScoreKDA_Death()
{
	m_tKdaStat.iDeathCount++;

	m_pScoreInfo->Update_DeathCont(m_tKdaStat.iDeathCount);
	CUser::Get_Instance()->Sort_ScoreInfo();
}

void CPlayer::Change_NearPath()
{
	_float4 vUnitPosition = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
	CPath* pPath = CGameSystem::Get_Instance()->Get_NearPath(vUnitPosition);
	if (nullptr == pPath)
		return;
	Set_NewPath(pPath->Clone());

	Make_BestRoute(m_pCurPath->Get_vecPositions()[0]);

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
	if (!m_pCurrentUnit)
		return;

	if (!m_bIsMainPlayer)
		Update_TargetLock();
	// 행복버튼~ 즐코
	//if (!m_bIsMainPlayer)
	//{
	//	if (KEY(J, TAP))
	//	{
	//		m_pCurrentUnit->Get_Status().fHP = 0.f;
	//		m_pCurrentUnit->Teleport_Unit(_float4(0.f, 20.f, 0.f));
	//	}
	//}

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

	if (m_pUnitHUD && m_pCurrentUnit)
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

	if (!m_pCurrentUnit)
		return;
	

	_float4 vPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
	vPos.y += 2.f;
	m_bIsInFrustum = GAMEINSTANCE->isIn_Frustum_InWorldSpace(vPos.XMLoad(), 0.3f);

	if (m_pCurrentUnit->Get_Status().fHP > 0.f)
	{
		if (m_pUnitHUD)
		{
			Frustum_UnitHUD();
			TransformProjection();
		}
	}

	if (!m_bIsInFrustum)
	{
		if (m_pUnitHUD)
		{
			m_pUnitHUD->Disable_RevivalUI();
			m_pUnitHUD->Get_UnitHP()->SetActive(false);
		}
	}
	else
	{
		if (!Get_Team())
			return;

		if (Get_Team()->IsMainPlayerTeam())
		{
			if (m_bAbleRevival)
			{
				if (m_pUnitHUD)
					m_pUnitHUD->Enable_RevivalUI();
			}
		}
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
	if (m_pCurrentUnit)
		m_pCurrentUnit->On_ChangeBehavior(pBehaviorDesc);
}

void CPlayer::Set_NewPath(CPath* pPath)
{
	if (m_pStartMainPath)
	{
		if (m_pStartMainPath == m_pCurPath)
		{
			m_pCurPath = nullptr;
		}
	}

	SAFE_DELETE(m_pCurPath);
	m_pCurPath = pPath;
	if (m_pCurPath)
		m_pCurPath->Init_Indices();

	if (m_pCurPath)
	m_pCurPath->m_vPrevPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
}

void CPlayer::Set_NewMainPath(CPath* pPath)
{
	if (m_pStartMainPath == m_pCurPath)
	{
		SAFE_DELETE(m_pCurPath);
		m_pStartMainPath = nullptr;
		m_pCurPath = nullptr;
	}

	SAFE_DELETE(m_pCurPath);
	SAFE_DELETE(m_pStartMainPath);
	m_pStartMainPath = pPath;
	if (m_pStartMainPath)
		m_pStartMainPath->Init_Indices();

	m_pStartMainPath->m_vPrevPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);


	m_pCurPath = m_pStartMainPath;

}

_float4 CPlayer::Get_LookDir()
{
	return m_pCurrentUnit->Get_Transform()->Get_World(WORLD_LOOK);
}

void CPlayer::Set_MainPlayerStartPath(_uint iTriggerType)
{
	if (CGameSystem::Get_Instance()->m_eCurStageType == CGameSystem::eSTAGE_PADEN)
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
	else // Hwara
	{
		switch (iTriggerType)
		{
		case 0:
			m_strStartPath = (m_pMyTeam->Get_TeamType() == eTEAM_TYPE::eBLUE) ? ("Hwara_BlueTeam_ToCenter_0") : ("Hwara_RedTeam_ToCenter_0");
			break;
		case 1:
			m_strStartPath = (m_pMyTeam->Get_TeamType() == eTEAM_TYPE::eBLUE) ? ("Hwara_BlueTeam_ToRespawn_0") : ("Hwara_RedTeam_ToRespawn_0");
			break;
		default:
			break;
		}
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
		if (KEY(O, TAP))
		{
			m_fGauge = 100.f;
		}

		CUser::Get_Instance()->Set_HeroGauge(m_fGauge, m_fMaxGauge);
	}

	if (!m_bAbleHero) //CChangeHero_Player, HUD
	{

		_float fGaugeSpeed = fDT(0) * 0.1f;

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
			else if (0.f >= m_fGauge)
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
			m_bKillVoice = true;
		}
	}
}
#ifdef _DEBUG
void CPlayer::Add_DebugObject(_float4 vPosition)
{
	PxTransform tTransform;
	ZeroMemory(&tTransform, sizeof(PxTransform));
	tTransform.p.x = vPosition.x;
	tTransform.p.y = vPosition.y;
	tTransform.p.z = vPosition.z;
	m_pRouteDebug.push_back(
		CDebugObject::Create(tTransform)
	);
	CREATE_GAMEOBJECT(m_pRouteDebug.back(), GROUP_PHYSX);
}

void CPlayer::Clear_DebugObject()
{
	for (auto& Debug : m_pRouteDebug)
	{
		DELETE_GAMEOBJECT(Debug);
	}
	m_pRouteDebug.clear();
}

#endif

void CPlayer::Update_TargetLock()
{
	if (m_bTargetLocked)
	{
		m_fTargetAcc += fDT(0);

		if (m_fTargetAcc >= m_fTargetMaxTime)
		{
			m_fTargetAcc = 0.f;
			Target_UnLock();
		}
	}

	if (m_bAllyLocked)
	{
		m_fAllyAcc += fDT(0);

		if (m_fAllyAcc >= m_fAllyMaxTime)
		{
			m_fAllyAcc = 0.f;
			m_bAllyLocked = false;
		}
	}

}

_bool CPlayer::Is_OpenCell()
{
	if (nullptr == m_pCurrentUnit)
		return false;
	CNavigation* pNaviComponent = m_pCurrentUnit->Get_NaviCom();
	if (nullptr == pNaviComponent)
		return false;

	_float4 vUnitPos = m_pCurrentUnit->Get_Transform()->Get_World(WORLD_POS);
	CCell* pCell = pNaviComponent->Get_CurCell(vUnitPos, CGameSystem::Get_Instance()->Get_CellLayer());

	if (nullptr == pCell)
		return false;

	if (pCell->Check_Attribute(CELL_BLOCKED))
		return false;

	return true;
}
void CPlayer::Make_BestRoute(_float4 vPosition)
{
	m_bFindRoute = true;
#ifdef _DEBUG
	Clear_DebugObject();
#endif

	m_pCurrentUnit->Get_NaviCom()->Make_Route(&m_CurRoute, CGameSystem::Get_Instance()->Get_CellLayer(), Get_WorldPos(), vPosition);


#ifdef _DEBUG
	m_CurNodeList = m_pCurrentUnit->Get_NaviCom()->m_DebugRouteNode;
#endif

	//if (m_CurRoute.empty())
	//	Set_IsFindRoute(false);
	//else
	//	Set_IsFindRoute(true);
}

void CPlayer::On_AbleHero()
{
	m_bAbleHero = true;
	m_fGauge = m_fMaxGauge;

	if (m_bIsMainPlayer)
	{
		m_pCurrentUnit->Get_CurStateP()->Play_Voice(m_pCurrentUnit, L"Voice_Transform", 0.5f);

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

	STATE_TYPE eCurState = m_pCurrentUnit->Get_CurState();

	if (m_pCurrentUnit->Get_CurState() != STATE_END)
		m_pCurrentUnit->Get_CurStateP()->Exit(m_pCurrentUnit, GET_COMPONENT_FROM(m_pCurrentUnit, CAnimator));

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

		if (m_pMyTeam)
		{
			if (m_pMyTeam->IsMainPlayerTeam())
			{
				m_fRevivalEffectAcc += fDT(0);

				if (m_fRevivalEffectAcc > m_fMaxRevivalEffectTime)
				{


					CFunctor::Play_Sound(L"Effect_Death", CHANNEL_EFFECTS, Get_CurrentUnit()->Get_Transform()->Get_World(WORLD_POS), 0.8f);
					m_fRevivalEffectAcc = 0.f;
				}
			}
		}

		if (m_pUnitHUD)
			m_pUnitHUD->Set_RevivalGauge(m_fRevivalAcc, m_fMaxRevivalTime);

		if (m_fRevivalAcc >= m_fMaxRevivalTime)
		{

			if (!m_bIsMainPlayer)
			{
				_float4 vStartPos;

				// 1. respawn 거점 먹었으면 respawn에서
				if (m_pMyTeam->Has_RespawnTrigger())
				{
					if (CUser::Get_Instance()->Get_CurLevel() == LEVEL_HWARA)
						vStartPos = m_pMyTeam->Find_RespawnPosition("Hwara_Respawn");
					else
						vStartPos = m_pMyTeam->Find_RespawnPosition("Paden_Trigger_R");

				}
				/*else if (m_pMyTeam->Has_CenterTrigger())
				{
					vStartPos = m_pMyTeam->Find_RespawnPosition("Hwara_Center");
				}*/
				else
				{
					vStartPos = m_pMyTeam->Find_RespawnPosition_Start();
				}

				Respawn_Unit(vStartPos, m_eCurrentClass);
			}
			else
			{
				m_bAbleRevival = false;
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
	if (m_pUnitHUD)
	{
		CREATE_GAMEOBJECT(m_pUnitHUD, GROUP_UI);
		DISABLE_GAMEOBJECT(m_pUnitHUD);
	}
}

void CPlayer::Frustum_UnitHUD()
{
	if (!m_pCurrentUnit->Is_Valid())
		return;

	if (m_pUnitHUD)
		m_pUnitHUD->Set_UnitHP(Get_CurrentUnit()->Get_Status().fHP, Get_CurrentUnit()->Get_Status().fMaxHP);

	_float fDis = CUtility_Transform::Get_FromCameraDistance(m_pCurrentUnit);
	if (fDis < m_fEnable_UnitHUDis)
	{
		m_fHUDTime = 0.f;
		m_pUnitHUD->Set_UnitDis(fDis);
		m_pUnitHUD->SetActive(m_bIsInFrustum);
	}
	else
	{
		m_fHUDTime += fDT(0);
		if (m_fHUDTime > 3.f)
		{
			m_fHUDTime = 0.f;
			m_pUnitHUD->SetActive(false);
		}
	}
}

void CPlayer::TransformProjection()
{
	if (m_pCurrentUnit)
	{
		if (m_pUnitHUD)
			m_pUnitHUD->Set_ProjPos(m_pCurrentUnit->Get_Transform());
	}
}


