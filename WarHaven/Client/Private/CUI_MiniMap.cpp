#include "CUI_MiniMap.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Loading_Manager.h"

HRESULT CUI_MiniMap::Initialize_Prototype()
{
	Create_MiniMap();

	return S_OK;
}

HRESULT CUI_MiniMap::Initialize()
{
	return S_OK;
}

HRESULT CUI_MiniMap::Start()
{
	__super::Start();

	Init_MiniMap();

	return S_OK;
}

void CUI_MiniMap::My_Tick()
{
	__super::My_Tick();
}

void CUI_MiniMap::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_MiniMap::SetActive_MiniMap(_bool value)
{
	m_pMiniMap->SetActive(value);
}

void CUI_MiniMap::OnEnable()
{
	__super::OnEnable();

	SetActive_MiniMap(true);
}

void CUI_MiniMap::OnDisable()
{
	__super::OnDisable();

	SetActive_MiniMap(false);
}

void CUI_MiniMap::Create_MiniMap()
{
	m_pMiniMap = CUI_Object::Create();

	m_pMiniMap->Set_Color(_float4(1.f, 1.f, 1.f, 0.5f));

	CREATE_GAMEOBJECT(m_pMiniMap, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pMiniMap);
}

void CUI_MiniMap::Init_MiniMap()
{
	m_eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	switch (m_eLoadLevel)
	{
	case Client::LEVEL_PADEN:

		m_pMiniMap->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Map/MiniMap/T_MinimapPadenBlack.dds"));
		m_pMiniMap->Set_Pos(-500.f, 250.f);
		m_pMiniMap->Set_Scale(250.f);

		break;
	case Client::LEVEL_HWARA:
		break;
	}
}
