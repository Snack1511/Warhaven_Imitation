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

public:
	void SetMouseTarget(_bool value) { m_bIsMouseTarget = value; }

public:
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);

protected:
	map<_hashcode, CUI_Component*> m_pUIComponentMap;

	CTexture* m_pTexture = nullptr;

	VTXTEX_PASS_TYPE m_ePass;
	_bool m_bIsOnMouse = false;

protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();
	virtual void	My_Tick();
	virtual void	My_LateTick();

protected:
	HRESULT SetTexture(const _tchar* pFilePath, _uint iIndex = 1);

protected:
	virtual void OnMouse();

private:
	_float4 m_vPosition = {};
	_float4 m_vScale = {};

private:
	_bool m_bIsMouseTarget = false;

};

END