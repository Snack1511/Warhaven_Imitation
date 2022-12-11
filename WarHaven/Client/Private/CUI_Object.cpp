#include "CUI_Object.h"
#include "GameInstance.h"
#include "Texture.h"
#include "Loading_Manager.h"
#include "Transform.h"
#include "Functor.h"
#include "CUI_Renderer.h"
#include "CShader.h"
#include "CButton.h"
#include "CFader.h"

#include "Easing_Utillity.h"

CUI_Object::CUI_Object()
{
}


CUI_Object::~CUI_Object()
{
}

HRESULT CUI_Object::Initialize_Prototype()
{
	CButton* pButton = CButton::Create(0);
	Add_Component<CButton>(pButton);

	__super::Initialize_Prototype();

	SetTexture(TEXT("../Bin/Resources/Textures/White.png"));

	GET_COMPONENT(CUI_Renderer)->Set_Pass(VTXTEX_PASS_UI_Color);

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

void CUI_Object::Lerp_ScaleY(_float fStart, _float fEnd, _float fDuration)
{
	m_bLerpScaleY = true;

	m_fStart = Min(fStart);
	m_fEnd = Min(fEnd);
	m_fDuration = fDuration;

	m_fAccScale = m_fEnd > m_fStart ? true : false;

	Set_ScaleY(m_fStart);
}

void CUI_Object::Lerp_PosX(_float fStart, _float fEnd, _float fDuration)
{
	m_bLerpPosX = true;

	m_fStart = fStart;
	m_fEnd = fEnd;
	m_fDuration = fDuration;

	m_fAccScale = m_fEnd > m_fStart ? true : false;
}

void CUI_Object::Lerp_PosY(_float fStart, _float fEnd, _float fDuration)
{
	m_bLerpPosY = true;

	m_fStart = fStart;
	m_fEnd = fEnd;
	m_fDuration = fDuration;

	m_fAccScale = m_fEnd > m_fStart ? true : false;
}

void CUI_Object::DoMove(_float fPosX, _float fPosY, _float fDuration)
{
	m_bIsDoMove = true;

	m_vOriginPos = Get_Pos();
	//m_vOriginPos = m_pTransform->Get_World(WORLD_POS);

	m_fGoalPosX = fPosX;
	m_fGoalPosY = fPosY;

	m_fMoveDuration = fDuration;
	m_fMoveAccTime = 0.f;
}

void CUI_Object::DoMoveY(_float fMoveValue, _float fDuration)
{
	m_bIsDoMoveY = true;

	m_vOriginPos = Get_Pos();
	m_fMoveValue = fMoveValue;
	m_fMoveDurationY = fDuration;
	m_fMoveYAccTime = 0.f;
}

void CUI_Object::DoMoveX(_float fMoveValue, _float fDuration)
{
	m_bIsDoMoveX = true;

	m_vOriginPos = Get_Pos();
	m_fMoveValue = fMoveValue;
	m_fMoveDurationX = fDuration;
	m_fMoveXAccTime = 0.f;
}

void CUI_Object::DoScale(_float fScaleValue, _float fDuration)
{
	m_bIsDoScale = true;

	m_vOriginScale = Get_Scale();
	m_fScaleValue = fScaleValue;
	m_fScaleDuration = fDuration;
	m_fScaleAccTime = 0.f;
}

void CUI_Object::DoScaleX(_float fScaleValue, _float fDuration)
{
	m_bIsDoScaleX = true;

	m_vOriginScale.x = Get_Scale().x;
	m_fScaleValue = fScaleValue;
	m_fScaleDuration = fDuration;
	m_fScaleAccTime = 0.f;
}

void CUI_Object::Fade_Font(_bool value, _float fDuration)
{
	m_bIsFontFade = true;

	m_bIsFadeIn = value;
	m_fDuration = fDuration;
}

void CUI_Object::Set_FadeDesc(_float fDuration)
{
	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));

	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;

	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = FADE_NONE;

	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = fDuration;

	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = fDuration;

	GET_COMPONENT_FROM(this, CFader)->Get_FadeDesc() = tFadeDesc;
}

void CUI_Object::Set_FadeDesc(_float fFadeIn, _float fFadeOut, _bool bAutoFadeOut)
{
	FADEDESC tFadeDesc;
	ZeroMemory(&tFadeDesc, sizeof(FADEDESC));

	tFadeDesc.eFadeOutType = FADEDESC::FADEOUT_DISABLE;
	tFadeDesc.eFadeStyle = FADEDESC::FADE_STYLE_DEFAULT;

	tFadeDesc.bFadeInFlag = FADE_NONE;
	tFadeDesc.bFadeOutFlag = bAutoFadeOut ? FADE_TIME : FADE_NONE;

	tFadeDesc.fFadeInStartTime = 0.f;
	tFadeDesc.fFadeInTime = fFadeIn;

	tFadeDesc.fFadeOutStartTime = 0.f;
	tFadeDesc.fFadeOutTime = fFadeOut;

	GET_COMPONENT_FROM(this, CFader)->Get_FadeDesc() = tFadeDesc;
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

	RenderText();

	Lerp_Scale();
	Lerp_Position();

	DoMove();
	DoScale();

	Fade_Font();
}

void CUI_Object::My_LateTick()
{
	__super::My_LateTick();
}

void CUI_Object::RenderText()
{
	GET_COMPONENT(CUI_Renderer)->Set_RenderText(m_bIsRenderText);

	if (m_bIsRenderText)
	{
		GET_COMPONENT(CUI_Renderer)->Set_Text(m_bIsBold, m_wstrText, m_vOffset, m_vFontColor, m_fFontScale, m_bIsCenter);
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
				m_fAccTime = 0.f;
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
				m_fAccTime = 0.f;
				m_bLerpScaleX = false;
			}
		}
	}

	if (m_bLerpScaleY)
	{
		m_fAccTime += fDT(0);

		_float4 vScale = m_pTransform->Get_Scale();
		_float fSpeed = ((m_fStart - m_fEnd) / m_fDuration) * fDT(0);

		if (!m_fAccScale)
		{
			vScale -= fabs(fSpeed);
			vScale.y = Min(vScale.y);

			Set_ScaleY(vScale.y);

			if (vScale.y <= m_fEnd)
			{
				Set_ScaleY(m_fEnd);
				m_fAccTime = 0.f;
				m_bLerpScaleY = false;
			}
		}
		else
		{
			vScale += fabs(fSpeed);
			Set_ScaleY(vScale.y);

			if (vScale.y >= m_fEnd)
			{
				Set_ScaleY(m_fEnd);
				m_fAccTime = 0.f;
				m_bLerpScaleY = false;
			}
		}
	}
}

void CUI_Object::Lerp_Position()
{
	if (m_bLerpPosX)
	{
		m_fAccTime += fDT(0);

		_float fCurPos = Get_PosX();
		_float fMoveValue = m_fEnd - m_fStart;
		_float fMoveSpeed = (fMoveValue / m_fDuration) * fDT(0);

		Set_PosX(fCurPos + fMoveSpeed);

		if (m_fAccTime >= m_fDuration)
		{
			m_fAccTime = 0.f;
			m_bLerpPosX = false;
		}
	}

	if (m_bLerpPosY)
	{
		m_fAccTime += fDT(0);

		_float fCurPos = Get_PosY();
		_float fMoveValue = m_fEnd - m_fStart;
		_float fMoveSpeed = (fMoveValue / m_fDuration) * fDT(0);

		Set_PosY(fCurPos + fMoveSpeed);

		if (m_fAccTime >= m_fDuration)
		{
			m_fAccTime = 0.f;
			m_bLerpPosY = false;
		}
	}
}

void CUI_Object::DoMove()
{
	if (m_bIsDoMove)
	{
		m_fMoveAccTime += fDT(0);
		if (m_fMoveAccTime > m_fMoveDuration)
		{
			Set_Pos(m_fGoalPosX, m_fGoalPosY);

			m_fMoveAccTime = 0.f;
			m_bIsDoMove = false;
			return;
		}

		_float4 vGoalPos = m_vOriginPos;
		vGoalPos.x = m_fGoalPosX;
		vGoalPos.y = m_fGoalPosY;


		_float4 vResult = CEasing_Utillity::QuadOut(m_vOriginPos, vGoalPos, m_fMoveAccTime, m_fMoveDuration);
		Set_Pos(vResult);

		/*_float fDisPosX = fabs(m_vOriginPos.x - m_fGoalPosX);
		_float fDisPosY = fabs(m_vOriginPos.y - m_fGoalPosY);

		fDisPosX = (m_vOriginPos.x > m_fGoalPosX) ? -fDisPosX : fDisPosX;
		fDisPosY = (m_vOriginPos.y > m_fGoalPosY) ? -fDisPosY : fDisPosY;

		_float fMoveValueX = (fDisPosX / m_fMoveDuration) * fDT(0);
		_float fMoveValueY = (fDisPosY / m_fMoveDuration) * fDT(0);

		_float4 vMovePos = _float4(fMoveValueX, fMoveValueY, 0.f);
		_float4 vSetPos = Get_Pos() + vMovePos;

		Set_Pos(vSetPos);*/

		
	}

	if (m_bIsDoMoveY)
	{
		m_fMoveYAccTime += fDT(0);

		_float fCurPosY = Get_PosY();
		_float fMoveValue = (m_fMoveValue / m_fMoveDurationY) * fDT(0);
		_float fResultPos = fCurPosY + fMoveValue;

		Set_PosY(fResultPos);

		if (m_fMoveYAccTime >= m_fMoveDurationY)
		{
			_float fResultPosY = m_vOriginPos.y + m_fMoveValue;
			Set_PosY(fResultPosY);

			m_fMoveYAccTime = 0.f;
			m_bIsDoMoveY = false;
		}
	}

	if (m_bIsDoMoveX)
	{
		m_fMoveXAccTime += fDT(0);

		_float fCurPosX = Get_PosX();
		_float fMoveValue = (m_fMoveValue / m_fMoveDurationX) * fDT(0);
		_float fResultPos = fCurPosX + fMoveValue;

		Set_PosX(fResultPos);

		if (m_fMoveXAccTime >= m_fMoveDurationX)
		{
			_float fResultPosY = m_vOriginPos.x + m_fMoveValue;
			Set_PosX(fResultPosY);

			m_fMoveXAccTime = 0.f;
			m_bIsDoMoveX = false;
		}
	}
}

void CUI_Object::DoScale()
{
	if (m_bIsDoScale)
	{
		m_fScaleAccTime += fDT(0);

		_float4 vCurScale = Get_Scale();

		_float fScaleValue = (m_fScaleValue / m_fScaleDuration) * fDT(0);

		vCurScale.x += fScaleValue;
		vCurScale.y += fScaleValue;

		Set_Scale(vCurScale.x, vCurScale.y);

		if (Get_FontRender())
		{
			_float fFontScale = Get_FontScale();
			_float fAddValue = 0.01f * fScaleValue;
			_float fResultFontScale = fFontScale + fAddValue;
			Set_FontScale(fResultFontScale);

			_float4 vFontOffset = Get_FontOffset();
			vFontOffset.x += fScaleValue * 0.4f;
			vFontOffset.y -= fScaleValue * 0.4f;
			Set_FontOffset(vFontOffset.x, vFontOffset.y);
		}

		if (m_fScaleAccTime >= m_fScaleDuration)
		{
			_float fResultScaleX = m_vOriginScale.x + m_fScaleValue;
			_float fResultScaleY = m_vOriginScale.y + m_fScaleValue;
			Set_Scale(fResultScaleX, fResultScaleY);

			m_fScaleAccTime = 0.f;
			m_bIsDoScale = false;
		}
	}

	if (m_bIsDoScaleX)
	{
		m_fScaleAccTime += fDT(0);

		_float vCurScaleX = Get_Scale().x;

		_float fScaleValueX = (m_fScaleValue / m_fScaleDuration) * fDT(0);

		vCurScaleX += fScaleValueX;

		Set_ScaleX(vCurScaleX);

		if (m_fScaleAccTime >= m_fScaleDuration)
		{
			_float fResultScaleX = m_vOriginScale.x + m_fScaleValue;
			Set_ScaleX(fResultScaleX);

			m_fScaleAccTime = 0.f;
			m_bIsDoScaleX = false;
		}
	}
}

void CUI_Object::Fade_Font()
{
	if (m_bIsFontFade)
	{
		m_fAccTime += fDT(0) * m_fDuration; // 0.3f

		_float4 vStartColor = Get_FontColor();

		// ���̵� �ƿ�
		if (!m_bIsFadeIn)
		{
			vStartColor.x -= m_fAccTime;
			vStartColor.y -= m_fAccTime;
			vStartColor.z -= m_fAccTime;
			vStartColor.w -= m_fAccTime;

			Set_FontColor(vStartColor);

			if (vStartColor.w <= 0.f)
			{
				m_fAccTime = 0.f;
				m_bIsFontFade = false;

				DISABLE_GAMEOBJECT(this);
			}
		}
		else
		{
			vStartColor.x += m_fAccTime;
			vStartColor.y += m_fAccTime;
			vStartColor.z += m_fAccTime;
			vStartColor.w += m_fAccTime;

			Set_FontColor(vStartColor);

			if (vStartColor.w >= 1.f)
			{
				m_fAccTime = 0.f;
				m_bIsFontFade = false;
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
