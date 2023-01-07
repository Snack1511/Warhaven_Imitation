#pragma once
#include "CUI_Wrapper.h"
class CUI_Barracks : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Barracks);
	DECLARE_GAMEOBJECT(CUI_Barracks);

private:
	CUI_Barracks();
	virtual ~CUI_Barracks();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void On_PointerEnter_Port(const _uint& iEventNum);
	virtual void On_PointerExit_Port(const _uint& iEventNum);
	virtual void On_PointerDown_Port(const _uint& iEventNum);

	virtual void On_PointerEnter_Btn(const _uint& iEventNum);
	virtual void On_PointerStay_Btn(const _uint& iEventNum);
	virtual void On_PointerExit_Btn(const _uint& iEventNum);

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	list<CUI_Object*> m_pUIList;

	_float4 m_vColorGold = _float4(0.773f, 0.714f, 0.596f, 1.f);

	_float4 m_vColorBG = _float4(0.1f, 0.1f, 0.1f, 1.f);
	_float4 m_vColorOutline = _float4(0.2f, 0.2f, 0.2f, 1.f);

	int m_iPrvEventNum = -1;
	int m_iCurEventNum = -1;

	_float m_fDuration = 0.3f;

	_float m_fOriginPosY = -250.f;
	_float m_fTargetPosY = -240.f;

	_bool m_bColorLerp = false;
	_float m_fColorLerpTime = 0.f;

	_float4 m_vLerpStartColor;
	_float4 m_vLerpEndColor;
	CUI_Object* m_pLerpTargetUI = nullptr;

private:
	enum ClassPort { Port_BG, Port_Char, Port_Class, Port_Highlight, Port_Outline, Port_Underline, Port_End };
	CUI_Object* m_pClassPort[Port_End];
	CUI_Object* m_pArrClassPort[CLASS_END][Port_End];

	enum ClassInfo { Info_BG, Info_Class, Info_End };
	CUI_Object* m_pClassInfo[Info_End];

	enum ClassBtn { Btn_BG, Btn_Outline, Btn_Deco, Btn_Text, Btn_LockText, Btn_Lock, Btn_End };
	CUI_Object* m_pClassBtn[Btn_End];
	CUI_Object* m_pArrClassBtn[3][Btn_End];

private:
	void Create_ClassPort();
	void Create_ClassInfo();
	void Create_ClassBtn();

	void Init_ClassPort();
	void Init_ClassInfo();
	void Init_ClassBtn();

	void Bind_Btn();

	void Set_ClassInfoText(_uint iEventNum);

	void Lerp_Color(CUI_Object* pUI, _float4 vColor);
	void Update_Color();
};

