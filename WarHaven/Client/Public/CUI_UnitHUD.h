#pragma once
#include "CUI_Wrapper.h"
#include "CUnit.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class CUnit;
class CUI_UnitHP;
class CUI_Revive;

class CUI_UnitHUD : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_UnitHUD);
	DECLARE_GAMEOBJECT(CUI_UnitHUD);

	enum UnitUI { UI_Hp, UI_Revive, UI_End };

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
	virtual void OnEnable() override;
	virtual void OnDisable() override;
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

public:
	CUI_UnitHP* Get_UnitHP();
	CUI_Revive* Get_ReviveUI();

	void Set_ProjPos(CTransform* pTransform);
	void Set_UnitStatus(CUnit::UNIT_STATUS tStatus) { m_tStatus = tStatus; }
	void Set_UnitDis(_float fDis) { m_fUnitDis = fDis; }

	void Set_OwnerPlayer(CPlayer* pOwner) { m_pOwner = pOwner; }

	void Enable_RevivalUI();
	void Disable_RevivalUI();
	void Set_RevivalGauge(_float fCurTime, _float fMaxTime);

	void Set_RevivalIcon(_uint iIconIdx);
	void SetActive_UnitHP(_bool value);

	void SetActive_TargetUI(_uint iIdx, _bool value);

private:
	CUI_Wrapper* m_pUnitUI[UI_End];

	CPlayer* m_pOwner = nullptr;

	CUnit::UNIT_STATUS m_tStatus;

	_float m_fLeaderIconScale = 24.f;
	_float m_fHeroIconScale = 32.f;

	_float4 m_vColorOrigin = _float4(1.f, 1.f, 1.f, 1.f);
	_float4 m_vColorRed = _float4(0.8f, 0.2f, 0.2f, 1.f);
	_float4 m_vColorGreen = _float4(0.2f, 0.5f, 0.2f, 1.f);
	_float4 m_vColorLightGreen = _float4(0.6f, 0.85f, 0.3f, 1.f);
	_float4 m_vColorBlue = _float4(0.25f, 0.65f, 0.9f, 1.f);
	_float4 vColorAlpha = _float4(1.f, 0.f, 0.f, 0.f);

	CUI_Object* m_pUnitNameText = nullptr;

	enum TargetUI { Target_Point, Target_Blink, Target_End };
	CUI_Object* m_pTargetUI[Target_End];

	_float4 m_vOffset;

	_float m_fUnitDis = 0.f;

	_bool m_bEnableTargetUI = false;
	_float m_fEanbleTargetUITime = 0.f;
	_float m_fMaxEanbleTargetUITime = 0.2f;
	_float m_fTargetRotValue = 0.f;
	_float m_fBlinkTime = 0.f;

	_uint m_fPrvRatio = 0.f;
	_uint m_fCurRatio = 0.f;

	_float m_fEnableHpTime = 0.f;
	_float m_fDisableHpTime = 5.f;

private:
	void Create_UnitHUD();
	void Create_TargetUI();

	void Init_UnitNameText();

private:
	void Tick_UnitHP();
	void Tick_TargetUI();
};

END