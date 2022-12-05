#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_HpBar : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_HpBar);
	DECLARE_GAMEOBJECT(CUI_HpBar);

private:
	CUI_HpBar();
	virtual ~CUI_HpBar();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_Shader_Bar(CShader* pShader, const char* pConstName);

public:
	void Set_HP(_float fCurValue, _float fMaxValue);

	void SetActive_HP(_bool value);

private:
	enum HPUI { HP_BG, HP_Bar, HP_Text, HP_End };
	CUI_Object* m_pHPUI[HP_End];

	_float m_fPrvHpValue = 0.f;
	_float m_fCurHpValue = 0.f;
	_float m_fMaxHpValue = 0.f;
	_float m_fHpRatio = 0.f;

private:
	void Create_HPUI();

private:
	virtual void My_Tick() override;
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Set_Pass();
	void Bind_Shader();
};

END