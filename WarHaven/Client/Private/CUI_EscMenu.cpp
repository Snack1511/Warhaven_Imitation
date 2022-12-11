#include "CUI_EscMenu.h"
#include "CUI_Object.h"
#include "GameInstance.h"
#include "Loading_Manager.h"

CUI_EscMenu::CUI_EscMenu()
{
}

CUI_EscMenu::~CUI_EscMenu()
{
}

HRESULT CUI_EscMenu::Initialize_Prototype()
{
	Create_EscMenu();

	return S_OK;
}

HRESULT CUI_EscMenu::Start()
{
	__super::Start();

	Bind_Btn();

	return S_OK;
}

void CUI_EscMenu::On_PointDown_MenuBtn(const _uint& iEventNum)
{
	CLoading_Manager::Get_Instance()->Reserve_Load_Level(LEVEL_MAINMENU);
}

void CUI_EscMenu::SetActive_EscMenu(_bool value)
{
	for (auto& iter : m_pEscList)
	{
		iter->SetActive(value);
	}
}

void CUI_EscMenu::OnEnable()
{
	__super::OnEnable();

	Fadenable_EscMenu();
}

void CUI_EscMenu::OnDisable()
{
	__super::OnDisable();

	SetActive_EscMenu(false);
}

void CUI_EscMenu::Bind_Btn()
{
	m_pMenuBtn->CallBack_PointDown += bind(&CUI_EscMenu::On_PointDown_MenuBtn, this, 0);
}

void CUI_EscMenu::Create_EscMenu()
{
	m_pBG = CUI_Object::Create();
	m_pIcon = CUI_Object::Create();
	m_pMenuBtn = CUI_Object::Create();

	m_pBG->Set_FadeDesc(0.15f);
	m_pIcon->Set_FadeDesc(0.15f);
	m_pMenuBtn->Set_FadeDesc(0.15f);

	m_pBG->Set_Sort(0.01f);
	m_pIcon->Set_Sort(0.009f);
	m_pMenuBtn->Set_Sort(0.009f);

	m_pBG->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Rect/Black.png"));
	m_pBG->Set_Color(_float4(0.f, 0.f, 0.f, 0.95f));
	m_pBG->Set_Scale(1280.f, 720.f);

	m_pIcon->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Esc/T_Logo.dds"));
	m_pIcon->Set_Pos(-420.f, 225.f);
	m_pIcon->Set_Scale(360.f);

	m_pMenuBtn->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
	m_pMenuBtn->Set_Pos(-500.f, 100.f);
	m_pMenuBtn->Set_Scale(180.f, 50.f);

	m_pMenuBtn->Set_MouseTarget(true);

	m_pMenuBtn->Set_FontRender(true);
	m_pMenuBtn->Set_FontStyle(true);
	m_pMenuBtn->Set_FontCenter(true);
	m_pMenuBtn->Set_FontOffset(3.f, 3.f);
	m_pMenuBtn->Set_FontScale(0.5f);
	m_pMenuBtn->Set_FontText(TEXT("메인메뉴"));

	CREATE_GAMEOBJECT(m_pBG, GROUP_UI);
	CREATE_GAMEOBJECT(m_pIcon, GROUP_UI);
	CREATE_GAMEOBJECT(m_pMenuBtn, GROUP_UI);

	DISABLE_GAMEOBJECT(m_pBG);
	DISABLE_GAMEOBJECT(m_pIcon);
	DISABLE_GAMEOBJECT(m_pMenuBtn);

	m_pEscList.push_back(m_pBG);
	m_pEscList.push_back(m_pIcon);
	m_pEscList.push_back(m_pMenuBtn);
}

void CUI_EscMenu::Fadenable_EscMenu()
{
	for (auto& iter : m_pEscList)
	{
		Enable_Fade(iter, 0.3f);
	}
}
