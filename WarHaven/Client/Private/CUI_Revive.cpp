#include "CUI_Revive.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Transform.h"
#include "CUtility_Transform.h"

CUI_Revive::CUI_Revive()
{
}

CUI_Revive::~CUI_Revive()
{
}

HRESULT CUI_Revive::Initialize_Prototype()
{
	Create_ReviveIcon();

	return S_OK;
}

HRESULT CUI_Revive::Initialize()
{
	return S_OK;
}

HRESULT CUI_Revive::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Revive::Set_Position(CTransform* pReviveUnitTransform)
{
	_float4 vReviveUnitPos = CUtility_Transform::Get_ProjPos(pReviveUnitTransform);
	vReviveUnitPos.y += 5.f;

	m_pReviveIcon->Set_Pos(vReviveUnitPos);
}

void CUI_Revive::My_Tick()
{
	__super::My_Tick();
}

void CUI_Revive::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Revive::OnEnable()
{
	__super::OnEnable();

	m_pReviveIcon->SetActive(true);
}

void CUI_Revive::OnDisable()
{
	__super::OnDisable();

	m_pReviveIcon->SetActive(false);
}

void CUI_Revive::Create_ReviveIcon()
{
	m_pReviveIcon = CUI_Object::Create();

	m_pReviveIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/White/T_WhiteEKeyIcon.png"));

	m_pReviveIcon->Set_Scale(20.f);

	m_pReviveIcon->Set_FontRender(true);
	m_pReviveIcon->Set_FontStyle(true);
	m_pReviveIcon->Set_FontCenter(true);

	m_pReviveIcon->Set_FontOffset(50.f, 3.f);
	m_pReviveIcon->Set_FontScale(0.2f);

	m_pReviveIcon->Set_FontText(TEXT("¼Ò»ý"));
	
	CREATE_GAMEOBJECT(m_pReviveIcon, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pReviveIcon);
}
