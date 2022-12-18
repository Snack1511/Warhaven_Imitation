#include "CUI_KillName.h"
#include "GameInstance.h"
#include "CUI_Object.h"

CUI_KillName::CUI_KillName()
{
}

CUI_KillName::~CUI_KillName()
{
}

HRESULT CUI_KillName::Initialize_Prototype()
{
	Create_KillText();

	return S_OK;
}

HRESULT CUI_KillName::Initialize()
{
	return S_OK;
}

HRESULT CUI_KillName::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_KillName::Enable_KillName(wstring enermyName)
{
	m_pKillTextUI[Text_Name]->Set_FontText(enermyName);

	_float fTextHalfSize = (enermyName.size() * 0.5f) * 10.f;
	_float fPosX = m_pKillTextUI[Text_Name]->Get_PosX();
	_float fBenchmarkPosX = fPosX + fTextHalfSize;
	_float fAttackerPosX = fBenchmarkPosX + 35.f;

	m_pKillTextUI[Text_Kill]->Set_PosX(fAttackerPosX);

	SetActive_KillName(true);
}

void CUI_KillName::Set_OriginPosY()
{
	m_pKillTextUI[Text_Name]->Set_PosY(m_fOriginPosY);
}

void CUI_KillName::SetActive_KillName(_bool value)
{
	for (int i = 0; i < Text_End; ++i)
	{
		if (value == true)
		{
			Enable_Fade(m_pKillTextUI[i], m_fFadeTime);
		}
		else
		{
			Disable_Fade(m_pKillTextUI[i], m_fFadeTime);
		}
	}
}

void CUI_KillName::My_Tick()
{
	__super::My_Tick();

	_float4 vKillNamePos = m_pKillTextUI[Text_Name]->Get_Pos();
	m_pKillTextUI[Text_Kill]->Set_PosY(vKillNamePos.y);

	if (!m_bIsDisable)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > m_fDisableTime)
		{
			m_fAccTime = 0.f;
			m_bIsDisable = true;

			SetActive_KillName(false);
		}
	}
	else
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > m_fFadeTime)
		{
			this->SetActive(false);
		}
	}
}

void CUI_KillName::OnEnable()
{
	__super::OnEnable();

	SetActive_KillName(true);
}

void CUI_KillName::OnDisable()
{
	__super::OnDisable();

	m_bIsDisable = false;
}

void CUI_KillName::Create_KillText()
{
	for (int i = 0; i < Text_End; ++i)
	{
		m_pKillTextUI[i] = CUI_Object::Create();

		m_pKillTextUI[i]->Set_FadeDesc(m_fFadeTime);

		switch (i)
		{
		case Text_Name:

			m_pKillTextUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pKillTextUI[i]->Set_FontRender(true);
			m_pKillTextUI[i]->Set_FontStyle(true);
			m_pKillTextUI[i]->Set_FontCenter(true);
			m_pKillTextUI[i]->Set_FontScale(0.3f);
			m_pKillTextUI[i]->Set_FontOffset(0.f, -0.5f);
			m_pKillTextUI[i]->Set_FontColor(vColorRed);

			break;

		case Text_Kill:

			m_pKillTextUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Kill.png"));
			m_pKillTextUI[i]->Set_Scale(70.f, 48.f);
			m_pKillTextUI[i]->Set_Sort(0.5f);

			break;
		}

		CREATE_GAMEOBJECT(m_pKillTextUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pKillTextUI[i]);
	}
}