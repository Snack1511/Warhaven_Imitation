#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
END

BEGIN(Client)
class CDebugObject
	: public CGameObject
{
	DECLARE_PROTOTYPE(CDebugObject);

private:
	CDebugObject();
	virtual ~CDebugObject();

public:
	static CDebugObject* Create(_float4 vPos, _float4 vScale, _float4 vAngle = ZERO_VECTOR);

public:
	void	SetUp_ShaderResources(CShader* pShaderCom, const char* pConstantName);

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize() override;
	virtual HRESULT	Start() override;

private:
	_float4		m_vColor = _float4(0.f, 1.f, 0.f, 1.f);

private:
	HRESULT		SetUp_PhysXCollider(_float4 vPos, _float4 vScale, _float4 vAngle);
};
END
