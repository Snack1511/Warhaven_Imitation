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
#include "CState_Manager.h"
#include "CNavigation.h"
#include "CTerrain.h"
#include "CCell.h"

#include "CCamera_Free.h"

#include "CUI_HUD.h"
#include "CBloodOverlay.h"
#include "CUI_Damage.h"
#include "CUI_Training.h"

#include "CUI_Cursor.h"

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
		//ÇÁ¸®Ä·ÀÌ¸é
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

void CUser::On_EnterLevel()
{
	

	ENABLE_GAMEOBJECT(m_pCursor);

}

void CUser::On_ExitLevel()
{
	DISABLE_GAMEOBJECT(m_pCursor);

}

void CUser::On_EnterStageLevel()
{
	if (!m_pUI_HUD)
	{
		m_pUI_HUD = CUI_HUD::Create();
		CREATE_GAMEOBJECT(m_pUI_HUD, GROUP_UI);
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

	SetUp_BloodOverlay();
}

void CUser::On_ExitStageLevel()
{
	m_pBloodOverlay = nullptr;
	m_pUI_HUD = nullptr;

	for (_uint i = 0; i < 5; ++i)
		m_pUI_Damage[i] = nullptr;

	m_pUI_Training = nullptr;
	m_pPlayer = nullptr;
}

void CUser::Set_HUD(CLASS_TYPE eClass)
{
	m_pUI_HUD->Set_HUD(eClass);
}

void CUser::Set_HP(_float fMaxHP, _float fCurHP)
{
	m_pUI_HUD->Set_HP(fMaxHP, fCurHP);
}

void CUser::Set_HeroGauge(_float fMaxGauge, _float fCurGauge)
{
	m_pUI_HUD->Set_HeroGauge(fMaxGauge, fCurGauge);
}

void CUser::SetActive_HeroPortrait(_bool value)
{
	m_pUI_HUD->SetActive_HeroPortrait(value);
}

void CUser::SetActive_OxenJumpText(_bool value)
{
	m_pUI_HUD->SetActive_OxenJumpText(value);
}

void CUser::Set_SkillCoolTime(_uint iSkillType, _float fSkillCoolTime, _float fMaxCoolTime)
{
	m_pUI_HUD->Set_SkillCoolTime(iSkillType, fSkillCoolTime, fMaxCoolTime);
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

