#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_HpBar : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_HpBar);
	DECLARE_GAMEOBJECT(CUI_HpBar);

	typedef struct tagHpBarHud
	{
		enum SKILLHUDNAME { BG, Bar, NAME_END };
		CUI_Object* m_pUIInstance[NAME_END] = {};
	}HpBar;

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
	HpBar m_tHpBar;

	_float m_fValue = 0.f;
};

END