#include "stdafx.h"
#include "CGame_Manager_HR.h"
#include "MainApp.h"
#include "GameInstance.h"

#include "GameObject.h"

#include "UsefulHeaders.h"

IMPLEMENT_SINGLETON(CGame_Manager_HR);

CGame_Manager_HR::CGame_Manager_HR()
{
}
CGame_Manager_HR::~CGame_Manager_HR()
{
	Delete_AllThreads();
}

int	Func_Ray(list<CPlayer*> listPlayer, CGame_Manager_HR* pManager)
{
	while (1)
	{
		if (pManager->Is_ThreadsFinished())
			break;

		for (auto& pPlayer : listPlayer)
		{
			if (!pPlayer->Get_CurrentUnit())
				continue;

			if (!pPlayer->Get_CurrentUnit()->Is_Valid())
				continue;

			if (pPlayer->Is_TargetLocked())
				continue;

			list<CPlayer*>* pSortedEnemies = pPlayer->Get_SortedEnemiesP();

			_float4 vRayStartPos = pPlayer->Get_WorldPos();
			vRayStartPos.y += 0.5f;

			CPlayer* pTargetPlayer = nullptr;
			for (auto& elem : (*pSortedEnemies))
			{
				if (!elem->Is_Valid())
					continue;

				_float4 vDir = elem->Get_WorldPos() - pPlayer->Get_WorldPos();
				_float fLength = vDir.Length();

				if (!GAMEINSTANCE->Shoot_RaytoStaticActors(nullptr, nullptr, vRayStartPos, vDir, fLength))
				{
					pTargetPlayer = elem;
					break;
				}

			}

			pPlayer->ReserveTargetPlayer(pTargetPlayer);
		}
	}

	return 0;
}

void CGame_Manager_HR::Create_RayThread(list<CPlayer*> listPlayer)
{
	std::future<int>	newThread = std::async(std::launch::async, bind(Func_Ray, listPlayer, this));


	return;
}

HRESULT CGame_Manager_HR::Initialize()
{
	m_eCurrentLV = LEVEL_MAINMENU; //¸ÞÀÎ¾Û Reserve_Load_Level

	return S_OK;
}

void CGame_Manager_HR::Tick()
{
}

