#pragma once
#include "CUI_Wrapper.h"

BEGIN(Client)

class CUI_Portrait final : public CUI_Wrapper
{
	enum PortIndex { User, Hero1, Hero4 = 4, PortEnd = 5 };
	enum UI_TYPE { BG, Port, Key, Effect, Type_End };

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
	void Set_Portrait(_uint iIndex);

protected:
	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
	CUI_Object* m_Prototypes[Type_End] = {};
	CUI_Object* m_arrPortraitUI[5][Type_End] = {};

	_float m_fEffectValue = 0.f;

	_bool m_bIsHero = false;
	_uint m_iPrvPort = 0;
	_uint m_iCurPort = 0;

	_bool m_bIsUserLerp = false;
	_bool m_bAbleRotationPort = false;
	_uint m_iRotationCount = 0.f;

	_bool m_bIsHeroLerp = false;
	_uint m_iHeroLerpStartCount = Hero1;
	_uint m_iHeroLerpEndCount = Hero4;

	_float m_fMinValue = 0.01f;

private:
	void Enable_UserPortrait();
	void Enable_HeroPortrait();

	void Set_Pass();
	void Bind_Shader();

	void Change_UserPort();

	void Enable_HeroPort();
	void Disable_HeroPort();

	void PortSizeUP();
	void PortSizeDown();
};

END