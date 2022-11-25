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

public:
	void SetActive_HpBar(_bool value);
	void Set_HpRatio(_float value) { m_fHealthRatio = value; }

private:
	CUI_Object* m_Prototypes[Type_End] = {};

	_float m_fHealthRatio = 0.f;
	_float m_fUVAnimSpeed = 0.f;

private:
	void Set_Pass();
	void Bind_Shader();
};

END