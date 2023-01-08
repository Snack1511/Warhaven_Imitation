#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Cannon : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Cannon);
	DECLARE_GAMEOBJECT(CUI_Cannon);

private:
	CUI_Cannon();
	virtual ~CUI_Cannon();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual HRESULT	Start();

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

public:
	virtual void Set_Shader_CoolTime(CShader* pShader, const char* pConstName);

public:
	void Set_CoolTime(_float fTime, _float fMaxTime);

	void SetActive_CoolTime(_bool value);

private:
	CUI_Object* m_pCannonUI = nullptr;
	CUI_Object* mPArrCannonUI[2];

	enum CoolTime { Cool_BG, Cool_Gauge, Cool_End };
	CUI_Object* m_pCoolTime[Cool_End];

	_float m_fCoolTimeRatio = 1.f;

private:
	void Create_CannonUI();
	void Create_CoolTime();

	void Init_CannonUI();
	void Init_CoolTime();
};

END