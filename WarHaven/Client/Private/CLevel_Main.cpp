#include "CLevel_Main.h"
#include "Loading_Manager.h"
#include "GameInstance.h"
#include "CUI_LobbyBG.h"

CLevel_Main::CLevel_Main()
{
}

CLevel_Main::~CLevel_Main()
{
}

CLevel_Main* CLevel_Main::Create()
{
	CLevel_Main* pLevel = new CLevel_Main();

	pLevel->Initialize();

	return pLevel;
}

HRESULT CLevel_Main::Initialize()
{
	return S_OK;
}

HRESULT CLevel_Main::SetUp_Prototypes()
{
	return S_OK;
}

HRESULT CLevel_Main::Enter()
{
	CUI_LobbyBG* pUI_Lobby = CUI_LobbyBG::Create();
	Ready_GameObject(pUI_Lobby, GROUP_UI);

	__super::Enter();

	return S_OK;
}

void CLevel_Main::Tick()
{
	if (true == CLoading_Manager::Get_Instance()->IsFinished())
	{
		if (KEY(ENTER, TAP))
		{
			CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_TEST);
		}
	}
}

void CLevel_Main::Late_Tick()
{
}

HRESULT CLevel_Main::Render()
{
	return S_OK;
}

HRESULT CLevel_Main::Exit()
{
	__super::Exit();

	return S_OK;
}
