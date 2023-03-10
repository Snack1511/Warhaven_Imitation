#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Portrait final : public CUI_Wrapper
{
private:
	enum HeroPortActive { Enable, Disable, End };

	DECLARE_PROTOTYPE(CUI_Portrait);
	DECLARE_GAMEOBJECT(CUI_Portrait);

private:
	CUI_Portrait();
	virtual ~CUI_Portrait();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();
	virtual void My_Tick() override;

public:
	void Set_UserPort(_uint iClass);
	void Set_HeroPort(_uint iType);

	void SetActive_UserPort(_bool value);
	void SetActive_HeroPort(_bool value);


private:
	_uint m_iPrvClass = -1;
	_uint m_iCurClass = 0;

	_bool m_bChangeUserPort = false;
	_bool m_bDoScaleUserPort = false;
	_uint m_iChangeUserPortCount = 1;
	_float m_fLerpUserPortDuration = 0.f;
	_float m_fLerpUserPortValue = 63.9f;

	CTexture* m_pUserPortTexture = nullptr;

private:
	enum UserPortrait { UP_BG, UP_Port, UP_Effect, UP_End };
	CUI_Object* m_pUserPortrait[UP_End];

private:
	void Create_UserPort();

	void Change_UserPort();
	void SetTexture_UserPort();
	void DoScale_UserPort(_bool value);
	void Set_FadeUserPort(_float fSpeed);

private:
	enum HeroPortrait { HP_BG, HP_Port, HP_Effect, HP_Key, HP_End };
	CUI_Object* m_pHeroPortrait[HP_End];
	CUI_Object* m_pArrHeroPortrait[HP_End][4];

	HeroPortActive m_eAcitveType = HeroPortActive::End;
	_bool m_bAbleHero = false;
	_bool m_bLerpHeroPort = false;
	_uint m_iFirstHeroPort = 0;
	_uint m_iLastHeroPort = 3;

private:
	void Create_HeroPort();
	void Set_FadeHeroPort(CUI_Object* pUI);

	void Active_HeroPort();

private:
	virtual void OnEnable() override;
	virtual void OnDisable() override;

private:
	void Set_Pass();
};

END