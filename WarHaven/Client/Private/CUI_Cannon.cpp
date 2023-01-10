#include "CUI_Cannon.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "GameInstance.h"
#include "CShader.h"
#include "CUI_Renderer.h"

CUI_Cannon::CUI_Cannon()
{
}

CUI_Cannon::~CUI_Cannon()
{
}

HRESULT CUI_Cannon::Initialize_Prototype()
{
	Create_CannonUI();
	Create_CoolTime();

	return S_OK;
}

HRESULT CUI_Cannon::Start()
{
	__super::Start();

	Init_CannonUI();
	Init_CoolTime();

	GET_COMPONENT_FROM(m_pCoolTime[Cool_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Cannon::Set_Shader_CoolTime, this, placeholders::_1, "g_fValue");

	return S_OK;
}

void CUI_Cannon::OnEnable()
{
	__super::OnEnable();

	for (int i = 0; i < 2; ++i)
		mPArrCannonUI[i]->SetActive(true);
}

void CUI_Cannon::OnDisable()
{
	__super::OnDisable();

	for (int i = 0; i < 2; ++i)
		mPArrCannonUI[i]->SetActive(false);
}

void CUI_Cannon::Set_Shader_CoolTime(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fCoolTimeRatio, sizeof(_float));
}

void CUI_Cannon::Set_CoolTime(_float fTime, _float fMaxTime)
{
	m_fCoolTimeRatio = fTime / fMaxTime;

	_tchar  szTemp[MAX_STR] = {};
	swprintf_s(szTemp, TEXT("%.1f"), fTime);
	m_pCoolTime[Cool_BG]->Set_FontText(szTemp);
}

void CUI_Cannon::SetActive_CoolTime(_bool value)
{
	for (int i = 0; i < Cool_End; ++i)
		m_pCoolTime[i]->SetActive(value);
}

void CUI_Cannon::Create_CannonUI()
{
	m_pCannonUI = CUI_Object::Create();

	GET_COMPONENT_FROM(m_pCannonUI, CTexture)->Remove_Texture(0);
	Read_Texture(m_pCannonUI, "/KeyIcon/Keyboard/White", "Key");

	m_pCannonUI->Set_PosY(-250.f);
	m_pCannonUI->Set_Sort(0.5f);

	m_pCannonUI->Set_Scale(20.f);

	m_pCannonUI->Set_FontRender(true);
	m_pCannonUI->Set_FontStyle(true);
	m_pCannonUI->Set_FontOffset(15.f, -15.f);
	m_pCannonUI->Set_FontScale(0.3f);

	for (int i = 0; i < 2; ++i)
		mPArrCannonUI[i] = m_pCannonUI->Clone();
}

void CUI_Cannon::Create_CoolTime()
{
	for (int i = 0; i < Cool_End; ++i)
	{
		m_pCoolTime[i] = CUI_Object::Create();

		m_pCoolTime[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Charging_Gauge.png"));
		m_pCoolTime[i]->Set_RotationZ(180.f);
	}

	m_pCoolTime[Cool_BG]->Set_Sort(0.5f);
	m_pCoolTime[Cool_BG]->Set_Color(RGB(0, 0, 0));
	m_pCoolTime[Cool_BG]->Set_FontRender(true);
	m_pCoolTime[Cool_BG]->Set_FontStyle(true);
	m_pCoolTime[Cool_BG]->Set_FontCenter(true);
	m_pCoolTime[Cool_BG]->Set_FontOffset(100.f, 3.f);
	m_pCoolTime[Cool_BG]->Set_FontScale(0.4f);

	m_pCoolTime[Cool_Gauge]->Set_Sort(0.49f);
	GET_COMPONENT_FROM(m_pCoolTime[Cool_Gauge], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_VerticalGauge);
}

void CUI_Cannon::Init_CannonUI()
{
	CREATE_GAMEOBJECT(m_pCannonUI, GROUP_UI);
	DISABLE_GAMEOBJECT(m_pCannonUI);

	for (int i = 0; i < 2; ++i)
	{
		CREATE_GAMEOBJECT(mPArrCannonUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(mPArrCannonUI[i]);
	}

	mPArrCannonUI[0]->Set_TextureIndex(61);
	mPArrCannonUI[0]->Set_FontText(TEXT("발사"));
	mPArrCannonUI[0]->Set_PosX(-150.f);

	mPArrCannonUI[1]->Set_TextureIndex(32);
	mPArrCannonUI[1]->Set_FontText(TEXT("탑승 해제"));
	mPArrCannonUI[1]->Set_PosX(100.f);
}

void CUI_Cannon::Init_CoolTime()
{
	for (int i = 0; i < Cool_End; ++i)
	{
		CREATE_GAMEOBJECT(m_pCoolTime[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pCoolTime[i]);
	}
}
