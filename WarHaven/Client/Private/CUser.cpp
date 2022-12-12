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

#include "CBloodOverlay.h"
#include "CUI_Damage.h"
#include "CUI_Training.h"
#include "CUI_Paden.h"
#include "CUI_Dead.h"
#include "CUI_Oper.h"
#include "CUI_EscMenu.h"
#include "CUI_Popup.h"

#include "CUI_Cursor.h"
#include "CUI_Animation.h"

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
			dCurFPSLimit = 1. / 120.;
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

	CREATE_GAMEOBJECT(m_pFire, GROUP_UI);

	DISABLE_GAMEOBJECT(m_pFire);
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

void CUser::SetActive_Result(_uint iResult, _bool value)
{
	m_pUI_Result->SetActive_Result(iResult, value);
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

void CUser::Interat_PointUI(_bool bIsMainPlayer, _bool bIsMainPlayerTeam, string strPadenPointKey, _uint iTriggerState)
{
	m_pUI_Paden->Interact_PointUI(bIsMainPlayer, bIsMainPlayerTeam, strPadenPointKey, iTriggerState);
}

void CUser::Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime)
{
	m_pUI_Paden->Set_ConquestTime(strPadenPointKey, fConquestTime, fMaxConquestTime);
}

void CUser::Set_Score(_uint iTeamType, _uint iScore, _uint iMaxScore)
{
	m_pUI_Paden->Set_Score(iTeamType, iScore, iMaxScore);
}

void CUser::Set_PointUI_ProjectionTransform(_uint iPointIdx, CTransform* pTransform, _bool isInFrustum)
{
	m_pUI_Paden->Set_PointUI_ProjectionTransform(iPointIdx, pTransform, isInFrustum);
}

void CUser::Conquest_PointUI(string strPointName, _bool bIsMainPlayerTeam)
{
	m_pUI_Paden->Conquest_PointUI(strPointName, bIsMainPlayerTeam);
}

void CUser::Set_TargetPointPos(_uint iTargetIdx)
{
	m_pUI_Paden->Set_TargetPointPos(iTargetIdx);
}

void CUser::SetActive_TargetPoint(_bool value)
{
	if (m_pUI_Oper->Get_SelectTargetPoint() == false)
		return;

	m_pUI_Paden->SetActive_TargetPoint(value);
}

void CUser::SetActive_PadenUI(_bool value)
{
	m_pUI_Paden->SetActive(value);
}

void CUser::Set_Respawn(_bool value)
{
	if (m_pUI_Oper)
		m_pUI_Oper->Set_Respawn(value);
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
	}

	if (!m_pUI_Damage[0])
	{
		for (int i = 0; i < 5; ++i)
		{
			m_pUI_Damage[i] = CUI_Damage::Create();

			CREATE_GAMEOBJECT(m_pUI_Damage[i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pUI_Damage[i]);
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

	if (m_eLoadLevel > LEVEL_BOOTCAMP)
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
	}

	if (m_pUI_Oper)
		m_pUI_Oper = nullptr;

	if (m_pUI_Paden)
		m_pUI_Paden = nullptr;

	if (m_pUI_Training)
		m_pUI_Training = nullptr;

	m_pPlayer = nullptr;
	m_pFire = nullptr;
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
void CUser::Enable_Popup(_uint iPopupType)
{
	if (m_pUI_Popup)
		m_pUI_Popup->Enable_Popup((CUI_Popup::ePOPUP_TYPE)iPopupType);

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

