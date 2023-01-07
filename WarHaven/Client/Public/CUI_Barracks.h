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
	virtual void On_PointerDown_Btn(const _uint& iEventNum);

	virtual void On_PointerDown_TopBtn(const _uint& iEventNum);

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

	_uint m_iSelectClass = 0;

	int m_iPrvSelectSkin = -1;
	int m_iCurSelectSkin = 0;

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
	CUI_Object* m_pTopBtn[4];

	// 왼쪽 아이템 설명
	enum Skin_Info { Skin_Name, Skin_Tier, Skin_End };
	CUI_Object* m_pSkinInfo[Skin_End];

	// 오른쪽 아이템 창
	enum SkinBtn { SB_BG, SB_Outline, SB_Select, SB_Lock, SB_Blind, SB_End };
	CUI_Object* m_pSkinBtn[SB_End];
	CUI_Object* m_pArrSkinBtn[3][SB_End];

	enum Skin { Clothes, Weapon, Hat, Glider, End };
	CUI_Object* m_pSkin[Skin::End];
	CUI_Object* m_pArrSkin[3][Skin::End];

private:
	void Create_ClassPort();
	void Create_ClassInfo();
	void Create_ClassBtn();

	void Create_TopBtn();
	void Create_SkinInfo();
	void Create_SkinBtn();
	void Crate_Skin();

	void Init_ClassPort();
	void Init_ClassInfo();
	void Init_ClassBtn();

	void Init_TopBtn();
	void Init_SkinInfo();
	void Init_SkinBtn();
	void Init_Skin();

	void Bind_Btn();

	void Set_ClassInfoText(_uint iEventNum);

private:
	_bool m_bIsEnable = false;
	_float m_fEnableTime = 0.f;
	_float m_fEnableMaxTime = 0.1f;

	_bool m_bIsSkinEnable = false;
	_float m_fSkinEnableTime = 0.f;
	_float m_fSkinEnableMaxTime = 0.1f;

	void Late_Enable();
	void Late_SkinEnable();

	void Set_SkinIdx(CLASS_TYPE eClass);
};

