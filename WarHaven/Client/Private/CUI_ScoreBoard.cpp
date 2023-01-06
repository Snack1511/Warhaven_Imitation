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

	return S_OK;
}

HRESULT CUI_ScoreBoard::Start()
{
	__super::Start();

	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	Init_ScoreMiniMap();
	Init_ScorePlayerList();
	Init_Squad();

	Set_Squad();

	return S_OK;
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

void CUI_ScoreBoard::Init_ScoreMiniMap()
{
	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:
		m_pScoreMiniMap[Map_Map]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/T_MinimapPaden.dds"));
		break;

	case Client::LEVEL_HWARA:
		m_pScoreMiniMap[Map_Map]->Set_Texture(TEXT(""));
		break;
	}

	CREATE_GAMEOBJECT(m_pScoreBG, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pScoreBG);

	m_pScoreList.push_back(m_pScoreBG);

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

		pair.second.sort([](CUI_ScoreInfo* p1, CUI_ScoreInfo* p2)
			{
				return p1->Get_DeathCnt() < p2->Get_DeathCnt();
			});
	}
}
