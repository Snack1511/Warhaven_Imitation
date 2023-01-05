#include "CUI_ScoreInfo.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "CPlayer.h"
#include "CPlayerInfo.h"
#include "Functor.h"
#include "CTeamConnector.h"

_uint CUI_ScoreInfo::m_iSquadIdx = 0;
CUI_ScoreInfo::CUI_ScoreInfo()
{
}

CUI_ScoreInfo::~CUI_ScoreInfo()
{
}

HRESULT CUI_ScoreInfo::Initialize_Prototype()
{
	Create_Info();

	return S_OK;
}

HRESULT CUI_ScoreInfo::Start()
{
	__super::Start();

	Init_Info();

	return S_OK;
}

void CUI_ScoreInfo::Set_Type(_uint iType)
{
	m_iType = iType;
}

void CUI_ScoreInfo::My_Tick()
{
	__super::My_Tick();

	_tchar  szTemp[MAX_STR] = {};
	swprintf_s(szTemp, TEXT("%0d"), m_iKillCnt);
	m_pInfo[Info_Kill]->Set_FontText(szTemp);

	_float fTapPosY = 170.f - ((m_iRank - 1) * 20.f);
	_float fResultPosY = 190.f - ((m_iRank - 1) * 20.f);

	_float fInfoPosY = m_iType == 0 ? fTapPosY : fResultPosY;
	for (int i = 0; i < Info_End; ++i)
	{
		m_pInfo[i]->Set_PosY(fInfoPosY);
	}
}

void CUI_ScoreInfo::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_ScoreInfo::OnEnable()
{
	__super::OnEnable();

	for (int i = 0; i < Info_Num; ++i)
	{
		m_pInfo[i]->SetActive(true);
	}

	if (pOwnerPlayer->IsMainPlayer())
	{
		m_pInfo[Info_Num]->SetActive(true);
	}
	else if (pOwnerPlayer->Get_Team()->IsMainPlayerTeam())
	{
		if (pOwnerPlayer->Get_OutlineType() == CPlayer::eSQUADMEMBER)
		{
			m_pInfo[Info_Num]->SetActive(true);
		}
	}
}

void CUI_ScoreInfo::OnDisable()
{
	__super::OnDisable();

	for (int i = 0; i < Info_End; ++i)
	{
		m_pInfo[i]->SetActive(false);
	}
}

void CUI_ScoreInfo::Create_Info()
{
	for (int i = 0; i < Info_End; ++i)
	{
		m_pInfo[i] = CUI_Object::Create();

		switch (i)
		{
		case Info_BG:
			m_pInfo[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.f));
			m_pInfo[i]->Set_Scale(200.f, 20.f);
			break;

		case Info_Rank:
			m_pInfo[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.f));
			m_pInfo[i]->Set_FontRender(true);
			m_pInfo[i]->Set_FontStyle(true);
			m_pInfo[i]->Set_FontCenter(true);
			m_pInfo[i]->Set_FontScale(0.2f);
			m_pInfo[i]->Set_FontOffset(3.f, 3.f);
			break;

		case Info_ClassName:
			GET_COMPONENT_FROM(m_pInfo[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pInfo[i], "/Oper", "Class");
			m_pInfo[i]->Set_Scale(16.f);

			m_pInfo[i]->Set_FontRender(true);
			m_pInfo[i]->Set_FontStyle(true);
			m_pInfo[i]->Set_FontScale(0.2f);
			m_pInfo[i]->Set_FontOffset(15.f, -12.f);
			break;

		case Info_Kill:
			m_pInfo[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.f));
			m_pInfo[i]->Set_FontRender(true);
			m_pInfo[i]->Set_FontStyle(true);
			m_pInfo[i]->Set_FontCenter(true);
			m_pInfo[i]->Set_FontScale(0.2f);
			m_pInfo[i]->Set_FontOffset(5.f, 3.f);
			break;

		case Info_Num:
			GET_COMPONENT_FROM(m_pInfo[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pInfo[i], "/Oper", "Num");
			m_pInfo[i]->Set_Scale(16.f);
			// m_pInfo[i]->Set_PosX(135.f);
			break;
		}
	}
}

void CUI_ScoreInfo::Init_Info()
{
	for (int i = 0; i < Info_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pInfo[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pInfo[i]);
	}

	_float fPosX = m_pInfo[Info_BG]->Get_PosX();

	m_pInfo[Info_Rank]->Set_PosX(fPosX - 85.f);
	m_pInfo[Info_ClassName]->Set_PosX(fPosX - 65.f);
	m_pInfo[Info_Kill]->Set_PosX(fPosX + 85.f);
}

void CUI_ScoreInfo::Set_Player(CPlayer* pPlayer)
{
	pOwnerPlayer = pPlayer;

	if (pOwnerPlayer->IsMainPlayer())
	{
		m_pInfo[Info_BG]->Set_Color(_float4(1.f, 1.f, 1.f, 1.f));
		m_pInfo[Info_ClassName]->Set_Color(_float4(0.f, 0.f, 0.f, 1.f));
		m_pInfo[Info_ClassName]->Set_FontColor(_float4(0.f, 0.f, 0.f, 1.f));
		m_pInfo[Info_Rank]->Set_FontColor(_float4(0.f, 0.f, 0.f, 1.f));
		m_pInfo[Info_Kill]->Set_FontColor(_float4(0.f, 0.f, 0.f, 1.f));
		m_pInfo[Info_Num]->Set_TextureIndex(m_iSquadIdx);

		m_iSquadIdx++;
	}
	else if (pOwnerPlayer->Get_Team()->IsMainPlayerTeam())
	{
		if (pOwnerPlayer->Get_OutlineType() == CPlayer::eSQUADMEMBER)
		{
			m_pInfo[Info_BG]->Set_Color(_float4(0.2f, 0.5f, 0.2f, 1.f));
			m_pInfo[Info_Num]->Set_TextureIndex(m_iSquadIdx);

			m_iSquadIdx++;
		}
	}

	CPlayerInfo* pPlayerInfo = pOwnerPlayer->Get_PlayerInfo();

	_uint iClassNum = pPlayerInfo->Get_ChonsenClass();
	wstring wstrPlayerName = pPlayerInfo->Get_PlayerName();

	m_pInfo[Info_ClassName]->Set_TextureIndex(iClassNum);
	m_pInfo[Info_ClassName]->Set_FontText(wstrPlayerName);
}

void CUI_ScoreInfo::Set_Rank(_uint iRank)
{
	m_iRank = iRank + 1;

	_tchar  szTemp[MAX_STR] = {};
	swprintf_s(szTemp, TEXT("%d"), m_iRank);
	m_pInfo[Info_Rank]->Set_FontText(szTemp);
}

void CUI_ScoreInfo::Set_PosX(_float fPosX)
{
	m_pInfo[Info_BG]->Set_PosX(fPosX);
}
