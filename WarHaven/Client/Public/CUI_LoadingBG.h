#pragma once
#include "CUI.h"

BEGIN(Client)

class CLoading_Manager;
class CUI_Object;

class CUI_LoadingBG final : public CUI
{
	DECLARE_PROTOTYPE(CUI_LoadingBG);
	DECLARE_GAMEOBJECT(CUI_LoadingBG);

private:
	CUI_LoadingBG();
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

	enum LoadingText { LT_Map, LT_Loading, LT_Mode, LT_Gibberish, LT_End };

	CUI_Object* m_pLoadingText[LT_End];

private:
	void Create_LoadingText(LEVEL_TYPE_CLIENT eLevel);

	wstring Init_GibberishText();
};

END