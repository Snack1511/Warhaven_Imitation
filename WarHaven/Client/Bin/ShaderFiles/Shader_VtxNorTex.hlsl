
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

vector g_vRimLightFlag;
vector g_vOutLineFlag;

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
	float4		vTileFlag : TEXCOORD1;
	float4		vWorldPos : TEXCOORD2;
	float4		vProjPos : TEXCOORD3;
};

struct PS_IN_LIGHT
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vTileFlag : TEXCOORD1;
	float4		vWorldPos : TEXCOORD2;
	float4		vProjPos : TEXCOORD3;
};


struct PS_LIGHTOUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vFlag : SV_TARGET3;
	vector		vOutlineFlag : SV_TARGET4;
	vector		vRimLightFlag : SV_TARGET5;
	vector	vPBR : SV_TARGET6;

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

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.0f, 1.f, 0.f);

	Out.vFlag = g_vFlag;

	return Out;
}



struct PS_SHADOW_OUT
{
	vector		vLightDepth : SV_TARGET0;
};

PS_SHADOW_OUT PS_SHADOW_MAIN(PS_IN_LIGHT In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	float2 staticUV = float2(
		(In.vProjPos.x / In.vProjPos.w) * 0.5f + 0.5f,
		(In.vProjPos.y / In.vProjPos.w) * -0.5f + 0.5f
		);

	Out.vLightDepth.rgb = In.vProjPos.w / 1500.f;
	Out.vLightDepth.g = Out.vLightDepth.r * Out.vLightDepth.r;

	vector vStaticDesc = g_StaticShadowTexture.Sample(ShadowSampler, staticUV);

	if (vStaticDesc.r < Out.vLightDepth.r)
		discard;


	Out.vLightDepth.a = 1.f;

	return Out;
}


PS_SHADOW_OUT PS_STATICSHADOW_MAIN(PS_IN_LIGHT In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	Out.vLightDepth.rgb = In.vProjPos.w / 1500.f;
	//Out.vLightDepth.g = Out.vLightDepth.r * Out.vLightDepth.r;

	Out.vLightDepth.a = 1.f;

	return Out;
}



struct VS_OUT_TESS
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vTileFlag : TEXCOORD1;
};

VS_OUT_TESS VS_MAIN_TESS(VS_DEFAULT_IN In)
{
	VS_OUT_TESS		Out = (VS_OUT_TESS)0;

	matrix		matWV, matWVP;

	Out.vPosition = In.vPosition;
	Out.vNormal = In.vNormal;
	Out.vTexUV = In.vTexUV;
	Out.vTileFlag = In.vColor;

	return Out;
}



struct PatchTess
{
	float EdgeTess[3] : SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;

	// Geometry cubic generated control points
	float3 f3B210    : POSITION3;
	float3 f3B120    : POSITION4;
	float3 f3B021    : POSITION5;
	float3 f3B012    : POSITION6;
	float3 f3B102    : POSITION7;
	float3 f3B201    : POSITION8;
	float3 f3B111    : CENTER;

	// Normal quadratic generated control points
	float3 f3N110    : NORMAL3;
	float3 f3N011    : NORMAL4;
	float3 f3N101    : NORMAL5;
};

PatchTess ConstantHS(InputPatch<VS_OUT_TESS, 3> Patch, uint PatchID : SV_PrimitiveID)
{
	PatchTess		pt;

	/* 몇개 쪼갤 지 */
	pt.EdgeTess[0] = 5;
	pt.EdgeTess[1] = 5;
	pt.EdgeTess[2] = 5;

	pt.InsideTess = (pt.EdgeTess[0] + pt.EdgeTess[1] + pt.EdgeTess[2]) / 3.f;

	// Assign Positions
	float3 f3B003 = Patch[0].vPosition;
	float3 f3B030 = Patch[1].vPosition;
	float3 f3B300 = Patch[2].vPosition;
	// And Normals
	float3 f3N002 = Patch[0].vNormal;
	float3 f3N020 = Patch[1].vNormal;
	float3 f3N200 = Patch[2].vNormal;

	// Compute the cubic geometry control points
	// Edge control points
	pt.f3B210 = ((2.0f * f3B003) + f3B030 - (dot((f3B030 - f3B003), f3N002) * f3N002)) / 3.0f;
	pt.f3B120 = ((2.0f * f3B030) + f3B003 - (dot((f3B003 - f3B030), f3N020) * f3N020)) / 3.0f;
	pt.f3B021 = ((2.0f * f3B030) + f3B300 - (dot((f3B300 - f3B030), f3N020) * f3N020)) / 3.0f;
	pt.f3B012 = ((2.0f * f3B300) + f3B030 - (dot((f3B030 - f3B300), f3N200) * f3N200)) / 3.0f;
	pt.f3B102 = ((2.0f * f3B300) + f3B003 - (dot((f3B003 - f3B300), f3N200) * f3N200)) / 3.0f;
	pt.f3B201 = ((2.0f * f3B003) + f3B300 - (dot((f3B300 - f3B003), f3N002) * f3N002)) / 3.0f;

	// Center control point
	float3 f3E = (pt.f3B210 + pt.f3B120 + pt.f3B021 + pt.f3B012 + pt.f3B102 + pt.f3B201) / 6.0f;
	float3 f3V = (f3B003 + f3B030 + f3B300) / 3.0f;
	pt.f3B111 = f3E + ((f3E - f3V) / 2.0f);

	// Compute the quadratic normal control points, and rotate into world space
	float fV12 = 2.0f * dot(f3B030 - f3B003, f3N002 + f3N020) / dot(f3B030 - f3B003, f3B030 - f3B003);
	pt.f3N110 = normalize(f3N002 + f3N020 - fV12 * (f3B030 - f3B003));
	float fV23 = 2.0f * dot(f3B300 - f3B030, f3N020 + f3N200) / dot(f3B300 - f3B030, f3B300 - f3B030);
	pt.f3N011 = normalize(f3N020 + f3N200 - fV23 * (f3B300 - f3B030));
	float fV31 = 2.0f * dot(f3B003 - f3B300, f3N200 + f3N002) / dot(f3B003 - f3B300, f3B003 - f3B300);
	pt.f3N101 = normalize(f3N200 + f3N002 - fV31 * (f3B003 - f3B300));

	return pt;
}

struct HullOut
{
	float3	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float4		vTileFlag : TEXCOORD1;

};


[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(9.0f)]

HullOut HS_MAIN(InputPatch<VS_OUT_TESS, 3> p,
	uint i : SV_OutputControlPointID,
	uint patchID : SV_PrimitiveID)
{
	HullOut		hout;

	hout.vPosition = p[i].vPosition;
	hout.vNormal = p[i].vNormal;
	hout.vTexUV = p[i].vTexUV;
	hout.vTileFlag = p[i].vTileFlag;

	return hout;
}

struct DomainOut
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vTileFlag : TEXCOORD1;
	float4		vWorldPos : TEXCOORD2;
	float4		vProjPos : TEXCOORD3;

};

[domain("tri")]
DomainOut DS_MAIN(PatchTess PatchTess, float3 uvw : SV_DomainLocation,
	const OutputPatch<HullOut, 3> tri)
{
	// The barycentric coordinates
	float fU = uvw.x;
	float fV = uvw.y;
	float fW = uvw.z;

	// Precompute squares and squares * 3 
	float fUU = fU * fU;
	float fVV = fV * fV;
	float fWW = fW * fW;
	float fUU3 = fUU * 3.0f;
	float fVV3 = fVV * 3.0f;
	float fWW3 = fWW * 3.0f;

	DomainOut		Out = (DomainOut)0;

	float3 p = tri[0].vPosition * fWW * fW +
		tri[1].vPosition * fUU * fU +
		tri[2].vPosition * fVV * fV +
		PatchTess.f3B210 * fWW3 * fU +
		PatchTess.f3B120 * fW * fUU3 +
		PatchTess.f3B201 * fWW3 * fV +
		PatchTess.f3B021 * fUU3 * fV +
		PatchTess.f3B102 * fW * fVV3 +
		PatchTess.f3B012 * fU * fVV3 +
		PatchTess.f3B111 * 6.0f * fW * fU * fV;

	// Compute normal from quadratic control points and barycentric coords
	float3 n = tri[0].vNormal * fWW +
		tri[1].vNormal * fUU +
		tri[2].vNormal * fVV +
		PatchTess.f3N110 * fW * fU +
		PatchTess.f3N011 * fU * fV +
		PatchTess.f3N101 * fW * fV;

	matrix		matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(p, 1.f), matWVP);
	Out.vTexUV = tri[0].vTexUV * fW + tri[1].vTexUV * fU + tri[2].vTexUV * fV;
	Out.vNormal = normalize(mul(vector(n, 0.f), g_WorldMatrix));
	Out.vWorldPos = (vector)0.f;
	Out.vProjPos = Out.vPosition;

	Out.vTileFlag = tri[1].vTileFlag;
	Out.vTileFlag.b = tri[0].vTileFlag.b * fW + tri[1].vTileFlag.b * fU + tri[2].vTileFlag.b * fV;

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
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DEFAULT_MAIN();
	}

	pass Navigation
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_DEFAULT_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_NAVIGATION_MAIN();
	}

	pass RealTerrain
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);


		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass StaticShadow
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_TESS();
		GeometryShader = NULL;
		HullShader = compile hs_5_0 HS_MAIN();
		DomainShader = compile ds_5_0 DS_MAIN();
		PixelShader = compile ps_5_0 PS_STATICSHADOW_MAIN();
	}


	/*pass StaticShadow
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		HullShader = NULL;
		DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_STATICSHADOW_MAIN();
	}*/

	pass TerrainTess
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_TESS();
		GeometryShader = NULL;
		HullShader = compile hs_5_0 HS_MAIN();
		DomainShader = compile ds_5_0 DS_MAIN();
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

}