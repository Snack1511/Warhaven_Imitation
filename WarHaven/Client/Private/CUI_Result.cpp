#include "CUI_Result.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "Easing_Utillity.h"
#include "CUser.h"
#include "CUI_ScoreInfo.h"
#include "CPlayer.h"
#include "CPlayerInfo.h"
#include "CUI_Renderer.h"
#include "CShader.h"

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
	Create_Fade();

	return S_OK;
}

HRESULT CUI_Result::Start()
{
	__super::Start();

	Init_ResultScoreBG();
	Init_ResultMVP();
	Init_ResultScoreList();

	GET_COMPONENT_FROM(m_pResultMVP[MVP_Blind], CShader)->CallBack_SetRawValues += bind(&CUI_Result::Set_Shader_MVP, this, placeholders::_1, "g_fValue");

	return S_OK;
}

void CUI_Result::Set_Shader_MVP(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fDissolveValue, sizeof(_float));
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

	Progress_Result();
}

void CUI_Result::OnEnable()
{
	__super::OnEnable();

	SetActive_Result(m_iResult, true);
}

void CUI_Result::OnDisable()
{
	__super::OnDisable();

	m_bLerpLine = false;
	m_bLerpText0 = false;
	m_bLerpText1 = false;

	SetActive_Result(m_iResult, false);

	for (int i = 0; i < Score_End; ++i)
	{
		m_pResultScoreBG[i]->SetActive(false);
	}

	for (int i = 0; i < MVP_End; ++i)
	{
		m_pResultMVP[i]->SetActive(false);
	}

	for (int i = 0; i < Team_End; ++i)
	{
		for (int j = 0; j < Score_End; ++j)
		{
			m_pArrResultScoreList[i][j]->SetActive(false);
		}
	}
}

void CUI_Result::Create_Fade()
{
	m_pFade = CUI_Object::Create();

	m_pFade->Set_FadeDesc(0.3f, 0.3f, 0.4f, true);

	m_pFade->Set_Sort(0.f);
	m_pFade->Set_Scale(1280.f, 720.f);
	m_pFade->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Rect/Black.png"));

	m_pBG = CUI_Object::Create();

	m_pBG->Set_FadeDesc(1.f);

	m_pBG->Set_Sort(0.15f);
	m_pBG->Set_Scale(1280.f, 720.f);
	m_pBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Rect/White.png"));
	m_pBG->Set_Color(_float4(0.f, 0.f, 0.f, 0.8f));

	CREATE_GAMEOBJECT(m_pFade, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pFade);

	CREATE_GAMEOBJECT(m_pBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pBG);
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
			m_pResultScoreBG[i]->Set_Color(_float4(1.f, 1.f, 1.f, 1.f));
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

		case Score_Circle0:
			m_pResultScoreBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_BgHero2.dds"));
			m_pResultScoreBG[i]->Set_Scale(1024.f);
			m_pResultScoreBG[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.1f));
			break;

		case Score_Circle1:
			m_pResultScoreBG[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_BgHero3.dds"));
			m_pResultScoreBG[i]->Set_Scale(1024.f);
			m_pResultScoreBG[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.1f));
			break;
		}
	}
}

void CUI_Result::Create_ResultMVP()
{
	// 뒤에 글로우 확대 

	for (int i = 0; i < MVP_End; ++i)
	{
		m_pResultMVP[i] = CUI_Object::Create();

		m_pResultMVP[i]->Set_Sort(0.19f);
		m_pResultMVP[i]->Set_PosX(-450.f);

		switch (i)
		{
		case MVP_Text:
			m_pResultMVP[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
			m_pResultMVP[i]->Set_PosY(225.f);

			m_pResultMVP[i]->Set_FontRender(true);
			m_pResultMVP[i]->Set_FontStyle(true);
			m_pResultMVP[i]->Set_FontCenter(true);
			m_pResultMVP[i]->Set_FontScale(0.5f);
			m_pResultMVP[i]->Set_FontText(TEXT("최고의 플레이어"));
			break;

		case MVP_Blind:
			m_pResultMVP[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Profile/T_ProfileBg0010.dds"));
			m_pResultMVP[i]->SetTexture(TEXT("../Bin/Resources/Textures/UI/Result/T_Pattern_06.dds"));
			m_pResultMVP[i]->Set_Scale(456.f);
			m_pResultMVP[i]->Set_Sort(0.18f);
			m_pResultMVP[i]->Set_UIShaderFlag(SH_UI_BLOOM);
			GET_COMPONENT_FROM(m_pResultMVP[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_Dissolve);

			break;

		case MVP_Player:

			m_pResultMVP[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Profile/T_ProfileCardDefaultWarrior.dds"));
			GET_COMPONENT_FROM(m_pResultMVP[i], CTexture)->Add_Texture(L"../Bin/Resources/Textures/UI/Profile/T_ProfileCardDefaultSpearman.dds");
			GET_COMPONENT_FROM(m_pResultMVP[i], CTexture)->Add_Texture(L"../Bin/Resources/Textures/UI/Profile/T_ProfileCardDefaultArcher.dds");
			GET_COMPONENT_FROM(m_pResultMVP[i], CTexture)->Add_Texture(L"../Bin/Resources/Textures/UI/Profile/T_ProfileCardDefaultPaladin.dds");
			GET_COMPONENT_FROM(m_pResultMVP[i], CTexture)->Add_Texture(L"../Bin/Resources/Textures/UI/Profile/T_ProfileCardDefaultPriest.dds");
			GET_COMPONENT_FROM(m_pResultMVP[i], CTexture)->Add_Texture(L"../Bin/Resources/Textures/UI/Profile/T_ProfileCardDefaultEngineer.dds");

			m_pResultMVP[i]->Set_Scale(178.f, 400.f);

			m_pResultMVP[i]->Set_FontStyle(true);
			m_pResultMVP[i]->Set_FontCenter(true);
			m_pResultMVP[i]->Set_FontScale(0.5f);
			m_pResultMVP[i]->Set_FontOffset(0.f, 230.f);
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

		m_pResultScoreList[i]->Set_Scale(40.f);
		m_pResultScoreList[i]->Set_PosY(220.f);
		m_pResultScoreList[i]->Set_Color(m_vColorGold);

		switch (i)
		{
		case List_BG:
			m_pResultScoreList[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_GradientSmall3.dds"));
			m_pResultScoreList[i]->Set_Scale(400.f);
			m_pResultScoreList[i]->Set_PosY(0.f);
			m_pResultScoreList[i]->Set_IsSlice(true);
			m_pResultScoreList[i]->Set_SliceRatio(_float4(0.f, 0.f, 0.f, 0.9f));
			break;

		case List_Line:
			m_pResultScoreList[i]->Set_Color(m_vColorGold);
			m_pResultScoreList[i]->Set_PosY(200.f);
			m_pResultScoreList[i]->Set_Scale(400.f, 1.f);
			break;

		case List_Team:
			m_pResultScoreList[i]->Set_FontRender(true);
			m_pResultScoreList[i]->Set_FontStyle(true);
			m_pResultScoreList[i]->Set_FontScale(0.4f);
			m_pResultScoreList[i]->Set_FontColor(m_vColorGold);
			m_pResultScoreList[i]->Set_FontOffset(35.f, -20.f);
			break;

		case List_Kill:
			m_pResultScoreList[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_ScoreBoardIconKill.dds"));
			break;

		case List_Dead:
			m_pResultScoreList[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_ScoreIconDeath.dds"));
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

	m_pArrResultScoreList[Team_Blue][List_Team]->Set_FontText(TEXT("아군"));
	m_pArrResultScoreList[Team_Blue][List_Team]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_64MaraSymbolIcon.dds"));
	m_pArrResultScoreList[Team_Blue][List_BG]->Set_Color(m_vColorBlue);
	_float fBluePosX = -100.f;
	m_pArrResultScoreList[Team_Blue][List_BG]->Set_PosX(fBluePosX);
	m_pArrResultScoreList[Team_Blue][List_Line]->Set_PosX(fBluePosX);
	m_pArrResultScoreList[Team_Blue][List_Team]->Set_PosX(fBluePosX - 185.f);
	m_pArrResultScoreList[Team_Blue][List_Kill]->Set_PosX(fBluePosX + 125.f);
	m_pArrResultScoreList[Team_Blue][List_Dead]->Set_PosX(fBluePosX + 185.f);

	m_pArrResultScoreList[Team_Red][List_Team]->Set_FontText(TEXT("적군"));
	m_pArrResultScoreList[Team_Red][List_Team]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_64UnionSymbolIcon.png"));
	m_pArrResultScoreList[Team_Red][List_BG]->Set_Color(m_vColorRed);
	_float fRedPosX = 350.f;
	m_pArrResultScoreList[Team_Red][List_BG]->Set_PosX(fRedPosX);
	m_pArrResultScoreList[Team_Red][List_Line]->Set_PosX(fRedPosX);
	m_pArrResultScoreList[Team_Red][List_Team]->Set_PosX(fRedPosX - 185.f);
	m_pArrResultScoreList[Team_Red][List_Kill]->Set_PosX(fRedPosX + 125.f);
	m_pArrResultScoreList[Team_Red][List_Dead]->Set_PosX(fRedPosX + 185.f);
}

void CUI_Result::Progress_Result()
{
	if (m_iResultProgressCnt == 0)
	{
		for (int i = Result_TextBG0; i < Result_Text0; ++i)
			m_pResultUI[i]->Set_RotationZ(m_fRotValue);

		if (!m_bLerpLine)
		{
			m_bLerpLine = true;
			m_pResultUI[Result_Line]->Lerp_ScaleX(0.f, 550.f, 1.f);
		}

		if (!m_bLerpText0)
		{
			m_fTextLerpRatio += fDT(0);
			if (m_fTextLerpRatio > 1.f)
			{
				m_fTextLerpRatio = 0.f;
				m_bLerpText0 = true;

				Enable_Fade(m_pResultUI[Result_Text1], 0.3f);
			}

			_float4 vOrigin = _float4(720.f, 422.f, 0.f);
			_float4 vTarget = _float4(360.f, 211.f, 0.f);
			_float4 vResult = CEasing_Utillity::Linear(vOrigin, vTarget, m_fTextLerpRatio, 1.f);
			m_pResultUI[Result_Text0]->Set_Scale(vResult.x, vResult.y);
		}
		else
		{
			if (!m_bLerpText1)
			{
				m_fTextLerpRatio += fDT(0);
				if (m_fTextLerpRatio > 1.f)
				{
					m_fTextLerpRatio = 0.f;
					m_bLerpText1 = true;

					Disable_Fade(m_pResultUI[Result_Text1], 0.3f);
				}

				_float4 vOrigin = _float4(360.f, 211.f, 0.f);
				_float4 vTarget = _float4(720.f, 422.f, 0.f);
				_float4 vResult = CEasing_Utillity::Linear(vOrigin, vTarget, m_fTextLerpRatio, 1.f);

				m_pResultUI[Result_Text1]->Set_Scale(vResult.x, vResult.y);
			}
			else
			{
				m_fAccTime += fDT(0);
				if (m_fAccTime > 3.f)
				{
					m_fAccTime = 0.f;
					m_iResultProgressCnt++;
				}
			}
		}
	}
	else if (m_iResultProgressCnt == 1)
	{
		Enable_Fade(m_pFade, 0.3f);
		m_pScoreInfoMap = CUser::Get_Instance()->Get_ScoreInfoMap();

		m_fAccTime += fDT(0);
		if (m_fAccTime > 0.5f)
		{
			m_fAccTime = 0.f;
			m_iResultProgressCnt++;
		}
	}
	else if (m_iResultProgressCnt == 2)
	{
		for (int i = 0; i < Result_End; ++i)
			Disable_Fade(m_pResultUI[i], 0.3f);

		m_pResultScoreBG[Score_Result]->Set_TextureIndex(m_iResult);

		wstring wstText = m_iResult == 0 ? TEXT("군사력 우세") : TEXT("군사력 열세");
		m_pResultScoreBG[Score_Text]->Set_FontText(wstText);

		for (auto& pair : m_pScoreInfoMap)
		{
			pair.second.sort([](CUI_ScoreInfo* p1, CUI_ScoreInfo* p2)
				{
					return p1->Get_KillCnt() > p2->Get_KillCnt();
				});
		}

		m_fAccTime += fDT(0);
		if (m_fAccTime > 0.5f)
		{
			m_fAccTime = 0.f;
			m_iResultProgressCnt++;
		}
	}
	else if (m_iResultProgressCnt == 3)
	{
		for (int i = 0; i < Score_End; ++i)
			m_pResultScoreBG[i]->SetActive(true);

		for (int i = 0; i < MVP_End; ++i)
			m_pResultMVP[i]->SetActive(true);

		for (int i = 0; i < Team_End; ++i)
		{
			for (int j = 0; j < List_End; ++j)
				m_pArrResultScoreList[i][j]->SetActive(true);
		}

		for (auto& pair : m_pScoreInfoMap)
		{
			for (auto& iter : pair.second)
			{
				iter->Set_Type(1);
				iter->Set_ScaleX(400.f);
				iter->SetActive(true);
			}
		}

		_uint iBlueKill = m_pScoreInfoMap[0].front()->Get_KillCnt();
		_uint iRedKill = m_pScoreInfoMap[1].front()->Get_KillCnt();

		_uint iTeam = iBlueKill > iRedKill ? 0 : 1;

		CPlayer* pMVP = m_pScoreInfoMap[iTeam].front()->Get_OwnerPlayer();

		wstring wstrPlayerName = pMVP->Get_PlayerInfo()->Get_PlayerName();
		_uint iClassNum = pMVP->Get_PlayerInfo()->Get_ChonsenClass();

		m_pResultMVP[MVP_Player]->Set_FontText(wstrPlayerName);
		m_pResultMVP[MVP_Player]->Set_TextureIndex(iClassNum);

		m_fAccTime += fDT(0);
		if (m_fAccTime > 1.5f)
		{
			m_fAccTime = 0.f;
			m_iResultProgressCnt++;
		}
	}
	else if (m_iResultProgressCnt == 4)
	{
		m_fDissolveValue -= fDT(0) * 0.3f;
		if (m_fDissolveValue < 0.f)
		{
			m_fDissolveValue = 0.f;
			m_iResultProgressCnt++;

			m_pResultMVP[MVP_Player]->Set_FontRender(true);
		}
	}
	else if (m_iResultProgressCnt == 5)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_iResultProgressCnt++;

			Enable_Fade(m_pBG, 0.3f);
		}
	}
	else if (m_iResultProgressCnt == 6)
	{
		m_fAccTime += fDT(0);
		if (m_fAccTime > 1.f)
		{
			m_fAccTime = 0.f;
			m_iResultProgressCnt++;

			CUser::Get_Instance()->Enable_SkinPopup(1);
		}
	}
}