#include "CUI_Interact.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Transform.h"
#include "CUtility_Transform.h"

CUI_Interact::CUI_Interact()
{
}

CUI_Interact::~CUI_Interact()
{
}

HRESULT CUI_Interact::Initialize_Prototype()
{
	Create_InteractUI();

	return S_OK;
}

HRESULT CUI_Interact::Initialize()
{
	return S_OK;
}

HRESULT CUI_Interact::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Interact::Set_InteractKey(_uint iKeyIndex)
{
	m_pInteractUI->Set_TextureIndex(iKeyIndex);
}

void CUI_Interact::Set_InteractText(wstring wstrText)
{
	m_pInteractUI->Set_FontText(wstrText);
}

void CUI_Interact::Set_InteractTarget(CGameObject* pInteractTarget)
{
	m_pTargetTransform = pInteractTarget->Get_Transform();
}

void CUI_Interact::My_Tick()
{
	__super::My_Tick();
}

void CUI_Interact::My_LateTick()
{
	__super::My_LateTick();

	if (m_pTargetTransform)
	{
		_float4 vTargetPos = CUtility_Transform::Get_ProjPos(m_pTargetTransform);
		vTargetPos.x -= 15.f;

		m_pInteractUI->Set_Pos(vTargetPos);
	}
	else
	{
		_float fPosX = m_pInteractUI->Get_FontSizeX() * 0.5f;
		m_pInteractUI->Set_Pos(-fPosX, -50.f);
	}
}

void CUI_Interact::OnEnable()
{
	__super::OnEnable();

	m_pInteractUI->SetActive(true);
}

void CUI_Interact::OnDisable()
{
	__super::OnDisable();

	m_pInteractUI->SetActive(false);
}

void CUI_Interact::Create_InteractUI()
{
	m_pInteractUI = CUI_Object::Create();

	GET_COMPONENT_FROM(m_pInteractUI, CTexture)->Remove_Texture(0);
	Read_Texture(m_pInteractUI, "/KeyIcon/Keyboard/White", "Key");

	m_pInteractUI->Set_Scale(20.f);

	m_pInteractUI->Set_FontRender(true);
	m_pInteractUI->Set_FontStyle(true);

	m_pInteractUI->Set_FontOffset(15.f, -15.f);
	m_pInteractUI->Set_FontScale(0.3f);

	CREATE_GAMEOBJECT(m_pInteractUI, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pInteractUI);
}
