#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_HpBar : public CUI_Wrapper
{
	enum WindowType { BG, Bar, Type_End };

	DECLARE_PROTOTYPE(CUI_HpBar);
	DECLARE_GAMEOBJECT(CUI_HpBar);

private:
	CUI_HpBar();
	virtual ~CUI_HpBar();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;;

public:
	void Set_ShaderResourcesBar(CShader* pShader, const char* pConstName);
	void Set_ShaderResourcesBG(CShader* pShader, const char* pConstName);

private:
	CUI_Object* m_Prototypes[Type_End] = {};

	_float m_fValue = 0.f;

private:
	void Set_Pass();
	void Bind_Shader();
};

END