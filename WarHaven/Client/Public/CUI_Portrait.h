#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Portrait final : public CUI_Wrapper
{
	enum PortIndex { User, Hero1, Hero4 = 4, PortEnd = 5 };
	enum WindowType { BG, Port, Key, Effect, Type_End };

public:
	enum HeroPortAnimType { Enable, Disable, AnimEnd };

	DECLARE_PROTOTYPE(CUI_Portrait);
	DECLARE_GAMEOBJECT(CUI_Portrait);

private:
	CUI_Portrait();
	virtual ~CUI_Portrait();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize();
	virtual HRESULT	Start();

public:
	void Set_ShaderEffect(CShader* pShader, const char* constName);

public:
	void Start_Portrait(_uint iIndex);
	void Set_Portrait(_uint iIndex);

	void Set_HeroPort(HeroPortAnimType eState);

protected:
	virtual void My_Tick() override;

private:
	CUI_Object* m_Prototypes[Type_End] = {};
	CUI_Object* m_arrPortraitUI[5][Type_End] = {};

	_float m_fEffectValue = 0.f;

	_uint m_iPrvPort = 0;
	_uint m_iCurPort = 0;

	_bool m_bIsUserLerp = false;
	_bool m_bAbleRotationPort = false;
	_uint m_iRotationCount = 0.f;

	HeroPortAnimType m_eHeroPortAnimType = HeroPortAnimType::AnimEnd;
	_bool m_bAbleHero = false;
	_bool bIsHero = false;

	_bool m_bIsHeroLerp = false;
	_uint m_iHeroStartIdx = Hero1;
	_uint m_iHeroEndIdx = Hero4;
	_uint m_iHeroActiveCount = 0;

	_float m_fMinValue = 0.01f;

private:	// Shader
	void Set_Pass();
	void Bind_Shader();

	void Set_FadeUserPort(_float fSpeed);
	void Set_FadeHeroPort();

private:
	void Change_UserPort();
	void PortSizeUP(_float fDuration);
	void PortSizeDown(_float fDuration);

	void Enable_HeroLerp(_bool value, _float fDuration);

private:
	void Ready_Portrait();
};

END