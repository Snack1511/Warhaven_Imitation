#include "CUI_Popup.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CFader.h"
CUI_Popup::CUI_Popup()
{
}

CUI_Popup::~CUI_Popup()
{
}

HRESULT CUI_Popup::Initialize_Prototype()
{
	Create_ConquestPopup();

	return S_OK;
}

HRESULT CUI_Popup::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Popup::Enable_Popup(ePOPUP_TYPE ePopupType)
{
	switch (ePopupType)
	{
	case Client::CUI_Popup::eConquest:
		
	{
		for (int i = 0; i < CP_End; ++i)
		{
			m_pConquestPopup[i]->Set_FadeDesc(0.2f, 0.2f, 1.f, true);
		}

		_bool bIsFontRender = m_pConquestPopup[CP_BG]->Get_FontRender();
		if (bIsFontRender)
			m_pConquestPopup[CP_BG]->Set_FontText(L"거점 점령");

		for (int i = 0; i < CP_ICON; ++i)
		{
			Enable_Fade(m_pConquestPopup[i], 0.15f);
		}
	}
		

		return;
	case Client::CUI_Popup::eKILL:
		Enable_ConquestPopup(L"적 처치", ePopupType);
		break;

	case Client::CUI_Popup::eKILL2:
		Enable_ConquestPopup(L"2연속 처치", ePopupType);
		break;
	case Client::CUI_Popup::eKILL3:
		Enable_ConquestPopup(L"3연속 처치", ePopupType);
		break;
	case Client::CUI_Popup::eKILL4:
		Enable_ConquestPopup(L"전장의 화신", ePopupType);
		break;
	case Client::CUI_Popup::eBURGERKING:
		Enable_ConquestPopup(L"버거킹", ePopupType);
		break;
	case Client::CUI_Popup::eHEADHUNTER:
		Enable_ConquestPopup(L"헤드 헌터", ePopupType);
		break;
	case Client::CUI_Popup::eKILLELITE:
		Enable_ConquestPopup(L"영웅 처치", ePopupType);
		break;
	case Client::CUI_Popup::eCNT:
		break;
	default:
		break;
	}

	for (int i = 0; i < CP_End; ++i)
	{
		m_pConquestPopup[i]->Set_FadeDesc(0.3f, 0.3f, 2.f, true);
	}
}

void CUI_Popup::Enable_ConquestPopup(wstring Text, _uint iIconIndex)
{
	_bool bIsFontRender = m_pConquestPopup[CP_BG]->Get_FontRender();
	if (bIsFontRender)
		m_pConquestPopup[CP_BG]->Set_FontText(Text);

	GET_COMPONENT_FROM(m_pConquestPopup[CP_ICON], CTexture)->Set_CurTextureIndex(iIconIndex);

	for (int i = 0; i < CP_End; ++i)
	{
		Enable_Fade(m_pConquestPopup[i], 1.f);
	}
}

void CUI_Popup::OnEnable()
{
	__super::OnEnable();
}

void CUI_Popup::OnDisable()
{
	__super::OnDisable();
}

void CUI_Popup::Create_ConquestPopup()
{
	for (int i = 0; i < CP_End; ++i)
	{
		m_pConquestPopup[i] = CUI_Object::Create();

		m_pConquestPopup[i]->Set_FadeDesc(0.15f, 0.15f, 1.f, true);

		m_pConquestPopup[i]->Set_PosY(200.f);

		switch (i)
		{
		case CP_Line:

			m_pConquestPopup[i]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

			m_pConquestPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_PopupLine.png"));

			m_pConquestPopup[i]->Set_Scale(300.f, 51.f);
			m_pConquestPopup[i]->Set_Sort(0.55f);

			break;

		case CP_Blur:

			m_pConquestPopup[i]->Set_UIShaderFlag(SH_UI_HARDBLOOM);

			m_pConquestPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_PopupBlur.dds"));

			m_pConquestPopup[i]->Set_Color(_float4(1.f, 0.9f, 0.8f, 1.f));

			m_pConquestPopup[i]->Set_Scale(160.f);
			m_pConquestPopup[i]->Set_Sort(0.55f);

			break;

		case CP_BG:

			m_pConquestPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_PopupIconbox.dds"));

			m_pConquestPopup[i]->Set_Scale(80.f);
			m_pConquestPopup[i]->Set_Sort(0.54f);

			m_pConquestPopup[i]->Set_FontRender(true);
			m_pConquestPopup[i]->Set_FontStyle(true);
			m_pConquestPopup[i]->Set_FontCenter(true);
			m_pConquestPopup[i]->Set_FontOffset(3.f, 75.f);
			m_pConquestPopup[i]->Set_FontScale(0.3f);

			break;

		case CP_ICON:

			m_pConquestPopup[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_0.png"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_2.png"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_3.png"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/Kill_4.png"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowBlock.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowFoothold.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowHeadshot.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowKill.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowKIllElite.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_GlowRevenge.dds"));
			GET_COMPONENT_FROM(m_pConquestPopup[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_HighValueTargetPlayer.dds"));


			m_pConquestPopup[i]->Set_PosY(210.f);

			m_pConquestPopup[i]->Set_Scale(60.f);
			m_pConquestPopup[i]->Set_Sort(0.53f);

			m_pConquestPopup[i]->Set_FontRender(true);
			m_pConquestPopup[i]->Set_FontStyle(true);
			m_pConquestPopup[i]->Set_FontCenter(true);
			m_pConquestPopup[i]->Set_FontOffset(3.f, 75.f);
			m_pConquestPopup[i]->Set_FontScale(0.3f);

			break;
		}

		CREATE_GAMEOBJECT(m_pConquestPopup[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pConquestPopup[i]);
	}
}
