#include "CUI_Training.h"
#include "CUI_Object.h"

#include "GameInstance.h"
#include "Texture.h"

CUI_Training::CUI_Training()
{
}

CUI_Training::~CUI_Training()
{
}

HRESULT CUI_Training::Initialize_Prototype()
{
	m_pPopup = CUI_Object::Create();

	return S_OK;
}

HRESULT CUI_Training::Initialize()
{
	return S_OK;
}

HRESULT CUI_Training::Start()
{
	Init_Popup();

	__super::Start();

	return S_OK;
}

void CUI_Training::My_Tick()
{
	__super::My_Tick();
}

void CUI_Training::Enable_Popup(_uint iIndex)
{
	if (!m_pPopup)
		return;

	GET_COMPONENT_FROM(m_pPopup, CTexture)->Set_CurTextureIndex(iIndex);

	if (iIndex > 3)
	{
		m_pPopup->Set_Scale(m_vLongPopupScale);

		if (iIndex == 11)
		{
			m_pPopup->Set_Scale(m_vLongPopupScale3);
		}
	}
	else
	{
		m_pPopup->Set_Scale(m_vShotPopupScale);
	}

	ENABLE_GAMEOBJECT(m_pPopup);
}

void CUI_Training::Disable_Popup()
{
	DISABLE_GAMEOBJECT(m_pPopup);
}

void CUI_Training::Init_Popup()
{
	Read_Texture(m_pPopup, "/Training", "Popup");

	GET_COMPONENT_FROM(m_pPopup, CTexture)->Remove_Texture(0);

	m_pPopup->Set_Sort(0.8f);
	m_pPopup->Set_PosY(250.f);

	CREATE_GAMEOBJECT(m_pPopup, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pPopup);
}
