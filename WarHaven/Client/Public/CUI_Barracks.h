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

private:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	list<CUI_Object*> m_pUIList;

	_float4 m_vColorGold = _float4(0.773f, 0.714f, 0.596f, 1.f);

	int m_iPrvEventNum = -1;
	int m_iCurEventNum = -1;

	_float m_fDuration = 0.3f;

	_float m_fOriginPosY = -250.f;
	_float m_fTargetPosY = -240.f;

private:
	enum CharacterPort { Port_BG, Port_Char, Port_Class, Port_Highlight, Port_Outline, Port_Underline, Port_End };
	CUI_Object* m_pCharacterPort[Port_End];
	CUI_Object* m_pArrCharacterPort[CLASS_END][Port_End];

private:
	void Create_CharacterPort();

	void Init_CharacterPort();

	void Bind_Btn();
};

