#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CModel;
class CShader;
END

BEGIN(Client)

class CUI_Component;

class CUI abstract : public CGameObject
{
protected:
	CUI();
	CUI(const CUI& Prototype);
	virtual ~CUI();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize() PURE;
	virtual HRESULT	Start();

public:
	void Set_Pos(_float fX, _float fY);

public:
	void Set_Scale(_float value);
	void Set_Scale(_float fX, _float fY);
	void Set_ScaleRatio(_float value);

public:
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);

public:
	void Set_Color(_float4 vColor) { m_vColor = vColor; }

protected:
	_float4 m_vPosition = {};

	_float4 m_vOriginScale;
	_float m_fScaleMulitple = 1.f;
	_float4 m_vResultScale;

	RECT m_tRect;
	POINT m_ptMouse;

	CTexture* m_pTexture = nullptr;

	_float4 m_vColor = { 1.f, 1.f, 1.f, 1.f };

protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();
	virtual void	My_Tick();
	virtual void	My_LateTick();

protected:
	HRESULT SetTexture(const _tchar* pFilePath, _uint iIndex = 1);

	void CheckInRect();
};

END