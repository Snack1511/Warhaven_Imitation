#include "stdafx.h"
#include "CUser.h"
#include "MainApp.h"
#include "GameInstance.h"

#include "Texture.h"
#include "GameObject.h"
#include "CFader.h"
#include "CShader.h"
#include "Model.h"
#include "HIerarchyNode.h"
#include "Transform.h"
#include "Loading_Manager.h"

#include "CFadeDark.h"

#include "CPlayer.h"

#include "CEffects_Factory.h"
#include "Functor.h"
#include "Functor.h"
#include "CState_Manager.h"
#include "CNavigation.h"
#include "CTerrain.h"
#include "CCell.h"

#include "CCamera_Free.h"

#include "CUI_HUD.h"
#include "CUI_Portrait.h"
#include "CUI_HpBar.h"	
#include "CUI_HeroGauge.h"
#include "CUI_Skill.h"
#include "CUI_CrossHair.h"

#include "CBloodOverlay.h"
#include "CUI_Damage.h"
#include "CUI_Training.h"
#include "CUI_Paden.h"
#include "CUI_Dead.h"
#include "CUI_Oper.h"
#include "CUI_EscMenu.h"
#include "CUI_Popup.h"
#include "CUI_KillLog.h"
#include "CUI_KillName.h"
#include "CUI_Revive.h"
#include "CUI_UnitHUD.h"
#include "CUI_Interact.h"
#include "CUI_MiniMap.h"
#include "CUI_ScoreBoard.h"
#include "CUI_ScoreInfo.h"
#include "CUI_Cannon.h"
#include "CUI_Main.h"
#include "CUI_Barracks.h"

#include "CUI_Cursor.h"
#include "CUI_Animation.h"
#include "CUI_AbleHeroFire.h"

#include "CGameSystem.h"
#include "CUI_Result.h"

IMPLEMENT_SINGLETON(CUser);

CUser::CUser()
{
}

CUser::~CUser()
{
}

HRESULT CUser::Initialize()
{
	POINT ptMouse;

	ptMouse.x = g_iWinCX >> 1;
	ptMouse.y = g_iWinCY >> 1;
	m_ptCenter = ptMouse;

	m_KeyCommands.push_back(KEY::LAST);

	return S_OK;
}

void CUser::Tick()
{
	Fix_CursorPosToCenter();

	if (KEY(TAB, TAP))
	{
		static _bool bShowCursor = false;

		bShowCursor = !bShowCursor;

		ShowCursor(bShowCursor);
	}

	Update_KillLog();
	Update_KillName();
}

CUnit* CUser::Get_Player()
{
	return m_pPlayer->Get_CurrentUnit();
}

void CUser::Fix_CursorPosToCenter()
{
	if (KEY(F4, TAP))
	{
		//프리캠이면
		if (dynamic_cast<CCamera_Free*>(GAMEINSTANCE->Get_CurCam()))
		{
			GAMEINSTANCE->Change_Camera(L"PlayerCam");
		}
		else
			GAMEINSTANCE->Change_Camera(L"FreeCam");
	}

	if (GetFocus() != g_hWnd || !m_bFixCursor)
		return;

	/* Set Mouse Cursor on Center of Screen */
	POINT ptMouse;
	ptMouse = m_ptCenter;

	::ClientToScreen(g_hWnd, &ptMouse);
	::SetCursorPos(ptMouse.x, ptMouse.y);
}

void CUser::KeyInput_FPSSetter()
{
	if (KEY(F1, TAP))
		Time_Slow();
	else if (KEY(F2, TAP))
	{
		_double dCurFPSLimit = CGameInstance::Get_Instance()->Get_FPSLimitTime();

		if (dCurFPSLimit <= 0.)
		{
			dCurFPSLimit = 1. / 90.;
		}
		else
		{
			dCurFPSLimit = 0.;
		}
		CGameInstance::Get_Instance()->Set_FPSLimitTIme(dCurFPSLimit);

	}
	else if (KEY(F3, TAP))
	{
		_double dCurFPSLimit = CGameInstance::Get_Instance()->Get_FPSLimitTime();

		if (dCurFPSLimit <= 0.)
		{
			dCurFPSLimit = 1. / 60.;
		}
		else
		{
			dCurFPSLimit = 0.;
		}
		CGameInstance::Get_Instance()->Set_FPSLimitTIme(dCurFPSLimit);

	}
}

void CUser::Update_KeyCommands()
{
	vector<CKey_Manager::tKeyInfo>& vecKeys = CGameInstance::Get_Instance()->Get_KeyList();

	for (_uint i = 0; i < (_uint)KEY::LAST; ++i)
	{
		if (vecKeys[i].eState == KEY_STATE::TAP)
		{
			m_KeyCommands.push_back((KEY)i);
		}
	}


}

void CUser::Time_Slow()
{
	static _bool	bSlow = false;

	if (bSlow = !bSlow)
		GAMEINSTANCE->Set_TimeSpeed(0, 0.05f);
	else
		GAMEINSTANCE->Set_TimeSpeed(0, 1.f);
}

void CUser::Set_CrossHairPos(_float4 vPosition)
{
	if (m_pUI_HUD)
		m_pUI_HUD->Set_Crosshair_Pos(vPosition);
}

void CUser::SetUp_BloodOverlay()
{
	if (m_pBloodOverlay)
		return;

	m_pBloodOverlay = CBloodOverlay::Create();

	CREATE_GAMEOBJECT(m_pBloodOverlay, GROUP_UI);

	DISABLE_GAMEOBJECT(m_pBloodOverlay);
}

void CUser::Turn_BloodOverLay(_float fHpRatio)
{
	if (!m_pBloodOverlay)
		return;

	ENABLE_GAMEOBJECT(m_pBloodOverlay);
	m_pBloodOverlay->Trun_BloodOverlay(fHpRatio);
}

void CUser::SetUp_HeroGaugeFire()
{
	if (m_pFire)
		return;

	m_pFire = CUI_Animation::Create();
	m_pUVFire = CUI_AbleHeroFire::Create();

	CREATE_GAMEOBJECT(m_pFire, GROUP_UI);
	CREATE_GAMEOBJECT(m_pUVFire, GROUP_UI);

	DISABLE_GAMEOBJECT(m_pFire);
	DISABLE_GAMEOBJECT(m_pUVFire);
}

void CUser::Turn_HeroGaugeFire(_bool bTurnOn)
{
	if (!m_pFire)
		return;

	Client::CUI_Animation::ANIMINFO tAniminfo;

	tAniminfo.bBlackBackGround = true; //이미지 배경이 검은색인지 
	tAniminfo.bPlayOnce = false;
	tAniminfo.fDuration = 0.035f; //프레임당 시간
	tAniminfo.iHeightSize = 8; //가로 세로 프레임
	tAniminfo.iWidthSize = 8;
	tAniminfo.vPos = _float2(550.f, -270.f);
	tAniminfo.vScale = _float2(120.f, 120.f);

	if (bTurnOn)
	{
		ENABLE_GAMEOBJECT(m_pFire);
		m_pFire->Trun_UIAnimation(tAniminfo);
	}
	else
		DISABLE_GAMEOBJECT(m_pFire);
}

void CUser::Turn_AbleHeroFire()
{
	ENABLE_GAMEOBJECT(m_pUVFire); //화신 가능 UV미는 불꽃 이미지
}

CUI_Wrapper* CUser::Get_HUD(_uint eHUD)
{
	return m_pUI_HUD->Get_HUD(eHUD);
}

void CUser::Set_UserPort(_uint iClass)
{
	m_pUI_Portrait->Set_UserPort(iClass);
}

void CUser::Set_HeroPort(_uint iType)
{
	m_pUI_Portrait->Set_HeroPort(iType);
}

void CUser::Set_HP(_float fCurValue, _float fMaxValue)
{
	m_pUI_HP->Set_HP(fCurValue, fMaxValue);
}

void CUser::Set_HeroGauge(_float fCurValue, _float fMaxValue)
{
	m_pUI_HeroGauge->Set_HeroGauge(fCurValue, fMaxValue);
}

void CUser::SetActive_SquardInfo(_bool value)
{
	m_pUI_HUD->SetActive_SquardInfo(value);
}

void CUser::SetActive_Result(_uint iResult, _bool value)
{
	m_pUI_Result->Set_Result(iResult);
	m_pUI_Result->SetActive(true);
	/*if (m_pUI_Result->Is_Valid())
		m_pUI_Result->SetActive_Result(iResult, value);*/
}

void CUser::SetActive_HUD(_bool value)
{
	m_pUI_HUD->SetActive_HUD(value);
}

void CUser::SetActive_UnitHUD(_bool value)
{
	m_pPlayer->SetActive_UnitHUD(value);
}

void CUser::SetActive_AbleHeroText(_bool value)
{
	m_pUI_HeroGauge->SetActive_AbleHeroText(value);
}

void CUser::Transform_SkillUI(_uint iClass)
{
	m_pUI_Skill->Transform_SkillUI(iClass);
}

void CUser::Interat_PointUI(_bool bIsMainPlayerTeam, string strPadenPointKey)
{
	if (m_pUI_Paden)
		m_pUI_Paden->Interact_PointUI(bIsMainPlayerTeam, strPadenPointKey);
}

void CUser::Move_PointUI(string strPadenPointKey, _uint iTriggerState)
{
	if (m_pUI_Paden)
		m_pUI_Paden->Move_PointUI(strPadenPointKey, iTriggerState);
}

void CUser::Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime)
{
	if (m_pUI_Paden)
		m_pUI_Paden->Set_ConquestTime(strPadenPointKey, fConquestTime, fMaxConquestTime);
}

void CUser::Set_Team(CTeamConnector* pAllyTeam, CTeamConnector* pEnemyTeam)
{
	if (m_pUI_Paden)
		m_pUI_Paden->Set_Team(pAllyTeam, pEnemyTeam);
}

void CUser::Set_PointUI_ProjectionTransform(_uint iPointIdx, CTransform* pTransform, _bool isInFrustum)
{
	if (m_pUI_Paden)
		m_pUI_Paden->Set_PointUI_ProjectionTransform(iPointIdx, pTransform, isInFrustum);
}

void CUser::Set_MiniMapConquestTime(_uint iPoinIdx, _float fConquestTime, _float fMaxConquestTime)
{
	if (m_pMiniMap)
		m_pMiniMap->Set_ConquestTime(iPoinIdx, fConquestTime, fMaxConquestTime);
}

void CUser::Set_MiniMapGaugeColor(_bool IsMainTeam, _uint iPointIdx)
{
	if (m_pMiniMap)
		m_pMiniMap->Set_GaugeColor(IsMainTeam, iPointIdx);
}

void CUser::Set_MiniMapPointColor(_bool IsMainTeam, _uint iPointIdx)
{
	if (m_pMiniMap)
		m_pMiniMap->Set_PointColor(IsMainTeam, iPointIdx);
}

void CUser::Set_MiniMapPlayer(CPlayer* pPlayer)
{
	if (m_pMiniMap)
		m_pMiniMap->Set_Player(pPlayer);
}

void CUser::Conquest_PointUI(string strPointName, _bool bIsMainPlayerTeam)
{
	if (m_pUI_Paden)
		m_pUI_Paden->Conquest_PointUI(strPointName, bIsMainPlayerTeam);
}

void CUser::Set_TargetTransform(CTransform* pTargetTransform)
{
	m_pUI_Paden->Set_TargetTransform(pTargetTransform);
}

void CUser::Set_TargetPointPos(_uint iTargetIdx)
{
	if (m_pUI_Paden)
		m_pUI_Paden->Set_TargetPointPos(iTargetIdx);
}

void CUser::SetActive_TargetPoint(_bool value)
{
	if (m_pUI_Paden)
		m_pUI_Paden->SetActive_TargetPoint(value);
}

void CUser::SetActive_PadenUI(_bool value)
{
	if (m_pUI_Paden)
		m_pUI_Paden->SetActive(value);
}

void CUser::SetActive_ScoreBoard(_bool value)
{
	if (m_pScoreBoard)
	{
		m_pScoreBoard->SetActive(value);
	}
}

void CUser::Set_Respawn(_bool value)
{
	if (m_pUI_Oper)
		m_pUI_Oper->Set_Respawn(value);
}

void CUser::Set_OperPointColor(_bool IsMainTeam, _uint iPoinIdx)
{
	if (m_pUI_Oper)
		m_pUI_Oper->Set_PointColor(IsMainTeam, iPoinIdx);
}

void CUser::Set_OperPlayer(CPlayer* pPlayer)
{
	if (m_pUI_Oper)
		m_pUI_Oper->Set_Player(pPlayer);
}

void CUser::SetActive_OperUI(_bool value)
{
	if (m_pUI_Oper)
		m_pUI_Oper->SetActive(value);
}

_bool CUser::Get_SelectTargetPoint()
{
	if (m_pUI_Oper)
		return m_pUI_Oper->Get_SelectTargetPoint();

	return false;
}

void CUser::On_EnterLevel()
{
	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();
	if (m_eLoadLevel == LEVEL_MAINMENU)
	{
		if (!m_pMainUI)
		{
			m_pMainUI = CUI_Main::Create();

			CREATE_GAMEOBJECT(m_pMainUI, GROUP_UI);
		}

		if (!m_pBarracks)
		{
			m_pBarracks = CUI_Barracks::Create();

			CREATE_GAMEOBJECT(m_pBarracks, GROUP_UI);
			DISABLE_GAMEOBJECT(m_pBarracks);
		}
	}

	DISABLE_GAMEOBJECT(m_pCursor);
	ENABLE_GAMEOBJECT(m_pCursor);
}

void CUser::On_ExitLevel()
{
	DISABLE_GAMEOBJECT(m_pCursor);
}

void CUser::On_EnterStageLevel()
{
	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();


	if (!m_pUI_HUD)
	{
		m_pUI_HUD = CUI_HUD::Create();
		CREATE_GAMEOBJECT(m_pUI_HUD, GROUP_UI);

		m_pUI_Portrait = static_cast<CUI_Portrait*>(CUser::Get_Instance()->Get_HUD(CUI_HUD::HUD_Port));
		m_pUI_HP = static_cast<CUI_HpBar*>(CUser::Get_Instance()->Get_HUD(CUI_HUD::HUD_HP));
		m_pUI_HeroGauge = static_cast<CUI_HeroGauge*>(CUser::Get_Instance()->Get_HUD(CUI_HUD::HUD_HeroGauge));
		m_pUI_Skill = static_cast<CUI_Skill*>(CUser::Get_Instance()->Get_HUD(CUI_HUD::HUD_Skill));
		m_pUI_Crosshair = static_cast<CUI_Crosshair*>(CUser::Get_Instance()->Get_HUD(CUI_HUD::HUD_Crosshair));
	}

	if (!m_pInteractUI)
	{
		m_pInteractUI = CUI_Interact::Create();

		CREATE_GAMEOBJECT(m_pInteractUI, GROUP_UI);
		DISABLE_GAMEOBJECT(m_pInteractUI);
	}

	if (!m_pUI_Damage[0])
	{
		for (int i = 0; i < 5; ++i)
		{
			m_pUI_Damage[i] = CUI_Damage::Create();

			CREATE_GAMEOBJECT(m_pUI_Damage[i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pUI_Damage[i]);

			m_pKillName[i] = CUI_KillName::Create();

			CREATE_GAMEOBJECT(m_pKillName[i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pKillName[i]);
		}
	}

	if (!m_pUI_Training)
	{
		m_pUI_Training = CUI_Training::Create();
		CREATE_GAMEOBJECT(m_pUI_Training, GROUP_UI);
	}

	if (!m_pUI_Dead)
	{
		m_pUI_Dead = CUI_Dead::Create();

		CREATE_GAMEOBJECT(m_pUI_Dead, GROUP_UI);
		DISABLE_GAMEOBJECT(m_pUI_Dead);
	}

	if (!m_pUI_Popup)
	{
		m_pUI_Popup = CUI_Popup::Create();

		CREATE_GAMEOBJECT(m_pUI_Popup, GROUP_UI);
		DISABLE_GAMEOBJECT(m_pUI_Popup);
	}

	if (!m_pKillLog[0])
	{
		for (int i = 0; i < 10; ++i)
		{
			m_pKillLog[i] = CUI_KillLog::Create();

			CREATE_GAMEOBJECT(m_pKillLog[i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pKillLog[i]);
		}

		m_pKillLogList.clear();
	}

	if (m_eLoadLevel >= LEVEL_PADEN)
	{
		if (!m_pUI_Oper)
		{
			m_pUI_Oper = CUI_Oper::Create();

			CREATE_GAMEOBJECT(m_pUI_Oper, GROUP_UI);
			DISABLE_GAMEOBJECT(m_pUI_Oper);
		}

		if (!m_pUI_Paden)
		{
			m_pUI_Paden = CUI_Paden::Create();

			CREATE_GAMEOBJECT(m_pUI_Paden, GROUP_UI);
			DISABLE_GAMEOBJECT(m_pUI_Paden);
		}

		if (!m_pUI_Result)
		{
			m_pUI_Result = CUI_Result::Create();

			CREATE_GAMEOBJECT(m_pUI_Result, GROUP_UI);
			DISABLE_GAMEOBJECT(m_pUI_Result);
		}

		if (!m_pMiniMap)
		{
			m_pMiniMap = CUI_MiniMap::Create();

			CREATE_GAMEOBJECT(m_pMiniMap, GROUP_UI);
			// DISABLE_GAMEOBJECT(m_pMiniMap);
		}

		if (!m_pScoreBoard)
		{
			m_pScoreBoard = CUI_ScoreBoard::Create();

			CREATE_GAMEOBJECT(m_pScoreBoard, GROUP_UI);
			DISABLE_GAMEOBJECT(m_pScoreBoard);
		}

		if (!m_pCannonUI)
		{
			m_pCannonUI = CUI_Cannon::Create();

			CREATE_GAMEOBJECT(m_pCannonUI, GROUP_UI);
			DISABLE_GAMEOBJECT(m_pCannonUI);
		}
	}

	SetUp_BloodOverlay();
	SetUp_HeroGaugeFire();
}

void CUser::On_ExitStageLevel()
{
	m_pBloodOverlay = nullptr;

	if (m_pUI_HUD)
		m_pUI_HUD = nullptr;

	if (m_pUI_Dead)
		m_pUI_Dead = nullptr;

	for (_uint i = 0; i < 5; ++i)
	{
		if (m_pUI_Damage[i])
			m_pUI_Damage[i] = nullptr;

		if (m_pKillName[i])
			m_pKillName[i] = nullptr;
	}
	m_pKillNameList.clear();

	for (int i = 0; i < 10; ++i)
	{
		if (m_pKillLog[i])
			m_pKillLog[i] = nullptr;
	}
	m_pKillLogList.clear();

	if (m_pUI_Oper)
		m_pUI_Oper = nullptr;

	if (m_pUI_Paden)
		m_pUI_Paden = nullptr;

	if (m_pUI_Training)
		m_pUI_Training = nullptr;

	if (m_pInteractUI)
		m_pInteractUI = nullptr;

	if (m_pMiniMap)
		m_pMiniMap = nullptr;

	if (m_pCannonUI)
		m_pCannonUI = nullptr;

	if (m_pMainUI)
		m_pMainUI = nullptr;

	if (m_pBarracks)
		m_pBarracks = nullptr;

	m_pPlayer = nullptr;
	m_pFire = nullptr;
	m_pUI_Popup = nullptr;
	m_pUI_Result = nullptr;
	m_pScoreBoard = nullptr;
}

void CUser::Set_HUD(CLASS_TYPE eClass)
{
	m_pUI_HUD->Set_HUD(eClass);
}

void CUser::SetActive_OxenJumpText(_bool value)
{
	m_pUI_HUD->SetActive_OxenJumpText(value);
}

void CUser::SetActive_HeroTransformGauge(_bool value)
{
	m_pUI_HUD->SetActive_HeroTransformGauge(value);
}

_bool CUser::Is_OnHeroGauge()
{
	return m_pUI_HUD->Is_OnHeroGauge();
}

void CUser::Set_SkillCoolTime(_uint iSkillIdx, _float fSkillCoolTime, _float fSkillMaxCoolTime)
{
	if (!CGameObject::Is_Valid(m_pUI_Skill))
		return;

	m_pUI_Skill->Set_SkillCoolTime(iSkillIdx, fSkillCoolTime, fSkillMaxCoolTime);
}

void CUser::SetActive_CannonCrosshair(_bool value)
{
	m_pUI_Crosshair->SetActive_CannonCrosshair(value);
}

void CUser::SetActive_Cursor(_bool value)
{
	static_cast<CUI_Cursor*> (m_pCursor)->SetActive_Mouse(value);
}

void CUser::Enable_DamageFont(_uint eType, _float fDmg)
{
	m_pUI_Damage[m_iDamageFontIdx]->Enable_Damage(eType, fDmg);

	m_iDamageFontIdx++;
	if (m_iDamageFontIdx > 4)
	{
		m_iDamageFontIdx = 0;
	}
}

void CUser::Add_KillLog(CPlayer* attacker, CPlayer* victim)
{
	CUI_KillLog* pCurKillLog = m_pKillLog[m_iKillLogIdx++];

	if (m_iKillLogIdx >= 10)
		m_iKillLogIdx = 0;

	pCurKillLog->Set_LogName(attacker, victim);

	m_pKillLogList.push_front(pCurKillLog);

	if (m_pKillLogList.size() > 10)
		m_pKillLogList.pop_back();

	_uint iIndex = 0;
	for (auto& elem : m_pKillLogList)
	{
		elem->Set_KillLogIndex(iIndex++);
	}
}

void CUser::Update_KillLog()
{
	for (auto iter = m_pKillLogList.begin(); iter != m_pKillLogList.end();)
	{
		if (!(*iter)->Is_Valid())
		{
			iter = m_pKillLogList.erase(iter);
		}
		else
			++iter;
	}
}

void CUser::Add_KillName(wstring wstrEnemyName)
{
	CUI_KillName* pCurKillName = m_pKillName[m_iKillNameIdx++];

	if (m_iKillNameIdx >= 5)
		m_iKillNameIdx = 0;

	pCurKillName->Set_KillName(wstrEnemyName);

	m_pKillNameList.push_front(pCurKillName);

	if (m_pKillNameList.size() > 5)
		m_pKillNameList.pop_back();

	_uint iIndex = 0;
	for (auto& elem : m_pKillNameList)
	{
		elem->Set_Index(iIndex++);
	}
}

void CUser::Update_KillName()
{
	for (auto iter = m_pKillNameList.begin(); iter != m_pKillNameList.end();)
	{
		if (!(*iter)->Is_Valid())
		{
			iter = m_pKillNameList.erase(iter);
		}
		else
			++iter;
	}
}

void CUser::Set_ArcherPoint(_bool value)
{
	m_pUI_Crosshair->Set_ArcherPoint(value);
}

void CUser::SetActive_InteractUI(_bool value)
{
	m_pInteractUI->SetActive(value);
}

void CUser::Set_InteractKey(_uint iKeyIndex)
{
	m_pInteractUI->Set_InteractKey(iKeyIndex);
}

void CUser::Set_InteractText(wstring wstrText)
{
	m_pInteractUI->Set_InteractText(wstrText);
}

void CUser::Set_InteractTarget(CGameObject* pInteractTarget)
{
	m_pInteractUI->Set_InteractTarget(pInteractTarget);
}

void CUser::SetActive_CannonUI(_bool value)
{
	if (m_pCannonUI)
		m_pCannonUI->SetActive(value);
}

void CUser::Set_CannonCoolTime(_float fTime, _float fMaxTime)
{
	if (m_pCannonUI)
		m_pCannonUI->Set_CoolTime(fTime, fMaxTime);
}

void CUser::SetActive_CannonCoolTime(_bool value)
{
	if (m_pCannonUI)
		m_pCannonUI->SetActive_CoolTime(value);
}

void CUser::Get_ScoreInfo(CPlayer* pPlayer)
{
	if (m_pScoreBoard)
	{
		m_pScoreBoard->Get_ScoreInfo(pPlayer);
	}
}

map<_uint, list<CUI_ScoreInfo*>> CUser::Get_ScoreInfoMap()
{
	return m_pScoreBoard->Get_ScoreInfoMap();
}

void CUser::Sort_ScoreInfo()
{
	if (m_pScoreBoard)
	{
		m_pScoreBoard->Sort_ScoreInfo();
	}
}

void CUser::Set_ScoreBoardConquestTime(_uint iPointIdx, _float fConquestTime, _float fMaxConquestTime)
{
	if (m_pScoreBoard)
	{
		m_pScoreBoard->Set_ConquestTime(iPointIdx, fConquestTime, fMaxConquestTime);
	}
}

void CUser::Set_ScoreBoardGaugeColor(_bool IsMainTeam, _uint iPointIdx)
{
	if (m_pScoreBoard)
	{
		m_pScoreBoard->Set_GaugeColor(IsMainTeam, iPointIdx);
	}
}

void CUser::Set_ScoreBoardPointColor(_bool IsMainTeam, _uint iPoinIdx)
{
	if (m_pScoreBoard)
	{
		m_pScoreBoard->Set_PointColor(IsMainTeam, iPoinIdx);
	}
}

void CUser::Set_ScoreBoardPlayer(CPlayer* pPlayer)
{
	if (m_pScoreBoard)
	{
		m_pScoreBoard->Set_Player(pPlayer);
	}
}

void CUser::SetActive_MainTopBtn(_bool value)
{
	if (m_pMainUI)
		m_pMainUI->SetActive_TopBtn(value);
}

void CUser::Set_TopBtnEffectPosX(_float fPosX)
{
	if (m_pMainUI)
		m_pMainUI->Set_TopBtnEffectPosX(fPosX);
}

void CUser::SetActive_Barracks(_bool value)
{
	if (m_pBarracks)
		m_pBarracks->SetActive(value);
}

void CUser::Unlock_RabbitHat()
{
	if (m_pBarracks)
		m_pBarracks->Unlock_RabbitHat();
}

void CUser::SetActive_SkinPopup(_bool value)
{
	if (m_pUI_Popup)
		m_pUI_Popup->SetActive_SkinPopup(value);
}

void CUser::Set_BreezeTime(_float fCurTime, _float fMaxTime)
{
	m_pUI_Crosshair->Set_BreezeTime(fCurTime, fMaxTime);
}

void CUser::SetActive_Gauge(_bool value)
{
	m_pUI_Crosshair->SetActive_Gauge(value);
}

void CUser::Set_TargetInfo(CPlayerInfo* pTargetInfo)
{
	m_pUI_Dead->Set_TargetInfo(pTargetInfo);
}

void CUser::Toggle_DeadUI(_bool value, _bool isFall)
{
	if (!m_pUI_Dead)
	{
		Call_MsgBox(TEXT("m_pUI_Dead is nullptr"));

		return;
	}

	if (!CGameObject::Is_Valid(m_pUI_Dead))
	{
		m_pUI_Dead->Toggle_DeadUI(value, isFall);
	}
}
void CUser::Disable_RevivalUI()
{
	m_pUI_Dead->Disable_RevivalUI();
}
void CUser::Enable_Popup(_uint iPopupType)
{
	if (m_pUI_Popup)
		m_pUI_Popup->Enable_Popup((CUI_Popup::ePOPUP_TYPE)iPopupType);
}

void CUser::Enable_SkinPopup(_uint iSkin)
{
	if (m_pUI_Popup)
	{
		m_pUI_Popup->SetActive(true);
		m_pUI_Popup->Enable_SkinPopup(iSkin);
	}
}

void CUser::SetActive_TrainingPopup(_bool value, _uint iIndex)
{
	if (!m_pUI_Training)
		return;

	if (value == true)
	{
		m_pUI_Training->Enable_Popup(iIndex);
	}
	else
	{
		m_pUI_Training->Disable_Popup();
	}
}

void CUser::SetActive_MiniMap(_bool value)
{
	if (m_pMiniMap)
		m_pMiniMap->SetActive_MiniMap(value);
}

