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
	Create_KillNameUI();

	return S_OK;
}

HRESULT CUI_KillName::Initialize()
{
	return S_OK;
}

HRESULT CUI_KillName::Start()
{
	__super::Start();

	for (int i = 0; i < Text_End; ++i)
	{
		m_pKillNameUI[i]->Set_Pos(m_vStartPosition);
	}

	return S_OK;
}

void CUI_KillName::Set_KillName(wstring wstrKillName)
{
	m_fAccTime = 0.f;

	m_pKillNameUI[Text_Name]->Set_PosY(m_vStartPosition.y);

	ENABLE_GAMEOBJECT(this);

	m_pKillNameUI[Text_Name]->Set_FontText(wstrKillName);

	_float fNameHalfSize = m_pKillNameUI[Text_Name]->Get_FontSizeX() * 0.5f;
	_float fKillHalfSize = m_pKillNameUI[Text_Kill]->Get_Scale().x * 0.5f;
	_float fKillPosX = fNameHalfSize + fKillHalfSize;

	m_pKillNameUI[Text_Kill]->Set_PosX(fKillPosX);
}

void CUI_KillName::My_Tick()
{
	__super::My_Tick();

	_float4 vTargetPos = m_vStartPosition;
	vTargetPos += (m_fStepY * (_float)m_iIndex);

	_float fCurPosY = m_pKillNameUI[Text_Name]->Get_PosY();
	if (fCurPosY < vTargetPos.y)
	{
		fCurPosY += (m_fStepY * 2.f) * fDT(0);
		if (fCurPosY > vTargetPos.y)
			fCurPosY = vTargetPos.y;
	}

	for (int i = 0; i < Text_End; ++i)
		m_pKillNameUI[i]->Set_PosY(fCurPosY);

	m_fAccTime += fDT(0);
	if (!m_bIsDisable)
	{
		if (m_fAccTime > m_fDisableTime)
		{
			m_fAccTime = 0.f;
			m_bIsDisable = true;

			SetActive_KillName(false);
		}
	}
	else
	{
		if (m_fAccTime > m_fFadeTime)
		{
			m_fAccTime = 0.f;
			SetActive(false);
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

void CUI_KillName::Create_KillNameUI()
{
	for (int i = 0; i < Text_End; ++i)
	{
		m_pKillNameUI[i] = CUI_Object::Create();

		m_pKillNameUI[i]->Set_FadeDesc(m_fFadeTime);

		switch (i)
		{
		case Text_Name:

			m_pKillNameUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pKillNameUI[i]->Set_FontRender(true);
			m_pKillNameUI[i]->Set_FontStyle(true);
			m_pKillNameUI[i]->Set_FontCenter(true);
			m_pKillNameUI[i]->Set_FontScale(0.3f);
			m_pKillNameUI[i]->Set_FontOffset(0.f, -0.5f);
			m_pKillNameUI[i]->Set_FontColor(vColorRed);

			break;

		case Text_Kill:

			m_pKillNameUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Kill.png"));
			m_pKillNameUI[i]->Set_Scale(70.f, 48.f);
			m_pKillNameUI[i]->Set_Sort(0.5f);

			break;
		}

		CREATE_GAMEOBJECT(m_pKillNameUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pKillNameUI[i]);
	}
}

void CUI_KillName::SetActive_KillName(_bool value)
{
	for (int i = 0; i < Text_End; ++i)
	{
		if (value == true)
		{
			Enable_Fade(m_pKillNameUI[i], m_fAccTime);
		}
		else
		{
			Disable_Fade(m_pKillNameUI[i], m_fAccTime);
		}
	}
}
