#pragma once
#include "CUI.h"

BEGIN(Client)
class CUnit;

class CBloodOverlay final : public CUI
{
	DECLARE_PROTOTYPE(CBloodOverlay);
	DECLARE_GAMEOBJECT(CBloodOverlay);

private:
	CBloodOverlay();
	CBloodOverlay(const CBloodOverlay& Prototype);
	virtual ~CBloodOverlay();

public:
	void Trun_BloodOverlay(_float fHpRatio);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
	virtual void OnEnable();
	virtual void OnDisable();

public:
	virtual void My_Tick();

public:
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);
	

private:
	_uint m_fImgIdx = 0;

};

END