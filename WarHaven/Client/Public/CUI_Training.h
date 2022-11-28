#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Training : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Training);
	DECLARE_GAMEOBJECT(CUI_Training);

private:
	CUI_Training();
	virtual ~CUI_Training();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void My_Tick() override;

public:
	void Enable_Popup(_uint iIndex);
	void Disable_Popup();

private:
	CUI_Object* m_pPopup = nullptr;

	// 작고 한 줄
	_float2 m_vShotPopupScale = _float2(437, 90.f);
	// 길고 한 줄
	_float2 m_vLongPopupScale = _float2(660.f, 90.f);
	// 길고 두 줄
	_float2 m_vLongPopupScale2 = _float2(880.f, 140.f);
	// 더 길고 두 줄
	_float2 m_vLongPopupScale3 = _float2(1500.f, 140.f);

private:
	void Init_Popup();
};

END