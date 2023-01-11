#include "CUI_UnitHUD.h"
#include "CUI_Object.h"

#include "CUnit.h"
#include "CUI_UnitHP.h"
#include "CUI_Revive.h"

#include "GameInstance.h"
#include "CUtility_Transform.h"
#include "Transform.h"
#include "Easing_Utillity.h"
#include "Texture.h"
#include "CUser.h"
#include "CPlayer.h"
#include "CTeamConnector.h"
#include "Functor.h"

HRESULT CUI_UnitHUD::Initialize_Prototype()
{
	Create_UnitHUD();
	Create_TargetUI();

	return S_OK;
}

HRESULT CUI_UnitHUD::Initialize()
{
	return S_OK;
}

HRESULT CUI_UnitHUD::Start()
{
	Init_UnitNameText();

	CREATE_GAMEOBJECT(m_pUnitNameText, GROUP_UI);

	for (int i = 0; i < UI_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pUnitUI[i], GROUP_UI);
	}

	__super::Start();

	return S_OK;
}

void CUI_UnitHUD::OnEnable()
{
	__super::OnEnable();

	ENABLE_GAMEOBJECT(m_pUnitNameText);
}

void CUI_UnitHUD::OnDisable()
{
	__super::OnDisable();

	DISABLE_GAMEOBJECT(m_pUnitNameText);

	for (int i = 0; i < Target_End; ++i)
		m_pTargetUI[i]->SetActive(false);

	for (int i = 0; i < UI_End; ++i)
		DISABLE_GAMEOBJECT(m_pUnitUI[i]);
}

void CUI_UnitHUD::My_Tick()
{
	__super::My_Tick();

	if (m_fUnitDis > 10.f)
	{
		m_vOffset = _float4(0.f, 1.9f, 0.f);

		_bool bHero = m_pOwner->Get_CurrentUnit()->Get_OwnerPlayer()->IsHero();
		if (bHero)
		{
			if (m_pOwner->Get_Team())
			{
				if (m_pOwner->Get_Team()->IsMainPlayerTeam())
				{
					m_pUnitNameText->Set_Color(m_vColorOrigin);
					m_pUnitNameText->Set_Scale(m_fHeroIconScale);

					if (GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Get_CurTextureIndex() != 2)
						GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Set_CurTextureIndex(2);
				}
				else
				{
					m_pUnitNameText->Set_Color(m_vColorOrigin);
					m_pUnitNameText->Set_Scale(m_fHeroIconScale);

					if (GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Get_CurTextureIndex() != 3)
						GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Set_CurTextureIndex(3);
				}
			}
		}
		else
		{
			if (m_pOwner->IsLeaderPlayer())
			{
				if (GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Get_CurTextureIndex() != 1)
				{
					m_pUnitNameText->Set_Scale(m_fLeaderIconScale);

					GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Set_CurTextureIndex(1);
				}
			}
			else
			{
				m_pUnitNameText->Set_Scale(8.f);

				if (GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Get_CurTextureIndex() != 0)
				{
					GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Set_CurTextureIndex(0);
				}
			}

			if (m_pOwner->Get_Team() && m_pOwner->Get_Team()->IsMainPlayerTeam())
			{
				if (m_pOwner->Get_OutlineType() == CPlayer::eSQUADMEMBER)
				{
					m_pUnitNameText->Set_Color(m_vColorLightGreen);
				}
				else
				{
					m_pUnitNameText->Set_Color(m_vColorBlue);
				}
			}
			else
			{
				m_pUnitNameText->Set_Color(m_vColorRed);
			}
		}

		if (m_pUnitNameText->Get_FontRender())
		{
			m_pUnitNameText->Set_FontRender(false);

			if (m_pOwner->Get_Team())
			{
				if (m_pOwner->Get_Team()->IsMainPlayerTeam())
				{
					if (m_pOwner->Get_OutlineType() == CPlayer::eSQUADMEMBER)
					{
						m_pUnitNameText->Set_Color(m_vColorLightGreen);
					}
					else
					{
						m_pUnitNameText->Set_Color(m_vColorBlue);
					}
				}
				else
				{
					m_pUnitNameText->Set_Color(m_vColorRed);
				}
			}

			SetActive_UnitHP(false);
		}
	}
	else
	{
		m_vOffset = _float4(0.f, 2.f, 0.f);

		if (m_pOwner->IsMainPlayer())
		{
			if (m_pOwner->Get_CurrentUnit()->Get_OwnerPlayer()->IsHero())
			{
				m_pUnitNameText->Set_Color(m_vColorOrigin);
				m_pUnitNameText->Set_Scale(m_fHeroIconScale);

				if (GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Get_CurTextureIndex() != 2)
					GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Set_CurTextureIndex(2);
			}
			else
			{
				if (GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Get_CurTextureIndex() != 1)
				{
					m_pUnitNameText->Set_Scale(m_fLeaderIconScale);
					m_pUnitNameText->Set_Color(m_vColorGreen);

					GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Set_CurTextureIndex(1);
				}
			}
		}

		if (m_pOwner->IsMainPlayer())
			return;

		if (!m_pUnitNameText->Get_FontRender())
		{
			m_pUnitNameText->Set_FontRender(true);
			m_pUnitNameText->Set_Color(vColorAlpha);
		}
	}

	Tick_UnitHP();
	Tick_TargetUI();
}

void CUI_UnitHUD::My_LateTick()
{
	__super::My_LateTick();
}

CUI_UnitHP* CUI_UnitHUD::Get_UnitHP()
{
	return static_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp]);;
}

CUI_Revive* CUI_UnitHUD::Get_ReviveUI()
{
	return static_cast<CUI_Revive*>(m_pUnitUI[UI_Revive]);
}

void CUI_UnitHUD::Set_ProjPos(CTransform* pTransform)
{
	_float4 vNewPos = CUtility_Transform::Get_ProjPos(pTransform, m_vOffset);

	m_pUnitNameText->Set_Pos(vNewPos);

	if (m_pOwner->Get_CurClass() == LANCER)
		m_pUnitNameText->Set_PosY(300.f);

	dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_ProjPos(pTransform);

	vNewPos.y -= 30.f;
	for (int i = 0; i < Target_End; ++i)
		m_pTargetUI[i]->Set_Pos(vNewPos);
}

void CUI_UnitHUD::Enable_RevivalUI()
{
	CUI_Revive* pRevivalUI = static_cast<CUI_Revive*>(m_pUnitUI[UI_Revive]);

	pRevivalUI->Set_ReviveUnitTransform(m_pOwner->Get_Transform());
	pRevivalUI->Set_ClassIcon(m_pOwner);

	if (!pRevivalUI->Is_Valid())
		pRevivalUI->SetActive(true);
}

void CUI_UnitHUD::Disable_RevivalUI()
{
	m_pUnitUI[UI_Revive]->SetActive(false);
}

void CUI_UnitHUD::Set_RevivalGauge(_float fCurTime, _float fMaxTime)
{
	static_cast<CUI_Revive*>(m_pUnitUI[UI_Revive])->Set_GaugeRatio(fCurTime, fMaxTime);
}

void CUI_UnitHUD::Set_RevivalIcon(_uint iIconIdx)
{
	static_cast<CUI_Revive*>(m_pUnitUI[UI_Revive])->Set_ReviveIcon(iIconIdx);
}

void CUI_UnitHUD::SetActive_TargetUI(_uint iIdx, _bool value)
{
	if (value == false)
	{
		for (int i = 0; i < Target_End; ++i)
			m_pTargetUI[i]->SetActive(false);
	}
	else
	{
		m_fMaxEanbleTargetUITime = 0.1f * (iIdx + 1.f);
		m_bEnableTargetUI = value;
	}
}

void CUI_UnitHUD::Create_UnitHUD()
{
	m_pUnitNameText = CUI_Object::Create();

	m_pUnitUI[UI_Hp] = CUI_UnitHP::Create();
	m_pUnitUI[UI_Revive] = CUI_Revive::Create();
}

void CUI_UnitHUD::Create_TargetUI()
{
	for (int i = 0; i < Target_End; ++i)
	{
		m_pTargetUI[i] = CUI_Object::Create();

		m_pTargetUI[i]->Set_Sort(0.5f);
		m_pTargetUI[i]->Set_Color(RGB(255, 0, 0));
		switch (i)
		{
		case Target_Point:
			m_pTargetUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Point2.dds"));
			m_pTargetUI[i]->Set_Scale(30.f);
			break;

		case Target_Blink:
			m_pTargetUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_32Circle.dds"));
			m_pTargetUI[i]->Set_Scale(1.f);
			m_pTargetUI[i]->Set_FadeDesc(0.f, 0.7f, true);
			break;
		}

		CREATE_GAMEOBJECT(m_pTargetUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pTargetUI[i]);
	}
}

void CUI_UnitHUD::Init_UnitNameText()
{
	m_pUnitNameText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_32Circle.dds"));

	GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/UnitHUD/T_IconSoldier.dds"));
	GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/UnitHUD/T_Elite2IconPlayer.dds"));
	GET_COMPONENT_FROM(m_pUnitNameText, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/UnitHUD/T_Elite2Icon.dds"));

	m_pUnitNameText->Set_Scale(8.f);
	m_pUnitNameText->Set_Sort(0.55f);

	m_pUnitNameText->Set_FontStyle(true);
	m_pUnitNameText->Set_FontCenter(true);
	m_pUnitNameText->Set_FontScale(0.2f);

	if (m_pOwner->Get_Team())
	{
		if (m_pOwner->Get_Team()->IsMainPlayerTeam())
		{
			if (m_pOwner->Get_OutlineType() == CPlayer::eSQUADMEMBER)
			{
				m_pUnitNameText->Set_FontColor(m_vColorLightGreen);
			}
			else
			{
				m_pUnitNameText->Set_FontColor(m_vColorBlue);
			}
		}
		else
		{
			m_pUnitNameText->Set_FontColor(m_vColorRed);
		}
	}

	wstring wstrUnitName = m_pOwner->Get_PlayerName();
	m_pUnitNameText->Set_FontText(wstrUnitName);
}

void CUI_UnitHUD::SetActive_UnitHP(_bool value)
{
	if (value == true)
	{
		if (!m_pUnitUI[UI_Hp]->Is_Valid())
		{
			if (m_pOwner->Get_Team())
			{
				if (m_pOwner->Get_Team()->IsMainPlayerTeam())
				{
					if (m_pOwner->Get_OutlineType() == CPlayer::eSQUADMEMBER)
					{
						dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_UnitHPColor(m_vColorLightGreen);
					}
					else
					{
						dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_UnitHPColor(m_vColorBlue);
					}
				}
				else
				{
					dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_UnitHPColor(m_vColorRed);
				}
			}

			ENABLE_GAMEOBJECT(m_pUnitUI[UI_Hp]);
		}
	}
	else
	{
		if (m_pUnitUI[UI_Hp]->Is_Valid())
		{
			DISABLE_GAMEOBJECT(m_pUnitUI[UI_Hp]);
		}
	}
}

void CUI_UnitHUD::Tick_UnitHP()
{
	if (m_pOwner->IsMainPlayer())
		return;

	if (m_pUnitUI[UI_Hp]->Is_Valid())
	{
		_float fHpGaugeRatio = m_tStatus.fHP / m_tStatus.fMaxHP;
		dynamic_cast<CUI_UnitHP*>(m_pUnitUI[UI_Hp])->Set_GaugeRatio(fHpGaugeRatio);

		m_fEnableHpTime += fDT(0);
		if (m_fEnableHpTime > m_fDisableHpTime)
		{
			m_fEnableHpTime = 0.f;
			SetActive_UnitHP(false);
		}
		else if (fHpGaugeRatio <= 0.f)
		{
			SetActive_UnitHP(false);
		}
	}
}

void CUI_UnitHUD::Tick_TargetUI()
{
	if (m_tStatus.fHP <= 0.f)
	{
		for (int i = 0; i < Target_End; ++i)
		{
			if (m_pTargetUI[i]->Is_Valid())
				m_pTargetUI[i]->SetActive(false);
		}
	}

	if (m_bEnableTargetUI)
	{
		m_fEanbleTargetUITime += fDT(0);
		if (m_fEanbleTargetUITime > m_fMaxEanbleTargetUITime)
		{
			m_fEanbleTargetUITime = 0.f;
			m_bEnableTargetUI = false;

			m_pTargetUI[Target_Point]->SetActive(true);
			m_pTargetUI[Target_Point]->Lerp_Scale(70.f, 30.f, 0.3f);
		}
	}

	if (m_pTargetUI[Target_Point]->Is_Valid())
	{
		m_fTargetRotValue += fDT(0) * 10.f;
		m_pTargetUI[Target_Point]->Set_RotationZ(m_fTargetRotValue);

		if (!m_pTargetUI[Target_Blink]->Is_Valid())
		{
			Enable_Fade(m_pTargetUI[Target_Blink], 0.3f);
			m_pTargetUI[Target_Blink]->Lerp_Scale(1.f, 30.f, 0.7f);
		}
	}
}
