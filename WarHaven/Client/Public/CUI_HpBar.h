#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_HpBar : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_HpBar);
	DECLARE_GAMEOBJECT(CUI_HpBar);

private:
	CUI_HpBar();
	CUI_HpBar(const CUI_HpBar& Prototype);
	virtual ~CUI_HpBar();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;

public:
	void Set_ShaderResources(CShader* pShader, const char* pConstName);

private:
	_float m_fValue = 0.f;
};

END