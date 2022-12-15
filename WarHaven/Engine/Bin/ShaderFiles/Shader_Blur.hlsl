
matrix		g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
matrix		g_ProjMatrixInv, g_ViewMatrixInv;
matrix		g_OldProjMatrix, g_OldViewMatrix;
matrix		g_CurProjMatrix, g_CurViewMatrix;

int		g_numSamples = 5;

vector		g_vLightDir;

vector		g_vLightPos;
float		g_fRange; 
float		g_fToonStep = 1.f;

vector		g_vCamPosition;

vector		g_vLightDiffuse;
vector		g_vLightAmbient;
vector		g_vLightSpecular;

vector		g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector		g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);


texture2D	g_ShaderTexture;
texture2D	g_FlagTexture;
texture2D	g_DepthTexture;


float		g_fWinCX = 1280.f;
float		g_fWinCY = 720.f;

float		g_fTimeDelta = 0.f;

float		g_fShaderPower = 0.f;

bool		g_bMotionBlur = false;


sampler DefaultSampler = sampler_state
{
	filter = min_mag_mip_linear;
};

sampler MotionBlurSampler = sampler_state
{
	filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = clamp;
	AddressV = clamp;
};

struct VS_IN
{
	float3		position : POSITION;
	float2		tex : TEXCOORD0;
};

struct VS_DOWNSCALE_OUT
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

VS_DOWNSCALE_OUT VS_DOWNSCALE_MAIN(VS_IN In)
{
	VS_DOWNSCALE_OUT		Out = (VS_DOWNSCALE_OUT)0;

	matrix			matWV, matWVP;

	matWV = mul(g_WorldMatrix, g_ViewMatrix);
	matWVP = mul(matWV, g_ProjMatrix);

	Out.vPosition = mul(vector(In.position, 1.f), matWVP);
	Out.vTexUV = In.tex;

	return Out;
}


struct PS_DOWNSCALE_IN
{
	float4		vPosition : SV_POSITION;
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : SV_TARGET0;
};

PS_OUT PS_DOWNSCALE_MAIN(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_ShaderTexture.Sample(DefaultSampler, In.vTexUV);

	return Out;
}

PS_OUT	PS_RADIALBLUR_MAIN(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	//Out.vColor = 

	float2 center = float2(0.5f, 0.5f);

	int iMaxNumSamples = 6;
	int iNumSamples = 5;
	float2 vDirection = In.vTexUV - center;
	/*float fMaxLength = 0.35f;

	

	float fLength = length(vDirection);

	if (fLength < fMaxLength)
		iNumSamples = 1;*/

	//max까지는 1로가고
	//max부터 0.707 까지는 점차 늘으나게

	/*if (fLength < fMaxLength)
		iNumSamples = 1;
	else
	{
		float fRatio = (fLength - fMaxLength) /( 0.707f - fMaxLength);
		iNumSamples = max(min(iMaxNumSamples * fRatio, 6), 1);
	}*/

	float2 precompute = g_fShaderPower * vDirection;

	for (int i = 0; i < iNumSamples; ++i)
	{
		Out.vColor += g_ShaderTexture.Sample(MotionBlurSampler, In.vTexUV - (precompute * float(i)));
	}

	Out.vColor /= float(iNumSamples);
	return Out;

}

PS_OUT	PS_CHROMATIC_ABERRATION_MAIN(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	float fPower = g_fShaderPower;
	float2 vTexel = 1.f / float2(g_fWinCX, g_fWinCY);

	float2 vCoords = (In.vTexUV - 0.5f) * 2.f;
	float fCoordDot = dot(vCoords, vCoords);

	float2 precompute = fPower * fCoordDot * vCoords;
	float2 uvR = In.vTexUV - vTexel * precompute;
	float2 uvB = In.vTexUV + vTexel * precompute;

	Out.vColor.r = g_ShaderTexture.Sample(MotionBlurSampler, uvR).r;
	Out.vColor.g = g_ShaderTexture.Sample(MotionBlurSampler, In.vTexUV).g;
	Out.vColor.b = g_ShaderTexture.Sample(MotionBlurSampler, uvB).b;

	return Out;

}

PS_OUT	PS_GRAYSCALE_MAIN(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_ShaderTexture.Sample(MotionBlurSampler, In.vTexUV);

	vector GrayScaleColor = dot(Out.vColor, float3(0.2126f, 0.7152f, 0.0722f));
	//float fShaderPower = g_fShaderPower;

	

	Out.vColor = Out.vColor * (1.f - g_fShaderPower) + GrayScaleColor * g_fShaderPower;

	return Out;
}

PS_OUT PS_MOTIONBLUR_MAIN(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	if (!g_bMotionBlur)
	{
		Out.vColor = g_ShaderTexture.Sample(MotionBlurSampler, In.vTexUV);
		return Out;
	}

	//현재 뷰포트 포지션과
	//같은 월드포지션에 이전 VP매트릭스를 곱해서 이전 뷰포트 위치얻기
	vector vCurrentPos, vPrevPos;


	//현재 위치 담은 Depth와
	//이전 위치 담은 Depth 가져오기

	vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	Out.vColor = g_ShaderTexture.Sample(MotionBlurSampler, In.vTexUV);

	int iNumSamples = 6;

	if (vDepthDesc.y >= 0.95f)
	{
		iNumSamples = 1;
	}

	//현재 Depth를 통해 월드위치 구하고
	vector			vWorldPos;

	vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
	vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	//-1 ~ 1의 투영좌표 (w 나누기 수행 완료)
	//여기는 월드아님
	vCurrentPos = vWorldPos;

	float			fViewZ = vDepthDesc.y * 1500.f;
	//이게 w를 다시 곱하는거임
	vWorldPos = vWorldPos* fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	//world pos에다가 뷰투영 곱해서 이전 투영좌표
	vPrevPos = mul(vWorldPos, g_OldViewMatrix);
	vPrevPos = mul(vPrevPos, g_OldProjMatrix);

	//이전 프레임의 -1 ~ 1의 투영좌표? ㅇㅇ w 나누기 수행 완료
	vPrevPos /= vPrevPos.w;

	//방향이랑 힘 구하기
	//이거 투영좌표계에서 투영좌표계 뺸거야 그럼이거 값이 -0.5~0.5아님? 1은나올수가없어 맞지
	float2 velocity = (vCurrentPos.xy - vPrevPos.xy) * 0.5f;

	velocity /= (iNumSamples);

	//타임델타가 높다 = 프레임이 낮다 = velocity가 낮아져야함
	velocity /= g_fTimeDelta;
	velocity *= 0.015f;
	velocity *= max(min(vDepthDesc.y / 0.01f, 1.f), 0.5f);

	for (int i = 1; i < iNumSamples; ++i)
	{
		In.vTexUV += velocity;
		//velocity *= 0.5f;
		// 속도 벡터를 따라 컬러 버퍼를 샘플링합니다.
		vector vCurrentDiffuse = g_ShaderTexture.Sample(MotionBlurSampler, In.vTexUV);
		// 현재 색상을 색상 합계에 추가합니다.
		Out.vColor += vCurrentDiffuse;
	} 
	// 최종 블러 색상을 얻기 위해 모든 샘플의 평균을 냅니다.

	Out.vColor /= float(iNumSamples);




	return Out;
}


PS_OUT PS_BLOOM_MAIN(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);

	if (vEffectFlagDesc.g < 0.1f)
		discard;

	if (vEffectFlagDesc.g > 0.9f)
	{
		Out.vColor = g_ShaderTexture.Sample(DefaultSampler, In.vTexUV);

		float fBrightness = dot(Out.vColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
		//float fBrightness = Out.vColor.r * Out.vColor.g * Out.vColor.b;

		if (fBrightness > 0.99f)
		{
			Out.vColor = float4(Out.vColor.xyz, fBrightness);
		}
		else
			discard;
	}
	else
	{
		Out.vColor = g_ShaderTexture.Sample(DefaultSampler, In.vTexUV);

		float fBrightness = dot(Out.vColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
		//float fBrightness = Out.vColor.r * Out.vColor.g * Out.vColor.b;

		Out.vColor = float4(Out.vColor.xyz, Out.vColor.a);
	}

	

	return Out;
}

PS_OUT PS_BLOOMHARD_MAIN(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector vEffectFlagDesc = g_FlagTexture.Sample(DefaultSampler, In.vTexUV);

	if (vEffectFlagDesc.g < 0.99f)
		discard;

	Out.vColor = g_ShaderTexture.Sample(DefaultSampler, In.vTexUV);

	float fBrightness = dot(Out.vColor.rgb, float3(0.2126f, 0.7152f, 0.0722f));
	//float fBrightness = Out.vColor.r * Out.vColor.g * Out.vColor.b;

	Out.vColor = float4(Out.vColor.xyz, Out.vColor.a);


	return Out;
}


struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};

VS_OUT VS_VERTICAL_MAIN(VS_IN input)
{
	VS_OUT		output = (VS_OUT)0;

	matrix			matWV, matWVP;

	float texelSize;


	// Change the position vector to be 4 units for proper matrix calculations.

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(vector(input.position, 1.f), g_WorldMatrix);
	output.position = mul(output.position, g_ViewMatrix);
	output.position = mul(output.position, g_ProjMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / g_fWinCY;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.tex + float2(0.0f, texelSize * -4.0f);
	output.texCoord2 = input.tex + float2(0.0f, texelSize * -3.0f);
	output.texCoord3 = input.tex + float2(0.0f, texelSize * -2.0f);
	output.texCoord4 = input.tex + float2(0.0f, texelSize * -1.0f);
	output.texCoord5 = input.tex + float2(0.0f, texelSize * 0.0f);
	output.texCoord6 = input.tex + float2(0.0f, texelSize * 1.0f);
	output.texCoord7 = input.tex + float2(0.0f, texelSize * 2.0f);
	output.texCoord8 = input.tex + float2(0.0f, texelSize * 3.0f);
	output.texCoord9 = input.tex + float2(0.0f, texelSize * 4.0f);

	return output;
}

VS_OUT VS_HORIZONTAL_MAIN(VS_IN input)
{
	VS_OUT		output = (VS_OUT)0;

	matrix			matWV, matWVP;

	float texelSize;


	// Change the position vector to be 4 units for proper matrix calculations.

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(vector(input.position, 1.f), g_WorldMatrix);
	output.position = mul(output.position, g_ViewMatrix);
	output.position = mul(output.position, g_ProjMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// Determine the floating point size of a texel for a screen with this specific height.
	texelSize = 1.0f / g_fWinCX;

	// Create UV coordinates for the pixel and its four vertical neighbors on either side.
	output.texCoord1 = input.tex + float2(texelSize * -4.0f, 0.0f);
	output.texCoord2 = input.tex + float2(texelSize * -3.0f, 0.0f);
	output.texCoord3 = input.tex + float2(texelSize * -2.0f, 0.0f);
	output.texCoord4 = input.tex + float2(texelSize * -1.0f, 0.0f);
	output.texCoord5 = input.tex + float2(texelSize * 0.0f, 0.0f);
	output.texCoord6 = input.tex + float2(texelSize * 1.0f, 0.0f);
	output.texCoord7 = input.tex + float2(texelSize * 2.0f, 0.0f);
	output.texCoord8 = input.tex + float2(texelSize * 3.0f, 0.0f);
	output.texCoord9 = input.tex + float2(texelSize * 4.0f, 0.0f);

	return output;
}

struct PS_IN
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float2 texCoord1 : TEXCOORD1;
	float2 texCoord2 : TEXCOORD2;
	float2 texCoord3 : TEXCOORD3;
	float2 texCoord4 : TEXCOORD4;
	float2 texCoord5 : TEXCOORD5;
	float2 texCoord6 : TEXCOORD6;
	float2 texCoord7 : TEXCOORD7;
	float2 texCoord8 : TEXCOORD8;
	float2 texCoord9 : TEXCOORD9;
};

PS_OUT	PS_VERTICAL_MAIN(PS_IN input)
{
	PS_OUT	Output = (PS_OUT)1;

	float weight0, weight1, weight2, weight3, weight4;
	float normalization;


	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	Output.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine vertical pixels to the color by the specific weight of each.
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord1) * weight4;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord2) * weight3;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord3) * weight2;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord4) * weight1;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord5) * weight0;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord6) * weight1;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord7) * weight2;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord8) * weight3;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord9) * weight4;

	//옆 픽셀의 이펙트 플래그 조사, 내 픽셀의 이펙트 플래그 조사.

	// Set the alpha channel to one.
	//Output.vColor.a = 1.0f;

	return Output;
}



PS_OUT	PS_HORIZONTAL_MAIN(PS_IN input)
{
	PS_OUT	Output = (PS_OUT)1;

	float weight0, weight1, weight2, weight3, weight4;
	float normalization;


	// Create the weights that each neighbor pixel will contribute to the blur.
	weight0 = 1.0f;
	weight1 = 0.9f;
	weight2 = 0.55f;
	weight3 = 0.18f;
	weight4 = 0.1f;

	// Create a normalized value to average the weights out a bit.
	normalization = (weight0 + 2.0f * (weight1 + weight2 + weight3 + weight4));

	// Normalize the weights.
	weight0 = weight0 / normalization;
	weight1 = weight1 / normalization;
	weight2 = weight2 / normalization;
	weight3 = weight3 / normalization;
	weight4 = weight4 / normalization;

	// Initialize the color to black.
	Output.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// Add the nine vertical pixels to the color by the specific weight of each.
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord1) * weight4;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord2) * weight3;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord3) * weight2;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord4) * weight1;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord5) * weight0;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord6) * weight1;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord7) * weight2;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord8) * weight3;
	Output.vColor += g_ShaderTexture.Sample(DefaultSampler, input.texCoord9) * weight4;

	// Set the alpha channel to one.
	//Output.vColor.a = 1.0f;
	return Output;
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
	pass DOWNSCALE
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_DOWNSCALE_MAIN();
	}

	pass VERTICAL
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_VERTICAL_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_VERTICAL_MAIN();
	}

	pass HORIZONTAL
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_HORIZONTAL_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_VERTICAL_MAIN();
	}

	pass BLOOM
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLOOM_MAIN();
	}

	pass BLOOMHARD
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_BLOOMHARD_MAIN();
	}

	pass MOTIONBLUR
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_MOTIONBLUR_MAIN();
	}

	pass RADIALBLUR
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_RADIALBLUR_MAIN();
	}

	pass CHROMATIC_ABERRATION
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_CHROMATIC_ABERRATION_MAIN();
	}

	pass GRAYSCALE
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;
		PixelShader = compile ps_5_0 PS_GRAYSCALE_MAIN();
	}

}