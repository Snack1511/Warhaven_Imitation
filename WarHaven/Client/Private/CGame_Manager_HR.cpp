#include "stdafx.h"
#include "CGame_Manager_HR.h"
#include "MainApp.h"
#include "GameInstance.h"

#include "GameObject.h"

IMPLEMENT_SINGLETON(CGame_Manager_HR);

CGame_Manager_HR::CGame_Manager_HR()
{
}
CGame_Manager_HR::~CGame_Manager_HR()
{
}


void CGame_Manager_HR::Set_TeamCnt(_uint* pTeamCnt)
{
	for (int i = 0; i < (_uint)eTEAM_TYPE::eCOUNT; i++)
	{
		m_iTeamCnt[i] = pTeamCnt[i];
	}
}

HRESULT CGame_Manager_HR::Initialize()
{
	m_eCurrentLV = LEVEL_MAINMENU; //¸ÞÀÎ¾Û Reserve_Load_Level

	return S_OK;
}

void CGame_Manager_HR::Tick()
{
}

