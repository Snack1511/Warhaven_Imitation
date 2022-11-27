#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Damage : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Damage);
	DECLARE_GAMEOBJECT(CUI_Damage);

public:
	enum DamageIcon { Head, Guard, Default, IconEnd };

private:
	CUI_Damage();
	virtual ~CUI_Damage();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void My_Tick() override;
	virtual void OnEnable() override;

public:
	void Enable_Damage(_uint eIcon, _float fDmg);

private:
	CUI_Object* m_pDmgIcon = nullptr;
	CUI_Object* m_pDmgNum = nullptr;
	CUI_Object* m_pArrDmgNum[3];

private:
	_float4 m_vColorWhite = _float4(1.f, 1.f, 1.f, 1.f);
	_float4 m_vColorGray = _float4(0.9f, 0.9f, 0.9f, 0.9f);
	_float4 m_vColorRed = _float4(0.9f, 0.f, 0.f, 1.f);

	_float2 m_vFontScale = _float2(60.f, 71.f);
	_float m_vHeadShotScale = 70.f;

	_float m_fFadeInTime = 0.3f;
	_float m_fFadeOutTime = 0.1f;
	_float m_fMaintainTime = 1.f;

	_float m_fScaleUpTime = 0.1f;
	_float m_fScaleDownTime = 0.2f;
	_float m_fScaleValue = 200.f;

	_uint m_iDamageValue = 0;

	DamageIcon m_eDamageIcon = DamageIcon::Default;

	_bool m_bIsHeadShot = false;
	_bool m_bIsScaleDown = false;

	vector<_uint> m_vecDigitDmg;

private:
	void Init_HeadShot();
	void Init_DmgNum();

	void Set_FadeDesc(CUI_Object* pOther);
};

END