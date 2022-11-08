#pragma once
#include "CUI.h"



BEGIN(Client)

class CUI_LoadingIcon : public CUI
{
	DECLARE_PROTOTYPE(CUI_LoadingIcon);
	DECLARE_GAMEOBJECT(CUI_LoadingIcon);

private:
	CUI_LoadingIcon();
	CUI_LoadingIcon(const CUI_LoadingIcon& Prototype);
	virtual ~CUI_LoadingIcon();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	virtual void My_Tick();

public:
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);
private:
	CTexture* m_pIconDissolveTexture = nullptr;

	_float m_fCurValue = 0.f;

	bool m_bIsAppear = true;
};

END