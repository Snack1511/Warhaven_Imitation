#define SHADOW_ON

matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix		g_ProjMatrixInv, g_ViewMatrixInv, g_LightViewMatrix, g_LightVeiwProjMatrix;


vector		g_vLightDir;

vector		g_vLightPos;
float		g_fRange;

vector		g_vCamPosition;
vector		g_vCamLook;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);


float		g_fFogAlpha = 0.f;

texture2D	g_SkyTexture;
texture2D	g_SpecularTexture;
texture2D	g_DepthTexture;
texture2D	g_DiffuseTexture;
texture2D	g_ShadeTexture;
texture2D	g_NormalTexture;
texture2D	g_Texture;
texture2D	g_FlagTexture;
texture2D	g_OutlineTexture;
texture2D	g_ShadowDepthTexture;
texture2D	g_ShadowTexture;
texture2D	g_BlurTexture;
texture2D	g_EffectFlagTexture;
texture2D	g_GlowTexture;
texture2D	g_EffectDiffuseTexture;
texture2D	g_EffectBlurTexture;
texture2D	g_BloomTexture;
texture2D	g_BloomOriginTexture;
texture2D	g_DistortionTexture;
texture2D	g_FogTexture;
texture2D	g_RimLightTexture;

bool		g_bBilateral;

float		g_fWinCX = 1280.f;
float		g_fWinCY = 720.f;
float		g_fCoord[3] = { -1.f, 0.f, 1.f };


float		g_fDarkScreen = 0.f;

float		g_fLaplacianMask[9] = {
	-1,-1, -1,
	-1, 8, -1,
	-1, -1, -1 };

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

float g_fUVPlusX = 0.f;
float g_fUVPlusY = 0.f;

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

PS_OUT PS_MAIN_DEBUG(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}

struct PS_OUT_LIGHT
{
	vector		vShade : SV_TARGET0;
	vector		vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)1;

	vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);

	//For Diffuse 
	if (vFlagDesc.r > 0.f)
	{
		vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
		vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
		float			fViewZ = vDepthDesc.y * 1500.f;
		/* 0 -> -1, 1 -> 1*/
		vector			vNormal = normalize(vector(vNormalDesc.xyz * 2.f - 1.f, 0.f));

		float		fShade = saturate(saturate(dot(normalize(g_vLightDir) * -1.f, vNormal)));


		Out.vShade = g_vLightDiffuse * fShade + (g_vLightAmbient * g_vMtrlAmbient);

		Out.vShade.a = 1.f;



		//For spec
		if (vFlagDesc.r > 0.99f)
		{
			vector			vReflect = reflect(normalize(g_vLightDir), vNormal);

			vector			vWorldPos;

			vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
			vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
			vWorldPos.z = vDepthDesc.x;
			vWorldPos.w = 1.0f;

			vWorldPos *= fViewZ;

			vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

			vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

			vector			vLook = normalize(vWorldPos - g_vCamPosition);

			Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f);

		}
	}
	Out.vSpecular.a = 0.f;






	return Out;
}

PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
{
	PS_OUT_LIGHT		Out = (PS_OUT_LIGHT)1;
	vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);

	if (vFlagDesc.r > 0.f)
	{

		vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
		vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
		float			fViewZ = vDepthDesc.y * 1500.f;
		vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

		vector			vWorldPos;

		vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
		vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
		vWorldPos.z = vDepthDesc.x;
		vWorldPos.w = 1.0f;
		vWorldPos *= fViewZ;

		vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

		vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

		vector			vLightDir = vWorldPos - g_vLightPos;

		float			fDistance = length(vLightDir);

		float			fAtt = saturate((g_fRange - fDistance) / g_fRange);


		float		fShade = saturate(saturate(dot(normalize(vLightDir) * -1.f, vNormal)));

		Out.vShade = g_vLightDiffuse * fShade * fAtt + (g_vLightAmbient * g_vMtrlAmbient) * fAtt;

		//점조명이구, 최대거리보다 작은 범위내에 있으면
		/*if (g_fRange < 100.f && g_fRange > fDistance)
		{
			Out.vShade.a = 0.95f;
		}
		else*/
		Out.vShade.a = 1.f;

		if (vFlagDesc.r > 0.99f)
		{
			vector			vReflect = reflect(normalize(vLightDir), vNormal);
			vector			vLook = normalize(vWorldPos - g_vCamPosition);

			Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * pow(saturate(dot(normalize(vReflect) * -1.f, vLook)), 30.f) * fAtt;
		}
	}

	//Out.vShade.xyz = 0.f;
	Out.vSpecular.a = 0.f;

	return Out;
}



PS_OUT PS_MAIN_FORWARDBLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)Out;
	vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);


	vector			vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = vDiffuse * vShade;



	//Shadow
#ifdef SHADOW_ON
	vector			vShadowDesc = g_ShadowTexture.Sample(DefaultSampler, In.vTexUV);
	Out.vColor *= vShadowDesc;
#endif

	Out.vColor *= 2.2f;

	//RimLight
	vector			vRimLightDesc = g_RimLightTexture.Sample(DefaultSampler, In.vTexUV);
	//if (vRimLightDesc.a > 0.1f)
	Out.vColor.xyz += vRimLightDesc.xyz;



	vector			vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexUV);
	if (vSpecular.r < 1.f)
	{
		if (dot((vDiffuse.xyz * vShadowDesc.xyz), float3(0.333f, 0.333f, 0.333f)) > 0.5f)
			Out.vColor += vSpecular;
	}



	if (Out.vColor.a <= 0.f)
	{
		vector			vSkyDesc = g_SkyTexture.Sample(DefaultSampler, In.vTexUV);
		Out.vColor = vSkyDesc;
		//Out.vColor.a = 0.1f;
	}
	
	Out.vColor.a = 1.f;

	//

	////white
	//Out.vColor.xyz += vFlagDesc.g;

	

	////Forward Fog
	//In.vTexUV.x += g_fUVPlusX;
	//In.vTexUV.y += g_fUVPlusY;

	//vector			vFogColor = g_FogTexture.Sample(DefaultSampler, In.vTexUV * 0.3f);

	//if (vFogColor.x < 0.2f)
	//	return Out;

	//vFogColor.xyz = float3(1.1f, 0.f, 0.f) * (vFogColor.x);

	//float fDepthRatio = saturate(vDepthDesc.y / 0.01f);

	////0~1을 0.5~1로 바꿔야함
	//fDepthRatio = (fDepthRatio + 1.f) * 0.5f * g_fFogAlpha;
	//Out.vColor.xyz = Out.vColor.xyz * (1.f - fDepthRatio) + vFogColor * fDepthRatio;

	

	return Out;
}

PS_OUT PS_MAIN_BLOOMBLEND(PS_IN In)
{
	//블룸합치기
	PS_OUT			Out = (PS_OUT)0;
	vector			vDefaultDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	//BloomDesc의 알파가 반영되어야함.
	vector			vBloomDesc = g_BloomTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vBloomOriginDesc = g_BloomOriginTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	float fPower = 2.2f;

	float4		vBloom = pow(pow(abs(vBloomDesc), fPower) + pow(abs(vBloomOriginDesc), fPower), 1.f / fPower);

	Out.vColor = vDefaultDesc;


	//DOF
	//if (g_bBilateral)
	{
		/* 멀수록 Ratio가 강하게 */

		if (vDepthDesc.y < 0.9f)
		{
			float		fMaxDepth = 0.15f;

			float fRatio = saturate(vDepthDesc.y / fMaxDepth);


			//멀수록 강해짐

			vector			vBlurDesc = g_BlurTexture.Sample(DefaultSampler, In.vTexUV);

			Out.vColor = Out.vColor * (1.f - fRatio) + vBlurDesc * fRatio;

			//Out.vColor.xyz += (fRatio * 0.3f);
		}

	}




	//if (vBloomOriginDesc.a > 0.f)
	Out.vColor.xyz += vBloom.xyz;

	//Outline

	//a에다가 outline 깊이는 저장할 수 있음
	vector			vOutlineDesc = g_OutlineTexture.Sample(DefaultSampler, In.vTexUV);

	//1. OutLine 그려야할 때
	if (vOutlineDesc.y > 0.f)
	{
		//if (vOutlineDesc.a + 0.1f < vDepthDesc.y)
			Out.vColor.xyz = Out.vColor.xyz * 0.5f + vOutlineDesc * 0.5f;
	}




	


	return Out;
}

PS_OUT PS_MAIN_FINALBLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	//vector			vFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	vector			vForwardDesc = g_Texture.Sample(DefaultSampler, In.vTexUV);
	vector			vEffectDiffuseDesc = g_EffectDiffuseTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vEffectBlurDesc = g_EffectBlurTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vEffectFlagDesc = g_EffectFlagTexture.Sample(DefaultSampler, In.vTexUV);

	In.vTexUV.x += g_fUVPlusX;
	In.vTexUV.y += g_fUVPlusY;
	//vector			vFogColor = g_FogTexture.Sample(DefaultSampler, In.vTexUV * 0.3f);

	In.vTexUV.x -= g_fUVPlusX;
	In.vTexUV.y -= g_fUVPlusY;

	bool bFog = false;

	Out.vColor = vForwardDesc;

	if (vEffectDiffuseDesc.a > 0.f)
		bFog = true;


	


	//Blur
	if (vEffectFlagDesc.a > 0.9f && vEffectFlagDesc.a < 0.99f)
	{
		Out.vColor = vEffectBlurDesc;
		bFog = true;
	}


	//Glow
	


	vector			vGlowDesc = g_GlowTexture.Sample(DefaultSampler, In.vTexUV);
	vGlowDesc.a *= 2.f;
	saturate(vGlowDesc.a);
	vGlowDesc.a = sqrt(vGlowDesc.a);

	//멀면 글로우 약하게
	/*if (vDepthDesc.y > 0.05f)
	{
		vGlowDesc.a *= saturate( (0.2f - vDepthDesc.y) / 0.2f);
	}*/

	if (vGlowDesc.a > 0.01f)
	{
		//이펙트 안그려진 부분만 섞기
		if (vEffectFlagDesc.r <= 0.1f)
		{
			//안개켜져있고, 그리는곳 depth가 안개범위 밖이면
			if (vDepthDesc.y > 0.01f)
			{
				
			}
			else
			{
				//차후 깊이 비교까지 추가
				Out.vColor.xyz = Out.vColor.xyz * (1.f - vGlowDesc.a) + vGlowDesc.xyz * (vGlowDesc.a);
			}

		}
		else
		{
			//이펙트 그려진 부분엔 이펙트의 알파 기준으로
			Out.vColor.xyz = Out.vColor.xyz * (vEffectFlagDesc.a) + vGlowDesc.xyz * (1.f - vEffectFlagDesc.a);
		}
		bFog = true;
	}

	//Bloom

	//BloomDesc의 알파가 반영되어야함.
	vector			vBloomDesc = g_BloomTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vBloomOriginDesc = g_BloomOriginTexture.Sample(DefaultSampler, In.vTexUV);

	if (vBloomDesc.a > 0.f)
		bFog = true;

	float4		vBloom = pow(pow(abs(vBloomDesc), 2.2f) + pow(abs(vBloomOriginDesc), 2.2f), 1.f / 2.2f);
	//Out.vColor.xyz += vBloom.xyz;


	Out.vColor.xyz += vBloom.xyz * ((vBloom.a));


	if (Out.vColor.a <= 0.f)
		discard;

	//이펙트나 OutLine 그려진 곳만 안개 추가로 씌우기


	//if (bFog == true)
	//{

	//	if (vFogColor.x < 0.2f)
	//		return Out;

	//	vFogColor.xyz = float3(1.1f, 0.f, 0.f) * (vFogColor.x);

	//	float fDepthRatio = saturate(vDepthDesc.y / 0.01f);


	//	//0~1을 0.5~1로 바꿔야함
	//	fDepthRatio = (fDepthRatio + 1.f) * 0.5f * g_fFogAlpha;

	//	if (fDepthRatio >= 1.f)
	//		Out.vColor.xyz = vFogColor;
	//	else
	//	{
	//		fDepthRatio *= vEffectDiffuseDesc.a;
	//		Out.vColor.xyz = Out.vColor.xyz * (1.f - fDepthRatio) + vFogColor * fDepthRatio;

	//	}
	//}

	



	return Out;
}

float normpdf(in float x, in float sigma)
{
	return 0.39894 * exp(-0.5 * x * x / (sigma * sigma)) / sigma;
}

float normpdf3(in float3 v, in float sigma)
{
	return 0.39894 * exp(-0.5 * dot(v, v) / (sigma * sigma)) / sigma;
}

#define SIGMA 10.0
#define BSIGMA 0.1
#define MSIZE 15

PS_OUT PS_MAIN_POSTEFFECT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)Out;

	vector			vDistortionDesc = g_DistortionTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	//vector			vFogColor = g_FogTexture.Sample(DefaultSampler, In.vTexUV);

	//if (vDistortionDesc.x > 0.f)
	{
		//vector		vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
		//depth에 비례하자
		//depth.w는 0~1사이구 가까울수록 값이 작다?
		//vDepthDesc.y = saturate(vDepthDesc.y / 0.1f);

		//depth 비교
		if (vDistortionDesc.z < vDepthDesc.y)
			In.vTexUV += vDistortionDesc.r * 0.03f;
	}


	Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexUV);



	
		 
	if (g_bBilateral)
	{
		/*const float A = 2.51, B = 0.03, C = 2.43, D = 0.59, E = 0.14;
		Out.vColor = saturate((Out.vColor * (A * Out.vColor + B)) / (Out.vColor * (C * Out.vColor + D) + E));
		Out.vColor.w = 1;*/
	}






	//if (g_bBilateral)
	//{
	//	float3 c = g_Texture.Sample(DefaultSampler, In.vTexUV).xyz;

	//	//declare stuff
	//	const int kSize = (MSIZE - 1) / 2;
	//	float kernel[MSIZE];
	//	float3 final_colour = 0;

	//	//create the 1-D kernel
	//	float Z = 0.0;
	//	for (int j = 0; j <= kSize; ++j)
	//	{
	//		kernel[kSize + j] = kernel[kSize - j] = normpdf(float(j), SIGMA);
	//	}


	//	float3 cc;
	//	float factor;
	//	float bZ = 1.0 / normpdf(0.0, BSIGMA);

	//	float2 fResolution = float2(1280.f, 720.f);
	//	//read out the texels
	//	for (int i = -kSize; i <= kSize; ++i)
	//	{
	//		for (int j = -kSize; j <= kSize; ++j)
	//		{

	//			float2 vPlusUV = float2 (i, j) / fResolution.xy;
	//			cc = g_Texture.Sample(ShadowSampler, (In.vTexUV + vPlusUV)).xyz;

	//			factor = normpdf3(cc - c, BSIGMA) * bZ * kernel[kSize + j] * kernel[kSize + i];
	//			Z += factor;
	//			final_colour += factor * cc;

	//		}
	//	}


	//	Out.vColor = float4(final_colour / Z, 1.0);
	//}












	if (Out.vColor.a <= 0.f)
		discard;

	return Out;
}


PS_OUT PS_MAIN_UIBLEND(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector			vDefaultDesc = g_Texture.Sample(DefaultSampler, In.vTexUV);

	vector			vUIDesc = g_DiffuseTexture.Sample(DefaultSampler, In.vTexUV);

	vector			vBloomDesc = g_BloomTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vBloomOriginDesc = g_BloomOriginTexture.Sample(DefaultSampler, In.vTexUV);

	float fPower = 2.2f;

	float4		vBloom = pow(pow(abs(vBloomDesc), fPower) + pow(abs(vBloomOriginDesc), fPower), 1.f / fPower);

	//UI추가하고
	Out.vColor.xyz = vUIDesc.xyz * vUIDesc.a + (vDefaultDesc * (1.f - vUIDesc.a));
	//UI 블룸 추가
	Out.vColor.xyz += vBloom.xyz;


	/* 어둡게 하기 */
	Out.vColor.xyz *= saturate(1.f - g_fDarkScreen);



	Out.vColor.a = 1;


	return Out;
}

PS_OUT PS_MAIN_OUTLINE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float4 vFinalColor = 0;

	vector DepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);
	//vector NormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);

	bool bOutline = false;

	for (unsigned int i = 0; i < 9; ++i)
	{
		float2 OtherTexUV = In.vTexUV + float2(g_fCoord[i % 3] / g_fWinCX, g_fCoord[i / 3] / g_fWinCY);

		vector	vOutLineFlagDesc = g_FlagTexture.Sample(DefaultSampler, OtherTexUV);

		//OutLineFlag 조건 : 내 옆에있는 녀석이 OutLine을 요구하면
		if (vOutLineFlagDesc.a > 0.f)
		{
			//vector OtherNormalDesc = g_NormalTexture.Sample(DefaultSampler, OtherTexUV);
			vector OtherDepthDesc = g_DepthTexture.Sample(DefaultSampler, OtherTexUV);


			//Add Normal and Depth
			float fDepthSub = length(OtherDepthDesc - DepthDesc);
			//float fNormalSub = abs(dot(normalize(OtherNormalDesc), normalize(NormalDesc)));

				//Depth
			//내 깊이랑 옆에있는 깊이가 차이가 좀 나면
				if (fDepthSub > 0.0005f)
				{
					vFinalColor = vOutLineFlagDesc;
					bOutline = true;
					Out.vColor.a = OtherDepthDesc.y;

					break;
				}
				/*if (fNormalSub <= 0.5f)
				{
					vFinalColor = vOutLineFlagDesc;
					bOutline = true;
					Out.vColor.a = OtherDepthDesc.y;

					break;

				}*/


			
		}
	}


	if (bOutline)
		Out.vColor.xyz = vFinalColor.xyz;

	return Out;
}

PS_OUT PS_MAIN_RIMLIGHT(PS_IN In)
{
	//림라이트 필요한거
	//1. 림라이트 플래그 확인
	PS_OUT		Out = (PS_OUT)0;
	vector			vRimLightFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);

	if (vRimLightFlagDesc.a <= 0.001f)
		return Out;

	//2. viewdir : 버텍스에서 바라보는 카메라의 방향
	
	//버텍스의 world pos를 구해야한다
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

	vector vViewNormal = (g_vCamPosition - vWorldPos);

	//2. Normal과 ViewDir을 dot연산
	vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexUV);
	vector			vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

	//float fRim = 1 - saturate(dot(vViewNormal, vNormal));

	float		fShade = 1.f - saturate(saturate(dot(normalize(vViewNormal.xyz), normalize(vNormal.xyz))));

	//노말 벡터가 카메라로 향하면 1
	//노말 벡터가 카메라를 안보면 0
	/*if (fShade > 0.5f)
		return Out;*/

	//int iRimPower = (int)(vRimLightFlagDesc.a * 10.f);
	
	float fRimPower = vRimLightFlagDesc.a * 10.f;
	fShade = pow(fShade, fRimPower);

	Out.vColor.xyz = (fShade * vRimLightFlagDesc.xyz);


	Out.vColor.a = fShade;

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

BlendState BS_OriginAlphaBlending
{
	BlendEnable[0] = true;

	SrcBlend = src_alpha;
	DestBlend = inv_src_alpha;
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
	pass Debug
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
	}

	pass Light_Directional
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
	}

	pass Light_Point
	{
		SetBlendState(BS_AlphaBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
	}

	pass ForwardBlend
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FORWARDBLEND();
	}

	pass Outline
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_OUTLINE();
	}

	pass FinalBlend
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FINALBLEND();
	}

	pass POSTEFFECT
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_POSTEFFECT();
	}

	pass BLOOMBLEND
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_BLOOMBLEND();
	}

	pass UIBLEND
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_UIBLEND();
	}

	pass RIMLIGHT
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_RIMLIGHT();
	}

}