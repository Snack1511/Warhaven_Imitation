#include "..\Public\Render_Manager.h"

#include "GameObject.h"
#include "Transform.h"
#include "GameInstance.h"
#include "CModel_Renderer.h"

#include "Camera_Manager.h"

#include "CTarget_Manager.h"
#include "CLight_Manager.h"
#include "CRenderTarget.h"
#include "CLight.h"

#include "CShader.h"
#include "CMesh_Rect.h"
#include "Texture.h"

#include "Level.h"

#define SHADOW_ON

IMPLEMENT_SINGLETON(CRender_Manager)

#define	SHADER_DEFERRED	0
#define	SHADER_BLUR	1
#define	SHADER_GLOW	2
#define	SHADER_SHADOW	3


CRender_Manager::CRender_Manager()
{
}

CRender_Manager::~CRender_Manager()
{
	Release();
}

HRESULT CRender_Manager::Add_Renderer(RENDER_GROUP eGroup, CRenderer* pRenderer)
{
	if (eGroup >= RENDER_END ||
		nullptr == pRenderer)
		return E_FAIL;

	if (eGroup == RENDER_ALPHA)
	{
		m_pAlphaRenderers.push_back(make_pair(0.f, pRenderer));
		return S_OK;
	}

	m_Renderers[eGroup].push_back(pRenderer);

	return S_OK;
}

void CRender_Manager::Set_SunUV(_float2 vSunUV)
{
	if (KEY(LBUTTON, HOLD))
	{
		m_vSunUV = vSunUV;
	}
}

CShader* CRender_Manager::Get_DeferredShader()
{
	return m_vecShader[SHADER_DEFERRED];
}

void CRender_Manager::Start_RadialBlur(_float fTargetPower)
{
	m_fRadialTargetPower = fTargetPower;
	m_bRadialBlur = true;
}

void CRender_Manager::Stop_RadialBlur()
{
	m_bRadialBlur = false;
}

void CRender_Manager::Start_ChromaticAberration(_float fTargetPower)
{
	m_fChromaticAberrationTargetPower = fTargetPower;
	m_bChromaticAberration = true;
}

void CRender_Manager::Stop_ChromaticAberration()
{
	m_bChromaticAberration = false;

}

void CRender_Manager::Start_GrayScale(_float fTargetPower)
{
	m_fGrayScaleTargetPower = fTargetPower;
	m_bGrayScale = true;
}

void CRender_Manager::Stop_GrayScale()
{
	m_bGrayScale = false;
}

void CRender_Manager::Start_DarkScreen(_float fTime)
{
	m_fDarkScreenAcc = 0.f;
	m_bDarkScreen = true;
	m_fDarkScreenTime = fTime;
}

void CRender_Manager::Stop_DarkScreen(_float fTime)
{
	m_fDarkScreenAcc = fTime;
	m_fDarkScreenTime = fTime;
	m_bDarkScreen = false;
}

void CRender_Manager::Start_MotionBlur(_float fTime)
{
	m_bMotionBlur = true;
	m_fMotionBlurAcc = fTime;
}

void CRender_Manager::Bake_StaticShadow(vector<CGameObject*>& vecObjs, _float4 vCenterPos, _float fDistance, _float4 vSunLook, _bool bLensFlare)
{
	m_ShadowViewMatrix.Identity();
	_float4 vLook = vSunLook.Normalize();

	*((_float4*)&m_ShadowViewMatrix.m[2]) = vLook;

	_float4 vUp = { 0.f, 1.f, 0.f, 0.f };
	_float4 vRight = vUp.Cross(vLook);
	*((_float4*)&m_ShadowViewMatrix.m[0]) = vRight;
	vUp = vLook.Cross(vRight);
	*((_float4*)&m_ShadowViewMatrix.m[1]) = vUp;

	_float4 vPos = vCenterPos + vLook * -1.f * fDistance;
	vPos.w = 1.f;

	*((_float4*)&m_ShadowViewMatrix.m[3]) = vPos;
	//m_vSunWorldPos = vCenterPos + (vLook * -1.f * 500.f);

	_float4 vSunDir = vSunLook;
	vSunDir.y *= 0.3f;
	vSunDir.Normalize();


	m_vSunWorldPos = vCenterPos + vSunDir * 500.f * -1.f;
	m_vSunWorldPos.w = 1.f;

	if (CLight_Manager::Get_Instance()->Get_FirstLight())
		CLight_Manager::Get_Instance()->Get_FirstLight()->Get_LightDesc_Modify().vPosition = m_vSunWorldPos;

	if (!bLensFlare)
		m_vSunWorldPos.y = -9000.f;

	m_ShadowViewMatrix.Inverse();
	m_ShadowViewMatrix.Transpose();




	m_pTarget_Manager->Begin_Shadow(TEXT("MRT_StaticShadowAcc"));

	//???? ???????? ??,???? ????????
	CShader_Manager::Get_Instance()->Set_RawValue_All("g_ProjMatrix", &m_ShadowProjMatrix, sizeof(_float4x4));


	for (auto& elem : vecObjs)
	{
		GET_COMPONENT_FROM(elem, CRenderer)->Bake_Shadow(m_ShadowViewMatrix);
	}


	m_pTarget_Manager->End_Shadow();

}

HRESULT CRender_Manager::Initialize()
{
	m_pNoiseTexture = CTexture::Create(0, L"../bin/resources/textures/effects/noise/T_EFF_PixelNoise.dds", 1);

	m_pTarget_Manager = CTarget_Manager::Get_Instance();
	m_pLight_Manager = CLight_Manager::Get_Instance();

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));

	_uint		iNumViewports = 1;

	DEVICE_CONTEXT->RSGetViewports(&iNumViewports, &ViewPortDesc);


#pragma region SHADOW DSV
	ComPtr < ID3D11Texture2D> pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = 8000;
	//TextureDesc.Width = 1280;
	_float fRatio = (_float)TextureDesc.Width / 1280.f;
	TextureDesc.Height = (_uint)(ViewPortDesc.Height * fRatio);
	//TextureDesc.Height = 720;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;


	if (FAILED(DEVICE->CreateTexture2D(&TextureDesc, nullptr, pDepthStencilTexture.GetAddressOf())))
		return E_FAIL;

	if (FAILED(DEVICE->CreateDepthStencilView(pDepthStencilTexture.Get(), nullptr, m_pShadowDSV.GetAddressOf())))
		return E_FAIL;



	ZeroMemory(&m_ShadowViewPort, sizeof(D3D11_VIEWPORT));
	m_ShadowViewPort.TopLeftX = 0;
	m_ShadowViewPort.TopLeftY = 0;
	m_ShadowViewPort.Width = (_float)TextureDesc.Width;
	m_ShadowViewPort.Height = (_float)TextureDesc.Height;
	m_ShadowViewPort.MinDepth = 0.f;
	m_ShadowViewPort.MaxDepth = 1.f;

	m_pTarget_Manager->Set_ShadowDSV(m_pShadowDSV, m_ShadowViewPort);



	m_ShadowViewMatrix.Identity();
	_float4 vLook = _float4(-1.f, -2.f, -1.f, 0.f).Normalize();

	*((_float4*)&m_ShadowViewMatrix.m[2]) = vLook;

	_float4 vUp = { 0.f, 1.f, 0.f, 0.f };
	_float4 vRight = vUp.Cross(vLook);
	*((_float4*)&m_ShadowViewMatrix.m[0]) = vRight;
	vUp = vLook.Cross(vRight);
	*((_float4*)&m_ShadowViewMatrix.m[1]) = vUp;


	*((_float4*)&m_ShadowViewMatrix.m[3]) = _float4(200.f, 400.f, 200.f, 1.f);
	m_ShadowViewMatrix.Inverse();
	m_ShadowViewMatrix.Transpose();

	m_ShadowProjMatrix.Identity();
	m_ShadowProjMatrix = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.f),
		1280.f / 720.f,
		0.2f, 1500.f);
	m_ShadowProjMatrix.Transpose();



#pragma endregion

	/* ???????????? ????????. */

#pragma region RENDERTARGETS

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Diffuse"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.5f, 0.5f, 0.5f, 0.f))))
		return E_FAIL;

	/* For.Target_PBR */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_PBR"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_SkyBox"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.5f, 0.9f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_Normal*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Normal"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Depth"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Shade"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;

	/* For.Target_Specular */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Specular"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Flag */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Flag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Target_Shadow */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Shadow"), TextureDesc.Width, TextureDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_StaticShadow"), TextureDesc.Width, TextureDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_ShadowBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_ChromaticAberration"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_LensFlare"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	

	/* For.Target_Forward */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Forward"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	/* For.Target_EffectDiffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_EffectDiffuse"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/*Glow*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Glowable"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Glow"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_GlowFlag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_EffectFlag */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_EffectFlag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_EffectDepth"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 1.f, 0.f, 1.f))))
		return E_FAIL;


	/* For.Blur_Flag */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_DownScale"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_HorizonBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_VerticalBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Outline */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Outline"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_OutlineFlag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*RimLight*/
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_RimLight"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_RimLightFlag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Bloom */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Bloom"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_BloomBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	/* For.FinalBlend */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_FinalBlend"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.EffectBlur */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_EffectBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Distortion */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_Distortion"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* For.Distortion */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_SSD"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* NonAlpha Bloom */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_ForwardBloom"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_ForwardBloomBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_BloomBlend"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* UI SHADER */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_UIForward"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_UIBloom"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_UIBloomBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_UIBlend"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_UIFlag"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_PostEffect"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* BLURS */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_MotionBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_SSAO"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_RadialBlur"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_GrayScale"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(TEXT("Target_HDR"), ViewPortDesc.Width, ViewPortDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/* SkyBox */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SkyBox"), TEXT("Target_SkyBox"))))
		return E_FAIL;

	/* For.MRT_Deferred */
	/* ?????????? ?????? ???????? ???? ??, ???? ???????? ?????? ?????? ???????????? ?????????? (Diffuse + Normal ) */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_Flag"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_OutlineFlag"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_RimLightFlag"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Deferred"), TEXT("Target_PBR"))))
		return E_FAIL;

	/* For.MRT_Effect */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_BloomBlend"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_EffectFlag"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_EffectDiffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effect"), TEXT("Target_GlowFlag"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Distortion"), TEXT("Target_Distortion"))))
		return E_FAIL;

	/* For.MRT_LightAcc */
	/* ?????????? ?????? ?????????????? ???? (Shade + Specular) */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	/*For Blur*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_DownScaleAcc"), TEXT("Target_DownScale"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_VerticalBlurAcc"), TEXT("Target_VerticalBlur"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_HorizonBlurAcc"), TEXT("Target_HorizonBlur"))))
		return E_FAIL;

	/*For Glow*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GlowableAcc"), TEXT("Target_Glowable"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GlowAcc"), TEXT("Target_Glow"))))
		return E_FAIL;

	/*For Outline*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_OutlineAcc"), TEXT("Target_Outline"))))
		return E_FAIL;

	/*For RimLight*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_RimLightAcc"), TEXT("Target_RimLight"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ChromaticAberrationAcc"), TEXT("Target_ChromaticAberration"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_LensFlareAcc"), TEXT("Target_LensFlare"))))
		return E_FAIL;

	/*For Shadow*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowAcc"), TEXT("Target_Shadow"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_StaticShadowAcc"), TEXT("Target_StaticShadow"))))
		return E_FAIL;

	/*For Forward*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ForwardAcc"), TEXT("Target_Forward"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_HDRAcc"), TEXT("Target_HDR"))))
		return E_FAIL;

	/*For Bloom*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomAcc"), TEXT("Target_Bloom"))))
		return E_FAIL;

	/*For Bloom*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlurAcc"), TEXT("Target_BloomBlur"))))
		return E_FAIL;

	/*For Bloom*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ForwardBloomAcc"), TEXT("Target_ForwardBloom"))))
		return E_FAIL;

	/*For Bloom*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ForwardBloomBlurAcc"), TEXT("Target_ForwardBloomBlur"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GrayScaleAcc"), TEXT("Target_GrayScale"))))
		return E_FAIL;

	/*For Bloom*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlendAcc"), TEXT("Target_BloomBlend"))))
		return E_FAIL;

	/*For Distortion*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_FinalBlendAcc"), TEXT("Target_FinalBlend"))))
		return E_FAIL;

	/*For EffectBlur*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_EffectBlurAcc"), TEXT("Target_EffectBlur"))))
		return E_FAIL;

	/*For MotionBlur*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_MotionBlurAcc"), TEXT("Target_MotionBlur"))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAOAcc"), TEXT("Target_SSAO"))))
		return E_FAIL;

	/*For Decal*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_Forward"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_EffectDiffuse"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_EffectFlag"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_GlowFlag"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_PostEffectAcc"), TEXT("Target_PostEffect"))))
		return E_FAIL;
	
	/*SHadow Blur*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowBlurAcc"), TEXT("Target_ShadowBlur"))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_RadialBlurAcc"), TEXT("Target_RadialBlur"))))
		return E_FAIL;

	/*UI*/

	
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_UIForwardAcc"), TEXT("Target_UIForward"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_UIForwardAcc"), TEXT("Target_UIFlag"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_UIBloomAcc"), TEXT("Target_UIBloom"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_UIBloomBlurAcc"), TEXT("Target_UIBloomBlur"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_UIBlendAcc"), TEXT("Target_UIBlend"))))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_EffectDiffuse"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSDAcc"), TEXT("Target_EffectFlag"))))
		return E_FAIL;*/

#ifdef _DEBUG	
	_float fSize = 100.f;
	_float fStep = fSize * 0.5f;

	_float fPosX = fStep;
	_float fPosY = fStep;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Flag"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_RimLight"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosX += fSize;
	fPosY = fStep;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Specular"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_StaticShadow"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shadow"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	

	/*Forward*/
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Forward"), 75.f, 645.f, 150.f, 150.f)))
		return E_FAIL;

	fPosX = 1280.f - fStep;
	fPosY = fStep;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_VerticalBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Outline"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Distortion"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;
	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_MotionBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosX -= fSize;
	fPosY = fStep;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_EffectFlag"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Glow"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	fPosY += fSize;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BloomBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ForwardBloomBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;

	fPosY += fSize;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_UIBloomBlur"), fPosX, fPosY, fSize, fSize)))
		return E_FAIL;
	
#endif // _DEBUG

	
#pragma endregion



	_matrix			WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height, 0.f, 0.f);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixTranspose(WorldMatrix));

	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width * 0.5f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height * 0.5f, 0.f, 0.f);

	XMStoreFloat4x4(&m_DownScaleWorldMatrix, XMMatrixTranspose(WorldMatrix));

	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width * 0.25f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height * 0.25f, 0.f, 0.f);

	XMStoreFloat4x4(&m_SuperDownScaleWorldMatrix, XMMatrixTranspose(WorldMatrix));

	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width * 2.f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height * 2.f, 0.f, 0.f);

	XMStoreFloat4x4(&m_UpScaleWorldMatrix, XMMatrixTranspose(WorldMatrix));

	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(ViewPortDesc.Width * 4.f, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, ViewPortDesc.Height * 4.f, 0.f, 0.f);

	XMStoreFloat4x4(&m_SuperUpScaleWorldMatrix, XMMatrixTranspose(WorldMatrix));


	WorldMatrix = XMMatrixIdentity();
	WorldMatrix.r[0] = XMVectorSet(TextureDesc.Width, 0.f, 0.f, 0.f);
	WorldMatrix.r[1] = XMVectorSet(0.f, TextureDesc.Height, 0.f, 0.f);

	XMStoreFloat4x4(&m_ShadowWorldMatrix, XMMatrixTranspose(WorldMatrix));



	if (FAILED(CShader_Manager::Get_Instance()->Load_EffectFile(TEXT("../Bin/Shaderfiles/Shader_Deferred.hlsl"))))
		return E_FAIL;

	if (FAILED(CShader_Manager::Get_Instance()->Load_EffectFile(TEXT("../Bin/Shaderfiles/Shader_Blur.hlsl"))))
		return E_FAIL;

	if (FAILED(CShader_Manager::Get_Instance()->Load_EffectFile(TEXT("../Bin/Shaderfiles/Shader_Glow.hlsl"))))
		return E_FAIL;

	if (FAILED(CShader_Manager::Get_Instance()->Load_EffectFile(TEXT("../Bin/Shaderfiles/Shader_Shadow.hlsl"))))
		return E_FAIL;

	m_vecShader.push_back( CShader::Create(0,0, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	if (nullptr == m_vecShader[SHADER_DEFERRED])
		return E_FAIL;

	m_vecShader.push_back(CShader::Create(0, SHADER_BLUR, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	if (nullptr == m_vecShader[SHADER_BLUR])
		return E_FAIL;

	m_vecShader.push_back(CShader::Create(0, SHADER_GLOW, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	if (nullptr == m_vecShader[SHADER_GLOW])
		return E_FAIL;

	m_vecShader.push_back(CShader::Create(0, SHADER_SHADOW, VTXTEX_DECLARATION::Element, VTXTEX_DECLARATION::iNumElements));
	if (nullptr == m_vecShader[SHADER_SHADOW])
		return E_FAIL;

	m_pMeshRect = CMesh_Rect::Create(0);
	if (nullptr == m_pMeshRect)
		return E_FAIL;

	m_pBlackTexture = CTexture::Create(0, L"../bin/resources/textures/black.png", 1);
	if (nullptr == m_pBlackTexture)
		return E_FAIL;

	return S_OK;
}

void CRender_Manager::Update()
{
	//SetUp_SRV();

	if (m_bRadialBlur)
	{
		if (m_fRadialPower < m_fRadialTargetPower)
			m_fRadialPower += fDT(0) * m_fRadialTargetPower * 1.5f;
		else
			m_fRadialPower = m_fRadialTargetPower;
	}
	else
	{
		if (m_fRadialPower > 0.f)
			m_fRadialPower -= fDT(0) * m_fRadialTargetPower * 1.5f;
		else
			m_fRadialPower = 0.f;

	}

	if (m_bChromaticAberration)
	{
		if (m_fChromaticAberrationPower < m_fChromaticAberrationTargetPower)
			m_fChromaticAberrationPower += fDT(0) * m_fChromaticAberrationTargetPower * 1.5f;
		else
			m_fChromaticAberrationPower = m_fChromaticAberrationTargetPower;
	}
	else
	{
		if (m_fChromaticAberrationPower > 0.f)
			m_fChromaticAberrationPower -= fDT(0) * m_fChromaticAberrationTargetPower * 1.5f;
		else
			m_fChromaticAberrationPower = 0.f;

	}

	if (m_bGrayScale)
	{
		if (m_fGrayScalePower < m_fGrayScaleTargetPower)
			m_fGrayScalePower += fDT(0) * m_fGrayScaleTargetPower * 1.f;
		else
			m_fGrayScalePower = m_fGrayScaleTargetPower;
	}
	else
	{
		if (m_fGrayScalePower > 0.f)
			m_fGrayScalePower -= fDT(0) * m_fGrayScaleTargetPower * 2.f;
		else
			m_fGrayScalePower = 0.f;

	}

	if (m_bDarkScreen)
	{
		if (m_fDarkScreenAcc < m_fDarkScreenTime)
			m_fDarkScreenAcc += fDT(0);
		else
			m_fDarkScreenAcc = m_fDarkScreenTime;
	}
	else
	{
		if (m_fDarkScreenAcc > 0.f)
			m_fDarkScreenAcc -= fDT(0);
		else
			m_fDarkScreenAcc = 0.f;
	}

	if (m_bMotionBlur)
	{
		if (m_fMotionBlurAcc > 0.f)
		{
			m_fMotionBlurAcc -= fDT(0);
		}
		else
		{
			m_bMotionBlur = false;
		}
	}


	/* LENS FLARE */
	_float4 vPos = m_vSunWorldPos;

	if (!CFrustum_Manager::Get_Instance()->isIn_Frustum_InWorldSpace(vPos.XMLoad(), 0.1f))
	{
		if (0.f < m_fLensPower)
		{
			m_fLensPower -= 3.f * fDT(0);
			m_bLensFlare = true;
		}
		else
		{
			m_fLensPower = 0.f;
			m_bLensFlare = false;

		}


		return;
	}



	/* ray ???? */
	_float4 vOutPos;
	_float fMinDist;
	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	_float4 vDir = vPos - vCamPos;
	if (CPhysX_Manager::Get_Instance()->Shoot_RaytoStaticActors(&vOutPos, &fMinDist,
		vCamPos, vDir.Normalize(), 1500.f
	))
	{

		if (0.f < m_fLensPower)
		{
			m_fLensPower -= 3.f * fDT(0);
			m_bLensFlare = true;
		}
		else
		{
			m_fLensPower = 0.f;
			m_bLensFlare = false;

		}
		return;
	}

	m_bLensFlare = true;

	if (1.f > m_fLensPower)
		m_fLensPower += 3.f * fDT(0);
	else
		m_fLensPower = 1.f;
	

	_float4x4 matVP = GAMEINSTANCE->Get_CurViewMatrix() * GAMEINSTANCE->Get_CurProjMatrix();
	vPos = vPos.MultiplyCoord(matVP);
	m_vSunUV.x = vPos.x;
	m_vSunUV.y = vPos.y;


}

HRESULT CRender_Manager::Render()
{

	if (FAILED(Render_Priority()))
		return E_FAIL;

	
	/* Shadow Baking */
#ifdef SHADOW_ON
	if (FAILED(Bake_Shadow()))
		return E_FAIL;
#endif 

	

	/* Deferred MRT */
	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_Deferred"))))
		return E_FAIL;

	if (FAILED(Render_Group(RENDER_NONALPHA)))
		return E_FAIL;

	

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	
	//Ortho

	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources(true)))
		return E_FAIL;

	if (FAILED(Render_Lights()))
		return E_FAIL;

	if (FAILED(Render_ShadowBlur()))
		return E_FAIL;

	if (FAILED(Render_RimLight()))
		return E_FAIL;

	

	//RimLight, Outline, Lights, Shadow, DOF
	if (FAILED(Render_ForwardBlend()))
		return E_FAIL;

	if (FAILED(Render_SSAO()))
		return E_FAIL;

	if (FAILED(Render_ForwardBloom()))
		return E_FAIL;

	if (FAILED(Render_Outline()))
		return E_FAIL;

	//DOF
	if (FAILED(Render_DOF()))
		return E_FAIL;

	if (FAILED(Render_BloomBlend()))
		return E_FAIL;

	


	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources()))
		return E_FAIL;

	//???????????? ???? ???? ?? ???? ???????? ??????

	/*if (FAILED(Render_SSD()))
		return E_FAIL;*/

	if (FAILED(Render_AlphaGroup()))
		return E_FAIL;

	

	if (FAILED(Render_Distortion()))
		return E_FAIL;

	

	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources(true)))
		return E_FAIL;

	if (FAILED(Render_Glow()))
		return E_FAIL;



	if (FAILED(Render_Bloom()))
		return E_FAIL;

	//Blur
	if (FAILED(Render_EffectBlur()))
		return E_FAIL;

	if (FAILED(Render_FinalBlend()))
		return E_FAIL;

	

	if (FAILED(Render_PostEffect()))
		return E_FAIL;

	wstring wstrRenderTargetName = L"Target_PostEffect";



	if (m_bLensFlare)
	{
		if (FAILED(Render_LensFlare(wstrRenderTargetName.c_str())))
			return E_FAIL;

		wstrRenderTargetName = L"Target_LensFlare";
	}

	if (m_bMotionBlur)
	{
		if (FAILED(Render_MotionBlur(wstrRenderTargetName.c_str())))
			return E_FAIL;

		wstrRenderTargetName = L"Target_MotionBlur";
	}

	if (m_fChromaticAberrationPower > 0.f)
	{
		if (FAILED(Render_ChromaticAberration(wstrRenderTargetName.c_str())))
			return E_FAIL;

		wstrRenderTargetName = L"Target_ChromaticAberration";
	}

	if (m_fRadialPower > 0.f)
	{
		if (FAILED(Render_RadialBlur(wstrRenderTargetName.c_str())))
			return E_FAIL;

		wstrRenderTargetName = L"Target_RadialBlur";
	}

	if (m_fGrayScalePower > 0.f)
	{
		if (FAILED(Render_GrayScale(wstrRenderTargetName.c_str())))
			return E_FAIL;

		wstrRenderTargetName = L"Target_GrayScale";
	}

	/* UI */
	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_UIBloom()))
		return E_FAIL;

	if (FAILED(Render_UIBlend(wstrRenderTargetName.c_str())))
		return E_FAIL;


	

#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#else
#ifdef TmpRender
	Callback_TmpRender();
	Callback_TmpRender.Clear();

#endif
#endif // _DEBUG



	return S_OK;
}

void CRender_Manager::Release()
{
#ifdef _DEBUG
	for (auto& elem : m_DebuggingShaders_OutCreate)
	{
		SAFE_DELETE(elem);
	}
#else
#ifdef TmpRender
	for (auto& elem : m_TmpRnderShaders_OutCreate)
	{
		SAFE_DELETE(elem);
	}
	Callback_TmpRender.Clear();
	
#endif
#endif

	for (auto& elem : m_vecShader)
		SAFE_DELETE(elem);
	SAFE_DELETE(m_pMeshRect);
	SAFE_DELETE(m_pBlackTexture);
	SAFE_DELETE(m_pNoiseTexture);

	m_pShadowDSV.Reset();

}
#ifdef _DEBUG


HRESULT CRender_Manager::Render_Debug()
{
	Callback_DebugRender();
	Callback_DebugRender.Clear();

	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	if (!m_bDebugRender)
		return S_OK;

	m_pTarget_Manager->Render_Debug(TEXT("MRT_Deferred"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_LightAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_OutlineAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_VerticalBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_ShadowAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_ForwardAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_GlowAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_BloomBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_Distortion"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_StaticShadowAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_ShadowBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_ForwardBloomBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_UIBloomBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_RimLightAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);
	m_pTarget_Manager->Render_Debug(TEXT("MRT_MotionBlurAcc"), m_vecShader[SHADER_DEFERRED], m_pMeshRect);



	return S_OK;
}
void CRender_Manager::Tick_Debug()
{
	if (KEY(CTRL, HOLD) && KEY(F7, TAP))
	{
		m_bDebugRender = !m_bDebugRender;
	}
}
#endif // _DEBUG

HRESULT CRender_Manager::Render_Lights()
{
	/* ?????? ?????? ?????? ??????????. */
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	/* ???? ???? ?? ??????????(????)?? ?????? ???????? ??????????.
	???? ?????? ???? ?????????? ????. */
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_NormalTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_PBRTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_PBR")))))
		return E_FAIL;
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));


	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurViewMatrix().Inverse().XMLoad()));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurProjMatrix().Inverse().XMLoad()));

	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	_float4 vCamLook = GAMEINSTANCE->Get_CurCamLook();
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4));
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_vCamLook", &vCamLook, sizeof(_float4));

	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_bPBR", &m_bPBR, sizeof(_bool));

	m_pLight_Manager->Render_Lights(m_vecShader[SHADER_DEFERRED], m_pMeshRect);

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}
HRESULT CRender_Manager::Render_ShadowBlur()
{
	//?????? ???? + ??????
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_SHADOW]->Set_ShaderResourceView("g_ShadowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Shadow")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_SHADOW]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;

	

	_float4x4		ViewMatrixInv, ProjMatrixInv;
	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurViewMatrix().Inverse().XMLoad()));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurProjMatrix().Inverse().XMLoad()));
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));
	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4));
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_LightViewMatrix", &m_ShadowViewMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_LightVeiwProjMatrix", &m_ShadowProjMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_SHADOW]->Begin(1);
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	//????
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ShadowBlurAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_fShaderPower", &m_fShadowQuality, sizeof(_float));

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(12);

	m_pMeshRect->Render();
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	////3. ????
	//if (FAILED(m_pTarget_Manager->End_MRT()))
	//	return E_FAIL;


	//if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ShadowBlurAcc"))))
	//	return E_FAIL;
	//if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
	//	return E_FAIL;
	//m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	//m_vecShader[SHADER_BLUR]->Begin(1);

	//m_pMeshRect->Render();

	//if (FAILED(m_pTarget_Manager->End_MRT()))
	//	return E_FAIL;

	//_uint iNumBlur = 2;
	//
	//for (_uint i = 0; i < iNumBlur; ++i)
	//{
	//	//????
	//	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
	//		return E_FAIL;

	//	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_ShadowBlur")))))
	//		return E_FAIL;

	//	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	//	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_DownScaleWorldMatrix, sizeof(_float4x4));

	//	m_vecShader[SHADER_BLUR]->Begin(2);

	//	m_pMeshRect->Render();

	//	//3. ????
	//	if (FAILED(m_pTarget_Manager->End_MRT()))
	//		return E_FAIL;


	//	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ShadowBlurAcc"))))
	//		return E_FAIL;
	//	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
	//		return E_FAIL;
	//	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_UpScaleWorldMatrix, sizeof(_float4x4));

	//	m_vecShader[SHADER_BLUR]->Begin(1);

	//	m_pMeshRect->Render();

	//	if (FAILED(m_pTarget_Manager->End_MRT()))
	//		return E_FAIL;

	//}

	

	




	return S_OK;
}
HRESULT CRender_Manager::Render_RimLight()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_RimLightAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_RimLightFlag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_NormalTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurViewMatrix().Inverse().XMLoad()));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurProjMatrix().Inverse().XMLoad()));

	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_vCamPosition", &vCamPos, sizeof(_float4));

	m_vecShader[SHADER_DEFERRED]->Begin(9);

	/* ?????? ?????? ?????????? ????????. */
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}
HRESULT CRender_Manager::Render_SSAO()
{
	//HDR
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HDRAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Forward")))))
		return E_FAIL;

	

	_float3 fHDR = _float3(m_fBrightness, m_fContrast, m_fSaturation);
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_vHDR", &fHDR, sizeof(_float3));

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(11);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	//SSAO
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_SSAOAcc"))))
		return E_FAIL;

	/*Shader Option*/
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_fShaderPower", &m_fSSAOPower, sizeof(_float));

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_NormalTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(L"Target_HDR"))))
		return E_FAIL;

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurViewMatrix().Inverse().XMLoad()));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurProjMatrix().Inverse().XMLoad()));

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	if (FAILED(m_vecShader[SHADER_BLUR]->Begin(10)))
		return E_FAIL;

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}
HRESULT CRender_Manager::Render_ForwardBloom()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ForwardBloomAcc"))))
		return E_FAIL;

	//1. Bloom ???? ????
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_SSAO")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(3);
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	//2. ????

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	//1. ???? ??????
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_ForwardBloom")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_DownScaleWorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(0);
	m_pMeshRect->Render();

	//2. ????????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);

	m_pMeshRect->Render();

	//3. ????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ForwardBloomBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_UpScaleWorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}
HRESULT CRender_Manager::Render_AlphaGroup()
{
	Sort_AlphaList();

	//0?? ?????????? Forward ????
	if (FAILED(m_pTarget_Manager->Begin_Effects(TEXT("MRT_Effect"))))
		return E_FAIL;


	for (auto& elem : m_pAlphaRenderers)
	{
		if (FAILED(elem.second->Render()))
			return E_FAIL;
	}

	for (auto& elem : m_Renderers[RENDER_TRAIL])
	{
		if (FAILED(elem->Render()))
			return E_FAIL;
	}

	m_pAlphaRenderers.clear();
	m_Renderers[RENDER_TRAIL].clear();
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_Distortion()
{
	//0?? ?????????? Forward ????
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_Distortion"))))
		return E_FAIL;

	for (auto& elem : m_Renderers[RENDER_DISTORTION])
	{
		if (FAILED(elem->Render()))
			return E_FAIL;
	}

	m_Renderers[RENDER_DISTORTION].clear();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}


HRESULT CRender_Manager::Render_ForwardBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ForwardAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_SkyTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_SkyBox")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Diffuse")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_ShadeTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Shade")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_SpecularTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Specular")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_ShadowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_ShadowBlur")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_RimLightTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_RimLight")))))
		return E_FAIL;

	

	

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_DEFERRED]->Begin(3);

	/* ?????? ?????? ?????????? ????????. */
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_BloomBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_BloomBlendAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_SSAO")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BloomTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_ForwardBloomBlur")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BloomOriginTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_ForwardBloom")))))
		return E_FAIL;
	// DOF
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_VerticalBlur")))))
		return E_FAIL;


	/*Shader Option*/
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_fDOFPower", &m_fDOFPower, sizeof(_float));
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_DEFERRED]->Begin(7);

	/* ?????? ?????? ?????????? ????????. */
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_Outline()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_OutlineAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_OutlineFlag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Forward")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_NormalTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Normal")))))
		return E_FAIL;

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_DEFERRED]->Begin(4);

	/* ?????? ?????? ?????????? ????????. */
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::SetUp_SRV()
{
	if (FAILED(m_pBlackTexture->Set_ShaderResourceView(m_vecShader[SHADER_DEFERRED], "g_FogTexture")))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Bake_Shadow()
{
	if (FAILED(m_pTarget_Manager->Begin_Shadow(TEXT("MRT_ShadowAcc"))))
		return E_FAIL;


	//???????????????? ????
	if (FAILED(m_vecShader[SHADER_SHADOW]->Set_ShaderResourceView("g_StaticShadowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_StaticShadow")))))
		return E_FAIL;

	/*?????? ???? ????*/
	if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources(SHADER_SHADOW, true)))
		return E_FAIL;

	m_vecShader[SHADER_SHADOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_SHADOW]->Begin(0);
	m_pMeshRect->Render();


	//???? ???????? ??,???? ????????
	CShader_Manager::Get_Instance()->Set_RawValue_All("g_ProjMatrix", &m_ShadowProjMatrix, sizeof(_float4x4));


	//???? ?????? ???? ?????? ?????? ????
	for (auto& elem : m_Renderers[RENDER_SHADOW])
	{
		if (FAILED(elem->Get_ShaderCom()->Set_ShaderResourceView("g_StaticShadowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_StaticShadow")))))
			return E_FAIL;

		if (FAILED(elem->Bake_Shadow(m_ShadowViewMatrix)))
			return E_FAIL;
	}

	m_Renderers[RENDER_SHADOW].clear();

	if (FAILED(m_pTarget_Manager->End_Shadow()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_DOF()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	//1. ???? ??????
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_SSAO")))))
		return E_FAIL;

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_DownScaleWorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(0);

	m_pMeshRect->Render();

	//2. ????????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);
	
	m_pMeshRect->Render();

	//3. ????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_VerticalBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_UpScaleWorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_Priority()
{
	for (auto& elem : m_Renderers[RENDER_PRIORITY])
	{
		if (elem->Get_Ortho())
		{
			if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources(true)))
				return E_FAIL;

		}
		else
		{
			if (FAILED(CCamera_Manager::Get_Instance()->SetUp_ShaderResources()))
				return E_FAIL;

		}

		if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_SkyBox"))))
			return E_FAIL;

		elem->Render();

		if (FAILED(m_pTarget_Manager->End_MRT()))
			return E_FAIL;

	}

	m_Renderers[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRender_Manager::Render_Group(RENDER_GROUP eGroup)
{
	for (auto& elem : m_Renderers[eGroup])
	{
		if (FAILED(elem->Render()))
			return E_FAIL;
	}
		

	m_Renderers[eGroup].clear();

	return S_OK;
}

void CRender_Manager::Sort_AlphaList()
{
	_float4 vCamPos = GAMEINSTANCE->Get_ViewPos();
	_float4 vCamLook = GAMEINSTANCE->Get_CurCamLook();
	//vCamPos *= 1500.f;


	for (auto& elem : m_pAlphaRenderers)
	{
		//???????????? ??????
		/*_float4 vNormalLook;
		vNormalLook = elem.second->Get_Owner()->Get_Transform()->Get_World(WORLD_LOOK);
		vNormalLook.Normalize();*/

		_float4 vMyPos = elem.second->Get_WorldPosition();


		_float fDist1 = (vMyPos - vCamPos).Length();


		//?????????????? ?????? ?A ??????
		//_float4 vVector;
		//vVector = (XMLoadFloat4(&vMyPos) - vCamPos.XMLoad());

		////????
		//_float fDist1 = vCamLook.Dot(vVector);

		elem.first = fDist1;
	}

	m_pAlphaRenderers.sort(greater<pair<_float, CRenderer*>>());
}

void CRender_Manager::Sort_UIGroup()
{
	m_Renderers[RENDER_UI].sort([](CRenderer* p1, CRenderer* p2)
		{
			return p1->Get_FinalPos().z > p2->Get_FinalPos().z;
		});
}

HRESULT CRender_Manager::SetUp_Cascade(_uint iShadowMapWidth, _uint iShadowMapHeight, _uint iArrayCount)
{
	D3D11_TEXTURE2D_DESC texDesc = {
	iShadowMapWidth, iShadowMapHeight, 1, iArrayCount, DXGI_FORMAT_R32_TYPELESS, 1, 0, D3D11_USAGE_DEFAULT,
	D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 0, 0
	};

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {
		DXGI_FORMAT_D32_FLOAT, D3D11_DSV_DIMENSION_TEXTURE2DARRAY, 0 };
	dsvDesc.Texture2DArray.FirstArraySlice = 0;
	dsvDesc.Texture2DArray.ArraySize = iArrayCount;
	dsvDesc.Texture2DArray.MipSlice = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC	srvDesc = {
		DXGI_FORMAT_R32_FLOAT, D3D11_SRV_DIMENSION_TEXTURE2DARRAY, 0, 0 };
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = iArrayCount;
	srvDesc.Texture2DArray.MipLevels = 1;
	srvDesc.Texture2DArray.MostDetailedMip = 0;

	
	ID3D11Texture2D* pTex = nullptr;

	if (FAILED(DEVICE->CreateTexture2D(&texDesc, NULL, &pTex)))
		return E_FAIL;

	if (FAILED(DEVICE->CreateDepthStencilView(pTex, &dsvDesc, &m_pCascadeDSV)))
		return E_FAIL;

	if (FAILED(DEVICE->CreateShaderResourceView(pTex, &srvDesc, &m_pCascadeSRV)))

	pTex->Release();

	return S_OK;
}

HRESULT CRender_Manager::Render_Glow()
{
	//1. ???? ?????? + Glowable ????
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectDiffuse")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectFlag")))))
		return E_FAIL;

	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	//0. ?????? ?? ???? ????????
	m_vecShader[SHADER_GLOW]->Begin(0);
	m_pMeshRect->Render();
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	//2. ????????
	
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;
	

	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_GlowFlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_GlowFlag")))))
		return E_FAIL;

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	//?? ???? ????
	m_vecShader[SHADER_GLOW]->Begin(3);

	m_pMeshRect->Render();

	//3. ????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_GlowAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_GLOW]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	//???? ??

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;


	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Glow")))))
		return E_FAIL;
	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_GLOW]->Begin(2);

	m_pMeshRect->Render();

	//3. ????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_GlowAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_GLOW]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	
	m_vecShader[SHADER_GLOW]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_GLOW]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_Bloom()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_BloomAcc"))))
		return E_FAIL;

	//1. Bloom ???? ????
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectDiffuse")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectFlag")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(4);
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	//2. ????

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	//1. ???? ??????
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Bloom")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_DownScaleWorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(0);
	m_pMeshRect->Render();

	//2. ????????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);

	m_pMeshRect->Render();

	//3. ????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_BloomBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_UpScaleWorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_EffectBlur()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	//1. ???? ??????
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_BloomBlend")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_DownScaleWorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(0);
	m_pMeshRect->Render();

	//2. ????????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Flag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);

	m_pMeshRect->Render();

	//3. ????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_EffectBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_UpScaleWorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_PostEffect()
{
	


	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_PostEffectAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_Texture", m_pTarget_Manager->Get_SRV(TEXT("Target_FinalBlend")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_EffectFlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectFlag")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;


	

	//7. Distortion
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DistortionTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Distortion")))))
		return E_FAIL;

	


	m_vecShader[SHADER_DEFERRED]->Begin(6);

	m_pMeshRect->Render();
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;








	return S_OK;


}

HRESULT CRender_Manager::Render_SSD()
{
	if (FAILED(m_pTarget_Manager->Begin_SSD(TEXT("MRT_SSDAcc"))))
		return E_FAIL;

	//Depth ???? ??????????
	

	for (auto& elem : m_Renderers[RENDER_SSD])
	{
		if (FAILED(elem->Get_ShaderCom()->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
			return E_FAIL;

		if (FAILED(elem->Render()))
			return E_FAIL;
	}

	m_Renderers[RENDER_SSD].clear();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_FinalBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_FinalBlendAcc"))))
		return E_FAIL;

	//1. Forward
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_Texture", m_pTarget_Manager->Get_SRV(TEXT("Target_BloomBlend")))))
		return E_FAIL;

	//2. EffectFlag
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_EffectFlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectFlag")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_EffectDiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectDiffuse")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;

	//3. Outline
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_OutlineTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Outline")))))
		return E_FAIL;

	//4. Glow
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_GlowTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Glow")))))
		return E_FAIL;

	//5. Bloom
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BloomTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_BloomBlur")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BloomOriginTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Bloom")))))
		return E_FAIL;

	//6. Blur
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_EffectBlurTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_EffectBlur")))))
		return E_FAIL;

	

	if (FAILED(m_vecShader[SHADER_DEFERRED]->Begin(5)))
		return E_FAIL;

	/* ?????? ?????? ?????????? ????????. */
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_LensFlare(const _tchar* pRenderTargetName)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_LensFlareAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(pRenderTargetName))))
		return E_FAIL;

	
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_fShaderPower", &m_fLensPower, sizeof(_float));
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_vSunPos", &m_vSunUV, sizeof(_float2));
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	if (FAILED(m_vecShader[SHADER_BLUR]->Begin(9)))
		return E_FAIL;

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_MotionBlur(const _tchar* pRenderTargetName)
{
	//1. ???????? ?????? ????
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_MotionBlurAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_DepthTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_Depth")))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(pRenderTargetName))))
		return E_FAIL;

	//???? ???? ??????
	_float4x4		OldViewMatrix, OldProjMatrix;

	XMStoreFloat4x4(&OldViewMatrix, XMMatrixTranspose(CCamera_Manager::Get_Instance()->Get_OldView().XMLoad()));
	XMStoreFloat4x4(&OldProjMatrix, XMMatrixTranspose(CCamera_Manager::Get_Instance()->Get_OldProj().XMLoad()));

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_OldViewMatrix", &OldViewMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_OldProjMatrix", &OldProjMatrix, sizeof(_float4x4));

	_float4x4		ViewMatrixInv, ProjMatrixInv;

	XMStoreFloat4x4(&ViewMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurViewMatrix().Inverse().XMLoad()));
	XMStoreFloat4x4(&ProjMatrixInv, XMMatrixTranspose(GAMEINSTANCE->Get_CurProjMatrix().Inverse().XMLoad()));

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_ViewMatrixInv", &ViewMatrixInv, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_ProjMatrixInv", &ProjMatrixInv, sizeof(_float4x4));

	_float fTimeDelta = fDT(0);
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_fTimeDelta", &fTimeDelta, sizeof(_float));

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_fShaderPower", &m_fRadialPower, sizeof(_float));

	//bool

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_bMotionBlur", &m_bMotionBlur, sizeof(_bool));

	if (FAILED(m_vecShader[SHADER_BLUR]->Begin(5)))
		return E_FAIL;

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_RadialBlur(const _tchar* pRenderTargetName)
{
	//1. ???????? ?????? ????
	

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_RadialBlurAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(pRenderTargetName))))
		return E_FAIL;

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_fShaderPower", &m_fRadialPower, sizeof(_float));
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	if (FAILED(m_vecShader[SHADER_BLUR]->Begin(6)))
		return E_FAIL;

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_ChromaticAberration(const _tchar* pRenderTargetName)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_ChromaticAberrationAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(pRenderTargetName))))
		return E_FAIL;

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_fShaderPower", &m_fChromaticAberrationPower, sizeof(_float));
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	if (FAILED(m_vecShader[SHADER_BLUR]->Begin(7)))
		return E_FAIL;

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_GrayScale(const _tchar* pRenderTargetName)
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_GrayScaleAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(pRenderTargetName))))
		return E_FAIL;

	m_vecShader[SHADER_BLUR]->Set_RawValue("g_fShaderPower", &m_fGrayScalePower, sizeof(_float));
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));


	if (FAILED(m_vecShader[SHADER_BLUR]->Begin(8)))
		return E_FAIL;

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	return S_OK;
}

HRESULT CRender_Manager::Render_UI()
{
	Sort_UIGroup();

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_UIForwardAcc"))))
		return E_FAIL;

	if (FAILED(Render_Group(RENDER_UI)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_UIBloom()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_UIBloomAcc"))))
		return E_FAIL;

	//1. Bloom ???? ????
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_UIForward")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_FlagTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_UIFlag")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(3);
	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;


	//2. ????

	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_DownScaleAcc"))))
		return E_FAIL;

	//1. ???? ??????
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_UIBloom")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_DownScaleWorldMatrix, sizeof(_float4x4));
	m_vecShader[SHADER_BLUR]->Begin(0);
	m_pMeshRect->Render();

	//2. ????????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_HorizonBlurAcc"))))
		return E_FAIL;

	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_DownScale")))))
		return E_FAIL;

	/* ???? ?????? ?????? ?????? ?? ?????? ???u???????? ???????? ?????????? ?????? ???? ?????? ?? ?????? ????. */
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(2);

	m_pMeshRect->Render();

	//3. ????
	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_UIBloomBlurAcc"))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_BLUR]->Set_ShaderResourceView("g_ShaderTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_HorizonBlur")))))
		return E_FAIL;
	m_vecShader[SHADER_BLUR]->Set_RawValue("g_WorldMatrix", &m_UpScaleWorldMatrix, sizeof(_float4x4));

	m_vecShader[SHADER_BLUR]->Begin(1);

	m_pMeshRect->Render();

	if (FAILED(m_pTarget_Manager->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRender_Manager::Render_UIBlend(const _tchar* pRenderTargetName)
{
	/*if (FAILED(m_pTarget_Manager->Begin_MRT(TEXT("MRT_UIBloomBlendAcc"))))
		return E_FAIL;*/

		if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_Texture", m_pTarget_Manager->Get_SRV(pRenderTargetName))))
			return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_DiffuseTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_UIForward")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BloomTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_UIBloomBlur")))))
		return E_FAIL;
	if (FAILED(m_vecShader[SHADER_DEFERRED]->Set_ShaderResourceView("g_BloomOriginTexture", m_pTarget_Manager->Get_SRV(TEXT("Target_UIBloom")))))
		return E_FAIL;

	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_WorldMatrix", &m_WorldMatrix, sizeof(_float4x4));

	_float fRatio;
	

	if (m_fDarkScreenTime <= 0.f)
		fRatio = 0.f;
	else
		fRatio = (m_fDarkScreenAcc / m_fDarkScreenTime);


	m_vecShader[SHADER_DEFERRED]->Set_RawValue("g_fDarkScreen", &fRatio, sizeof(_float));

	m_vecShader[SHADER_DEFERRED]->Begin(8);

	/* ?????? ?????? ?????????? ????????. */
	m_pMeshRect->Render();

	//if (FAILED(m_pTarget_Manager->End_MRT()))
	//	return E_FAIL;

	return S_OK;
}
