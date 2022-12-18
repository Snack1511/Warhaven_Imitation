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

//void CUI_KillName::Create_KillText()
//{
//	for (int i = 0; i < Text_End; ++i)
//	{
//		m_pKillTextUI[i] = CUI_Object::Create();
//
//		m_pKillTextUI[i]->Set_FadeDesc(m_fFadeTime);
//
//		switch (i)
//		{
//		case Text_Name:
//
//			m_pKillTextUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
//			m_pKillTextUI[i]->Set_FontRender(true);
//			m_pKillTextUI[i]->Set_FontStyle(true);
//			m_pKillTextUI[i]->Set_FontCenter(true);
//			m_pKillTextUI[i]->Set_FontScale(0.3f);
//			m_pKillTextUI[i]->Set_FontOffset(0.f, -0.5f);
//			m_pKillTextUI[i]->Set_FontColor(vColorRed);
//
//			break;
//
//		case Text_Kill:
//
//			m_pKillTextUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Kill.png"));
//			m_pKillTextUI[i]->Set_Scale(70.f, 48.f);
//			m_pKillTextUI[i]->Set_Sort(0.5f);
//
//			break;
//		}
//
//		CREATE_GAMEOBJECT(m_pKillTextUI[i], GROUP_UI);
//		DISABLE_GAMEOBJECT(m_pKillTextUI[i]);
//	}
//}