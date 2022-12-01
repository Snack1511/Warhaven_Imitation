#include "stdafx.h"
#include "CUI_Animation.h"
#include "CUI_Renderer.h"
#include "UsefulHeaders.h"

CUI_Animation::CUI_Animation()
{
}

CUI_Animation::~CUI_Animation()
{
}

HRESULT CUI_Animation::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/Effects/WarHaven/Texture/T_Fire_21.dds"));

	GET_COMPONENT(CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_ANIMATION);

	Set_Pos(500.f, -200.f);
	Set_Scale(200, 300);

	m_fTimeAcc = 0.f;
	m_fDuration = 0.020f; //프레임당 속도
	m_iHeightSize = 8; //애니메이션 가로세로 사이즈
	m_iWidthSize = 8;
	m_fRowX = 0.f;
	m_fColY = 0.f;

	return S_OK;
}

HRESULT CUI_Animation::Initialize()
{

	return S_OK;
}

HRESULT CUI_Animation::Start()
{
	__super::Start();


	return S_OK;
}

void CUI_Animation::OnEnable()
{
	m_fTimeAcc = 0.f;
	m_fRowX = 0.f;
	m_fColY = 0.f;
	__super::OnEnable();
}

void CUI_Animation::OnDisable()
{


	__super::OnDisable();
}

void CUI_Animation::My_Tick()
{
	__super::My_Tick();

	m_fTimeAcc += fDT(0);

	//UV넘기는 코드

	while (m_fTimeAcc > m_fDuration *
		((m_fRowX + 1.f) + m_iHeightSize * m_fColY))
	{
		m_fRowX += 1.f;
		if (m_fRowX >= m_iWidthSize)
		{
			m_fRowX = 0.f;
			m_fColY += 1.f;
			if (m_fColY >= m_iHeightSize)
			{
				//여기 들어왔다 : 한바퀴돌아서 1순한거임
				m_fTimeAcc = 0.f;
				m_fRowX = m_iWidthSize - 1.f;
				m_fColY = m_iHeightSize - 1.f;

				if (m_bPlayOnce)
					DISABLE_GAMEOBJECT(this);
				else
				{
					m_fRowX = 0.f;
					m_fColY = 0.f;
				}
			}
		}
	}
}

void CUI_Animation::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	pShader->Set_RawValue("g_bBlackBG", &m_bBlackBackGround, sizeof(_bool));
	pShader->Set_RawValue("g_iWidthSize", &m_iWidthSize, sizeof(_uint));
	pShader->Set_RawValue("g_iHeightSize", &m_iHeightSize, sizeof(_uint));
	pShader->Set_RawValue("g_fRoWX", &m_fRowX, sizeof(_float));
	pShader->Set_RawValue("g_fColY", &m_fColY, sizeof(_float));
}
