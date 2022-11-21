
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_NoiseTexture;
texture2D g_NormalTexture;

float g_fAlpha = 1.f;
float g_fProgress = 1.0f;
float g_fProgressY = 0.4f;

float4 g_vColor;

vector g_vFlag;
vector g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);

float g_fValue;
float g_fHpRatio;
float g_fHeroValue;

float4 g_SliceRatio;
float2 g_TextureSize;

bool g_bAppear;


struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vSize : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    matrix matWV, matWVP;
    
    // float3 vSize = (vector) 1;
    // Out.vSize = vSize.xy;
    // vSize = mul(vector(vSize, 0.f), g_WorldMatrix);
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;

    return Out;
}


struct VS_TRAIL_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float4 vProjPos : TEXCOORD1;
};

VS_TRAIL_OUT VS_TRAIL_MAIN(VS_IN In)
{
    VS_TRAIL_OUT Out = (VS_TRAIL_OUT) 0;

    matrix matWV, matWVP;

    matWV = g_ViewMatrix;
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vProjPos = Out.vPosition;

    return Out;
}

struct VS_SKILLICON_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float3 vLocalPos : TEXCOORD1;
};

VS_SKILLICON_OUT VS_SKILLICON_MAIN(VS_IN In)
{
    VS_SKILLICON_OUT Out = (VS_SKILLICON_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    Out.vLocalPos = In.vPosition;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
    float2 vSize : TEXCOORD1;
};

struct PS_OUT
{
    vector vColor : SV_TARGET0;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    if (Out.vColor.a < 0.1f)
        discard;

    return Out;
}

PS_OUT PS_COLOR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    //// 버텍스 셰이더에서 사이즈를 1을 사이즈로 나눠서
    //float2 vSize = 1.f;
    //float fX = vSize.x / g_TextureSize.x;
    //float fY = vSize.y / g_TextureSize.y;
    //
    //// 전체 그려져야 할 이미지의 픽셀 개수를 확인
    //float fPixelWidth = In.vTexUV.x * fX;
    //float fPixelHeight = In.vTexUV.y * fY;
    //
    //// 픽셀 개수를 가지고 UV 좌표를 구하고
    //float fUVLeftX = g_TextureSize.x * g_SliceRatio.x;
    //float fUVTopY = g_TextureSize.y * g_SliceRatio.y;
    //
    //float fUVRightX = vSize.x - (g_TextureSize.x * (1 - g_SliceRatio.z));
    //float fUVBottomX = vSize.y - (g_TextureSize.y * (1 - g_SliceRatio.w));
    //    
    //float HorizonLinear = (g_SliceRatio.z - g_SliceRatio.x) / (fUVRightX - fUVLeftX);
    //float VertiLinear = (g_SliceRatio.w - g_SliceRatio.y) / (fUVBottomX - fUVTopY);
    //
    //
    //float2 TexUV = In.vTexUV;
    //
    //if (fPixelWidth > fUVLeftX)
    //{
    //    In.vTexUV.x = HorizonLinear * fPixelWidth + g_SliceRatio.x;
    //}
    //else if (fPixelWidth < fUVRightX)
    //{
    //    
    //}
    //
    //if (fPixelHeight > fUVTopY)
    //{
    //    
    //}
    //else if (fPixelHeight < fUVBottomX)
    //{
    //    
    //}
    //
    //float LeftAnchor = g_SliceRatio.x;
    //float TopAnchor = g_SliceRatio.y;
    //float RightAnchor = g_SliceRatio.z;
    //float BottomAnchor = g_SliceRatio.w;
    //
    //float PaddleLeft = g_TextureSize.x * LeftAnchor;
    //float PaddleRight = (vRectSize.x - (g_TextureSize.x * (1 - RightAnchor)));
    //float PaddleTop = g_TextureSize.y * TopAnchor;
    //float PaddleBottom = (vRectSize.y - (g_TextureSize.y * (1 - BottomAnchor)));
	
    //float2 TexUV = In.vTexUV;
    //
    //float HorizonLinear = (RightAnchor - LeftAnchor) / (PaddleRight - PaddleLeft);
    //float VertiLinear = (BottomAnchor - TopAnchor) / (PaddleBottom - PaddleTop);
	
    //if (vRectSize.x > g_TextureSize.x)
    //{
    //    if (WidPixelNum > PaddleLeft && WidPixelNum < PaddleRight)
    //    {
    //        TexUV.x = HorizonLinear * WidPixelNum + LeftAnchor;
    //    }
    //    else if (WidPixelNum <= PaddleLeft)
    //    {
    //        TexUV.x = WidPixelNum / g_TextureSize.x; //찾아야될 픽셀 / 이미지 크기
    //    }
    //    else if (WidPixelNum >= PaddleRight)
    //    {
    //        TexUV.x = (vRectSize.x - WidPixelNum) / g_TextureSize.x; //찾아야될 픽셀 / 이미지 크기
    //    }
    //}
    //else
    //{
    //    TexUV.x = In.vTexUV.x;
    //}
    //
    //if (vRectSize.y > g_TextureSize.y)
    //{
    //    if (HeightPixelNum > PaddleTop && HeightPixelNum < PaddleBottom)
    //    {
    //        TexUV.y = VertiLinear * HeightPixelNum + TopAnchor;
    //    }
    //    else if (HeightPixelNum <= PaddleTop)
    //    {
    //        TexUV.y = HeightPixelNum / g_TextureSize.y; //찾아야될 픽셀 / 이미지 크기
    //    }
    //    else if (HeightPixelNum >= PaddleBottom)
    //    {
    //        TexUV.y = (vRectSize.y - HeightPixelNum) / g_TextureSize.y; //찾아야될 픽셀 / 이미지 크기
    //    }
    //}
    //else
    //{
    //    TexUV.y = In.vTexUV.y;
    //}
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;
    
    if (Out.vColor.w < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_HPBAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor.xyz *= saturate(vNoise.r + 0.95f);
    Out.vColor.xyz *= saturate(vNormal.r + 0.95f);
    
    if (In.vTexUV.x >= g_fHpRatio)
        discard;
    
    if (Out.vColor.w < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_LOADINGICON(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
	
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	
    if (Out.vColor.a < 0.1f)
        discard;
	
    if (Out.vColor.b >= 0.99f)
    {
        Out.vColor = 1.f;
        return Out;
    }
	
    if (g_bAppear)
    {
        if (Out.vColor.r >= g_fValue)
            discard;
    }
    else
    {
        if (Out.vColor.r <= g_fValue)
            discard;
    }
	
    Out.vColor.rgb = 1.f;
	
    return Out;
}

PS_OUT PS_HEROGAUGE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    In.vTexUV.x -= g_fValue;
    vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor *= (vNoise + vNoise);
    
    if (In.vTexUV.y < g_fHeroValue)
        discard;
	
    if (Out.vColor.a < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_RELIC(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
        
    In.vTexUV += g_fValue;
      
    vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
            
    if (Out.vColor.a < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_PORTEFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    In.vTexUV.y += g_fValue;
    
    vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor.a *= vNoise.r;
    // Out.vColor *= vNormal;
          
    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;
    
    if (Out.vColor.r <= 0.01f)
        discard;
    
    if (Out.vColor.a < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_MAINEFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV) * 2.f;
    
    In.vTexUV.x -= g_fValue;
    vector vMask = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor = vMask;
    Out.vColor.xyz = vColor.xyz;
    Out.vColor.a *= vColor.r;
    
    if (Out.vColor.a < 0.02f)
        discard;
    
    Out.vColor.a = vMask.a;
    
    return Out;
}

PS_OUT PS_LOBBYEFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    In.vTexUV.y += 0.44f;
    vector vNoise = g_NoiseTexture.Sample(ClampSampler, In.vTexUV);
    
    In.vTexUV.y -= g_fValue;
    vector vNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor = vColor;
    Out.vColor.a += vNoise.r;
    Out.vColor.a *= vColor.r;
    Out.vColor.a *= vNormal.r;
    
    return Out;
}

PS_OUT PS_UIColor_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
		
    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;

    if (Out.vColor.w < 0.01f)
        discard;

    return Out;
}

PS_OUT PS_LoadingBar_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    if (In.vTexUV.x >= g_fProgress)
    {
        Out.vColor.rgb = 0.6f;
    }

    Out.vColor.w *= g_fAlpha;

    if (Out.vColor.w < 0.1f)
        discard;

    return Out;
}

PS_OUT PS_NOBLACK_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w = Out.vColor.r * g_fAlpha;

    if (Out.vColor.w < 0.3f)
        discard;


    return Out;
}

PS_OUT PS_HPBAREFFECT_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w *= g_fAlpha;
    Out.vColor.x = 0.8f;
    Out.vColor.y = 0.f;
    Out.vColor.z = 0.f;

	/*Out.vColor.w = (Out.vColor.r * 2.f) * g_fAlpha;

	if (Out.vColor.w < 0.3f)
		discard;*/


    return Out;
}

PS_OUT PS_CROSSHAIR_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w *= g_fAlpha;
    if (Out.vColor.w >= 1.f)
        Out.vColor.w = 0.7f;

    if (Out.vColor.w < 0.1f)
        discard;

    return Out;
}

PS_OUT PS_COLORNOBLACK_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w = Out.vColor.r * g_fAlpha;

    if (Out.vColor.w < 0.3f)
        discard;

    Out.vColor *= g_vColor;


    return Out;
}

PS_OUT PS_HPBAR_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w *= g_fAlpha;

    if (Out.vColor.w < 0.1f)
        discard;


    Out.vColor.xyz = g_vColor.xyz;

    return Out;
}

PS_OUT PS_SKILLICON_MAIN(VS_SKILLICON_OUT In)
{
	//2. 원으로 출력
    float fRadius = length(In.vLocalPos);
    if (fRadius > 0.5f)
    {
		//반지름이 0.5보다 크면 안그림
        discard;
    }
	
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//1. 스킬 쿨타임에 맞춰 아래부터
    if (In.vLocalPos.y > -g_fProgressY)
    {
        Out.vColor.xyz *= 0.3f;

		//전역으로 -0.5~0.5의 Y값을 던져줌.
		//픽셀 y가 전역으로 던져준 높이보다 낮으면 원래 색으로, 아니면ㅇ ㅓ둡게

		//쿨 없을때 0.5고 쓸 수 이을때 -0.5임

		//- 0.5일때 다 출력되어야함
    }

    Out.vColor.w *= g_fAlpha;


    if (Out.vColor.w < 0.1f)
        discard;



    return Out;
}

PS_OUT PS_ENEMYHPBAR_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w = Out.vColor.r * g_fAlpha;

    if (Out.vColor.w < 0.1f)
        discard;

    Out.vColor = vector(1.f, 0.f, 0.f, 1.f);


    return Out;
}

PS_OUT PS_ENEMYHPBARLINE_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w *= g_fAlpha;

    if (Out.vColor.w < 0.8f)
        discard;

    Out.vColor = vector(0.f, 0.f, 0.f, 1.f);


    return Out;
}

PS_OUT PS_KOTO_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.x *= g_fAlpha;

    if (Out.vColor.x < 0.05f)
        discard;

    Out.vColor = vector(0.f, 0.f, 0.f, Out.vColor.x * 0.3f);


    return Out;
}

PS_OUT PS_DIALOG_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w *= g_fAlpha;

    if (In.vTexUV.x > 0.7f)
    {
        float fRatio = max(0.f, ((0.9f - In.vTexUV.x) / 0.2f));
        Out.vColor.w *= fRatio;
    }


    return Out;
}

struct PS_EFFECT_OUT
{
    vector vDiffuse : SV_TARGET0;
    vector vDepth : SV_TARGET1;
    vector vFlag : SV_TARGET2;
    vector vEffectDiffuse : SV_TARGET3;
    vector vGlowFlag : SV_TARGET4;

};

PS_EFFECT_OUT PS_TRAIL_MAIN(VS_TRAIL_OUT In)
{
    PS_EFFECT_OUT Out = (PS_EFFECT_OUT) 0;

    In.vTexUV.x = 1.f - In.vTexUV.x;
	//Mask
    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vDiffuse.a = Out.vDiffuse.r;


    In.vTexUV.x = 1.f - In.vTexUV.x;
    //In.vTexUV.y *= 5.f;
    Out.vDiffuse.xyz = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).xyz;

    Out.vDiffuse *= g_vColor;

    if (Out.vDiffuse.a < 0.05f)
        discard;

    //Out.vDiffuse.a = 1.f;

    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
    Out.vFlag = g_vFlag;
    Out.vGlowFlag = g_vGlowFlag;
    Out.vEffectDiffuse = Out.vDiffuse;

    return Out;
}

PS_EFFECT_OUT PS_FOOTTRAIL_MAIN(VS_TRAIL_OUT In)
{
    PS_EFFECT_OUT Out = (PS_EFFECT_OUT) 0;

    In.vTexUV.x = 1.f - In.vTexUV.x;
	//Mask
    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vDiffuse.a = Out.vDiffuse.r;


    In.vTexUV.x = 1.f - In.vTexUV.x;
    In.vTexUV.y *= 1.f;
    Out.vDiffuse.xyz = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).xyz;

    Out.vDiffuse *= g_vColor;

    if (Out.vDiffuse.a < 0.7f)
        discard;

    Out.vDiffuse.a = 0.4f;

    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
    Out.vFlag = g_vFlag;
    Out.vGlowFlag = g_vGlowFlag;
    Out.vEffectDiffuse = Out.vDiffuse;

    return Out;
}

PS_OUT PS_BOSSHP_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w *= g_fAlpha;
    Out.vColor.yz = 0.03f;
    Out.vColor.x = 1.f;
    if (In.vTexUV.x >= 0.6f)
    {
		//Ratio가 0부터 1까지 나와야함
		//x는 지금 0.~.0.3임
        float fRatio = max(0.f, ((In.vTexUV.x - 0.6f) / 0.4f));
        Out.vColor.y += 0.6f * fRatio;
    }

    if (In.vTexUV.y >= 0.35f && In.vTexUV.y <= 0.45f)
    {
		//0.6에서 0.8까지는 하얗게
		//Ratio가 0~1로 나옴
        float fRatio = max(0.f, ((In.vTexUV.y - 0.35f) / 0.1f));
        Out.vColor.xyz += 0.7f * fRatio;
    }
    else if (In.vTexUV.y > 0.45f && In.vTexUV.y <= 0.55f)
    {
		//0.45에서 0.55까지는 하얗게
		//Ratio가 0~1로 나옴
        float fRatio = max(0.f, ((In.vTexUV.y - 0.45f) / 0.1f));
        Out.vColor.xyz += 0.7f * (1.f - fRatio);
    }


    return Out;
}

PS_OUT PS_BLOODOVERLAY(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.a = Out.vColor.a * g_fAlpha;
    if (Out.vColor.a < 0.01f)
        discard;

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

    pass UI_Color
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_COLOR();
    }

    pass UI_LoadingIcon
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_LOADINGICON();
    }

    pass UI_HpBar
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_HPBAR();
    }

    pass UI_HeroGauge
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_HEROGAUGE();
    }

    pass UI_RELIC
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_RELIC();
    }

    pass UI_PortEffect
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PORTEFFECT();
    }

    pass UI_HeroKeySill
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PORTEFFECT();
    }

    pass UI_MainEffect
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAINEFFECT();
    }

    pass UI_LobbyEffect
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_LOBBYEFFECT();
    }

    pass ALPHA
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_UIColor_MAIN();
    }


    pass TRAIL
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_None);

        VertexShader = compile vs_5_0 VS_TRAIL_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TRAIL_MAIN();
    }

    pass BLOODOVERLAY
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_None);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_BLOODOVERLAY();
    }
}