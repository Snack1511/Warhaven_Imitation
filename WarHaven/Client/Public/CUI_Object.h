#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_Button;
class CUI_Text;

class CUI_Object : public CUI
{
	DECLARE_PROTOTYPE(CUI_Object);
	DECLARE_GAMEOBJECT(CUI_Object);

private:
	CUI_Object();
	CUI_Object(const CUI_Object& Prototype);
	virtual ~CUI_Object();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_Texture(const _tchar* pFilePath);

public:
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);

public:
	void SetMouseTarget(_bool value) { m_bIsMouseTarget = value; }

private:
	CUI_Text* m_pText = nullptr;
	CUI_Button* m_pButton = nullptr;

	_bool m_bIsMouseTarget = false;

	RECT m_tRect;
	POINT m_ptMouse;

	_bool m_bIsOnMouse = false;

private:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();
	virtual void	My_Tick();
	virtual void	My_LateTick();

private:
	void CheckInRect();
	void MouseEvent();

private:
	virtual void OnMouseEnter() {};
	virtual void OnMouseExit() {};

	void OnMouseClick() {};
	//void OnMouseDown();
	//void OnMouseUp();
};

END