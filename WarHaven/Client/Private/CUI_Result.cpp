#include "CUI_Result.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"

CUI_Result::CUI_Result()
{
}

CUI_Result::~CUI_Result()
{
}

HRESULT CUI_Result::Initialize_Prototype()
{
	Create_ResultUI();

	return S_OK;
}

HRESULT CUI_Result::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Result::OnEnable()
{
	__super::OnEnable();
}

void CUI_Result::OnDisable()
{
	__super::OnDisable();
}

void CUI_Result::Create_ResultUI()
{
	for (int i = 0; i < Result_End; ++i)
	{
		m_pResultUI[i] = CUI_Object::Create();

		m_pResultUI[i]->Set_FadeDesc(0.3f);

		switch (i)
		{
		case Result_BG:

			GET_COMPONENT_FROM(m_pResultUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pResultUI[i], "/Result", "Result");

			m_pResultUI[i]->Set_Scale(1280.f, 720.f);
			m_pResultUI[i]->Set_Sort(0.2f);

			break;

		case Result_TextBG0:

			m_pResultUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/T_BgHero2.dds"));

			m_pResultUI[i]->Set_PosY(350.f);
			m_pResultUI[i]->Set_Scale(750.f);
			m_pResultUI[i]->Set_Sort(0.19f);

			break;

		case Result_TextBG1:

			m_pResultUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/T_BgHero3.dds"));
			m_pResultUI[i]->Set_PosY(350.f);
			m_pResultUI[i]->Set_Scale(750.f);
			m_pResultUI[i]->Set_Sort(0.19f);

			break;

		case Result_Text0:

			GET_COMPONENT_FROM(m_pResultUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pResultUI[i], "/Result", "Text");

			m_pResultUI[i]->Set_PosY(350.f);
			m_pResultUI[i]->Set_Scale(360.f, 211.f);
			m_pResultUI[i]->Set_Sort(0.15f);

			break;

		case Result_Text1:

			GET_COMPONENT_FROM(m_pResultUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pResultUI[i], "/Result", "Text");

			m_pResultUI[i]->Set_PosY(200.f);
			m_pResultUI[i]->Set_Scale(360.f, 211.f);
			m_pResultUI[i]->Set_Sort(0.15f);

			break;

		case Result_Line:

			m_pResultUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/T_GlowLineThin.png"));
			m_pResultUI[i]->Set_PosY(100.f);
			m_pResultUI[i]->Set_Scale(500.f, 70.f);
			m_pResultUI[i]->Set_Sort(0.15f);

			break;
		}

		CREATE_GAMEOBJECT(m_pResultUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pResultUI[i]);
	}
}
