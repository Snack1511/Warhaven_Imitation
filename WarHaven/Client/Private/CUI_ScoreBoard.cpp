#include "CUI_ScoreBoard.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Loading_Manager.h"
#include "Texture.h"
#include "CPlayer.h"
#include "CSquad.h"
#include "CUnit.h"
#include "CUser.h"
#include "CTeamConnector.h"
#include "CUI_ScoreInfo.h"
#include "CShader.h"
#include "Transform.h"
#include "CUI_Renderer.h"

CUI_ScoreBoard::CUI_ScoreBoard()
{
}

CUI_ScoreBoard::~CUI_ScoreBoard()
{
}

HRESULT CUI_ScoreBoard::Initialize_Prototype()
{
	Create_ScoreMiniMap();
	Create_ScorePlayerList();
	Create_Squad();
	Create_PointUI();

	return S_OK;
}

HRESULT CUI_ScoreBoard::Start()
{
	__super::Start();

	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	Init_ScoreMiniMap();
	Init_ScorePlayerList();
	Init_Squad();
	Init_PointUI();

	Set_Squad();

	GET_COMPONENT_FROM(m_pArrPointUI[Point_A][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_ScoreBoard::Set_Shader_Guage_PointA, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrPointUI[Point_R][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_ScoreBoard::Set_Shader_Guage_PointR, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrPointUI[Point_C][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_ScoreBoard::Set_Shader_Guage_PointC, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrPointUI[Point_E][PU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_ScoreBoard::Set_Shader_Guage_PointE, this, placeholders::_1, "g_fValue");

	return S_OK;
}

void CUI_ScoreBoard::Set_Shader_Guage_PointA(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_A], sizeof(_float));
}

void CUI_ScoreBoard::Set_Shader_Guage_PointR(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_R], sizeof(_float));
}

void CUI_ScoreBoard::Set_Shader_Guage_PointC(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_C], sizeof(_float));
}

void CUI_ScoreBoard::Set_Shader_Guage_PointE(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fConquestRatio[Point_E], sizeof(_float));
}

void CUI_ScoreBoard::Get_ScoreInfo(CPlayer* pPlayer)
{
	CUI_ScoreInfo* pScoreInfo = pPlayer->Get_ScoreInfo();

	_bool IsMainTeam = pPlayer->Get_Team()->IsMainPlayerTeam();

	_float fPosX = IsMainTeam ? 180.f : 450.f;
	pScoreInfo->Set_PosX(fPosX);

	Team eTeam = IsMainTeam ? Team_Blue : Team_Red;
	auto pair = m_pScoreInfoMap.find(eTeam);
	if (pair == m_pScoreInfoMap.end())
	{
		list<CUI_ScoreInfo*> pScoreInfoList;
		pScoreInfoList.push_back(pScoreInfo);
		m_pScoreInfoMap.emplace(eTeam, pScoreInfoList);
	}
	else
	{
		pair->second.push_back(pScoreInfo);
	}
}

void CUI_ScoreBoard::My_Tick()
{
	__super::My_Tick();

	for (auto& pair : m_pScoreInfoMap)
	{
		auto iter = pair.second.begin();
		for (int i = 0; i < pair.second.size(); ++i)
		{
			(*iter)->Set_Rank(i);

			++iter;
		}
	}
}

void CUI_ScoreBoard::My_LateTick()
{
	__super::My_LateTick();

	if (m_eLoadLevel == LEVEL_PADEN)
	{
		if (m_pPlayer)
		{
			_float4 vPos = m_pPlayerTransform->Get_World(WORLD_POS) * 1.5f;
			vPos.z += -375.f;
			vPos.x += 15.f;
			m_pPlayerUI->Set_Pos(vPos.z, -vPos.x);
		}
	}
	else
	{
		if (m_pPlayer)
		{
			_float4 vPos = m_pPlayerTransform->Get_World(WORLD_POS) * 1.2f;
			vPos.z += -30.f;
			vPos.x += 370.f;
			m_pPlayerUI->Set_Pos(-vPos.x, vPos.z);
		}
	}
}

void CUI_ScoreBoard::OnEnable()
{
	__super::OnEnable();

	for (auto& iter : m_pScoreList)
		iter->SetActive(true);

	for (auto& pair : m_pScoreInfoMap)
	{
		for (auto& iter : pair.second)
		{
			iter->Set_ScaleX(250.f);
			iter->SetActive(true);
		}
	}

	if (m_eLoadLevel == LEVEL_PADEN)
	{
		for (int i = 0; i < PU_End; ++i)
			m_pArrPointUI[Point_E][i]->SetActive(false);
	}
}

void CUI_ScoreBoard::OnDisable()
{
	__super::OnDisable();

	for (auto& iter : m_pScoreList)
		iter->SetActive(false);

	for (auto& pair : m_pScoreInfoMap)
	{
		for (auto& iter : pair.second)
		{
			iter->SetActive(false);
		}
	}
}

map<_uint, list<CUI_ScoreInfo*>> CUI_ScoreBoard::Get_ScoreInfoMap()
{
	map<_uint, list<CUI_ScoreInfo*>> pScoreInfoMap;

	for (auto& pair : m_pScoreInfoMap)
	{
		if (pair.first == Team_Blue)
		{
			pScoreInfoMap.emplace(0, pair.second);

			for (auto& iter : pair.second)
			{
				iter->Set_PosX(-100.f);
			}
		}
		else
		{
			pScoreInfoMap.emplace(1, pair.second);

			for (auto& iter : pair.second)
			{
				iter->Set_PosX(350.f);
			}
		}
	}

	return pScoreInfoMap;
}

void CUI_ScoreBoard::Create_ScoreMiniMap()
{
	m_pScoreBG = CUI_Object::Create();

	m_pScoreBG->Set_Color(_float4(0.3f, 0.3f, 0.3f, 1.f));
	m_pScoreBG->Set_Scale(1280.f, 720.f);
	m_pScoreBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_ScoreBoardBg.dds"));
	m_pScoreBG->Set_Sort(0.4f);

	m_pPlayerUI = CUI_Object::Create();

	m_pPlayerUI->Set_Texture(TEXT("../Bin/Resources/Textures/UI/MiniMap/PlayerIcon0.dds"));
	m_pPlayerUI->Set_Scale(15.f);
	m_pPlayerUI->Set_Sort(0.3f);

	for (int i = 0; i < Map_End; ++i)
	{
		m_pScoreMiniMap[i] = CUI_Object::Create();

		m_pScoreMiniMap[i]->Set_PosX(-375.f);
		m_pScoreMiniMap[i]->Set_Scale(985.f);
		m_pScoreMiniMap[i]->Set_Sort(0.39f);

		switch (i)
		{
		case Map_Circle0:
			m_pScoreMiniMap[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_BgHero2.dds"));
			m_pScoreMiniMap[i]->Set_Color(m_vColorCircle);
			break;

		case Map_Circle1:
			m_pScoreMiniMap[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_BgHero3.dds"));
			m_pScoreMiniMap[i]->Set_Color(m_vColorCircle);
			break;

		case Map_Frame:
			m_pScoreMiniMap[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_MinimapFrame.dds"));
			m_pScoreMiniMap[i]->Set_Color(m_vColorGold);
			m_pScoreMiniMap[i]->Set_Scale(450.f);
			break;

		case Map_Map:
			m_pScoreMiniMap[i]->Set_Sort(0.38f);
			m_pScoreMiniMap[i]->Set_Scale(356.f);
			break;
		}
	}
}

void CUI_ScoreBoard::Create_ScorePlayerList()
{
	for (int i = 0; i < List_End; ++i)
	{
		m_pSocrePlayerList[i] = CUI_Object::Create();

		m_pSocrePlayerList[i]->Set_Sort(0.35f);
		m_pSocrePlayerList[i]->Set_Scale(32.f);
		m_pSocrePlayerList[i]->Set_PosY(205.f);

		switch (i)
		{
		case List_Team:
			m_pSocrePlayerList[i]->Set_Color(m_vColorGold);

			m_pSocrePlayerList[i]->Set_FontRender(true);
			m_pSocrePlayerList[i]->Set_FontStyle(true);
			m_pSocrePlayerList[i]->Set_FontOffset(35.f, -15.f);
			m_pSocrePlayerList[i]->Set_FontScale(0.3f);
			m_pSocrePlayerList[i]->Set_FontColor(m_vColorGold);
			break;

		case List_Kill:
			m_pSocrePlayerList[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_ScoreBoardIconKill.dds"));
			break;

		case List_Death:
			m_pSocrePlayerList[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_ScoreIconDeath.dds"));
			break;

		case List_Line:
			m_pSocrePlayerList[i]->Set_Color(m_vColorGold);
			m_pSocrePlayerList[i]->Set_PosY(185.f);
			m_pSocrePlayerList[i]->Set_Sort(0.34f);
			m_pSocrePlayerList[i]->Set_Scale(250.f, 1.f);
			break;

		case List_LineGlow:
			m_pSocrePlayerList[i]->Set_UIShaderFlag(SH_UI_HARDBLOOM);
			m_pSocrePlayerList[i]->Set_Color(m_vColorGold);
			m_pSocrePlayerList[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Popup/T_PopupBlur.dds"));
			m_pSocrePlayerList[i]->Set_PosY(184.f);
			m_pSocrePlayerList[i]->Set_Sort(0.35f);
			m_pSocrePlayerList[i]->Set_Scale(150.f, 16.f);
			break;

		case List_BG:
			m_pSocrePlayerList[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_GradientSmall3.dds"));
			m_pSocrePlayerList[i]->Set_PosY(0.f);
			m_pSocrePlayerList[i]->Set_Scale(250.f, 375.f);
			m_pSocrePlayerList[i]->Set_IsSlice(true);
			m_pSocrePlayerList[i]->Set_SliceRatio(_float4(0.f, 0.f, 0.f, 0.9f));
			break;
		}
	}
}

void CUI_ScoreBoard::Create_Squad()
{
	for (int i = 0; i < Squad_End; ++i)
	{
		m_pSquard[i] = CUI_Object::Create();

		m_pSquard[i]->Set_PosY(-250.f);

		switch (i)
		{
		case Squad_BG:
			m_pSquard[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Portrait/T_RoundPortraitBGSmall.dds"));
			m_pSquard[i]->Set_Sort(0.35f);
			m_pSquard[i]->Set_Scale(55.f);
			break;

		case Squad_Port:
			GET_COMPONENT_FROM(m_pSquard[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pSquard[i], "/HUD/Portrait", "Class");

			m_pSquard[i]->Set_Sort(0.34f);
			m_pSquard[i]->Set_Scale(55.f);

			m_pSquard[i]->Set_FontRender(true);
			m_pSquard[i]->Set_FontCenter(true);
			m_pSquard[i]->Set_FontOffset(5.f, 50.f);
			m_pSquard[i]->Set_FontScale(0.2f);
			break;

		case Squad_Num:
			GET_COMPONENT_FROM(m_pSquard[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pSquard[i], "/Oper", "Num");

			m_pSquard[i]->Set_Sort(0.33f);
			m_pSquard[i]->Set_Scale(16.f);
			m_pSquard[i]->Set_PosY(-265.f);
			break;
		}
	}
}

void CUI_ScoreBoard::Create_PointUI()
{
	for (int i = 0; i < PU_End; ++i)
	{
		m_pPointUI[i] = CUI_Object::Create();

		switch (i)
		{
		case PU_Outline:
			GET_COMPONENT_FROM(m_pPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pPointUI[i], "/Oper", "PointOutline");

			m_pPointUI[i]->Set_Sort(0.35f);
			m_pPointUI[i]->Set_Scale(24.f);
			break;

		case PU_Gauge:
			GET_COMPONENT_FROM(m_pPointUI[i], CTexture)->Remove_Texture(0);
			m_pPointUI[i]->Set_Scale(22.f);
			Read_Texture(m_pPointUI[i], "/Oper", "PointGauge");

			m_pPointUI[i]->Set_Sort(0.36f);
			GET_COMPONENT_FROM(m_pPointUI[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_CircleGauge);
			break;

		case PU_Text:
			GET_COMPONENT_FROM(m_pPointUI[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pPointUI[i], "/Oper", "PointText");

			m_pPointUI[i]->Set_Sort(0.33f);
			m_pPointUI[i]->Set_Scale(32.f);
			break;
		}
	}
}

void CUI_ScoreBoard::Init_ScoreMiniMap()
{
	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:
		m_pScoreMiniMap[Map_Map]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapPaden.dds"));
		break;

	case Client::LEVEL_HWARA:
		m_pScoreMiniMap[Map_Map]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapHwara.dds"));
		break;
	}

	CREATE_GAMEOBJECT(m_pScoreBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pScoreBG);

	CREATE_GAMEOBJECT(m_pPlayerUI, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pPlayerUI);

	m_pScoreList.push_back(m_pScoreBG);
	m_pScoreList.push_back(m_pPlayerUI);

	for (int i = 0; i < Map_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pScoreMiniMap[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pScoreMiniMap[i]);

		m_pScoreList.push_back(m_pScoreMiniMap[i]);
	}
}

void CUI_ScoreBoard::Init_ScorePlayerList()
{
	for (int i = 0; i < List_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pSocrePlayerList[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSocrePlayerList[i]);

		for (int j = 0; j < Team_End; ++j)
		{
			m_pArrSocrePlayerList[j][i] = m_pSocrePlayerList[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrSocrePlayerList[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrSocrePlayerList[j][i]);

			m_pScoreList.push_back(m_pArrSocrePlayerList[j][i]);
		}
	}

	m_pArrSocrePlayerList[Team_Blue][List_BG]->Set_Color(m_vColorBlue);
	m_pArrSocrePlayerList[Team_Blue][List_BG]->Set_PosX(180.f);
	_float fBluePosX = m_pArrSocrePlayerList[Team_Blue][List_BG]->Get_PosX();

	m_pArrSocrePlayerList[Team_Blue][List_Line]->Set_PosX(fBluePosX);
	m_pArrSocrePlayerList[Team_Blue][List_LineGlow]->Set_PosX(fBluePosX - 30.f);

	m_pArrSocrePlayerList[Team_Blue][List_Team]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_64MaraSymbolIcon.dds"));
	m_pArrSocrePlayerList[Team_Blue][List_Team]->Set_PosX(fBluePosX - 110.f);
	m_pArrSocrePlayerList[Team_Blue][List_Team]->Set_FontText(TEXT("아군"));

	m_pArrSocrePlayerList[Team_Blue][List_Kill]->Set_PosX(fBluePosX + 80.f);
	m_pArrSocrePlayerList[Team_Blue][List_Death]->Set_PosX(fBluePosX + 110.f);

	m_pArrSocrePlayerList[Team_Red][List_BG]->Set_Color(m_vColorRed);
	m_pArrSocrePlayerList[Team_Red][List_BG]->Set_PosX(450.f);
	_float fRedPosX = m_pArrSocrePlayerList[Team_Red][List_BG]->Get_PosX();

	m_pArrSocrePlayerList[Team_Red][List_Line]->Set_PosX(fRedPosX);
	m_pArrSocrePlayerList[Team_Red][List_LineGlow]->Set_PosX(fRedPosX - 30.f);

	m_pArrSocrePlayerList[Team_Red][List_Team]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KDA/T_64UnionSymbolIcon.png"));
	m_pArrSocrePlayerList[Team_Red][List_Team]->Set_PosX(fRedPosX - 110.f);
	m_pArrSocrePlayerList[Team_Red][List_Team]->Set_FontText(TEXT("적군"));

	m_pArrSocrePlayerList[Team_Red][List_Kill]->Set_PosX(fRedPosX + 80.f);
	m_pArrSocrePlayerList[Team_Red][List_Death]->Set_PosX(fRedPosX + 110.f);
}

void CUI_ScoreBoard::Init_Squad()
{
	for (int i = 0; i < Squad_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pSquard[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pSquard[i]);

		for (int j = 0; j < 4; ++j)
		{
			m_pArrSquard[j][i] = m_pSquard[i]->Clone();

			_float fPosX = -550.f + (j * 115.f);
			m_pArrSquard[j][i]->Set_PosX(fPosX);

			switch (i)
			{
			case Squad_Num:
				m_pArrSquard[j][i]->Set_PosX(fPosX + 20.f);
				break;
			}

			CREATE_GAMEOBJECT(m_pArrSquard[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrSquard[j][i]);

			m_pScoreList.push_back(m_pArrSquard[j][i]);
		}
	}
}

void CUI_ScoreBoard::Init_PointUI()
{
	for (int i = 0; i < PU_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pPointUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pPointUI[i]);

		for (int j = 0; j < Point_End; ++j)
		{
			m_pArrPointUI[j][i] = m_pPointUI[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrPointUI[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrPointUI[j][i]);

			m_pScoreList.push_back(m_pArrPointUI[j][i]);
		}
	}

	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		for (int i = 0; i < Point_End; ++i)
		{
			for (int j = 0; j < PU_End; ++j)
			{
				m_pArrPointUI[i][j]->Set_PosX(-375.f);
			}
		}

		m_pArrPointUI[Point_A][PU_Outline]->Set_TextureIndex(1);
		m_pArrPointUI[Point_A][PU_Gauge]->Set_TextureIndex(1);
		m_pArrPointUI[Point_A][PU_Text]->Set_TextureIndex(0);
		m_pArrPointUI[Point_A][PU_Outline]->Set_PosY(-17.f);
		m_pArrPointUI[Point_A][PU_Gauge]->Set_PosY(-17.f);
		m_pArrPointUI[Point_A][PU_Text]->Set_PosY(-19.f);

		m_pArrPointUI[Point_R][PU_Outline]->Set_TextureIndex(1);
		m_pArrPointUI[Point_R][PU_Gauge]->Set_TextureIndex(1);
		m_pArrPointUI[Point_R][PU_Text]->Set_TextureIndex(1);
		m_pArrPointUI[Point_R][PU_Outline]->Set_PosY(-85.f);
		m_pArrPointUI[Point_R][PU_Gauge]->Set_PosY(-85.f);
		m_pArrPointUI[Point_R][PU_Text]->Set_PosY(-87.f);

		m_pArrPointUI[Point_C][PU_Outline]->Set_TextureIndex(1);
		m_pArrPointUI[Point_C][PU_Gauge]->Set_TextureIndex(1);
		m_pArrPointUI[Point_C][PU_Text]->Set_TextureIndex(2);
		m_pArrPointUI[Point_C][PU_Outline]->Set_PosY(75.f);
		m_pArrPointUI[Point_C][PU_Gauge]->Set_PosY(75.f);
		m_pArrPointUI[Point_C][PU_Text]->Set_PosY(73.f);

		break;
	case Client::LEVEL_HWARA:

		for (int i = 0; i < Point_End; ++i)
		{
			for (int j = 0; j < PU_End; ++j)
			{
				m_pArrPointUI[i][j]->Set_PosX(-370.f);

				map<_hashcode, CPlayer*> mapPlayers = PLAYER->Get_OwnerPlayer()->Get_Squad()->Get_AllPlayers();
				auto iter = mapPlayers.begin();
				eTEAM_TYPE eTeam = iter->second->Get_Team()->Get_TeamType();
				if (eTeam == eTEAM_TYPE::eBLUE)
				{
					m_pArrPointUI[Point_A][j]->Set_PosX(-465.f);
					m_pArrPointUI[Point_E][j]->Set_PosX(-275.f);
				}
				else
				{
					m_pArrPointUI[Point_A][j]->Set_PosX(-275.f);
					m_pArrPointUI[Point_E][j]->Set_PosX(-465.f);
				}
			}
		}

		m_pArrPointUI[Point_C][PU_Outline]->Set_TextureIndex(1);
		m_pArrPointUI[Point_C][PU_Gauge]->Set_TextureIndex(1);
		m_pArrPointUI[Point_C][PU_Text]->Set_TextureIndex(2);
		m_pArrPointUI[Point_C][PU_Outline]->Set_PosY(-15.f);
		m_pArrPointUI[Point_C][PU_Gauge]->Set_PosY(-15.f);
		m_pArrPointUI[Point_C][PU_Text]->Set_PosY(-17.f);

		m_pArrPointUI[Point_R][PU_Outline]->Set_TextureIndex(1);
		m_pArrPointUI[Point_R][PU_Gauge]->Set_TextureIndex(1);
		m_pArrPointUI[Point_R][PU_Text]->Set_TextureIndex(1);
		m_pArrPointUI[Point_R][PU_Outline]->Set_PosY(35);
		m_pArrPointUI[Point_R][PU_Gauge]->Set_PosY(35);
		m_pArrPointUI[Point_R][PU_Text]->Set_PosY(33.f);

		m_pArrPointUI[Point_A][PU_Outline]->Set_TextureIndex(1);
		m_pArrPointUI[Point_A][PU_Gauge]->Set_TextureIndex(1);
		m_pArrPointUI[Point_A][PU_Text]->Set_TextureIndex(0);
		m_pArrPointUI[Point_A][PU_Outline]->Set_PosY(-15.f);
		m_pArrPointUI[Point_A][PU_Gauge]->Set_PosY(-15.f);
		m_pArrPointUI[Point_A][PU_Text]->Set_PosY(-17.f);

		m_pArrPointUI[Point_E][PU_Outline]->Set_TextureIndex(1);
		m_pArrPointUI[Point_E][PU_Gauge]->Set_TextureIndex(1);
		m_pArrPointUI[Point_E][PU_Text]->Set_TextureIndex(3);
		m_pArrPointUI[Point_E][PU_Outline]->Set_PosY(-15.f);
		m_pArrPointUI[Point_E][PU_Gauge]->Set_PosY(-15.f);
		m_pArrPointUI[Point_E][PU_Text]->Set_PosY(-17.f);

		break;
	}
}

void CUI_ScoreBoard::Set_Squad()
{
	map<_hashcode, CPlayer*> mapPlayers = PLAYER->Get_OwnerPlayer()->Get_Squad()->Get_AllPlayers();

	auto iter = mapPlayers.begin();

	for (int i = 0; i < 4; ++i)
	{
		_uint iTextureNum = iter->second->Get_PlayerInfo()->Get_ChonsenClass();
		wstring wstrPlayerName = iter->second->Get_PlayerName();

		m_pArrSquard[i][Squad_Num]->Set_TextureIndex(i);
		m_pArrSquard[i][Squad_Port]->Set_TextureIndex(iTextureNum);
		m_pArrSquard[i][Squad_Port]->Set_FontText(wstrPlayerName);

		++iter;
	}
}

void CUI_ScoreBoard::Sort_ScoreInfo()
{
	for (auto& pair : m_pScoreInfoMap)
	{
		pair.second.sort([](CUI_ScoreInfo* p1, CUI_ScoreInfo* p2)
			{
				return p1->Get_KillCnt() > p2->Get_KillCnt();
			});
	}
}

void CUI_ScoreBoard::Set_ConquestTime(_uint iPointIdx, _float fConquestTime, _float fMaxConquestTime)
{
	_float fConquestRatio = 1.f - (fConquestTime / fMaxConquestTime);
	m_fConquestRatio[iPointIdx] = fConquestRatio;
}

void CUI_ScoreBoard::Set_ConquestTime(string strPadenPointKey, _float fConquestTime, _float fMaxConquestTime)
{
	if (strPadenPointKey == "Hwara_Final_Blue")
	{
		_float fConquestRatio = 1.f - (fConquestTime / fMaxConquestTime);
		m_fConquestRatio[Point_A] = fConquestRatio;
	}
	else
	{
		_float fConquestRatio = 1.f - (fConquestTime / fMaxConquestTime);
		m_fConquestRatio[Point_E] = fConquestRatio;
	}
}

void CUI_ScoreBoard::Set_GaugeColor(_bool IsMainTeam, _uint iPointIdx)
{
	if (IsMainTeam)
	{
		m_pArrPointUI[iPointIdx][PU_Gauge]->Set_Color(_float4(0.f, 0.8f, 1.f, 1.f));
	}
	else
	{
		m_pArrPointUI[iPointIdx][PU_Gauge]->Set_Color(_float4(1.f, 0.2f, 0.f, 1.f));
	}
}

void CUI_ScoreBoard::Set_PointColor(_bool IsMainTeam, _uint iPoinIdx)
{
	_float4 vColor;
	vColor = IsMainTeam ? _float4(0.f, 0.8f, 1.f, 1.f) : _float4(1.f, 0.2f, 0.f, 1.f);

	for (int i = 0; i < PU_End; ++i)
		m_pArrPointUI[iPoinIdx][i]->Set_Color(vColor);
}

void CUI_ScoreBoard::Set_Player(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
	m_pPlayerTransform = pPlayer->Get_Transform();
}
