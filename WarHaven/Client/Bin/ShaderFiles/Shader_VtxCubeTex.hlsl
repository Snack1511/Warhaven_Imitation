
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

textureCUBE	g_DiffuseTexture;

vector	g_vFlag;
vector	g_vGlowFlag;

vector	g_vColor;




struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;
};

VS_OUT VS_MAIN_SKY(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;	
}

struct VS_DEBUG_OUT
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

};

VS_DEBUG_OUT VS_DEBUG_MAIN(VS_IN In)
{
	VS_DEBUG_OUT		Out = (VS_DEBUG_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}



struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;
};

struct PS_OUT
{	
	vector		vColor : SV_TARGET0;	
};

PS_OUT PS_MAIN_SKY(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//Out.vColor.a = 0.5f;

	return Out;	
}

struct PS_DEBUG_IN
{
	float4		vPosition : SV_POSITION;
	float3		vTexUV : TEXCOORD0;
	float4		vProjPos : TEXCOORD1;

};

struct PS_DEBUG_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vFlag : SV_TARGET3;
};

PS_DEBUG_OUT PS_DEBUG_MAIN(PS_DEBUG_IN In)
{
	PS_DEBUG_OUT		Out = (PS_DEBUG_OUT)0;

	//0 ~ 1À»
	//0.5 ~ 1·Î
	In.vTexUV.x += 1;
	In.vTexUV.x *= 0.5;

	Out.vDiffuse = g_vColor * (In.vTexUV.x);
	Out.vDiffuse.a = 1.f;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);



	return Out;
}


technique11 DefaultTechnique
{
	pass Sky
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Sky);

		VertexShader = compile vs_5_0 VS_MAIN_SKY();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SKY();
	}

	pass Debug
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DEBUG_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DEBUG_MAIN();
	}

}