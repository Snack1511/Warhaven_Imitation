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
		case LEVEL_TEST:
		Create_LoadingText(eLoadLevel);
		m_pLoadingText[LT_Map]->Set_FontText(TEXT("�׽�Ʈ����"));
		break;

		case LEVEL_BOOTCAMP:
		{
			_uint iMapTexture = random(21, 22);
			GET_COMPONENT(CTexture)->Set_CurTextureIndex(iMapTexture);
			Create_LoadingText(eLoadLevel);
			m_pLoadingText[LT_Map]->Set_FontText(TEXT("�Ʒ���"));
		}
		break;

		case LEVEL_PADEN:
		{
			_uint iMapTexture = random(9, 20);
			GET_COMPONENT(CTexture)->Set_CurTextureIndex(iMapTexture);
			Create_LoadingText(eLoadLevel);
			m_pLoadingText[LT_Map]->Set_FontText(TEXT("�ĵ�"));
			m_pLoadingText[LT_Mode]->Set_FontText(TEXT("������"));
		}
		break;

		case LEVEL_HWARA:
		{
			_uint iMapTexture = random(0, 8);
			GET_COMPONENT(CTexture)->Set_CurTextureIndex(iMapTexture);
			Create_LoadingText(eLoadLevel);
			m_pLoadingText[LT_Map]->Set_FontText(TEXT("�Ķ�"));
			m_pLoadingText[LT_Mode]->Set_FontText(TEXT("ȣ����"));
			break;
		}
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

void CUI_LoadingBG::Create_LoadingText(LEVEL_TYPE_CLIENT eLevel)
{
	for (int i = 0; i < LT_End; ++i)
	{
		m_pLoadingText[i] = CUI_Object::Create();

		m_pLoadingText[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Alpha0.png"));

		CREATE_GAMEOBJECT(m_pLoadingText[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pLoadingText[i]);
	}

	m_pLoadingText[LT_Map]->Set_Pos(-550.f, 150.f);
	m_pLoadingText[LT_Map]->Set_Sort(0.8f);
	m_pLoadingText[LT_Map]->Set_FontRender(true);
	m_pLoadingText[LT_Map]->Set_FontStyle(true);
	m_pLoadingText[LT_Map]->Set_FontOffset(0.f, -50.f);
	m_pLoadingText[LT_Map]->Set_FontScale(1.f);

	m_pLoadingText[LT_Loading]->Set_Pos(-600.f, -350.f);
	m_pLoadingText[LT_Loading]->Set_Sort(0.8f);
	m_pLoadingText[LT_Loading]->Set_FontRender(true);
	m_pLoadingText[LT_Loading]->Set_FontStyle(true);
	m_pLoadingText[LT_Loading]->Set_FontOffset(0.f, -50.f);
	m_pLoadingText[LT_Loading]->Set_FontScale(0.3f);
	m_pLoadingText[LT_Loading]->Set_FontColor(_float4(0.5f, 0.5f, 0.5f, 1.f));
	m_pLoadingText[LT_Loading]->Set_FontText(TEXT("���� �շ� ��"));

	m_pLoadingText[LT_Mode]->Set_Pos(-540.f, 50.f);
	m_pLoadingText[LT_Mode]->Set_Sort(0.8f);
	m_pLoadingText[LT_Mode]->Set_FontRender(true);
	m_pLoadingText[LT_Mode]->Set_FontStyle(true);
	m_pLoadingText[LT_Mode]->Set_FontOffset(0.f, -50.f);
	m_pLoadingText[LT_Mode]->Set_FontScale(0.4f);

	m_pLoadingText[LT_Gibberish]->Set_Pos(-535.f, -5.f);
	m_pLoadingText[LT_Gibberish]->Set_Sort(0.8f);
	m_pLoadingText[LT_Gibberish]->Set_FontRender(true);
	m_pLoadingText[LT_Gibberish]->Set_FontStyle(true);
	m_pLoadingText[LT_Gibberish]->Set_FontOffset(0.f, -50.f);
	m_pLoadingText[LT_Gibberish]->Set_FontScale(0.25f);
	m_pLoadingText[LT_Gibberish]->Set_FontText(Init_GibberishText());

	if (eLevel > 2)
	{
		ENABLE_GAMEOBJECT(m_pLoadingText[LT_Map]);
		ENABLE_GAMEOBJECT(m_pLoadingText[LT_Mode]);
		ENABLE_GAMEOBJECT(m_pLoadingText[LT_Gibberish]);
	}
}

wstring CUI_LoadingBG::Init_GibberishText()
{
	wstring wstrGibberish;

	_uint iRandText = random(0, 9);

	switch (iRandText)
	{
	case 0:
		wstrGibberish = TEXT("�˰� ��̳���?");
		break;
	case 1:
		wstrGibberish = TEXT("�˰� ��̳���?\n�������ڴ� ���� �� �������ϰ� �����ϴ�.");
		break;
	case 2:
		wstrGibberish = TEXT("�˰� ��̳���?\nȣ��Ʈ�� '��å'�� ���� ���� �Ӹ� �ƴ϶� ���� ���ص� ����մϴ�.");
		break;
	case 3:
		wstrGibberish = TEXT("�˰� ��̳���?\n��⿡ �����ϰ� ħ���ϰ� �÷����Ҽ��� �·��� �����մϴ�.");
		break;
	case 4:
		wstrGibberish = TEXT("���� �ʰ� ������ �Ѵٰ��?\n��� ��Ʈ��Ī�� �ϸ� �������� �ð��� ����������.");
		break;
	case 5:
		wstrGibberish = TEXT("�˰� ��̳���?\n���������� ���� ���� Ż���ϴ� �÷��̾�� ���縦 �޽��ϴ�.");
		break;
	case 6:
		wstrGibberish = TEXT("�׷��Ա��� �ؾ��ϳ�? �ؾ� �մϴ�.");
		break;
	case 7:
		wstrGibberish = TEXT("�˰� ��̳���?\n�ϱ� ������ �װ� �߿��� �̴ϴ�.");
		break;
	case 8:
		wstrGibberish = TEXT("�˰� ��̳���?\n�������� �װ� �ؾ� �ϴ� �̴ϴ�");
		break;
	case 9:
		wstrGibberish = TEXT("�˰� ��̳���?\n���� �̿��Ͽ� �Ʊ����� ������ �����ϰų� ������ �� �� �ֽ��ϴ�.");
		break;
	}

	return wstrGibberish;
}
