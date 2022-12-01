
#include "Client_Shader_Defines.hpp"

matrix	g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
vector		g_vColor = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vFlag = vector(0.f, 0.f, 0.f, 0.f);
vector		g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);
float		g_fOutlinePower = 1.f;

texture2D	g_DiffuseTexture, g_NoiseTexture, g_NormalTexture;

bool		g_bBillBoard;
float3		g_vCamLook;


int			g_iWidthSize;
int			g_iHeightSize;
float		g_iCurWidthIndex;
float		g_iCurHeightIndex;

bool		g_bBlackBG;
float		g_fDissolvePower = 5.f;
float		g_fDiscardPower = 0.01f;

vector		g_vPlusColor;
float		g_fColorPower;


struct VS_IN
{
	float3		vPosition : POSITION;
	float2		vTexUV : TEXCOORD0;

	float4		vRight : TEXCOORD1;
	float4		vUp : TEXCOORD2;
	float4		vLook : TEXCOORD3;
	float4		vTranslation : TEXCOORD4;
	float4		vColor : TEXCOORD5;
};

struct VS_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vColor : TEXCOORD3;

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
	Out.vTexUV = In.vTexUV;
	Out.vProjPos = Out.vPosition;
	Out.vColor = In.vColor;

	return Out;	
}

// w나누기연산을 수행하낟. (In 투영스페이스)
// 뷰포트 변환. (In 뷰포트(윈도우좌표))

// 래스터라이즈(픽셀정볼르 생성한다. )

struct PS_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
	float4		vWorldPos : TEXCOORD1;
	float4		vProjPos : TEXCOORD2;
	float4		vColor : TEXCOORD3;

};

struct PS_OUT
{
	vector		vDiffuse : SV_TARGET0;
	vector		vDepth : SV_TARGET1;
	vector		vFlag : SV_TARGET2;
	vector		vEffectDiffuse : SV_TARGET3;
	vector		vGlowFlag : SV_TARGET4;

};


PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//Only masking
	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.a = (Out.vDiffuse.a);

	if ((abs(In.vColor.x) > 0.f) || (abs(In.vColor.y) > 0.f) || (abs(In.vColor.z) > 0.f))
		Out.vDiffuse.xyz = In.vColor.xyz;

	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;

	Out.vDiffuse.a *= In.vColor.a;

	if (Out.vDiffuse.a < g_fDiscardPower)
		discard;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;	
}

PS_OUT PS_COLORMAP_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//masking
	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse.a = (Out.vDiffuse.r);

	vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);


	Out.vDiffuse.xyz = vColor.xyz;
	Out.vDiffuse.xyz += In.vColor.xyz;

	Out.vDiffuse.a *= In.vColor.a;

	if (Out.vDiffuse.a < 0.05f)
		discard;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}


PS_OUT PS_ANIMATION_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	
	vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);

	//알파가 줄어들면
	vector vDissolveDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV * g_fDissolvePower);

	


	float fStepX = 1.f / g_iWidthSize;

	//갯수만큼 나누고
	In.vTexUV.x /= g_iWidthSize;
	//현재 가로줄만큼 늘려
	In.vTexUV.x += fStepX * In.vColor.x;


	float fStepY = 1.f / g_iHeightSize;

	//갯수만큼 나누고
	In.vTexUV.y /= g_iHeightSize;
	//현재 세로줄만큼 늘려
	In.vTexUV.y += fStepY * In.vColor.y;


	//masking
	vector vMaskDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.xyz = vMaskDesc.xyz; //마스크의 색상까지 가져옴
	if (g_bBlackBG)
	{
		Out.vDiffuse.a = vMaskDesc.r;
	}
	else
		Out.vDiffuse.a = vMaskDesc.a;
	//알파는 마스크맵 검은곳에다가 기본 칼라까지
	

	if (Out.vDiffuse.a < 0.01f)
		discard;


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}

PS_OUT PS_ANIMATION_ALPHA_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	
	vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);

	float fStepX = 1.f / g_iWidthSize;

	//갯수만큼 나누고
	In.vTexUV.x /= g_iWidthSize;
	//현재 가로줄만큼 늘려
	In.vTexUV.x += fStepX * In.vColor.x;


	float fStepY = 1.f / g_iHeightSize;

	//갯수만큼 나누고
	In.vTexUV.y /= g_iHeightSize;
	//현재 세로줄만큼 늘려
	In.vTexUV.y += fStepY * In.vColor.y;


	//masking
	vector vMaskDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vMaskDesc; //마스크의 색상까지 가져옴

	if (g_bBlackBG)
	{
		Out.vDiffuse.a = vMaskDesc.r;
	}
	else
		Out.vDiffuse.a = vMaskDesc.a;

	if (Out.vDiffuse.a < 0.05f)
		discard;

	Out.vDiffuse.a *= In.vColor.a;
	
	if (Out.vDiffuse.a < g_fDiscardPower)
		discard;

	

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}




PS_OUT PS_DEBUG_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vDiffuse.xyz = 1;
	Out.vDiffuse.a *= In.vColor.a;

	if (Out.vDiffuse.a < 0.05f)
		discard;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}

PS_OUT PS_ANIMATION_DISSOLVE_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;


	vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);

	//알파가 줄어들면
	vector vDissolveDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV * g_fDissolvePower);




	float fStepX = 1.f / g_iWidthSize;

	//갯수만큼 나누고
	In.vTexUV.x /= g_iWidthSize;
	//현재 가로줄만큼 늘려
	In.vTexUV.x += fStepX * In.vColor.x;


	float fStepY = 1.f / g_iHeightSize;

	//갯수만큼 나누고
	In.vTexUV.y /= g_iHeightSize;
	//현재 세로줄만큼 늘려
	In.vTexUV.y += fStepY * In.vColor.y;


	//masking
	vector vMaskDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vMaskDesc; //마스크의 색상까지 가져옴
	Out.vDiffuse.xyz = g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;


	if (g_bBlackBG)
	{
		Out.vDiffuse.a = vMaskDesc.r;
	}
	else
		Out.vDiffuse.a = vMaskDesc.a;

	if (Out.vDiffuse.a < 0.01f)
		discard;
	//알파는 마스크맵 검은곳에다가 기본 칼라까지
	/*Out.vDiffuse.a *= ;

	if (Out.vDiffuse.a < 0.01f)
		discard;*/

	//내 알파가 이녀석 r보다 더 크면
	if (vDissolveDesc.r > In.vColor.a)
		discard;

	//if (vDissolveDesc.r >= Out.vDiffuse.a - 0.05 && vDissolveDesc.r <= Out.vDiffuse.a + 0.05)
	//	Out.vDiffuse = float4(1, 0, 0, 1); // 빨
	//else
	//	;

	//if (vDissolveDesc.r >= Out.vDiffuse.a - 0.03 && vDissolveDesc.r <= Out.vDiffuse.a + 0.03)
	//	Out.vDiffuse = float4(1, 1, 0, 1); // 노
	//else
	//	;

	//if (vDissolveDesc.r >= Out.vDiffuse.a - 0.025 && vDissolveDesc.r <= Out.vDiffuse.a + 0.025)
	//	Out.vDiffuse = float4(1, 1, 1, 1); // 흰
	//else
	//	;


	if (Out.vDiffuse.a < 0.01f)
		discard;


	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}

PS_OUT PS_BLACKBACKGROUND_TEXTURE(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//Only masking
	Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse.a = Out.vDiffuse.r;

	if ((abs(In.vColor.x) > 0.f) || (abs(In.vColor.y) > 0.f) || (abs(In.vColor.z) > 0.f))
		Out.vDiffuse.xyz = In.vColor.xyz;

	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;
	Out.vDiffuse.a *= In.vColor.a;

	if (Out.vDiffuse.a < g_fDiscardPower)
		discard;

	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}

PS_OUT PS_ANIMATION_ALPHACOLOR_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);

	float fStepX = 1.f / g_iWidthSize;

	//갯수만큼 나누고
	In.vTexUV.x /= g_iWidthSize;
	//현재 가로줄만큼 늘려
	In.vTexUV.x += fStepX * In.vColor.x;


	float fStepY = 1.f / g_iHeightSize;

	//갯수만큼 나누고
	In.vTexUV.y /= g_iHeightSize;
	//현재 세로줄만큼 늘려
	In.vTexUV.y += fStepY * In.vColor.y;


	
	//masking
	vector vMaskDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vDiffuse = vMaskDesc;

	if (g_bBlackBG)
	{
		Out.vDiffuse.a = vMaskDesc.r;
	}
	else
		Out.vDiffuse.a = vMaskDesc.a;

	if (Out.vDiffuse.a < 0.01f)
		discard;

	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;

	Out.vDiffuse.a *= In.vColor.a;

	if (Out.vDiffuse.a < g_fDiscardPower)
		discard;



	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}

PS_OUT PS_UVTEXTURESELECT_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);

	float fStepX = 1.f / g_iWidthSize;

	//갯수만큼 나누고
	In.vTexUV.x /= g_iWidthSize;
	//현재 가로줄만큼 늘려
	In.vTexUV.x += fStepX * In.vColor.x;


	float fStepY = 1.f / g_iHeightSize;

	//갯수만큼 나누고
	In.vTexUV.y /= g_iHeightSize;
	//현재 세로줄만큼 늘려
	In.vTexUV.y += fStepY * In.vColor.y;


	//masking
	vector vMaskDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vDiffuse = vMaskDesc; //마스크의 색상까지 가져옴

	if (g_bBlackBG)
	{
		Out.vDiffuse.a = vMaskDesc.r;
	}
	else
		Out.vDiffuse.a = vMaskDesc.a;

	if (Out.vDiffuse.a < 0.05f)
		discard;

	Out.vDiffuse.xyz += g_vPlusColor.xyz;
	Out.vDiffuse.xyz *= g_fColorPower;

	Out.vDiffuse.a *= In.vColor.a;

	if (Out.vDiffuse.a < g_fDiscardPower)
		discard;



	Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
	Out.vFlag = g_vFlag;
	Out.vGlowFlag = g_vGlowFlag;
	Out.vEffectDiffuse = Out.vDiffuse;

	return Out;
}

struct PS_UIFIRE_OUT
{
	vector vColor : SV_TARGET0;
	vector vFlag : SV_TARGET1;
};



technique11 DefaultTechnique
{
	pass Default
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN();
	}

	pass ColorMap
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_COLORMAP_MAIN();
	}

	pass DEBUG
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DEBUG_MAIN();
	}
	
	pass ANIMATION
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ANIMATION_MAIN();
	}

	pass ANIMATION_ALPHA
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ANIMATION_ALPHA_MAIN();
	}

	pass ANIMATION_DISSOLVE
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ANIMATION_DISSOLVE_MAIN();
	}

	pass BLACKBACKGROUND_TEXTURE
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLACKBACKGROUND_TEXTURE();
	}

	pass ANIMATION_ALPHACOLOR
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_ANIMATION_ALPHACOLOR_MAIN();
	}

	pass UVTEXTURESELECT
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_Default, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_UVTEXTURESELECT_MAIN();
	}

	pass FLARE
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_None);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLACKBACKGROUND_TEXTURE();
	}

	
}