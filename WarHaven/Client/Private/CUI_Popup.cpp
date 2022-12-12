#include "CUI_Popup.h"
#include "GameInstance.h"
#include "CUI_Object.h"

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

void CUI_Popup::Enable_ConquestPopup(wstring Text)
{
	_bool bIsFontRender = m_pConquestPopup[CP_BG]->Get_FontRender();
	if (bIsFontRender)
		m_pConquestPopup[CP_BG]->Set_FontText(Text);

	for (int i = 0; i < CP_End; ++i)
	{
		Enable_Fade(m_pConquestPopup[i], 0.15f);
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
		}

		CREATE_GAMEOBJECT(m_pConquestPopup[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pConquestPopup[i]);
	}
}
