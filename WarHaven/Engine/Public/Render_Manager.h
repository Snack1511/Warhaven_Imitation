#pragma once
#include "Engine_Defines.h"

/* This Manager has all Renderer Instances and execute all*/
#define CUR_ENGINESHADER		4


BEGIN(Engine)

class CRenderer;
class CMeshRenderer;
class CGameObject;

class ENGINE_DLL CRender_Manager
{
	DECLARE_SINGLETON(CRender_Manager);

private:
	CRender_Manager();
	virtual ~CRender_Manager();

public:
	HRESULT		Add_Renderer(RENDER_GROUP eGroup, CRenderer* pRenderer);

public:
	void	Set_SunUV(_float2 vSunUV);

public:
	class CShader* Get_DeferredShader();

public:
	void	Start_RadialBlur(_float fTargetPower);
	void	Stop_RadialBlur();

	void	Start_ChromaticAberration(_float fTargetPower);
	void	Stop_ChromaticAberration();

	void	Start_GrayScale(_float fTargetPower);
	void	Stop_GrayScale();

	void	Start_DarkScreen(_float fTime);
	void	Stop_DarkScreen(_float fTime);

	void	Start_MotionBlur(_float fTime);

public:
	void		Bake_StaticShadow(vector<CGameObject*>& vecObjs, _float4 vCenterPos, _float fDistance, _float4 vSunLook, _bool bLensFlare);

public:
	HRESULT		Initialize();
	void		Update();
	HRESULT		Render();
	void		Release();

	
private:
	CTexture* m_pNoiseTexture = nullptr;

private:
	list<CRenderer*>							m_Renderers[RENDER_END];
	list<pair<_float, CRenderer*>>				m_pAlphaRenderers;

private:
	_float4x4	m_DownScaleWorldMatrix;
	_float4x4	m_SuperDownScaleWorldMatrix;
	_float4x4	m_UpScaleWorldMatrix;
	_float4x4	m_SuperUpScaleWorldMatrix;
	_float4x4	m_WorldMatrix;
	_float4x4	m_ShadowWorldMatrix;
	class CTarget_Manager* m_pTarget_Manager = nullptr;
	class CLight_Manager* m_pLight_Manager = nullptr;

private:
	//Shadow
	ComPtr<ID3D11DepthStencilView> m_pShadowDSV;
	_float4x4	m_ShadowViewMatrix;
	_float4x4	m_ShadowProjMatrix;
	D3D11_VIEWPORT	m_ShadowViewPort;

private:
	vector<class CShader*> m_vecShader;
	class CMesh_Rect* m_pMeshRect = nullptr;
	class CTexture* m_pBlackTexture = nullptr;

private:
	_bool	m_bRadialBlur = false;
	_float	m_fRadialPower = 0.f;
	_float	m_fRadialTargetPower = 0.f;

private:
	_bool	m_bChromaticAberration = false;
	_float	m_fChromaticAberrationPower = 0.f;
	_float	m_fChromaticAberrationTargetPower = 0.f;

private:
	_bool	m_bGrayScale = false;
	_float	m_fGrayScalePower = 0.f;
	_float	m_fGrayScaleTargetPower = 0.f;

private:
	_bool	m_bDarkScreen = false;
	_float	m_fDarkScreenAcc = 0.f;
	_float	m_fDarkScreenTime = 0.f;

private:
	_bool	m_bMotionBlur = false;
	_float	m_fMotionBlurAcc = 0.f;

private:
	_bool	m_bLensFlare = true;
	_float4 m_vSunWorldPos;
	_float2 m_vSunUV = _float2(0.f, 0.f);

#ifdef _DEBUG
private:
	_bool	m_bDebugRender = false;
public:
	HRESULT	Render_Debug();
	void	Tick_Debug();
#endif
	
private:
	HRESULT	SetUp_SRV();
	HRESULT	Bake_Shadow();


private:
	HRESULT	Render_Lights();
	HRESULT	Render_ShadowBlur();
	HRESULT	Render_Outline();

	HRESULT	Render_RimLight();


	HRESULT	Render_SSAO();

	HRESULT	Render_ForwardBloom();
	HRESULT	Render_ForwardBlend();
	HRESULT	Render_BloomBlend();


private:
	HRESULT	Render_AlphaGroup();
	HRESULT	Render_Distortion();

	HRESULT	Render_DOF();

	HRESULT	Render_Glow();
	HRESULT	Render_Bloom();
	HRESULT	Render_EffectBlur();
	HRESULT	Render_PostEffect();
	HRESULT	Render_SSD();
	HRESULT	Render_FinalBlend();

	HRESULT Render_LensFlare(const _tchar* pRenderTargetName);
	HRESULT Render_MotionBlur(const _tchar* pRenderTargetName);
	HRESULT	Render_RadialBlur(const _tchar* pRenderTargetName);
	HRESULT	Render_ChromaticAberration(const _tchar* pRenderTargetName);
	HRESULT	Render_GrayScale(const _tchar* pRenderTargetName);

	HRESULT	Render_UI();
	HRESULT	Render_UIBloom();
	HRESULT	Render_UIBlend(const _tchar* pRenderTargetName);

private:
	HRESULT		Render_Group(RENDER_GROUP eGroup);
	void		Sort_AlphaList();
	void		Sort_UIGroup();

private:
	ComPtr<ID3D11ShaderResourceView> m_pCascadeSRV;
	ComPtr<ID3D11DepthStencilView> m_pCascadeDSV;



	HRESULT		SetUp_Cascade(_uint iShadowMapWidth, _uint iShadowMapHeight, _uint iArrayCount);
#ifdef _DEBUG
public:
	CDelegate<> Callback_DebugRender;
	list<CShader*> m_DebuggingShaders_OutCreate;
#endif
	


};

END