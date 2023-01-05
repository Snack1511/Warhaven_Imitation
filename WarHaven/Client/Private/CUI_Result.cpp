#include "CUI_Result.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "Easing_Utillity.h"
#include "CUser.h"

CUI_Result::CUI_Result()
{
}

CUI_Result::~CUI_Result()
{
}

HRESULT CUI_Result::Initialize_Prototype()
{
	Create_ResultUI();
	Create_ResultScoreBG();
	Create_ResultMVP();
	Create_ResultScoreList();

	return S_OK;
}

HRESULT CUI_Result::Start()
{
	__super::Start();

	Init_ResultScoreBG();
	Init_ResultMVP();
	Init_ResultScoreList();

	return S_OK;
}

void CUI_Result::SetActive_Result(_uint iResult, _bool value)
{
	m_iResult = iResult;

	CUser::Get_Instance()->SetActive_PadenUI(false);

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

	m_fScoreTime += fDT(0);
	if (m_fScoreTime < 3.f)
	{
		m_fAccTime += fDT(0);
		if (m_pResultUI[Result_TextBG0]->Is_Valid())
			m_pResultUI[Result_TextBG0]->Set_RotationZ(m_fAccTime);

		if (m_pResultUI[Result_TextBG1]->Is_Valid())
			m_pResultUI[Result_TextBG1]->Set_RotationZ(m_fAccTime);

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
	else
	{
		SetActive_Result(m_iResult, false);

		for (int i = 0; i < Score_End; ++i)
		{
			m_pResultScoreBG[i]->SetActive(true);
		}

		for (int i = 0; i < MVP_End; ++i)
		{
			m_pResultScoreBG[i]->SetActive(true);
		}

		for (int j = 0; j < Score_End; ++j)
		{
			m_pResultScoreBG[j]->SetActive(true);
		}
	}
}

void CUI_Result::OnEnable()
{
	__super::OnEnable();

	// SetActive_Result(true);
}

void CUI_Result::OnDisable()
{
	__super::OnDisable();

	m_bLerpLine = false;
	m_bLerpText0 = false;
	m_bLerpText1 = false;

	SetActive_Result(0, false);
}

void CUI_Result::Create_ResultScoreBG()
{
	for (int i = 0; i < Score_End; ++i)
	{
		m_pResultScoreBG[i] = CUI_Object::Create();

		m_pResultScoreBG[i]->Set_Sort(0.19f);

		// 원 추가

		switch (i)
		{
		case Score_BG:
			m_pResultScoreBG[i]->Set_Sort(0.2f);
			m_pResultScoreBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_ScoreBoardBg.dds"));
			m_pResultScoreBG[i]->Set_Scale(1280.f, 720.f);
			m_pResultScoreBG[i]->Set_Color(_float4(0.3f, 0.3f, 0.3f, 1.f));
			break;

		case Score_Result:
			m_pResultScoreBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Result/Text0_Win.png"));
			GET_COMPONENT_FROM(m_pResultScoreBG[i], CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/Result/Text1_Lose.png"));
			m_pResultScoreBG[i]->Set_Scale(222.f, 130.f);
			m_pResultScoreBG[i]->Set_PosY(300.f);
			m_pResultScoreBG[i]->Set_Color(m_vColorGold);
			break;

		case Score_Text:
			m_pResultScoreBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pResultScoreBG[i]->Set_Scale(1280.f, 720.f);
			m_pResultScoreBG[i]->Set_PosY(265.f);

			m_pResultScoreBG[i]->Set_FontRender(true);
			m_pResultScoreBG[i]->Set_FontStyle(true);
			m_pResultScoreBG[i]->Set_FontCenter(true);
			m_pResultScoreBG[i]->Set_FontOffset(3.f, 3.f);
			m_pResultScoreBG[i]->Set_FontScale(0.3f);
			m_pResultScoreBG[i]->Set_FontColor(m_vColorGold);
			break;
		}
	}
}

void CUI_Result::Create_ResultMVP()
{
	// 디졸브로 원래 이미지 지우고 뒤에 이미지 등장
	// 뒤에 글로우 확대 

	for (int i = 0; i < MVP_End; ++i)
	{
		m_pResultMVP[i] = CUI_Object::Create();

		m_pResultMVP[i]->Set_Sort(0.19f);
		m_pResultMVP[i]->Set_PosX(-450.f);

		switch (i)
		{
		case MVP_Text:
			m_pResultScoreBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pResultMVP[i]->Set_PosY(-225.f);

			m_pResultMVP[i]->Set_FontRender(true);
			m_pResultMVP[i]->Set_FontStyle(true);
			m_pResultMVP[i]->Set_FontCenter(true);
			m_pResultMVP[i]->Set_FontScale(0.5f);
			m_pResultMVP[i]->Set_FontText(TEXT("최고의 플레이어"));
			break;

		case MVP_Player:
			m_pResultMVP[i]->Set_Scale(178.f, 400.f);

			m_pResultMVP[i]->Set_FontRender(true);
			m_pResultMVP[i]->Set_FontStyle(true);
			m_pResultMVP[i]->Set_FontCenter(true);
			m_pResultMVP[i]->Set_FontScale(0.5f);
			m_pResultScoreBG[i]->Set_FontOffset(0.f, 230.f);
			break;
		}
	}
}

void CUI_Result::Create_ResultScoreList()
{
	for (int i = 0; i < List_End; ++i)
	{
		m_pResultScoreList[i] = CUI_Object::Create();

		m_pResultScoreList[i]->Set_Sort(0.19f);
		// Blue		100
		// Team		-285
		// Score	-50
		// Kill		25
		// Dead		85
		// Red		350

		m_pResultScoreList[i]->Set_Scale(40.f);

		// 라인 추가

		switch (i)
		{
		case List_BG:
			m_pResultScoreList[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_GradientSmall3.dds"));
			m_pResultScoreList[i]->Set_Scale(400.f);
			m_pResultScoreList[i]->Set_IsSlice(true);
			m_pResultScoreList[i]->Set_SliceRatio(_float4(0.f, 0.f, 0.f, 0.9f));
			break;

		case List_Team:
			m_pResultScoreList[i]->Set_FontRender(true);
			m_pResultScoreList[i]->Set_FontStyle(true);
			m_pResultScoreList[i]->Set_FontScale(0.4f);
			m_pResultScoreList[i]->Set_FontOffset(35.f, -20.f);
			break;

		case List_Score:
			break;

		case List_Kill:
			break;

		case List_Dead:
			break;
		}

		for (int j = 0; j < Team_End; ++j)
		{
			m_pArrResultScoreList[j][i] = m_pResultScoreList[i]->Clone();
		}
	}
}

void CUI_Result::Init_ResultScoreBG()
{
	for (int i = 0; i < Score_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pResultScoreBG[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pResultScoreBG[i]);
	}
}

void CUI_Result::Init_ResultMVP()
{
	for (int i = 0; i < MVP_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pResultMVP[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pResultMVP[i]);
	}
}

void CUI_Result::Init_ResultScoreList()
{
	for (int i = 0; i < List_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pResultScoreList[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pResultScoreList[i]);

		for (int j = 0; j < Team_End; ++j)
		{
			CREATE_GAMEOBJECT(m_pArrResultScoreList[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrResultScoreList[j][i]);
		}
	}
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
