#pragma once
#include "GameObject.h"
#include "Client_Defines.h"


BEGIN(Engine)
class CShader;
END

BEGIN(Client)

class CUnit;

class CCure_Effect final
	: public CGameObject
{
	DECLARE_PROTOTYPE(CCure_Effect);

private:
	CCure_Effect();
	virtual ~CCure_Effect();

public:
	static CCure_Effect* Create(CGameObject* pOwner);

public:
	virtual void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

	virtual void My_Tick() override;
	virtual void My_LateTick() override;

	virtual void OnEnable() override;
	virtual void OnDisable() override;


private:
	CGameObject* m_pOwner = nullptr;
	CGameObject* m_pOther = nullptr;

	list<CGameObject*> m_Smoke;
	list<CGameObject*> m_Particle;

	wstring m_wstrPath;

	_float4	m_vEffectFlag = SH_EFFECT_DEFAULT;
	_float4	m_vGlowFlag = GLOW_CHAKRA(1.f);

	_float	m_fAlpha = 1.f;
	_float	m_fCurUVPlusX = 0.f;
	_float	m_fCurUVPlusY = 0.f;

	_float	m_fUVSpeedX = 0.f;
	_float	m_fUVSpeedY = 0.f;

	_float4x4 m_matTrans;

	_uint		m_iPassType = 0;

	_byte		m_bEffectFlag = 0;

	_float4 m_vPlusColor = _float4(0.f, 0.f, 0.f, 0.f);
	_float	m_fColorPower = 1.f;

};
END