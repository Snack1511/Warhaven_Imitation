#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_Button;
class CUI_Text;

class CUI_Object final : public CUI
{
	DECLARE_PROTOTYPE(CUI_Object);
	DECLARE_GAMEOBJECT(CUI_Object);

private:
	CUI_Object();
	virtual ~CUI_Object();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	// 멀티 텍스쳐
	void Set_Texture(const _tchar* pFilePath);
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);

public:
	wstring Get_Name() { return m_wstrName; }
	void Set_Name(wstring str) { m_wstrName = str; }
	_bool Get_MouseTarget() { return m_bIsMouseTarget; }
	void Set_MouseTarget(_bool value) { m_bIsMouseTarget = value; }
	_bool Get_MultiTexture() { return m_bIsMultiTex; }
	void Set_MultiTexture(_bool value) { m_bIsMultiTex = value; }
	_bool Get_IsInMouse() { return m_bIsInMouse; }

public:	// Font
	_bool Get_FontRender() { return m_bIsRenderText; }
	void Set_FontRender(_bool value) { m_bIsRenderText = value; }
	_bool Get_FontStyle() { return m_bIsBold; }
	void Set_FontStyle(_bool value) { m_bIsBold = value; }
	wstring Get_FontText() { return m_wstrText; }
	void Set_FontText(wstring szText) { m_wstrText = szText; }
	_float4 Get_FontOffset() { return m_vOffset; }
	void Set_FontOffset(_float fX, _float fY);
	_float4 Get_FontColor() { return m_vFontColor; }
	void Set_FontColor(_float4 vColor) { m_vFontColor = vColor; }
	_float Get_FontScale() { return m_fFontScale; }
	void Set_FontScale(_float fValue) { m_fFontScale = fValue; }

	_bool Get_FontCenter() { return m_bIsCenter; }
	void Set_FontCenter(_bool value) { m_bIsCenter = value; }

public:	// Button
	_bool Get_IsClick() { return m_bIsClick; }
	void Set_IsClick(_bool value) { m_bIsClick = value; }

public:	// Lerp
	void Lerp_Scale(_float fStart, _float fEnd, _float fDuration);
	void Lerp_ScaleX(_float fStart, _float fEnd, _float fDuration);
	void Lerp_ScaleY(_float fStart, _float fEnd, _float fDuration);

	void Lerp_PosX(_float fStart, _float fEnd, _float fDuration);
	void Lerp_PosY(_float fStart, _float fEnd, _float fDuration);

	void DoMove(_float fPosX, _float fPosY, _float fDuration);

	void DoMoveY(_float fMoveValue, _float fDuration);
	void DoMoveX(_float fMoveValue, _float fDuration);

	void DoScale(_float fScaleValue, _float fDuration);
	void DoScaleX(_float fScaleValue, _float fDuration);

	void Fade_Font(_bool value, _float fDuration);

public:
	void Set_FadeDesc(_float fDuration);
	void Set_FadeDesc(_float fFadeIn, _float fFadeOut, _bool bAutoFade = false);

private:
	wstring m_wstrName;

private:	// Font
	_bool m_bIsRenderText = false;
	_bool m_bIsBold = false;
	_bool m_bIsCenter = false;
	wstring m_wstrText;
	_float4 m_vOffset;
	_float4 m_vFontColor = { 1.f,1.f,1.f,1.f };
	_float m_fFontScale = 1.f;
	_bool m_bIsFontFade = false;
	_bool m_bIsFadeIn = false;

private:	// Button
	_bool m_bIsMouseTarget = false;
	_bool m_bIsClick = false;

private:	// Etc
	_float m_fMinValue = 0.01f;
	_float m_fAccTime = 0.f;
	_bool m_bIsMultiTex = false;
	_bool m_bIsInMouse = false;

private:	// DoMove
	_bool m_bIsDoMove = false;

	_float4 m_vOriginPos;
	_float m_fGoalPosX = 0.f;
	_float m_fGoalPosY = 0.f;

	_float m_fMoveAccTime = 0.f;
	_float m_fMoveDuration = 0.f;

private:	// Lerp
	_float m_fMoveXAccTime = 0.f;
	_float m_fMoveYAccTime = 0.f;
	_bool m_bIsDoMoveY = false;
	_bool m_bIsDoMoveX = false;

	_float4 m_vOriginScale;
	_float m_fScaleAccTime = 0.f;
	_bool m_bIsDoScale = false;
	_bool m_bIsDoScaleX = false;

	_float m_fMoveDurationX = 0.f;
	_float m_fMoveDurationY = 0.f;
	_float m_fScaleDuration = 0.f;

	_float m_fMoveValue = 0.f;
	_float m_fScaleValue = 0.f;

	_bool m_bLerpPosX = false;
	_bool m_bLerpPosY = false;

	_bool m_bLerpScale = false;
	_bool m_bLerpScaleX = false;
	_bool m_bLerpScaleY = false;
	_bool m_fAccScale = false;
	_float m_fStart = 0.f;
	_float m_fEnd = 0.f;
	_float m_fDuration = 0.f;

private:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();
	virtual void	My_Tick();
	virtual void	My_LateTick();

private:
	void RenderText();
	void Lerp_Scale();
	void Lerp_Position();
	void DoMove();
	void DoScale();
	void Fade_Font();

private:
	_float Min(_float fValue);
};

END