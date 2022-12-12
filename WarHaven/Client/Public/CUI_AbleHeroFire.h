#pragma once
#include "Client_Defines.h"
#include "CUI.h"


BEGIN(Client)
class CUI_AbleHeroFire : public CUI
{
	DECLARE_GAMEOBJECT(CUI_AbleHeroFire);
	DECLARE_PROTOTYPE(CUI_AbleHeroFire);

private:
	CUI_AbleHeroFire();
	virtual ~CUI_AbleHeroFire();

public:
	void Turn_AbleHeroFire();
	// CGameObject을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT	Start();

protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();

public:
	virtual void My_Tick();

public:
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName) override;

private:
	_float	m_fScale = 0.f;
	_float	m_fTargetScale = 0.f;
	_float  m_fTimeAcc = 0.f;
	_float	m_fAlphaAcc = 0.f;
	_float	m_fCurUVPlusX = 0.f;
	_float	m_fCurUVPlusY = 0.f;

	_float	m_fUVSpeedX = 0.f;
	_float	m_fUVSpeedY = 0.f;

	_float	m_fAlpha = 1.f;
	_float4 m_vPlusColor = _float4(0.f, 0.f, 0.f, 0.f);


};

END
