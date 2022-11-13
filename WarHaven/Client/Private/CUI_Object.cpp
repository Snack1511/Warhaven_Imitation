#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Loading_Manager.h"
#include "Transform.h"
#include "Functor.h"
#include "Renderer.h"
#include "CShader.h"

CUI_Object::CUI_Object()
{
}

CUI_Object::CUI_Object(const CUI_Object& Prototype)
	: CUI(Prototype)
{
}

CUI_Object::~CUI_Object()
{
}

HRESULT CUI_Object::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/White.png"));

	GET_COMPONENT(CRenderer)->Set_Pass(VTXTEX_PASS_ALPHA);

	Set_Pos(0.f, 0.f);
	Set_Scale(100.f);

	m_wstrName = TEXT("UI_Object");

	return S_OK;
}

HRESULT CUI_Object::Initialize()
{
	return S_OK;
}

HRESULT CUI_Object::Start()
{
	__super::Start();

	return S_OK;
}

void CUI_Object::Set_Texture(const _tchar* pFilePath)
{
	if (m_bIsMultiTex)
	{
		CTexture* pTexture = GET_COMPONENT(CTexture);
		pTexture->Add_Texture(pFilePath);
		pTexture->Next_Texture();
	}
	else
	{
		CTexture* pTexture = GET_COMPONENT(CTexture);
		pTexture->Add_Texture(pFilePath);
		pTexture->Next_Texture();
		pTexture->Remove_Texture(0);
		pTexture->Set_CurTextureIndex(0);
	}
}

void CUI_Object::SetUp_ShaderResource(CShader* pShader, const char* pConstName)
{
	__super::SetUp_ShaderResource(pShader, pConstName);
}

void CUI_Object::Set_FontOffset(_float fX, _float fY)
{
	m_vOffset.x = fX;
	m_vOffset.y = fY;
	m_vOffset.z = 0.f;
}

void CUI_Object::Lerp_Scale(_float fStart, _float fEnd, _float fDuration)
{
	m_bLerpScale = true;

	m_fStart = Min(fStart);
	m_fEnd = Min(fEnd);
	m_fDuration = fDuration;

	m_fAccScale = m_fEnd > m_fStart ? true : false;

	Set_Scale(m_fStart);
}

void CUI_Object::Lerp_ScaleX(_float fStart, _float fEnd, _float fDuration)
{
	m_bLerpScaleX = true;

	m_fStart = Min(fStart);
	m_fEnd = Min(fEnd);
	m_fDuration = fDuration;

	m_fAccScale = m_fEnd > m_fStart ? true : false;

	Set_ScaleX(m_fStart);
}

void CUI_Object::OnEnable()
{
	__super::OnEnable();
}

void CUI_Object::OnDisable()
{
	__super::OnDisable();
}

void CUI_Object::My_Tick()
{
	__super::My_Tick();

	MouseEvent();

	RenderText();

	Lerp_Scale();
}

void CUI_Object::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Object::MouseEvent()
{
	CheckInRect();

	m_bIsInMouse = PtInRect(&m_tRect, m_ptMouse) ? true : false;

	if (m_bIsMouseTarget)
	{
		if (m_bIsInMouse)
		{
			OnMouseEnter();

			//if (m_pButton)
			//{
			//	// 버튼 이벤트 함수 호출

			//	/*if (KEY(LBUTTON, TAP))
			//	{
			//		OnMouseClick();
			//	}*/
			//}
		}
		else
		{
			OnMouseExit();
		}
	}
}

void CUI_Object::Create_Text(_bool value, wstring wstrText, _float4 vOffset, _float4 vColor, _float fScale)
{
	m_bIsBold = value;
	m_wstrName = wstrText;
	m_vOffset = vOffset;
	m_vColor = vColor;
	m_fFontScale = fScale;
}

void CUI_Object::RenderText()
{
	if (m_bIsRenderText)
	{
		_float4 vUIPos = m_pTransform->Get_World(WORLD_POS);
		_float2 vFontPos = { vUIPos.x + m_vOffset.x, vUIPos.y + m_vOffset.y };

		const _tchar* wszText = m_wstrText.c_str();

		if (m_bIsBold)
		{
			CGameInstance::Get_Instance()->Render_Font(TEXT("War_Bold"), wszText, vFontPos, m_vColor, m_fFontScale);
		}
		else
		{
			CGameInstance::Get_Instance()->Render_Font(TEXT("War_Regular"), wszText, vFontPos, m_vColor, m_fFontScale);
		}
	}
}

void CUI_Object::Lerp_Scale()
{
	if (m_bLerpScale)
	{
		m_fAccTime += fDT(0);

		_float4 vScale = m_pTransform->Get_Scale();
		_float fSpeed = ((m_fStart - m_fEnd) / m_fDuration) * fDT(0);

		if (!m_fAccScale)
		{
			vScale -= fabs(fSpeed);
			Set_Scale(vScale.x, vScale.y);

			if (vScale.x <= m_fEnd)
			{
				Set_Scale(vScale.x, vScale.y);
				m_bLerpScale = false;
			}
		}
		else
		{
			vScale += fabs(fSpeed);
			Set_Scale(vScale.x, vScale.y);

			if (vScale.x >= m_fEnd)
			{
				Set_Scale(vScale.x, vScale.y);
				m_bLerpScale = false;
			}
		}
	}

	if (m_bLerpScaleX)
	{
		m_fAccTime += fDT(0);

		_float4 vScale = m_pTransform->Get_Scale();
		_float fSpeed = ((m_fStart - m_fEnd) / m_fDuration) * fDT(0);

		if (!m_fAccScale)
		{
			vScale -= fabs(fSpeed);
			vScale.x = Min(vScale.x);

			Set_ScaleX(vScale.x);

			if (vScale.x <= m_fEnd)
			{
				Set_ScaleX(m_fEnd);
				m_bLerpScaleX = false;
			}
		}
		else
		{
			vScale += fabs(fSpeed);
			Set_ScaleX(vScale.x);

			if (vScale.x >= m_fEnd)
			{
				Set_ScaleX(m_fEnd);
				m_bLerpScaleX = false;
			}
		}
	}
}

_float CUI_Object::Min(_float fValue)
{
	if (fValue <= m_fMinValue)
	{
		return m_fMinValue;
	}
	else
	{
		return fValue;
	}
}
