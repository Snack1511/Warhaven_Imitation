#include "stdafx.h"
#include "CGame_Manager_JJ.h"
#include "MainApp.h"
#include "GameInstance.h"

#include "Texture.h"
#include "GameObject.h"
#include "CFader.h"
#include "CShader.h"
#include "Model.h"
#include "HIerarchyNode.h"
#include "Transform.h"
#include "Loading_Manager.h"

#include "CEffects_Factory.h"
#include "Functor.h"
#include "CState_Manager.h"
#include "CNavigation.h"
#include "CTerrain.h"
#include "CCell.h"


IMPLEMENT_SINGLETON(CGame_Manager_JJ);

CGame_Manager_JJ::CGame_Manager_JJ()
{
}
CGame_Manager_JJ::~CGame_Manager_JJ()
{
}


HRESULT CGame_Manager_JJ::Initialize()
{
	POINT ptMouse;

	ptMouse.x = g_iWinCX >> 1;
	ptMouse.y = g_iWinCY >> 1;
	m_ptCenter = ptMouse;

	m_KeyCommands.push_back(KEY::LAST);

	return S_OK;
}

void CGame_Manager_JJ::Tick()
{
	Fix_CursorPosToCenter();
}

void CGame_Manager_JJ::Fix_CursorPosToCenter()
{
	if (GetFocus() != g_hWnd || !m_bFixCursor)
		return;

	/* Set Mouse Cursor on Center of Screen */
	POINT ptMouse;
	ptMouse = m_ptCenter;
	::ClientToScreen(g_hWnd, &ptMouse);
	::SetCursorPos(ptMouse.x, ptMouse.y);
}

void CGame_Manager_JJ::Update_KeyCommands()
{
	vector<CKey_Manager::tKeyInfo>& vecKeys = CGameInstance::Get_Instance()->Get_KeyList();

	for (_uint i = 0; i < (_uint)KEY::LAST; ++i)
	{
		if (vecKeys[i].eState == KEY_STATE::TAP)
		{
			m_KeyCommands.push_back((KEY)i);
		}
	}
}

