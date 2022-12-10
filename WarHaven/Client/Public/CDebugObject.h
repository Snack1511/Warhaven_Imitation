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
	friend class CWindow_PhysX;
	friend class CWindow_Position;

	DECLARE_PROTOTYPE(CDebugObject);

private:
	CDebugObject();
	virtual ~CDebugObject();

public:
	static CDebugObject* Create(_float4 vPos = ZERO_VECTOR, _float4 vScale = _float4(1.f, 1.f, 1.f, 1.f), _float4 vAngle = ZERO_VECTOR);
	static CDebugObject* Create(PxTransform tTransform, _float4 vScale = _float4(1.f, 1.f, 1.f, 1.f));

public:
	void	SetUp_ShaderResources(CShader* pShaderCom, const char* pConstantName);

public:
	/* 박스 위치 옮김*/
	void			Synchronize_Box(PxTransform tTransform);
	void			ReScale_Box(_float4 vScale);

public:
	virtual HRESULT	Initialize_Prototype() override;
	virtual HRESULT	Initialize() override;
	virtual HRESULT	Start() override;

public:
	void		Set_Green();
	void		Set_Red();
	void		Set_Blue();

private:
	_float4		m_vAngles = ZERO_VECTOR;
	_float4		m_vFontColor = _float4(0.f, 1.f, 0.f, 1.f);

private:
	HRESULT		SetUp_PhysXCollider(_float4 vPos, _float4 vScale, _float4 vAngle);
};
END
