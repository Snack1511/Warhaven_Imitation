#include "CUI_LoadingBG.h"
#include "Texture.h"
#include "Easing_Utillity.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Loading_Manager.h"

CUI_LoadingBG::CUI_LoadingBG()
{
}

CUI_LoadingBG::~CUI_LoadingBG()
{
}

HRESULT CUI_LoadingBG::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/UI/Loading/Map_Training_01.dds"));
	GET_COMPONENT(CTexture)->Remove_Texture(0);

	Read_Texture("/Loading", "Map");

	Set_Pos(0.f, 0.f);
	Set_Scale(2280.f, 1720.f);

	Set_Sort(1.f);

	_uint iMaxIdx = GET_COMPONENT(CTexture)->Get_TextureSize();
	_uint iRandNum = random(0, iMaxIdx);
	GET_COMPONENT(CTexture)->Set_CurTextureIndex(iRandNum);

	LEVEL_TYPE_CLIENT eLoadLevel = CLoading_Manager::Get_Instance()->Get_LoadLevel();

	switch (eLoadLevel)
	{
		case LEVEL_LOGO:
			break;
		case LEVEL_LOADING:
			break;
		case LEVEL_MAINMENU:
			break;
		case LEVEL_TEST:
			Create_LoadingText(eLoadLevel, TEXT("테스트레벨"));
			break;
		case LEVEL_BOOTCAMP:
		{
			_uint iTraninigTexture = random(12, 13);
			GET_COMPONENT(CTexture)->Set_CurTextureIndex(iTraninigTexture);
			Create_LoadingText(eLoadLevel, TEXT("훈련장"));
		}
		break;
		case LEVEL_PADEN:
		{
			_uint iTraninigTexture = random(0, 11);
			GET_COMPONENT(CTexture)->Set_CurTextureIndex(iTraninigTexture);
			Create_LoadingText(eLoadLevel, TEXT("파덴"));
		}
		break;
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
}

void CUI_LoadingBG::Create_LoadingText(LEVEL_TYPE_CLIENT eLevel, wstring eName)
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

	m_pNextMapName->Set_FontText(eName);

	if (eLevel > 2)
	{
		ENABLE_GAMEOBJECT(m_pNextMapName);
	}

	CREATE_GAMEOBJECT(m_pLoddingText, GROUP_UI);
}
