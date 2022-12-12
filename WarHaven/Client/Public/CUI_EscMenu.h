#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_EscMenu : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_EscMenu);
	DECLARE_GAMEOBJECT(CUI_EscMenu);

private:
	CUI_EscMenu();
	virtual ~CUI_EscMenu();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

public:
	virtual void On_PointDown_MenuBtn(const _uint& iEventNum);

public:
	void SetActive_EscMenu(_bool value);

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Bind_Btn();

private:
	CUI_Object* m_pBG = nullptr;
	CUI_Object* m_pIcon = nullptr;
	CUI_Object* m_pMenuBtn = nullptr;

	list<CUI_Object*> m_pEscList;

private:
	void Create_EscMenu();
	void Fadenable_EscMenu();
	void Fadisable_EscMenu();
};

END