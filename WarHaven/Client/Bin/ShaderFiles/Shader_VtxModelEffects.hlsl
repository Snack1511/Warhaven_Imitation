
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_ViewMatrixInv, g_ProjMatrixInv, g_WorldMatrixInv;

texture2D	g_DiffuseTexture, g_BlendTexture, g_FilterTexture, g_NormalTexture, g_NoiseTexture, g_StaticShadowTexture, g_MaskTexture;
texture2D	g_DepthTexture;

float		g_fRange = 1.f;

float		g_fUVPlusX = 0.f;
float		g_fUVPlusY = 0.f;

float		g_fAlpha = 0.f;
float		g_fNoiseScale = 0.1f;

vector		g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vFlag = vector(0.f, 0.f, 0.f, 0.f);
vector		g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);

vector		g_vPlusColor = vector(0.f, 0.f, 0.f, 1.f);
float		g_fColorPower = 1.f;

float		g_fDissolvePower = 1.f;

vector		g_vCamPosition;
float		g_fOutlinePower = 1.f;

float		g_fBlue;
float		g_fRed;



struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
};

VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
	Out.vNormal = normalize(mul(vector(In.vNormal, 0.f), g_WorldMatrix));
	Out.vWorldPos = mul(vector(In.vPosition, 1.f), g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )



struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float4		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;

};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
	vector		vEffectFlag : SV_TARGET2;
	vector		vEffectDiffuse : SV_TARGET3;
	vector		vGlowFlag : SV_TARGET4;
};

PS_OUT PS_MAIN_DEFAULT(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);

	//DiffuseTexture : Color
	//g_MaskTexture : AlphaMap
	//

	//
	vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	vector vMtrlDiffuse = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.a = vMtrlDiffuse.r;
	Out.vDiffuse.a *= g_fAlpha;

	if (Out.vDiffuse.a <= 0.05f)
		discard;

	Out.vDiffuse.xyz = vColor.xyz;
	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;

	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;


	

	return Out;
}


struct PS_DISTORTION_OUT
{
	vector		vDistortionFlag : SV_TARGET0;
};

PS_DISTORTION_OUT PS_MAIN_DISTORTION(PS_IN In)
{
	PS_DISTORTION_OUT		Out = (PS_DISTORTION_OUT)0;

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;

	vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	vector vDiffuse;

	vDiffuse = vMtrlDiffuse;
	vDiffuse.a = vDiffuse.r;
	vDiffuse.a *= g_fAlpha;

	Out.vDistortionFlag.b = vDiffuse.a;

	if (vDiffuse.a <= 0.05f)
		discard;

	Out.vDistortionFlag.xyz = vDiffuse.a;
	Out.vDistortionFlag.a = 1;

	return Out;
}

PS_OUT PS_MAIN_DISOLVE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);

	//DiffuseTexture : Color
	//g_MaskTexture : AlphaMap

	vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	/*In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;*/

	vector vMask = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.a = vMask.r;

	if (Out.vDiffuse.a <= 0.01f)
		discard;

	vector vDissolve = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * g_fDissolvePower);


	if (vDissolve.r - (1.f - g_fAlpha) < 0.01f)
		discard;

	

	Out.vDiffuse.xyz = vColor.xyz;
	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;

	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;


	return Out;
}

PS_OUT PS_MAIN_CLAMP(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);

	//DiffuseTexture : Color
	//g_MaskTexture : AlphaMap
	//

	//
	vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	vector vMtrlDiffuse = g_MaskTexture.Sample(ClampSampler, In.vTexUV);

	Out.vDiffuse.a = vMtrlDiffuse.r;
	Out.vDiffuse.a *= g_fAlpha;

	if (Out.vDiffuse.a <= 0.05f)
		discard;

	Out.vDiffuse.xyz = vColor.xyz;
	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;

	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;




	return Out;
}

PS_OUT PS_MAIN_BORDER(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);

	//DiffuseTexture : Color
	//g_MaskTexture : AlphaMap
	//

	//
	vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	vector vMtrlDiffuse = g_MaskTexture.Sample(BorderSampler, In.vTexUV);

	Out.vDiffuse.a = vMtrlDiffuse.r;
	Out.vDiffuse.a *= g_fAlpha;

	if (Out.vDiffuse.a <= 0.05f)
		discard;

	Out.vDiffuse.xyz = vColor.xyz;
	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;

	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;




	return Out;
}

PS_OUT PS_MAIN_LIGHTNING (PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);


	if (In.vTexUV.x < g_fUVPlusX)
		discard;


	//DiffuseTexture : Color
	//g_MaskTexture : AlphaMap
	//

	//
	vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	vector vMtrlDiffuse = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.a = vMtrlDiffuse.r;
	Out.vDiffuse.a *= g_fAlpha;

	if (Out.vDiffuse.a <= 0.05f)
		discard;

	Out.vDiffuse.xyz = vColor.xyz;
	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;




	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;

	return Out;
}

PS_OUT PS_MAIN_DOMINION(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vEffectFlag = g_vFlag;
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);

	//DiffuseTexture : Color
	//g_MaskTexture : AlphaMap
	//

	//
	//vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	vector vMtrlDiffuse = g_MaskTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.a = vMtrlDiffuse.r;
	Out.vDiffuse.a *= g_fAlpha;

	if (Out.vDiffuse.a <= 0.05f)
		discard;

	Out.vDiffuse.xyz = g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;



	Out.vEffectDiffuse = Out.vDiffuse;
	Out.vGlowFlag = g_vGlowFlag;




	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEFAULT();
	}
	pass DISTORTION
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
	}
	pass DISOLVE
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DISOLVE();
	}
	pass CLAMP
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_CLAMP();
	}
	pass BORDER
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);
		
		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BORDER();
	}

	pass LIGHTNING
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHTNING();
	}
	

	pass DOMINION
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DOMINION();
	}
}