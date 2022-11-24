#pragma once
#include "CUI_Wrapper.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class CUnit;

class CUI_UnitHUD : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_UnitHUD);
	DECLARE_GAMEOBJECT(CUI_UnitHUD);

	enum UnitUI { UI_Hp, UI_End };

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

public:
	void Set_ProjPos(CTransform* pTransform);
	void Set_UnitStatus(CUnit::UNIT_STATUS tStatus) { m_tStatus = tStatus; }
	void Set_UnitDis(_float fDis) { m_fUnitDis = fDis; }

private:
	CUI_Wrapper* m_pUnitUI[UI_End];

	CUnit::UNIT_STATUS m_tStatus;

	_float4 vColorRed = _float4(1.f, 0.f, 0.f, 1.f);
	_float4 vColorAlpha = _float4(1.f, 0.f, 0.f, 0.f);

	CUI_Object* m_pUnitNameText = nullptr;
	
	_float m_fUnitDis = 0.f;

private:
	void Create_UnitHUD();

	void Init_UnitNameText();

private:
	void SetActive_UnitHP(_bool value);
};

END