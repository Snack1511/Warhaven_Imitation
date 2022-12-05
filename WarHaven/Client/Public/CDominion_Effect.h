#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)
class CTeamConnector;

class CDominion_Effect final
	: public CGameObject
{
	DECLARE_PROTOTYPE(CDominion_Effect);

private:
	CDominion_Effect();
	virtual ~CDominion_Effect();

public:
	static CDominion_Effect* Create(_float4 vScale, _float4 vPos);

public:
	virtual void	Set_ShaderResource(CShader* pShader, const char* pConstantName);

public:
	void Set_DominionColor(CTeamConnector* pConqueredTeam);

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Start() override;

	virtual void My_Tick() override;
	virtual void My_LateTick() override;

private:
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