#pragma once
#include "Renderer.h"

BEGIN(Engine)

class CCustomFont;

class ENGINE_DLL CUI_Renderer : public CRenderer
{
	DECLARE_PROTOTYPE(CUI_Renderer)

private:
	CUI_Renderer(_uint iGroupID);
	virtual ~CUI_Renderer();

public:
	static CUI_Renderer* Create(_uint iGroupID, const RENDER_GROUP& eRenderGroup,
		const _uint& iCurPass, const _float4& vOffsetPos = _float4(0.f, 0.f, 0.f, 1.f));

public:
	virtual HRESULT		Initialize_Prototype() override;
	virtual HRESULT		Initialize() override;
	virtual void		Start() override;
	virtual void		Tick() override;
	virtual void		Late_Tick() override;
	virtual HRESULT		Render();
	virtual void		Release() override;

public:
	void		Set_Pass(const _uint& iCurPass) { m_iCurPass = iCurPass; }

public:
	enum FontSort { Left, Center, Right, SortEnd };

public:
	void Set_UI(CGameObject* pUI) { m_pUI = pUI; }
	void Set_RenderText(_bool value) { m_bIsRenderText = value; }
	void Set_Text(_bool value, wstring wstrText, _float4 vOffset, _float4 vColor, _float fScale);

private:
	CGameObject* m_pUI = nullptr;

private: // Font
	FontSort m_eSort = SortEnd;
	_bool m_bIsRenderText = false;
	_bool m_bIsBold = false;
	wstring m_wstrText;
	_float4 m_vOffset;
	_float4 m_vFontColor = { 1.f, 1.f, 1.f, 1.f };
	_float m_fFontScale = 1.f;

private:
	CCustomFont* m_pFont = nullptr;
};

END