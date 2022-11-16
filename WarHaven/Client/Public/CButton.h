#pragma once
#include "Component.h"
#include "Client_Defines.h"


#define ADD_MOUSE_IN_EVENT(UIinstance, iEventNum) static_cast<CUI*>(UIinstance)->CallBack_MouseIn += bind(&On_MouseInEvent, this, iEventNum);
#define DELETE_MOUSE_IN_EVENT(UIinstance, iEventNum) static_cast<CUI*>(UIinstance)->CallBack_MouseIn -= bind(&On_MouseInEvent, this, iEventNum);

#define ADD_MOUSE_EXIT_EVENT(UIinstance, iEventNum) static_cast<CUI*>(UIinstance)->CallBack_MouseExit += bind(&On_MouseExitEvent, this, iEventNum);
#define DELETE_MOUSE_EXIT_EVENT(UIinstance, iEventNum) static_cast<CUI*>(UIinstance)->CallBack_MouseExit -= bind(&On_MouseExitEvent, this, iEventNum);

#define ADD_BUTTONCLICK_EVENT(UIinstance, iEventNum) static_cast<CUI*>(UIinstance)->CallBack_ButtonClick += bind(&On_ButtonClickEvent, this, iEventNum);
#define DELETE_BUTTONCLICK_EVENT(UIinstance, iEventNum) static_cast<CUI*>(UIinstance)->CallBack_ButtonClick -= bind(&On_ButtonClickEvent, this, iEventNum);

BEGIN(Engine)

class CShader;

END

BEGIN(Client)

class CUI;

class CButton : public CComponent
{
public:
	DECLARE_PROTOTYPE(CButton);

private:
	CButton(_uint iGroupIdx);
	~CButton();

public:
	static CButton* Create(_uint iGroupID);

public:
	virtual void	Set_ShaderResource(CShader* pShader, const char* pConstantName) override;

public:
	virtual void	On_MouseInEvent(const _uint& iEventNum);
	virtual void	On_MouseExitEvent(const _uint& iEventNum);
	virtual void	On_ButtonClickEvent(const _uint& iEventNum);

public:
	// CComponent을(를) 통해 상속
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;

	virtual void Start() override;
	virtual void Tick() override;
	virtual void Late_Tick() override;

public:
	void Set_OwnerUI(CUI* pUI) { m_pOwnerUI = pUI; }

private:
	CUI* m_pOwnerUI = nullptr;

	RECT m_tRect;
	POINT m_ptMouse;

	_bool m_bIsInMouse = false;

private:
	virtual void Release() override;

private:
	void Set_Rect();
};

END
