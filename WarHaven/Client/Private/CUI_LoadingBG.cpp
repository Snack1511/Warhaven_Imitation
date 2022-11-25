#include "CUI_LoadingBG.h"
#include "Texture.h"
#include "Easing_Utillity.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Loading_Manager.h"

CUI_LoadingBG::CUI_LoadingBG()
{
}

CUI_LoadingBG::CUI_LoadingBG(const CUI_LoadingBG& Prototype)
	: CUI(Prototype)
{
}

CUI_LoadingBG::~CUI_LoadingBG()
{
}

HRESULT CUI_LoadingBG::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/UI/Loading/Map_Training_01.dds"));

	Set_Pos(0.f, 0.f);
	Set_Scale(2280.f, 1720.f);

	Set_Sort(1.f);

	m_iBGIndex = 1;

	LEVEL_TYPE_CLIENT eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();
	if (eLoadLevel == LEVEL_TYPE_CLIENT::LEVEL_TEST)
	{
		Create_LoadingText(eLoadLevel);
	}

	return S_OK;
}

HRESULT CUI_LoadingBG::Initialize()
{
	return S_OK;
}

HRESULT CUI_LoadingBG::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_LoadingBG::My_Tick()
{
	__super::My_Tick();

	_float4 vScale = Get_Scale();
	_float4 vTargetScale = _float4(1280.f, 720.f, 1.f);

	m_fAccTime += fDT(0) * 0.001f;
	_float4 vResultScale = CEasing_Utillity::Linear(vScale, vTargetScale, m_fAccTime, 100.f);

	Set_Scale(vResultScale.x, vResultScale.y);
}

void CUI_LoadingBG::OnEnable()
{
	__super::OnEnable();

	GET_COMPONENT(CTexture)->Set_CurTextureIndex(m_iBGIndex);
}

void CUI_LoadingBG::Create_LoadingText(LEVEL_TYPE_CLIENT eLevel)
{
	m_pNextMapName = CUI_Object::Create();
	m_pLoddingText = CUI_Object::Create();

	m_pNextMapName->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));
	m_pLoddingText->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

	m_pNextMapName->Set_Pos(-550.f, 150.f);
	m_pNextMapName->Set_Sort(0.8f);
	m_pNextMapName->Set_FontRender(true);
	m_pNextMapName->Set_FontStyle(true);
	m_pNextMapName->Set_FontOffset(0.f, -50.f);
	m_pNextMapName->Set_FontScale(1.f);

	m_pLoddingText->Set_Pos(-600.f, -350.f);
	m_pLoddingText->Set_Sort(0.8f);
	m_pLoddingText->Set_FontRender(true);
	m_pLoddingText->Set_FontStyle(true);
	m_pLoddingText->Set_FontOffset(0.f, -50.f);
	m_pLoddingText->Set_FontScale(0.3f);
	m_pLoddingText->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));

	m_pLoddingText->Set_FontText(TEXT("전장 합류 중"));

	CREATE_GAMEOBJECT(m_pNextMapName, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pNextMapName);

	switch (eLevel)
	{
	case Client::LEVEL_LOGO:
		break;
	case Client::LEVEL_LOADING:
		break;
	case Client::LEVEL_MAINMENU:
		break;
	case Client::LEVEL_TEST:

		m_pNextMapName->Set_FontText(TEXT("테스트 레벨"));

		break;
	}

	if (eLevel > 2)
	{
		ENABLE_GAMEOBJECT(m_pNextMapName);
	}

	CREATE_GAMEOBJECT(m_pLoddingText, GROUP_UI);
}
