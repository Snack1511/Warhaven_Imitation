#include "CUI_KillLog.h"
#include "GameInstance.h"
#include "CUI_Object.h"

CUI_KillLog::CUI_KillLog()
{
}

CUI_KillLog::~CUI_KillLog()
{
}

HRESULT CUI_KillLog::Initialize_Prototype()
{
	Create_KillText();

	return S_OK;
}

HRESULT CUI_KillLog::Initialize()
{
	return S_OK;
}

HRESULT CUI_KillLog::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_KillLog::Enable_KillUI(UI_Type eKillType)
{
	m_eKillType = eKillType;

	switch (eKillType)
	{
	case Client::CUI_KillLog::UT_Kill:

		Enable_Fade(m_pKillText, m_fFadeTime);

		break;

	case Client::CUI_KillLog::UT_Log:

		Enable_Fade(m_pKillText, m_fFadeTime);

		break;
	}
}

void CUI_KillLog::My_Tick()
{
	__super::My_Tick();

	if (!m_bIsDisable)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > m_fDisableTime)
		{
			m_fAccTime = 0.f;
			m_bIsDisable = true;

			switch (m_eKillType)
			{
			case Client::CUI_KillLog::UT_Kill:

				Disable_Fade(m_pKillText, m_fFadeTime);

				break;
			case Client::CUI_KillLog::UT_Log:

				Disable_Fade(m_pKillText, m_fFadeTime);

				break;
			}

		}
	}
}

void CUI_KillLog::OnEnable()
{
	__super::OnEnable();
}

void CUI_KillLog::OnDisable()
{
	__super::OnDisable();

	m_bIsDisable = false;
}

void CUI_KillLog::Create_KillText()
{	
	//m_pKillNameText->Set_FadeDesc(0.3f, 0.3f, 5.f, true);
	
	//m_pKillNameText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Kill.png"));
	
	//m_pKillNameText->Set_Pos(140.f, -100.f);
	//m_pKillNameText->Set_Scale(87.f, 60.f);
	//m_pKillNameText->Set_Sort(0.5f);
	
	//m_pKillNameText->Set_FontRender(true);
	//m_pKillNameText->Set_FontStyle(true);
	//m_pKillNameText->Set_FontCenter(true);
	//m_pKillNameText->Set_FontScale(0.45f);
	//m_pKillNameText->Set_FontOffset(-140.f, -3.5f);
	//m_pKillNameText->Set_FontColor(_float4(1.f, 0.2f, 0.2f, 1.f));

	m_pKillText = CUI_Object::Create();

	m_pKillText->Set_FadeDesc(m_fFadeTime);



	CREATE_GAMEOBJECT(m_pKillText, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pKillText);
}
