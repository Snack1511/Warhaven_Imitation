#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_CharacterWindow : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_CharacterWindow);
	DECLARE_GAMEOBJECT(CUI_CharacterWindow);

private:
	CUI_CharacterWindow();
	virtual ~CUI_CharacterWindow();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT Start();

public:
	virtual void On_PointerEnter(const _uint& iEventNum);
	virtual void On_PointerExit(const _uint& iEventNum);
	virtual void On_PointerDown(const _uint& iEventNum);

public:
	void SetActive_CharacterWindow(_bool value);

private:
	_uint m_iPrvEventNum = 0;
	_uint m_iCurEventNum = 0;

	CLASS_TYPE m_eCurUnitClass = CLASS_TYPE::WARRIOR;

private:
	CUI_Object* m_pBG = nullptr;

private:
	void Create_BG();

private:
	enum CharacterUI { CU_BG, CU_Port, CU_Highlight, CU_Icon, CU_Line, CU_End };
	CUI_Object* m_pCharacterUI[CU_End];
	CUI_Object* m_pArrCharacterUI[CU_End][6];

	_float4 m_vColorGold = _float4(0.773f, 0.714f, 0.596f, 1.f);

private:
	void Create_CharacterUI();
	void Set_FadeDesc(CUI_Object* pUI, _float fSpeed);

private:
	enum ClassInfo { CI_BG, CI_Icon, CI_End };
	CUI_Object* m_pClassInfo[CI_End];

private:
	void Create_ClassInfo();

private:
	virtual void OnEnable();
	virtual void OnDisable();

	void Bind_Btn();

	void Init_CharacterWindow(CLASS_TYPE eCurUnitClass);
};

END