#include "stdafx.h"
#include "CUI_Animation.h"
#include "CUI_Renderer.h"
#include "UsefulHeaders.h"

CUI_Animation::CUI_Animation()
{
}

CUI_Animation::CUI_Animation(const CUI_Animation& Prototype)
	: CUI(Prototype)
{
}

CUI_Animation::~CUI_Animation()
{
}

void CUI_Animation::Trun_UIAnimation(ANIMINFO tAniminfo)
{
	m_tAniminfo = tAniminfo;
	
	Set_Pos(m_tAniminfo.vPos.x, m_tAniminfo.vPos.y);
	Set_Scale(m_tAniminfo.vScale.x, m_tAniminfo.vScale.y);

}

HRESULT CUI_Animation::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/Effects/WarHaven/Texture/T_Fire_09.dds"));

	GET_COMPONENT(CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_ANIMATION);

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

	while (m_fTimeAcc > m_tAniminfo.fDuration * 
		((m_fRowX + 1.f) + m_tAniminfo.iHeightSize * m_fColY))
	{
		m_fRowX += 1.f;
		if (m_fRowX >= m_tAniminfo.iWidthSize)
		{
			m_fRowX = 0.f;
			m_fColY += 1.f;
			if (m_fColY >= m_tAniminfo.iHeightSize)
			{
				//여기 들어왔다 : 한바퀴돌아서 1순한거임
				m_fTimeAcc = 0.f;
				m_fRowX = m_tAniminfo.iWidthSize - 1.f;
				m_fColY = m_tAniminfo.iHeightSize - 1.f;

				if (m_tAniminfo.bPlayOnce)
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
	pShader->Set_RawValue("g_bBlackBG", &m_tAniminfo.bBlackBackGround, sizeof(_bool));
	pShader->Set_RawValue("g_iWidthSize", &m_tAniminfo.iWidthSize, sizeof(_uint));
	pShader->Set_RawValue("g_iHeightSize", &m_tAniminfo.iHeightSize, sizeof(_uint));
	pShader->Set_RawValue("g_fRowX", &m_fRowX, sizeof(_float));
	pShader->Set_RawValue("g_fColY", &m_fColY, sizeof(_float));
}
