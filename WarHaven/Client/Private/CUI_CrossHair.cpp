#include "CUI_Crosshair.h"
#include "Functor.h"
#include "GameInstance.h"
#include "CUI_Object.h"
#include "Texture.h"
#include "CUnit.h"
#include "CUI_Renderer.h"
#include "CShader.h"
#include "Renderer.h"
#include "Easing_Utillity.h"

CUI_Crosshair::CUI_Crosshair()
{
}

CUI_Crosshair::~CUI_Crosshair()
{
}

HRESULT CUI_Crosshair::Initialize_Prototype()
{
	Create_Crosshair();
	Create_ArrowUI();
	Create_LancerUI();
	Create_GaugeUI();

	return S_OK;
}

HRESULT CUI_Crosshair::Initialize()
{
	return S_OK;
}

HRESULT CUI_Crosshair::Start()
{
	__super::Start();

	Bind_Shader();

	return S_OK;
}

void CUI_Crosshair::Set_Shader_Gauge(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fGaugeRatio, sizeof(_float));
}

void CUI_Crosshair::Set_Shader_LancerGauge0(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fLancerRatio[0], sizeof(_float));
}

void CUI_Crosshair::Set_Shader_LancerGauge1(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fLancerRatio[1], sizeof(_float));
}

void CUI_Crosshair::Set_Shader_LancerGauge2(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fLancerRatio[2], sizeof(_float));
}

void CUI_Crosshair::Set_Shader_LancerGauge3(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_fValue", &m_fLancerRatio[3], sizeof(_float));
}

void CUI_Crosshair::Set_Crosshair(_uint iClass)
{
	m_iClassIndex = iClass;
	if (m_iClassIndex < LANCER)
	{
		SetActive_LancerUI(false);
	}

	switch (m_iClassIndex)
	{
	case WARRIOR:
		Set_DefaultCrosshair();
		break;

	case ARCHER:
	case QANDA:
		Set_ArcherCrosshair();
		break;

	case ENGINEER:
		Set_DefaultCrosshair();
		break;

	case LANCER:
		SetActive_LancerUI(true);
		break;
	}

	Set_ArrowUI();
}

void CUI_Crosshair::SetActive_CannonCrosshair(_bool value)
{
	SetActive_LancerUI(false);

	if (value == true)
	{
		m_pCrosshair[CU_Point]->Set_TextureIndex(1);
		m_pCrosshair[CU_Point]->Set_Scale(32.f);
		m_pCrosshair[CU_Outline]->Set_TextureIndex(1);
	}
	else
	{
		m_pCrosshair[CU_Point]->Set_Scale(6.f);

		switch (m_iClassIndex)
		{
		case WARRIOR:
			Set_DefaultCrosshair();
			break;

		case ARCHER:
		case QANDA:
			Set_ArcherCrosshair();
			break;

		case ENGINEER:
			Set_DefaultCrosshair();
			break;

		case LANCER:
			SetActive_LancerUI(true);
			break;
		}
	}
}

void CUI_Crosshair::SetActive_Crosshair(_bool value)
{
	if (value == true)
	{
		for (int i = 0; i < CU_End; ++i)
		{
			ENABLE_GAMEOBJECT(m_pCrosshair[i]);
		}
	}
	else
	{
		for (int i = 0; i < CU_End; ++i)
		{
			DISABLE_GAMEOBJECT(m_pCrosshair[i]);
		}
	}
}

void CUI_Crosshair::SetActive_ArrowUI(_bool value)
{
	for (int i = 0; i < AU_End; ++i)
	{
		for (int j = 0; j < m_iArrowIndex; ++j)
		{
			if (m_pArrArrowUI[i][j]->Is_Valid() == !value)
			{
				m_pArrArrowUI[i][j]->SetActive(value);
			}
		}
	}
}

void CUI_Crosshair::SetActive_LancerUI(_bool value)
{
	for (int i = 0; i < 4; ++i)
		m_fLancerRatio[i] = 1.f;

	for (int i = 0; i < LU_Full; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			m_pArrLancerUI[j][i]->SetActive(value);
		}
	}
}

void CUI_Crosshair::Set_Position(_float4 vPos)
{
	for (int i = 0; i < CU_End; ++i)
	{
		m_pCrosshair[i]->Set_Pos(vPos);
	}

	for (int i = 0; i < AU_End; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			m_pArrArrowUI[i][j]->Set_Pos(vPos);
		}
	}
}

void CUI_Crosshair::Set_ArcherPoint(_bool value)
{
	if (m_iClassIndex != ARCHER && m_iClassIndex != QANDA)
		return;

	if (value == true)
	{
		m_pCrosshair[CU_Point]->Set_Color(_float4(0.8f, 0.2f, 0.2f, 1.f));
		GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(1);
	}
	else
	{
		m_pCrosshair[CU_Point]->Set_Color(_float4(1.f, 1.f, 1.f, 1.f));
		GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(0);
	}
}

void CUI_Crosshair::Set_LancerGauge(_uint iGaugeIdx, _float fCurTime, _float fMaxTime)
{
	m_fLancerRatio[iGaugeIdx] = 1.f - (fCurTime / fMaxTime);
	if (m_fLancerRatio[iGaugeIdx] <= 0.1f)
	{
		m_pArrLancerUI[iGaugeIdx][LU_Full]->SetActive(true);
		m_pArrLancerUI[iGaugeIdx][LU_Full]->Set_Scale(45.f, 95.f);
		m_pArrLancerUI[iGaugeIdx][LU_Full]->DoScale(-10.f, 0.2f);
	}
}

void CUI_Crosshair::Disable_LacnerGauge()
{
	for (int j = 0; j < 4; ++j)
		m_pArrLancerUI[j][LU_Full]->SetActive(false);
}

void CUI_Crosshair::Set_BreezeTime(_float fCurTime, _float fMaxTime)
{
	m_fGaugeRatio = fCurTime / fMaxTime;

	_tchar  szTemp[MAX_STR] = {};
	swprintf_s(szTemp, TEXT("%.1f"), -(fCurTime - 5.f));
	m_pGaugeUI[Gauge_BG]->Set_FontText(szTemp);
}

void CUI_Crosshair::SetActive_Gauge(_bool value)
{
	for (int i = 0; i < Gauge_End; ++i)
		m_pGaugeUI[i]->SetActive(value);
}

void CUI_Crosshair::Create_Crosshair()
{
	for (int i = 0; i < CU_End; ++i)
	{
		m_pCrosshair[i] = CUI_Object::Create();

		m_pCrosshair[i]->Set_Sort(0.5f);

		if (i == CU_Point)
		{
			m_pCrosshair[i]->Set_Scale(6.f);
		}
		else if (i == CU_Outline)
		{
			m_pCrosshair[i]->Set_Scale(50.f);
			m_pCrosshair[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.4f));
		}

		CREATE_GAMEOBJECT(m_pCrosshair[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pCrosshair[i]);
	}

	GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Remove_Texture(0);
	Read_Texture(m_pCrosshair[CU_Point], "/HUD/Crosshair", "Point");

	GET_COMPONENT_FROM(m_pCrosshair[CU_Outline], CTexture)->Remove_Texture(0);
	Read_Texture(m_pCrosshair[CU_Outline], "/HUD/Crosshair", "Outline");
}

void CUI_Crosshair::Set_DefaultCrosshair()
{
	GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(0);
	GET_COMPONENT_FROM(m_pCrosshair[CU_Outline], CTexture)->Set_CurTextureIndex(0);
}

void CUI_Crosshair::Set_ArcherCrosshair()
{
	GET_COMPONENT_FROM(m_pCrosshair[CU_Point], CTexture)->Set_CurTextureIndex(0);
	GET_COMPONENT_FROM(m_pCrosshair[CU_Outline], CTexture)->Set_CurTextureIndex(1);
}

void CUI_Crosshair::Create_ArrowUI()
{
	for (int i = 0; i < AU_End; ++i)
	{
		m_pArrowUI[i] = CUI_Object::Create();

		m_pArrowUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Arrow.png"));

		if (i == AU_BG)
		{
			m_pArrowUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.6f));
			m_pArrowUI[i]->Set_Sort(0.5f);
			m_pArrowUI[i]->Set_Scale(100.f);
		}
		else if (i == AU_Arrow)
		{
			m_pArrowUI[i]->Set_Color(_float4(1.f, 1.f, 1.f, 0.f));
			m_pArrowUI[i]->Set_Sort(0.49f);
			m_pArrowUI[i]->Set_Scale(130.f);
		}

		CREATE_GAMEOBJECT(m_pArrowUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pArrowUI[i]);

		for (int j = 0; j < 3; ++j)
		{
			m_pArrArrowUI[i][j] = m_pArrowUI[i]->Clone();

			CREATE_GAMEOBJECT(m_pArrArrowUI[i][j], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrArrowUI[i][j]);
		}
	}
}

void CUI_Crosshair::Set_ArrowUI()
{
	if (m_iClassIndex == SPEAR || m_iClassIndex == ARCHER || m_iClassIndex == QANDA)
	{
		m_iArrowIndex = 3;
	}
	else if (m_iClassIndex == PRIEST || m_iClassIndex == ENGINEER)
	{
		m_iArrowIndex = 2;
	}
	else
	{
		return;
	}

	for (int i = 0; i < AU_End; ++i)
	{
		for (int j = 0; j < m_iArrowIndex; ++j)
		{
			_float fRotZ = 0.f;

			if (m_iArrowIndex == 2)
			{
				fRotZ = -90.f - (j * 180.f);
			}
			else if (m_iArrowIndex == 3)
			{
				fRotZ = -120.f * j;
			}

			m_pArrArrowUI[i][j]->Set_RotationZ(fRotZ);
		}
	}
}

void CUI_Crosshair::Charge_Arrow()
{
	if (m_iClassIndex != ARCHER && m_iClassIndex != QANDA)
		return;

	_float fChargeWaitTime = m_iClassIndex == QANDA ? 0.75f : 0.3f;
	_float fRotateTime = fChargeWaitTime / (_float)m_iArrowIndex;
	_float fScaleTime = 0.1f;
	_float fAngle = 360.f / (_float)m_iArrowIndex;

	if (KEY(LBUTTON, HOLD))
	{
		if (!m_bIsCharge)
		{
			if (m_bIsChargeWait)
				return;

			m_bIsCharge = true;

			SetActive_ArrowUI(true);
		}
		else
		{
			m_fAccTime += fDT(0);
			if (m_fAccTime > fChargeWaitTime)
			{
				m_fAccTime = 0.f;

				if (m_bIsChargeWait)
				{
					if (m_iChargeCount > 2)
					{
						for (int i = 0; i < 3; ++i)
						{
							m_pArrArrowUI[AU_Arrow][i]->Set_Color(_float4(1.f, 0.f, 0.f, 1.f));
						}

						return;
					}

					Rotate_Arrow(fAngle, fRotateTime);

					m_bIsChargeWait = false;
					m_bIsCharge = false;
				}
				else
				{
					m_bIsChargeWait = true;

					if (m_iChargeCount > 2)
						return;

					m_pArrArrowUI[AU_Arrow][m_iChargeCount]->DoScale(-30.f, fScaleTime);
					m_pArrArrowUI[AU_Arrow][m_iChargeCount++]->Set_Color(m_vArrowColor);
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < 3; ++i)
		{
			m_pArrArrowUI[AU_Arrow][i]->Set_Scale(130.f);
			m_pArrArrowUI[AU_Arrow][i]->Set_Color(_float4(0.f, 0.f, 0.f, 0.f));
		}

		m_bIsCharge = false;
		m_bIsChargeWait = false;
		m_iChargeCount = 0;

		m_fAccTime = 0.f;

		Set_ArrowUI();
		SetActive_ArrowUI(false);
	}
}

void CUI_Crosshair::Rotate_Arrow(_float fAngle, _float fDuration)
{
	for (int i = 0; i < AU_End; ++i)
	{
		for (int j = 0; j < m_iArrowIndex; ++j)
		{
			m_pArrArrowUI[i][j]->DoRotate(fAngle, fDuration);
		}
	}
}

void CUI_Crosshair::Create_LancerUI()
{
	for (int i = 0; i < LU_End; ++i)
	{
		m_pLancerUI[i] = CUI_Object::Create();

		m_pLancerUI[i]->Set_PosY(-100.f);
		m_pLancerUI[i]->Set_Scale(35.f, 85.f);

		switch (i)
		{
		case LU_BG:
			m_pLancerUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Lancer_ArrowBG.png"));
			break;

		case LU_Gauge:
			GET_COMPONENT_FROM(m_pLancerUI[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_VerticalGauge);
			m_pLancerUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Lancer_ArrowGauge.png"));
			break;

		case LU_Full:
			m_pLancerUI[i]->Set_Scale(45.f, 95.f);
			m_pLancerUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/HUD/Crosshair/Lancer_ArrowFull.png"));
			break;
		}

		CREATE_GAMEOBJECT(m_pLancerUI[i], GROUP_UI);
		DELETE_GAMEOBJECT(m_pLancerUI[i]);

		for (int j = 0; j < 4; ++j)
		{
			m_pArrLancerUI[j][i] = m_pLancerUI[i]->Clone();

			_float fPosX = -45.f + (j * 30.f);
			m_pArrLancerUI[j][i]->Set_PosX(fPosX);

			CREATE_GAMEOBJECT(m_pArrLancerUI[j][i], GROUP_UI);
			DISABLE_GAMEOBJECT(m_pArrLancerUI[j][i]);
		}
	}
}

void CUI_Crosshair::Create_GaugeUI()
{
	for (int i = 0; i < Gauge_End; ++i)
	{
		m_pGaugeUI[i] = CUI_Object::Create();

		m_pGaugeUI[i]->Set_Texture(TEXT("../Bin/Resources/Textures/UI/Cannon/T_128HalfCircleOutline4px.png"));
		// m_pGaugeUI[i]->Set_RotationZ(180.f);

		switch (i)
		{
		case Gauge_BG:
			m_pGaugeUI[i]->Set_Sort(0.5f);
			m_pGaugeUI[i]->Set_Color(RGB(0, 0, 0));

			m_pGaugeUI[i]->Set_FontRender(true);
			m_pGaugeUI[i]->Set_FontStyle(true);
			m_pGaugeUI[i]->Set_FontCenter(true);
			m_pGaugeUI[i]->Set_FontOffset(100.f, 3.f);
			m_pGaugeUI[i]->Set_FontScale(0.4f);
			break;

		case Gauge_Bar:
			m_pGaugeUI[i]->Set_Sort(0.49f);
			GET_COMPONENT_FROM(m_pGaugeUI[i], CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_VerticalGauge);
			break;
		}

		CREATE_GAMEOBJECT(m_pGaugeUI[i], GROUP_UI);
		DISABLE_GAMEOBJECT(m_pGaugeUI[i]);
	}
}

void CUI_Crosshair::My_Tick()
{
	__super::My_Tick();

	Charge_Arrow();
}

void CUI_Crosshair::OnEnable()
{
	__super::OnEnable();

	SetActive_Crosshair(true);

	if (m_iClassIndex == LANCER)
		SetActive_LancerUI(true);
}

void CUI_Crosshair::OnDisable()
{
	__super::OnDisable();

	SetActive_Crosshair(false);
	SetActive_ArrowUI(false);
	SetActive_LancerUI(false);
}

void CUI_Crosshair::Bind_Shader()
{
	GET_COMPONENT_FROM(m_pGaugeUI[Gauge_Bar], CShader)->CallBack_SetRawValues += bind(&CUI_Crosshair::Set_Shader_Gauge, this, placeholders::_1, "g_fValue");

	GET_COMPONENT_FROM(m_pArrLancerUI[0][LU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Crosshair::Set_Shader_LancerGauge0, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrLancerUI[1][LU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Crosshair::Set_Shader_LancerGauge1, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrLancerUI[2][LU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Crosshair::Set_Shader_LancerGauge2, this, placeholders::_1, "g_fValue");
	GET_COMPONENT_FROM(m_pArrLancerUI[3][LU_Gauge], CShader)->CallBack_SetRawValues += bind(&CUI_Crosshair::Set_Shader_LancerGauge3, this, placeholders::_1, "g_fValue");
}
