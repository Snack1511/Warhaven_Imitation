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

	// 이미지를 추가한 후
	// 추가한 이미지 중 랜덤으로 하나 출력
	// 로딩 텍스트를 추가할 때 그 때 이미지를 추가하자

	// 처음에 폴더에 모든 이미지 추가 후
	// 메인 메뉴까지는 랜덤으로 출력

	// 이후 스테이지 이동 할 때는 해당 스테이지에 해당하는 인덱스만 랜덤으로 출력
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
	case Client::LEVEL_LOGO:
		break;
	case Client::LEVEL_LOADING:
		break;
	case Client::LEVEL_MAINMENU:
		break;
	case Client::LEVEL_TEST:
		Create_LoadingText(eLoadLevel, TEXT("테스트레벨"));
		break;
	case Client::LEVEL_BOOTCAMP:
	{
		_uint iTraninigTexture = random(12, 13);
		GET_COMPONENT(CTexture)->Set_CurTextureIndex(iTraninigTexture);
		Create_LoadingText(eLoadLevel, TEXT("훈련장"));
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
