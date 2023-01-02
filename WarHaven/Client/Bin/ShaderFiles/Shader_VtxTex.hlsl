
#include "Client_Shader_Defines.hpp"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

texture2D g_DiffuseTexture;
texture2D g_NoiseTexture;
texture2D g_NormalTexture;

float g_fAlpha = 1.f;
float g_fProgress = 1.0f;
float g_fProgressY = 0.4f;

float4 g_vColor;
float4 g_vPlusColor;

vector g_vFlag;
vector g_vGlowFlag = vector(0.f, 0.f, 0.f, 0.f);

float g_fValue;
bool bFlip;

float g_fHpRatio;
float g_fHeroGaugeRatio;

bool g_bIsDecrease;
float g_fHealthGauge;

bool g_bIsSlice;
float4 g_SliceRatio;
float4 g_vScale;
float2 g_TextureSize;

bool g_bAppear;

float g_fBloodRatio;
bool g_bDeadBlood;

bool g_bBlackBG;
int g_iWidthSize;
int g_iHeightSize;
float g_fRowX;
float g_fColY;

float g_fUVPlusX;
float g_fUVPlusY;
float g_fUVPower;

float g_fTimeAcc;

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
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
    Out.vTexUV = In.vTexUV;
    //Out.vSize = mul(g_vScale, matWVP).xy;
    
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

VS_TRAIL_OUT VS_TRAIL_UI_MAIN(VS_IN In)
{
    VS_TRAIL_OUT Out = (VS_TRAIL_OUT) 0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
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
    vector vFlag : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vFlag = g_vFlag;

    if (Out.vColor.a < 0.1f)
        discard;

    return Out;
}

PS_OUT PS_COLOR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

   // 새로 부여할 UV
    float2 TexUV = In.vTexUV;
    
    if (g_bIsSlice)
    {
        // 셰이더를 적용할 UI의 화면에 출력될 크기
        float2 vSize;
        vSize.x = g_vScale.x;
        vSize.y = g_vScale.y;
    
        // 출력된 UI의 UV좌표를 픽셀 좌표로 변환한 값
        float fPixelCoordX = In.vTexUV.x * vSize.x;
        float fPixelCoordY = In.vTexUV.y * vSize.y;
    
        // 이미지 상에서 늘어나야 할 픽셀의 좌표
        float fUV_CoordLeft = g_TextureSize.x * g_SliceRatio.x;
        float fUV_CoordRight = vSize.x - (g_TextureSize.x * (1 - g_SliceRatio.z));
        float fUV_CoordTop = g_TextureSize.y * g_SliceRatio.y;
        float fUV_CoordBottom = vSize.y - (g_TextureSize.y * (1 - g_SliceRatio.w));
        
        // 보간되는 범위의 비율
        float fRatioX = (g_SliceRatio.z - g_SliceRatio.x) / (fUV_CoordRight - fUV_CoordLeft);
        float fRatioY = (g_SliceRatio.w - g_SliceRatio.y) / (fUV_CoordBottom - fUV_CoordTop);
        
        // 화면에 출력된 UI의 크기가 이미지의 원본보다 크면
        if (vSize.x > g_TextureSize.x)
        {
            if (fPixelCoordX > fUV_CoordLeft && fPixelCoordX < fUV_CoordRight)
            {
                // 비율에 픽셀 좌표를 곱하면 UV좌표로 변환된 비율이 나오는데
                // 비율을 더하면 보간 시작 지점을 정할 수 있다.
                TexUV.x = fRatioX * fPixelCoordX + g_SliceRatio.x;
            }
            else if (fPixelCoordX <= fUV_CoordLeft)
            {
                // 픽셀 위치에서 픽셀 사이즈만큼 나누면 해당 픽셀의 UV좌표가 나옴
                TexUV.x = fPixelCoordX / g_TextureSize.x;
            }
            else if (fPixelCoordX >= fUV_CoordRight)
            {
                // 반대
                TexUV.x = -(vSize.x - fPixelCoordX) / g_TextureSize.x;
            }
        }
        else
        {
            TexUV.x = In.vTexUV.x;
        }
    
        if (vSize.y > g_TextureSize.y)
        {
            if (fPixelCoordY > fUV_CoordTop && fPixelCoordY < fUV_CoordBottom)
            {
                TexUV.y = fRatioY * fPixelCoordY + g_SliceRatio.y;
            }
            else if (fPixelCoordY <= fUV_CoordTop)
            {
                TexUV.y = fPixelCoordY / g_TextureSize.y;
            }
            else if (fPixelCoordY >= fUV_CoordBottom)
            {
                TexUV.y = -(vSize.y - fPixelCoordY) / g_TextureSize.y;
            }
        }
        else
        {
            TexUV.y = In.vTexUV.y;
        }
    }
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, TexUV);
    
    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;
    
    if (Out.vColor.w < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_PROFILE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

    // 새로 부여할 UV
    float2 TexUV = In.vTexUV;

    if (g_bIsSlice)
    {
        // 셰이더를 적용할 UI의 화면에 출력될 크기
        float2 vSize;
        vSize.x = g_vScale.x;
        vSize.y = g_vScale.y;

        // 출력된 UI의 UV좌표를 픽셀 좌표로 변환한 값
        float fPixelCoordX = In.vTexUV.x * vSize.x;
        float fPixelCoordY = In.vTexUV.y * vSize.y;

        // 이미지 상에서 늘어나야 할 픽셀의 좌표
        float fUV_CoordLeft = g_TextureSize.x * g_SliceRatio.x;
        float fUV_CoordRight = vSize.x - (g_TextureSize.x * (1 - g_SliceRatio.z));
        float fUV_CoordTop = g_TextureSize.y * g_SliceRatio.y;
        float fUV_CoordBottom = vSize.y - (g_TextureSize.y * (1 - g_SliceRatio.w));

        // 보간되는 범위의 비율
        float fRatioX = (g_SliceRatio.z - g_SliceRatio.x) / (fUV_CoordRight - fUV_CoordLeft);
        float fRatioY = (g_SliceRatio.w - g_SliceRatio.y) / (fUV_CoordBottom - fUV_CoordTop);

        // 화면에 출력된 UI의 크기가 이미지의 원본보다 크면
        if (vSize.x > g_TextureSize.x)
        {
            if (fPixelCoordX > fUV_CoordLeft && fPixelCoordX < fUV_CoordRight)
            {
                // 비율에 픽셀 좌표를 곱하면 UV좌표로 변환된 비율이 나오는데
                // 비율을 더하면 보간 시작 지점을 정할 수 있다.
                TexUV.x = fRatioX * fPixelCoordX + g_SliceRatio.x;
            }
            else if (fPixelCoordX <= fUV_CoordLeft)
            {
                // 픽셀 위치에서 픽셀 사이즈만큼 나누면 해당 픽셀의 UV좌표가 나옴
                TexUV.x = fPixelCoordX / g_TextureSize.x;
            }
            else if (fPixelCoordX >= fUV_CoordRight)
            {
                // 반대
                TexUV.x = -(vSize.x - fPixelCoordX) / g_TextureSize.x;
            }
        }
        else
        {
            TexUV.x = In.vTexUV.x;
        }

        if (vSize.y > g_TextureSize.y)
        {
            if (fPixelCoordY > fUV_CoordTop && fPixelCoordY < fUV_CoordBottom)
            {
                TexUV.y = fRatioY * fPixelCoordY + g_SliceRatio.y;
            }
            else if (fPixelCoordY <= fUV_CoordTop)
            {
                TexUV.y = fPixelCoordY / g_TextureSize.y;
            }
            else if (fPixelCoordY >= fUV_CoordBottom)
            {
                TexUV.y = -(vSize.y - fPixelCoordY) / g_TextureSize.y;
            }
        }
        else
        {
            TexUV.y = In.vTexUV.y;
        }
    }

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, TexUV);

    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;


    if (TexUV.y > 0.7f)
    {
        float fRatio = ((1.f - TexUV.y) / 0.3f);

       // 0~1을 0.3~1로 바꾸야함
        fRatio *= 0.7f;
        fRatio += 0.3f;
        Out.vColor *= (fRatio);

    }

    if (Out.vColor.w < 0.01f)
        discard;

    return Out;
}

PS_OUT PS_HealthGauge(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
               
    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;
    
    if (g_bIsDecrease)
    {
        if (In.vTexUV.x > g_fHealthGauge)
            discard;
    }
    
    if (Out.vColor.a < 0.01f)
        discard;
    
    return Out;
}

PS_OUT PS_HPBAR(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    //vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    //vector vNormal = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
    
    //Out.vColor.xyz *= saturate(vNoise.r + 0.95f);
    //Out.vColor.xyz *= saturate(vNormal.r + 0.95f);
    
    if (In.vTexUV.x > g_fHpRatio)
        discard;
    
    if (Out.vColor.w < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_LOADINGICON(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, float2(In.vTexUV.x, 1 - In.vTexUV.y));
    
    In.vTexUV.x -= g_fValue;
    vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor.a *= vNoise.r;
    
    if (In.vTexUV.y < g_fHeroGaugeRatio)
        discard;
	
    if (Out.vColor.a < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_RELIC(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;
    
    vector vColor = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    
    vector vNoise = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor = vColor;
    
    Out.vColor.rgb = vNoise.rgb;
    
    Out.vColor.a *= vNoise.r;
          
    Out.vColor.w *= g_fAlpha;
    
    if (Out.vColor.a < 0.01f)
        discard;
	
    return Out;
}

PS_OUT PS_MAINEFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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

PS_OUT PS_VerticalGauge(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;
    
    if (In.vTexUV.y < g_fValue)
        discard;
    
    return Out;
}

PS_OUT PS_HorizontalGauge(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;
    
    if (bFlip)
    {
        if (In.vTexUV.x < g_fValue)
            discard;
    }
    else
    {
        if (In.vTexUV.x > g_fValue)
            discard;
    }
    
    return Out;
}

PS_OUT PS_OPERSMOKE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;
    
    In.vTexUV.x -= g_fValue;
    vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV * 0.3f);
    
    Out.vColor = vColor;
    Out.vColor.a = vNoise.r;
    
    return Out;
}

PS_OUT PS_GLOWLINE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;
    
    vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor = vColor;
    
    if (vColor.r < 0.01f)
        discard;

    return Out;
}

PS_OUT PS_CIRCLEGAUGE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    
    float2 vPos = In.vTexUV - 0.5f;
    float fAngle = degrees(atan2(vPos.x, vPos.y)) + 180.f;
    
    float fRadius = length(vPos.xy);
    
    float fa = radians(fAngle - g_fValue * 360.f) * fRadius + 1.f;
    
    Out.vColor *= g_vColor;
    Out.vColor.w *= g_fAlpha;
    
    fa = saturate(fa);
    if (fa < 1.f)
        discard;
    
    return Out;
}

PS_OUT PS_SELECTEFFECT(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;
    
    vector vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);    
    vector vNoise = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    
    Out.vColor = vColor;
    
    if(Out.vColor.a<0.1f)
        discard;
    
    Out.vColor.a = vNoise.r;
        
    Out.vColor *= g_vColor;
    
    return Out;
}

PS_OUT PS_UIColor_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w = Out.vColor.r * g_fAlpha;

    if (Out.vColor.w < 0.3f)
        discard;


    return Out;
}

PS_OUT PS_HPBAREFFECT_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

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
    Out.vFlag = g_vFlag;

    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.w *= g_fAlpha;

    if (In.vTexUV.x > 0.7f)
    {
        float fRatio = max(0.f, ((0.9f - In.vTexUV.x) / 0.2f));
        Out.vColor.w *= fRatio;
    }


    return Out;
}

PS_OUT PS_DEBUG(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    Out.vFlag = g_vFlag;
    Out.vColor = 1;

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

    Out.vDepth = vector(In.vProjPos.z / In.vProjPos.w, In.vProjPos.w / 1500.f, 0.f, 0.f);
    Out.vFlag = g_vFlag;
    Out.vGlowFlag = g_vGlowFlag;
    Out.vEffectDiffuse = Out.vDiffuse;

    return Out;
}


PS_EFFECT_OUT PS_TRAIL_UI_MAIN(VS_TRAIL_OUT In)
{
    PS_EFFECT_OUT Out = (PS_EFFECT_OUT) 0;

    In.vTexUV.x += g_fUVPlusX;
    In.vTexUV.x = 1.f - In.vTexUV.x;
    In.vTexUV.x *= g_fUVPower;

    //Mask
    Out.vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, float2(In.vTexUV.y, In.vTexUV.x));
    Out.vDiffuse.a = Out.vDiffuse.r;


    Out.vDiffuse.xyz = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV).xyz;


    Out.vDiffuse *= g_vColor;

    if (Out.vDiffuse.a < 0.02f)
        discard;

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

    if (!g_bDeadBlood)
    {
        Out.vColor.a = Out.vColor.a * g_fAlpha;
    }
    else
    {
        Out.vColor.a = g_fBloodRatio;
    }

    if (Out.vColor.a < 0.01f)
        discard;

    return Out;
}


PS_OUT PS_UIFIRE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    float fStepX = 1.f / g_iWidthSize;
    //갯수만큼 나누고
    In.vTexUV.x /= g_iWidthSize;
    //현재 가로줄만큼 늘려
    In.vTexUV.x += fStepX * g_fRowX;


    float fStepY = 1.f / g_iHeightSize;
    //갯수만큼 나누고
    In.vTexUV.y /= g_iHeightSize;
    //현재 세로줄만큼 늘려
    In.vTexUV.y += fStepY * g_fColY;

    //masking
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
    if (g_bBlackBG)
    {
        Out.vColor.a = Out.vColor.r;
    }
     //알파는 마스크맵 검은곳에다가 기본 칼라까지


    if (Out.vColor.a < 0.01f)
        discard;
   

    //Out.vColor *= g_vColor;
    //Out.vColor.w *= g_fAlpha;

    Out.vFlag = g_vFlag;

    return Out;
}

struct PS_DISTORTION_OUT
{
    vector vDistortionFlag : SV_TARGET0;
};

PS_DISTORTION_OUT PS_MAIN_DISTORTION(VS_TRAIL_OUT In)
{
    PS_DISTORTION_OUT Out = (PS_DISTORTION_OUT) 0;

    //float fTime = sin(g_fTimeAcc);


    In.vTexUV.x = 1.f - In.vTexUV.x;

    //In.vTexUV.y += fTime;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    vector vDiffuse;

    vDiffuse = vMtrlDiffuse;
    vDiffuse.a = vDiffuse.r;
    //vDiffuse.a *= g_fAlpha;

    Out.vDistortionFlag.b = vDiffuse.a;

    if (vDiffuse.a <= 0.05f)
        discard;

    Out.vDistortionFlag.x = vDiffuse.a * 0.5f;
    Out.vDistortionFlag.z = In.vProjPos.w / 1500.f;
    Out.vDistortionFlag.a = 1.f;

    return Out;
}

PS_OUT PS_UVFIRE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    //mask
    vector vMtrlDiffuse = g_NoiseTexture.Sample(DefaultSampler, In.vTexUV);
    Out.vColor.a = vMtrlDiffuse.r;

    In.vTexUV.x += g_fUVPlusX;
    In.vTexUV.y += g_fUVPlusY;

    vector vDiff = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

    Out.vColor.a *= vDiff.r;

    Out.vColor.a *= g_fAlpha;

    if (Out.vColor.a <= 0.01f)
        discard;

    Out.vColor.xyz = vDiff.xyz;
    Out.vColor.xyz += g_vPlusColor.xyz;

    Out.vFlag = g_vFlag;

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

    pass UI_HealthGauge
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_HealthGauge();
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

    pass UI_VerticalGauge
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_VerticalGauge();
    }

    pass UI_HorizontalGauge
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_HorizontalGauge();
    }

    pass UI_OperSmoke
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_OPERSMOKE();
    }

    pass UI_GlowLine
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_GLOWLINE();
    }

    pass UI_CircleGauge
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_CIRCLEGAUGE();
    }

    pass UI_SelectEffect
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_SELECTEFFECT();
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

    pass Debug
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_None);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_DEBUG();
    }

    pass UI_ANIMATION
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_None);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_UIFIRE();
    }

    pass TRAIL_DISTORTION
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_None);

        VertexShader = compile vs_5_0 VS_TRAIL_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DISTORTION();
    }

    pass UVFIRE
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_None);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_UVFIRE();
    }

    pass PROFILE
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_None);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_PROFILE();
    }

    pass TRAIL_UI
    {
        SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_None);

        VertexShader = compile vs_5_0 VS_TRAIL_UI_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_TRAIL_UI_MAIN();
    }
    
}