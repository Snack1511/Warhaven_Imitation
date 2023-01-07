
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vFlag = vector(0.f, 0.f, 0.f, 0.f);
float		g_fOutlinePower = 1.f;

vector		g_vCamPosition;

texture2D	g_DiffuseTexture, g_NoiseTexture, g_NormalTexture;
texture2D	g_StaticShadowTexture;

float g_fUVPlusX;
float g_fUVPlusY;


struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float3		vTangent : TANGENT;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
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

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);
	Out.vPosition = mul(vPosition, matWVP);
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vNormal = mul(vector(In.vNormal, 0.f), TransformMatrix);
	Out.vNormal = normalize(mul(Out.vNormal, g_WorldMatrix));
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;

	return Out;	
}

struct VS_OUT_NORMAL
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;
};

VS_OUT_NORMAL VS_MAIN_NORMAL(VS_IN In)
{
	VS_OUT_NORMAL		Out = (VS_OUT_NORMAL)0;

	matrix			TransformMatrix = float4x4(In.vRight, In.vUp, In.vLook, In.vTranslation);

	vector			vPosition = mul(vector(In.vPosition, 1.f), TransformMatrix);
	float3			vNormal = normalize(mul(vector(In.vNormal, 0.f), TransformMatrix)).xyz;
	float3			vTangent = normalize(mul(vector(In.vTangent, 0.f), TransformMatrix)).xyz;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vPosition, matWVP);

	Out.vNormal = normalize(mul(vector(vNormal, 0.f), g_WorldMatrix)).xyz;
	Out.vWorldPos = mul(vPosition, g_WorldMatrix);
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vTangent = normalize(mul(vector(vTangent, 0.f), g_WorldMatrix)).xyz;
	Out.vBinormal = normalize(cross(Out.vNormal, Out.vTangent));

	return Out;
}

// w�����⿬���� �����ϳ�. (In ���������̽�)
// ����Ʈ ��ȯ. (In ����Ʈ(��������ǥ))

// �����Ͷ�����(�ȼ������� �����Ѵ�. )

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
	vector		vNormal : SV_TARGET1;
	vector		vDepth : SV_TARGET2;
	vector		vFlag : SV_TARGET3;
	vector		vOutlineFlag : SV_TARGET4;
	vector		vRimLightFlag : SV_TARGET5;
};

struct PS_SHADOW_OUT
{
	vector		vLightDepth : SV_TARGET0;
};
PS_SHADOW_OUT PS_SHADOW_MAIN(PS_IN In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	//������ �ִ� �༮�� ���� �׽�Ʈ�� ���־����.
	//In.vProjPos : w������ ���� ���� ������ǥ
	//w �������� -1~1������ ��ǥ


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


PS_SHADOW_OUT PS_STATICSHADOW_MAIN(PS_IN In)
{
	PS_SHADOW_OUT		Out = (PS_SHADOW_OUT)0;

	Out.vLightDepth.rgb = In.vProjPos.w / 1500.f;

	Out.vLightDepth.a = 1.f;

	return Out;
}



PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;
	Out.vDiffuse.a = 1.f;

	//Out.vDiffuse = 1;

	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 1.f);
	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	return Out;	
}


struct PS_IN_NORMAL
{
	float4		vPosition : SV_POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float3		vTangent : TANGENT;
	float3		vBinormal : BINORMAL;

};


PS_OUT PS_MAIN_NORMAL(PS_IN_NORMAL In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	if (Out.vDiffuse.a < 0.2f)
		discard;

	Out.vDiffuse *= g_vColor;
	Out.vDiffuse.a = 1.f;

	//float fDistance = length(In.vWorldPos - g_vCamPosition);

	/* 0 ~ 1 */
	float3		vPixelNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV * 1.f).xyz;

	/* -1 ~ 1 */
	vPixelNormal = normalize(vPixelNormal * 2.f - 1.f) * 1.f;
	if (vPixelNormal.b < 0.1f)
		vPixelNormal *= -1.f;

	float3x3	WorldMatrix = float3x3(In.vTangent, In.vBinormal, In.vNormal);

	vPixelNormal = mul(vPixelNormal, WorldMatrix);

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.0f, 0.f, 0.f);

	Out.vNormal = normalize(vector(vPixelNormal * 0.5f + 0.5f, 0.f));



	Out.vFlag = g_vFlag;



	return Out;
}


struct PS_Particle_OUT
{
	vector		vDiffuse : SV_TARGET0;

};

PS_Particle_OUT PS_Particle_MAIN(PS_IN In)
{
	PS_Particle_OUT		Out = (PS_Particle_OUT)0;

	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse *= g_vColor;
	Out.vDiffuse.a = 1.f;

	return Out;
}

technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}
	pass NormalMapping
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN_NORMAL();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_NORMAL();
	}

	pass Particle
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_Particle_MAIN();
	}

	pass SHADOW
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_SHADOW_MAIN();
	}

	pass STATICSHADOW
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_STATICSHADOW_MAIN();
	}
}