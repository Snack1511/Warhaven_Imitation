#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Damage : public CUI_Wrapper
{
	DECLARE_PROTOTYPE(CUI_Damage);
	DECLARE_GAMEOBJECT(CUI_Damage);

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

	virtual void OnEnable() override;

public:
	void Enable_Damage(_float fDmg, _bool bHeadShot = false);

private:
	CUI_Object* m_pHeadShot = nullptr;
	CUI_Object* m_pDmgNum = nullptr;
	CUI_Object* m_pArrDmgNum[3];

private:
	_float4 vColorWhite = _float4(1.f, 1.f, 1.f, 1.f);
	_float4 vColorRed = _float4(1.f, 0.f, 0.f, 1.f);

	_float m_fFadeInTime = 0.3f;
	_float m_fFadeOutTime = 0.1f;

	_uint m_iDamageValue = 0.f;

	_bool m_bIsHeadShot = false;

	vector<_float> m_vecDigitDmg;

private:
	void Init_HeadShot();
	void Init_DmgNum();

	void Set_FadeDesc(CUI_Object* pOther);
};

END