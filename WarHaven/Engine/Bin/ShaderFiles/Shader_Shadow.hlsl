#define		MAX_CASCADE_NUM		3



matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix		g_ProjMatrixInv, g_ViewMatrixInv, g_LightViewMatrix, g_LightVeiwProjMatrix;
vector		g_vCamPosition;


texture2D	g_ShadowTexture;
texture2D	g_StaticShadowTexture;
texture2D	g_DepthTexture;

Texture2DArray g_CascadeTextureArray;

float		g_fWinCX = 1280.f;
float		g_fWinCY = 720.f;

float		g_fDistance = 100.f;


sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
	AddressU = wrap;
	AddressV = wrap;
};

sampler ShadowSampler = sampler_state
{
	filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = clamp;
	AddressV = clamp;
};

struct VS_IN
{
	float3		vPosition : POSITION;	
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_MAIN_STATICSHADOW(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_StaticShadowTexture.Sample(ShadowSampler, In.vTexUV);

	if (Out.vColor.a <= 0.01f)
		discard;

	return Out;
}


//float shadowPCF(float3 px)
//{
//	// texture delta
//	float2 delta = 1. / iChannelResolution[0].xy;
//
//	float factor = 0.;
//	// filter size
//	const int r = 3;
//	for (int y = -r; y <= r; y++)
//	{
//		for (int x = -r; x <= r; x++)
//		{
//			float2 offset = delta * float2(x, y);
//			// count the number of shadow hits
//			factor += float(texture(iChannel0, px.xy + offset).x > px.z - 0.002);
//
//		}
//	}
//	int size = 2 * r + 1;
//
//	int elements = size * size;
//
//	// average of shadow hits
//	return factor / float(elements);
//}



PS_OUT PS_MAIN_SHADOWING(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)1;

	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	//Shadow
	vector			vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	float			fViewZ = vDepthDesc.y * 1500.f;
	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	float4 vLightViewSpacePos = mul(vWorldPos, g_LightViewMatrix);
	vector		vUVPos = mul(vLightViewSpacePos, g_LightVeiwProjMatrix);
	float2		vNewUV;

	vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
	vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

	vector			vShadowDesc = g_ShadowTexture.Sample(ShadowSampler, vNewUV);

	float fLit = 1.f;

	/*float fVariance = vShadowDesc.g - (vShadowDesc.r * vShadowDesc.r);
	fVariance = max(fVariance, 0.005f);

	float fragDepth = vLightViewSpacePos.z / 1500.f;

	float mD = (fragDepth - vShadowDesc.x);
	float mD_2 = mD * mD;
	float p = (fVariance / (fVariance + mD_2));

	*/

	//¿øº»
	/*if (vLightViewSpacePos.z - 0.5f > vShadowDesc.r * 1500.f)
	{
		fLit = p;
		fLit = (1.f - fLit) + 0.3f;
		if (fLit > 1.f)
			fLit = 1.f;
	}*/


	//fLit = max(p, fragDepth <= vShadowDesc.x);

	/*float fLit = max(p, fragDepth > vShadowDesc.x);
	fLit = (1.f - fLit) + 0.3f;
	if (fLit > 1.f)
		fLit = 1.f;*/



	if (vLightViewSpacePos.z - 0.3f > vShadowDesc.r * 1500.f)
	{
		fLit = 0.3f;
	}


	Out.vColor.xyz = fLit;

	return Out;
}











BlendState BS_Default
{
	BlendEnable[0] = false;
};

BlendState BS_AlphaBlending
{
	BlendEnable[0] = true;
	BlendEnable[1] = true;

	SrcBlend = one;
	DestBlend = one;
	BlendOp = add;
};

RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};

DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};

DepthStencilState DSS_ZEnable_ZWriteEnable_false
{
	DepthEnable = false;
	DepthWriteMask = zero;
};

technique11 DefaultTechnique
{
	pass StaticShadow
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_STATICSHADOW();
	}

	pass SHADOWING
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SHADOWING();
	}
	
	
	
}