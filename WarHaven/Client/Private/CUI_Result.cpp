#include "CUI_Result.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "Easing_Utillity.h"

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

	SetActive_Result(0, true);

	return S_OK;
}

void CUI_Result::SetActive_Result(_uint iResult, _bool value)
{
	GET_COMPONENT_FROM(m_pResultUI[Result_BG], CTexture)->Set_CurTextureIndex(iResult);

	GET_COMPONENT_FROM(m_pResultUI[Result_Text0], CTexture)->Set_CurTextureIndex(iResult);
	GET_COMPONENT_FROM(m_pResultUI[Result_Text1], CTexture)->Set_CurTextureIndex(iResult);

	for (int i = 0; i < Result_End; ++i)
	{
		if (value == true)
		{
			Enable_Fade(m_pResultUI[i], 0.3f);
		}
		else
		{
			Disable_Fade(m_pResultUI[i], 0.3f);
		}
	}
}

void CUI_Result::SetActive_Result(_bool value)
{
	for (int i = 0; i < Result_End; ++i)
	{
		if (i == Result_Text1)
			continue;

		m_pResultUI[i]->SetActive(value);
	}
}

void CUI_Result::My_Tick()
{
	__super::My_Tick();

	m_fAccTime += fDT(0);
	if (m_pResultUI[Result_TextBG0]->Is_Valid())
	{
		m_pResultUI[Result_TextBG0]->Set_RotationZ(m_fAccTime);
	}

	if (m_pResultUI[Result_TextBG1]->Is_Valid())
	{
		m_pResultUI[Result_TextBG1]->Set_RotationZ(m_fAccTime);
	}

	if (m_pResultUI[Result_Line]->Is_Valid())
	{
		if (!m_bLerpLine)
		{
			m_bLerpLine = true;

			m_pResultUI[Result_Line]->Lerp_ScaleX(0.f, 500.f, 1.f);
		}
	}

	if (m_pResultUI[Result_Text0]->Is_Valid())
	{
		if (!m_bLerpText0)
		{
			m_fAccTime += fDT(0);
			if (m_fAccTime > 1.f)
			{
				m_fAccTime = 0.f;
				m_bLerpText0 = true;

				Enable_Fade(m_pResultUI[Result_Text1], 0.3f);
			}

			_float4 vOrigin = _float4(720.f, 422.f, 0.f);
			_float4 vTarget = _float4(360.f, 211.f, 0.f);
			_float4 vResult = CEasing_Utillity::Linear(vOrigin, vTarget, m_fAccTime, 1.f);

			m_pResultUI[Result_Text0]->Set_Scale(vResult.x, vResult.y);
		}
		else
		{
			if (!m_bLerpText1)
			{
				m_fAccTime += fDT(0);
				if (m_fAccTime > 1.f)
				{
					m_fAccTime = 0.f;
					m_bLerpText1 = true;
				}

				_float4 vOrigin = _float4(360.f, 211.f, 0.f);
				_float4 vTarget = _float4(720.f, 422.f, 0.f);
				_float4 vResult = CEasing_Utillity::Linear(vOrigin, vTarget, m_fAccTime, 1.f);

				m_pResultUI[Result_Text1]->Set_Scale(vResult.x, vResult.y);
			}
		}
	}
}

void CUI_Result::OnEnable()
{
	__super::OnEnable();

	SetActive_Result(true);
}

void CUI_Result::OnDisable()
{
	__super::OnDisable();

	m_bLerpLine = false;
	m_bLerpText0 = false;
	m_bLerpText1 = false;

	SetActive_Result(false);
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

			m_pResultUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/T_ResultWinBG.dds"));
			GET_COMPONENT_FROM(m_pResultUI[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Result/T_ResultLoseBG.dds"));

			m_pResultUI[i]->Set_Scale(1280.f, 720.f);
			m_pResultUI[i]->Set_Sort(0.2f);

			break;

		case Result_TextBG0:

			m_pResultUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/T_BgHero2.dds"));
			m_pResultUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.3f));

			m_pResultUI[i]->Set_PosY(350.f);
			m_pResultUI[i]->Set_Scale(750.f);
			m_pResultUI[i]->Set_Sort(0.19f);

			break;

		case Result_TextBG1:

			m_pResultUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/T_BgHero3.dds"));
			m_pResultUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.3f));

			m_pResultUI[i]->Set_PosY(350.f);
			m_pResultUI[i]->Set_Scale(750.f);
			m_pResultUI[i]->Set_Sort(0.19f);

			break;

		case Result_Text0:

			m_pResultUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/Text0_Win.png"));
			GET_COMPONENT_FROM(m_pResultUI[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Result/Text1_Lose.png"));

			m_pResultUI[i]->Set_PosY(200.f);
			m_pResultUI[i]->Set_Scale(360.f, 211.f);
			m_pResultUI[i]->Set_Sort(0.15f);

			break;

		case Result_Text1:

			m_pResultUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/Text0_Win.png"));
			GET_COMPONENT_FROM(m_pResultUI[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Result/Text1_Lose.png"));


			m_pResultUI[i]->Set_FadeDesc(0.3f, 0.3f, 0.f, true);

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
