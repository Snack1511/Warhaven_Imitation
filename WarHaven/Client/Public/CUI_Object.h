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
	CUI_Object(const CUI_Object& Prototype);
	virtual ~CUI_Object();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	// ��Ƽ �ؽ���
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

public:
	void Lerp_Scale(_float fStart, _float fEnd, _float fDuration);
	void Lerp_ScaleX(_float fStart, _float fEnd, _float fDuration);

private:
	wstring m_wstrName;

	CUI_Text* m_pText = nullptr;
	CUI_Button* m_pButton = nullptr;

	_bool m_bIsMultiTex = false;
	_bool m_bIsMouseTarget = false;
	_bool m_bIsInMouse = false;	


	_bool m_bLerpScale = false;
	_bool m_bLerpScaleX = false;

	_bool m_fAccScale = false;

	_float m_fAccTime = 0.f;
	_float m_fStart = 0.f;
	_float m_fEnd = 0.f;
	_float m_fDuration = 0.f;

	_float m_fMinValue = 0.01f;

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

	_float Min(_float fValue);
};

END