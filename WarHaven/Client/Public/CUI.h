#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CModel;
class CShader;
END

BEGIN(Client)

class CFader;

class CUI abstract : public CGameObject
{
protected:
	CUI();
	virtual ~CUI();

public:
	virtual	HRESULT	Initialize_Prototype();
	virtual	HRESULT	Initialize() PURE;
	virtual HRESULT	Start();

public:
	void SetActive(_bool value);

public:
	_float4 Get_Pos() { return m_vPosition; }
	_float Get_PosX() { return m_vPosition.x; }
	_float Get_PosY() { return m_vPosition.y; }
	void Set_Pos(_float4 vPos);
	void Set_Pos(_float fX, _float fY);
	void Set_PosX(_float fX);
	void Set_PosY(_float fY);

	void Set_LerpPos(_float4 vOrigin, _float4 vTarget);

public:
	_float4 Get_Scale() { return m_vScale; }

	void Set_Scale(_float value);
	void Set_Scale(_float fX, _float fY);
	void Set_ScaleX(_float fX);
	void Set_ScaleY(_float fY);
	void Set_Scale(_float2 vScale);

public:
	void	Set_UIShaderFlag(_float4 vFlag) { m_vUIShaderFlag = vFlag; }

public:
	_float Get_RotationValue() { return m_fRotValue; }
	void Set_RotationZ(_float vAngle);

public:
	// ÄÄÆ÷³ÍÆ®
	HRESULT SetTexture(const _tchar* pFilePath, _uint iIndex = 1);
	virtual void SetUp_ShaderResource(CShader* pShader, const char* pConstName);
	void SetUp_ShaderResource_Flag(CShader* pShader, const char* pConstName);

	void Active_Fade(FADEDESC tFadeDesc);

public:
	_float4 Get_Color() { return m_vColor; }
	void Set_Color(_float4 vColor) { m_vColor = vColor; }

	_bool Get_IsSlice() { return m_bIsSlice; }
	void Set_IsSlice(_bool value) { m_bIsSlice = value; }

	_float4 Get_SliceRatio() { return m_vSliceRatio; }
	void Set_SliceRatio(_float4 vRatio) { m_vSliceRatio = vRatio; }
	_float2 Get_TextureSize() { return m_vTextureSize; }
	void Set_TextureSzie(_float2 vTextureSize) { m_vTextureSize = vTextureSize; }

public:
	_float Get_Sort() { return m_vPosition.z; }
	void Set_Sort(_float value);

public:
	CDelegate<const _uint&>		CallBack_PointEnter;
	CDelegate<const _uint&>		CallBack_PointStay;
	CDelegate<const _uint&>		CallBack_PointExit;

	CDelegate<const _uint&>		CallBack_PointDown;
	CDelegate<const _uint&>		CallBack_PointPress;
	CDelegate<const _uint&>		CallBack_PointUp;

protected:
	_float4		m_vUIShaderFlag = SH_UI_NONE;

protected:
	_float4 m_vRealOriginPos;

	_float4 m_vLerpStartPos;
	_float4 m_vLerpTargetPos;

	_float4 m_vRealTargetPos;

protected:
	_float4 m_vPosition;
	_float4 m_vScale;
	_float m_fRotValue = 0.f;

	RECT m_tRect;
	POINT m_ptMouse;

	_bool m_bIsSlice = false;
	_float4 m_vSliceRatio;
	_float2 m_vTextureSize;

	_float4 m_vColor = { 1.f, 1.f, 1.f, 1.f };
	CFader* m_pFader = nullptr;

protected:
	// These will be called by Set_Enable Func.
	virtual	void	OnEnable();
	virtual	void	OnDisable();
	virtual void	My_Tick();
	virtual void	My_LateTick();

protected:
	void CheckInRect();
	void Read_Texture(string strFolderPath, string key);
};

END