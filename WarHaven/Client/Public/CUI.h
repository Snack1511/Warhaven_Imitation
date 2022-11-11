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
	_float Get_RotationValue() { return m_fRotValue; }
	void Set_RotationZ(_float vAngle);

public:
	// ÄÄÆ÷³ÍÆ®
	HRESULT SetTexture(const _tchar* pFilePath, _uint iIndex = 1);
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);

public:
	_float4 Get_Color() { return m_vColor; }
	void Set_Color(_float4 vColor) { m_vColor = vColor; }

public:
	_float Get_Sort() { return m_vPosition.z; }
	void Set_Sort(_float value);
	
protected:
	_float4 m_vPosition;
	_float4 m_vScale;
	_float m_fRotValue = 0.f;

	RECT m_tRect;
	POINT m_ptMouse;

	_float4 m_vColor = { 1.f, 1.f, 1.f, 1.f };

protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();
	virtual void	My_Tick();
	virtual void	My_LateTick();

protected:
	void CheckInRect();
};

END