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

	CUI_Object* m_pNextMapName = nullptr;
	CUI_Object* m_pGibberish = nullptr;
	CUI_Object* m_pLoddingText = nullptr;
	CUI_Object* m_pModeText = nullptr;

private:
	void Create_LoadingText(LEVEL_TYPE_CLIENT eLevel, wstring eName);
};

END