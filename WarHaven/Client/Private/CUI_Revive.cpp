#include "CUI_Revive.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Transform.h"
#include "CUtility_Transform.h"
#include "Texture.h"
#include "CPlayer.h"
#include "CTeamConnector.h"

CUI_Revive::CUI_Revive()
{
}

CUI_Revive::~CUI_Revive()
{
}

HRESULT CUI_Revive::Initialize_Prototype()
{
	Create_ReviveIcon();
	Create_ClassIcon();

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

void CUI_Revive::Set_ReviveIcon(_uint iIconIndex)
{
	m_pReviveIcon->Set_TextureIndex(iIconIndex);
}

void CUI_Revive::Set_ClassIcon(CPlayer* pDeadPlayer)
{
	_uint iClassIdx = pDeadPlayer->Get_PlayerInfo()->Get_ChonsenClass();
	m_pClassIcon[Class_Icon]->Set_TextureIndex(iClassIdx);

	if (pDeadPlayer->Get_Team()->IsMainPlayerTeam())
	{
		if (pDeadPlayer->Get_OutlineType() == CPlayer::eSQUADMEMBER)
		{
			m_pClassIcon[Class_Icon]->Set_Color(_float4(0.2f, 0.5f, 0.2f, 1.f));
		}
		else
		{
			m_pClassIcon[Class_Icon]->Set_Color(_float4(0.25f, 0.65f, 0.9f, 1.f));
		}
	}
}

void CUI_Revive::My_Tick()
{
	__super::My_Tick();
}

void CUI_Revive::My_LateTick()
{
	__super::My_LateTick();

	Update_Pos();
}

void CUI_Revive::OnEnable()
{
	__super::OnEnable();

	m_fAccTime = 0.f;

	m_pReviveIcon->SetActive(true);

	for (int i = 0; i < Class_End; ++i)
		m_pClassIcon[i]->SetActive(true);
}

void CUI_Revive::OnDisable()
{
	__super::OnDisable();

	m_pReviveIcon->SetActive(false);

	for (int i = 0; i < Class_End; ++i)
		m_pClassIcon[i]->SetActive(false);
}

void CUI_Revive::Update_Pos()
{
	_float4 vReviveUnitPos = CUtility_Transform::Get_ProjPos(m_pReviveUnitTransform);

	for (int i = 0; i < Class_End; ++i)
		m_pClassIcon[i]->Set_Pos(vReviveUnitPos);

	vReviveUnitPos.x -= 15.f;
	vReviveUnitPos.y += 35.f;

	m_pReviveIcon->Set_Pos(vReviveUnitPos);
}

void CUI_Revive::Create_ReviveIcon()
{
	m_pReviveIcon = CUI_Object::Create();

	m_pReviveIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Revive/T_SaviorIconSmall.png"));
	GET_COMPONENT_FROM(m_pReviveIcon, CTexture)->Add_Texture(TEXT("../Bin/Resources/Textures/UI/KeyIcon/Keyboard/White/T_WhiteFKeyIcon.dds"));

	m_pReviveIcon->Set_Scale(20.f);

	m_pReviveIcon->Set_FontRender(true);
	m_pReviveIcon->Set_FontStyle(true);
	m_pReviveIcon->Set_FontCenter(true);

	m_pReviveIcon->Set_FontOffset(30.f, 3.f);
	m_pReviveIcon->Set_FontScale(0.2f);

	m_pReviveIcon->Set_FontText(TEXT("¼Ò»ý"));

	CREATE_GAMEOBJECT(m_pReviveIcon, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pReviveIcon);
}

void CUI_Revive::Create_ClassIcon()
{
	for (int i = 0; i < Class_End; ++i)
	{
		m_pClassIcon[i] = CUI_Object::Create();

		switch (i)
		{
		case Class_BG:

			m_pClassIcon[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Circle/T_256Circle.dds"));
			m_pClassIcon[i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.6f));

			m_pClassIcon[i]->Set_Scale(40.f);
			m_pClassIcon[i]->Set_Sort(0.5f);

			break;

		case Class_Icon:

			GET_COMPONENT_FROM(m_pClassIcon[i], CTexture)->Remove_Texture(0);
			Read_Texture(m_pClassIcon[i], "/Oper", "Class");

			m_pClassIcon[i]->Set_Scale(40.f);
			m_pClassIcon[i]->Set_Sort(0.49f);

			break;
		}

		CREATE_GAMEOBJECT(m_pClassIcon[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pClassIcon[i]);
	}
}
