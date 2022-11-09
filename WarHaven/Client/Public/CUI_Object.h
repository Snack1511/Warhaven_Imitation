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
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);

public:
	wstring Get_Name() { return m_wstrName; }
	void Set_Name(wstring str) { m_wstrName = str; }

	_bool Get_MouseTarget() { return m_bIsMouseTarget; }
	void Set_MouseTarget(_bool value) { m_bIsMouseTarget = value; }

	_bool Get_MultiTexture() { return m_bIsMultiTex; }
	void Set_MultiTexture(_bool value) { m_bIsMultiTex = value; }

private:
	wstring m_wstrName;

	CUI_Text* m_pText = nullptr;
	CUI_Button* m_pButton = nullptr;

	_bool m_bIsMultiTex = false;
	_bool m_bIsMouseTarget = false;
	_bool m_bIsOnMouse = false;

private:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();
	virtual void	My_Tick();
	virtual void	My_LateTick();

private:
	void MouseEvent();

private:
	virtual void OnMouseEnter() {};
	virtual void OnMouseExit() {};

	void OnMouseClick() {};
	//void OnMouseDown();
	//void OnMouseUp();
};

END