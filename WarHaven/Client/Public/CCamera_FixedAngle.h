#pragma once
#include "Camera.h"
#include "Client_Defines.h"
BEGIN(Engine)
class CTransform;
END

BEGIN(Client)
class CScript_FixedCam;
class CCamera_FixedAngle :
    public CCamera
{
	DECLARE_PROTOTYPE(CCamera_FixedAngle);
private:
	CCamera_FixedAngle();
	virtual ~CCamera_FixedAngle();
public:
	static CCamera_FixedAngle* Create(_float4 LookatDir, _float4 AbsUpDir = _float4(0.f, 1.f, 0.f, 0.f));


public:
	// CCamera을(를) 통해 상속됨
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT	Start() override;

private:
	HRESULT	SetUp_Components();
	HRESULT SetUp_LookAt(_float4 LookatDir, _float4 AbsUpDir);
private:
	virtual void My_Tick() override;
private:
	virtual void Late_Tick() override;
private:
	virtual void Input_KB_W(_float4& OutDir);
	virtual void Input_KB_A(_float4& OutDir);
	virtual void Input_KB_S(_float4& OutDir);
	virtual void Input_KB_D(_float4& OutDir);
private:
	CScript_FixedCam* m_pScriptableCom = nullptr;
	CTransform* m_pTransformCom = nullptr;
};
END