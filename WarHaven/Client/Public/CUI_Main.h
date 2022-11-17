#pragma once
#include "CUI_Wrapper.h"
class CUI_Main : public CUI_Wrapper
{
	enum MainUI { Btn, Key, Goods, MainEnd };
	enum UI_TYPE { Play, Barracks, Profile, TypeEnd };

	DECLARE_PROTOTYPE(CUI_Main);
	DECLARE_GAMEOBJECT(CUI_Main);

private:
	CUI_Main();
	virtual ~CUI_Main();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void Set_Shader_BtnHighlight(CShader* pShader, const char* pConstName);

public:
	virtual void On_PointEnter_TopBtn(const _uint& iEventNum);
	virtual void On_PointDown_TopBtn(const _uint& iEventNum);

private:
	CUI_Wrapper* m_Prototypes[TypeEnd] = {};

	CUI_Object* m_pPrototypeUI[MainEnd] = {};
	CUI_Object* m_pTopBtn[3] = {};
	CUI_Object* m_pKeyUI[2] = {};
	CUI_Object* m_pGoodsUI[3] = {};

	CUI_Object* m_pBtnHighlight = nullptr;

private:
	void Ready_MainUI();

	void Create_BtnHighlight();

	void Enable_MainUI();
	void Enable_MainWindow();
};

