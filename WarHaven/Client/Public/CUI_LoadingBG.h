#pragma once
#include "CUI.h"

BEGIN(Client)

class CUI_LoadingBG final : public CUI
{
	DECLARE_PROTOTYPE(CUI_LoadingBG);
	DECLARE_GAMEOBJECT(CUI_LoadingBG);

private:
	CUI_LoadingBG();
	CUI_LoadingBG(const CUI_LoadingBG& Prototype);
	virtual ~CUI_LoadingBG();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void	My_Tick() override;
	virtual	void	OnEnable();

public:
	void SetLoadingBG(_uint iIndex) { m_iBGIndex = iIndex; }

private:
	_uint m_iBGIndex = 0;
	_float m_fAccTime = 0.f;
};

END