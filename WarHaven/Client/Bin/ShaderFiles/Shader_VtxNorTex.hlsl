
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector	g_vCamPosition;

vector	g_vLightDir;
vector	g_vLightPos;
float	g_fRange = 1.f;
float	g_fTileRatio = 100.f;

vector	g_vBrushPos = vector(15.f, 0.f, 15.f, 1.f);
float	g_fBrushRange = 10.f;

vector	g_vLightDiffuse;
vector	g_vLightAmbient;
vector	g_vLightSpecular;

texture2D	g_DiffuseTexture;
// vector	g_vMtrlDiffuse;
texture2D	g_SourDiffTexture;
texture2D	g_DestDiffTexture;
texture2D	g_FilterTexture;
texture2D	g_BrushTexture;

texture2D	g_StaticShadowTexture;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);
float		g_fPower = 30.f;

vector		g_vFlag = vector(0.95f, 0.f, 0.f, 0.f);

int g_iNumTexture = 0;
#define TEXTURESIZE 3
texture2D g_DiffArray[TEXTURESIZE];
//Texture2DArray g_DiffArray;//[TEXTURESIZE];



struct VS_DEFAULT_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
};

struct VS_DEFAULT_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vColor : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_DEFAULT_OUT VS_DEFAULT_MAIN(VS_DEFAULT_IN In)
{
	VS_DEFAULT_OUT		Out = (VS_DEFAULT_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	//In.vPosition.y += 0.1f;
	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vNormal = mul(vector(In.vNormal, 0.f), g_WorldMatrix);
	Out.vColor = In.vColor;
	Out.vProjPos = Out.vPosition;

	return Out;
}

struct VS_OUT_LIGHT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTileFlag : TEXCOORD3;
};

struct PS_IN_LIGHT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vTileFlag : TEXCOORD3;
};

struct PS_LIGHTOUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vFlag : SV_TARGET3;
	vector		vOutlineFlag : SV_TARGET4;
	vector		vRimLightFlag : SV_TARGET5;
};

struct PS_NAVI_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
	vector		vEffectFlag : SV_TARGET2;
	vector		vEffectDiffuse : SV_TARGET3;
	vector		vGlowFlag : SV_TARGET4;
};



PS_LIGHTOUT PS_DEFAULT_MAIN(VS_DEFAULT_OUT In)
{
	PS_LIGHTOUT		Out = (PS_LIGHTOUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}

PS_NAVI_OUT PS_NAVIGATION_MAIN(VS_DEFAULT_OUT In)
{
	PS_NAVI_OUT		Out = (PS_NAVI_OUT)0;

	Out.vDiffuse = In.vColor;

	//간격마다 찐하게 주고싶은데..
	if (In.vTexUV.x >= 0.98f ||
		In.vTexUV.y >= 0.98f ||
		In.vTexUV.x <= 0.02f ||
		In.vTexUV.y <= 0.02f
		)
	Out.vDiffuse.xyz = 0.f;


	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);


	return Out;
}


VS_OUT_LIGHT VS_MAIN_NORMAL(VS_DEFAULT_IN In)
{
	VS_OUT_LIGHT		Out = (VS_OUT_LIGHT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTileFlag = In.vColor;

	return Out;
}

PS_LIGHTOUT PS_MAIN_NORMAL(PS_IN_LIGHT In)
{
	PS_LIGHTOUT		Out = (PS_LIGHTOUT)0;

	vector TextureDesc[TEXTURESIZE];

	//float fTileRatio = 4.f;
	TextureDesc[0] = g_DiffArray[0].Sample(DefaultSampler, In.vTexUV * g_fTileRatio);
	TextureDesc[1] = g_DiffArray[1].Sample(DefaultSampler, In.vTexUV * g_fTileRatio);
	TextureDesc[2] = g_DiffArray[2].Sample(DefaultSampler, In.vTexUV * g_fTileRatio);

	int iTileFlag = (int)(round(In.vTileFlag.r + 1.f));
	

	Out.vDiffuse 
		= (TextureDesc[iTileFlag] * In.vTileFlag.b)
		+ (TextureDesc[0] * (1.f - In.vTileFlag.b));


	Out.vDiffuse.a = 1.f;

	float fDistance = length(In.vWorldPos - g_vCamPosition);


	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.0f, 0.f, 0.f);

	Out.vFlag = g_vFlag;


	return Out;
}


struct PS_SHADOW_OUT
{
	vector		vLightDepth : SV_TARGET0;
};

PS_SHADOW_OUT PS_SHADOW_MAIN(VS_OUT_LIGHT In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	float2 staticUV = float2(
		(In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f,
		(In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f
		);

	Out.vLightDepth.rgb = In.vProjPos.w / 1500.f;

	vector vStaticDesc = g_StaticShadowTexture.Sample(ShadowSampler, staticUV);

	if (vStaticDesc.r < Out.vLightDepth.r)
		discard;


	Out.vLightDepth.a = 1.f;

	return Out;
}


PS_SHADOW_OUT PS_STATICSHADOW_MAIN(VS_OUT_LIGHT In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	Out.vLightDepth.rgb = In.vProjPos.w / 1500.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}


technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DEFAULT_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DEFAULT_MAIN();
	}

	pass Navigation
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_DEFAULT_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_NAVIGATION_MAIN();
	}

	pass RealTerrain
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);


		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass StaticShadow
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_STATICSHADOW_MAIN();
	}

}