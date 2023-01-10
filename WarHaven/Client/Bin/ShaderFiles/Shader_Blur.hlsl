
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
texture2D	g_NormalTexture;
texture2D	g_NoiseTexture;


float		g_fWinCX = 1280.f;
float		g_fWinCY = 720.f;

float		g_fTimeDelta = 0.f;

float		g_fShaderPower = 0.f;

bool		g_bMotionBlur = false;
bool		g_bSSAO = false;

float2		g_vSunPos;

float2		g_vResolution = float2(1280.f, 720.f);


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

	vector GrayScaleColor = dot(Out.vColor.xyz, float3(0.2126f, 0.7152f, 0.0722f));
	//float fShaderPower = g_fShaderPower;

	

	Out.vColor = Out.vColor * (1.f - g_fShaderPower) + GrayScaleColor * g_fShaderPower;

	return Out;
}

float rnd(float2 p)
{
	float f = frac(sin(dot(p, float2(12.1234, 72.8392)) * 45123.2));
	return f;
}

float rnd(float w)
{
	float f = frac(sin(w) * 1000.);
	return f;
}

float regShape(float2 p, int N)
{
	float f;


	float a = atan2(p.x, p.y) + .2;
	float b = 6.28319 / float(N);
	f = smoothstep(.5, .51, cos(floor(.5 + a / b) * b - a) * length(p.xy));


	return f;
}
float3 circle(float2 p, float size, float decay, float3 color, float3 color2, float dist, float2 mouse)
{
	//l is used for making rings.I get the length and pass it through a sinwave
	//but I also use a pow function. pow function + sin function , from 0 and up, = a pulse, at least
	//if you return the max of that and 0.0.

	float l = length(p + mouse * (dist * 4.)) + size / 2.;

	//l2 is used in the rings as well...somehow...
	float l2 = length(p + mouse * (dist * 4.)) + size / 3.;

	///these are circles, big, rings, and  tiny respectively
	float c = max(00.01 - pow(length(p + mouse * dist), size * 1.4), 0.0) * 50.;
	float c1 = max(0.001 - pow(l - 0.3, 1. / 40.) + sin(l * 30.), 0.0) * 3.;
	float c2 = max(0.04 / pow(length(p - mouse * dist / 2. + 0.09) * 1., 1.), 0.0) / 20.;
	float s = max(00.01 - pow(regShape(p * 5. + mouse * dist * 5. + 0.9, 6), 1.), 0.0) * 5.;

	color = 0.5 + 0.5 * sin(color);
	color = cos(float3(0.44, .24, .2) * 8. + dist * 4.) * 0.5 + .5;
	float3 f = c * color;
	f += c1 * color;

	f += c2 * color;
	f += s * color;
	return f - 0.01;
}

float sun(float2 p, float2 mouse)
{
	float f;

	float2 sunp = p + mouse;
	float sun = 1.0 - length(sunp) * 8.;
	return f;
}


float noise(float t)
{
	return g_NoiseTexture.Sample(DefaultSampler, float2(t, .0) / g_vResolution).x;
}
float noise(float2 t)
{
	return g_NoiseTexture.Sample(DefaultSampler, t / g_vResolution).x;
}

float3 lensflare(float2 uv, float2 pos)
{
	float2 main = uv - pos;
	float2 uvd = uv * (length(uv));

	float ang = atan2(main.x, main.y);
	float dist = length(main); dist = pow(dist, .1);
	float n = noise(float2(ang * 16.0, dist * 32.0));

	float f0 = 1.0 / (length(uv - pos) * 16.0 + 1.0);

	f0 = f0 + f0 * (sin(noise(sin(ang * 2. + pos.x) * 4.0 - cos(ang * 3. + pos.y)) * 16.) * .1 + dist * .1 + .8);

	float f1 = max(0.01 - pow(length(uv + 1.2 * pos), 1.9), .0) * 7.0;

	float f2 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.8 * pos), 2.0)), .0) * 00.25;
	float f22 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.85 * pos), 2.0)), .0) * 00.23;
	float f23 = max(1.0 / (1.0 + 32.0 * pow(length(uvd + 0.9 * pos), 2.0)), .0) * 00.21;

	float2 uvx = lerp(uv, uvd, -0.5);

	float f4 = max(0.01 - pow(length(uvx + 0.4 * pos), 2.4), .0) * 6.0;
	float f42 = max(0.01 - pow(length(uvx + 0.45 * pos), 2.4), .0) * 5.0;
	float f43 = max(0.01 - pow(length(uvx + 0.5 * pos), 2.4), .0) * 3.0;

	uvx = lerp(uv, uvd, -.4);

	float f5 = max(0.01 - pow(length(uvx + 0.2 * pos), 5.5), .0) * 2.0;
	float f52 = max(0.01 - pow(length(uvx + 0.4 * pos), 5.5), .0) * 2.0;
	float f53 = max(0.01 - pow(length(uvx + 0.6 * pos), 5.5), .0) * 2.0;

	uvx = lerp(uv, uvd, -0.5);

	float f6 = max(0.01 - pow(length(uvx - 0.3 * pos), 1.6), .0) * 6.0;
	float f62 = max(0.01 - pow(length(uvx - 0.325 * pos), 1.6), .0) * 3.0;
	float f63 = max(0.01 - pow(length(uvx - 0.35 * pos), 1.6), .0) * 5.0;

	float3 c = 0;

	c.r += f2 + f4 + f5 + f6; c.g += f22 + f42 + f52 + f62; c.b += f23 + f43 + f53 + f63;
	c = c * 1.3 - length(uvd) * .05;
	c += f0;

	return c;
}

float3 cc(float3 color, float factor, float factor2) // color modifier
{
	float w = color.x + color.y + color.z;
	return lerp(color, w * factor, w * factor2);
}

PS_OUT PS_MAIN_LENSFLARE(PS_DOWNSCALE_IN In)
{
	float2 vResolution = float2(1280.f, 720.f);

	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = g_ShaderTexture.Sample(DefaultSampler, In.vTexUV);

	//0~1 값을
	//-0.5 ~ 0.5 값으로 만든건가?
	float2 uv = In.vTexUV;

	//1. 일단 y좌표계 반전
	uv.y = 1.f - uv.y;

	//2. 변환
	uv -= 0.5f;

	uv.x *= vResolution.x / vResolution.y; // aspect

	// 현재 태양의 투영 좌표 (해상도 포함)
	// mm = -1 ~ 1 사이의 태양 위치 (오른쪽 위가 1, 1)
	float2 mm = g_vSunPos;


	mm *= 0.5f;
	mm.x *= vResolution.x / vResolution.y; // aspect

	float fLength = length(uv - mm);


	/*float fTemp = mm.x;
	mm.x = mm.y;
	mm.y = fTemp;*/


	//내 0,0 = 왼쪽위
	//쉐이더 토이의 0,0 = 중앙

	float3 circColor = float3(0.9, 0.2, 0.1);
	float3 circColor2 = float3(0.3, 0.1, 0.9);

	//now to make the sky not black
	float3 color = 0;//lerp(float3(0.3, 0.2, 0.02) / 0.9, float3(0.2, 0.5, 0.8), uv.y) * 3. - 0.52;//*sin(iTime);

	//this calls the function which adds three circle types every time through the loop based on parameters I
	//got by trying things out. rnd i*2000. and rnd i*20 are just to help randomize things more
	for (float i = 0.; i < 10.; i++) {
		color += circle(uv, pow(rnd(i * 2000.) * 1.8, 2.) + 1.41, 0.0, circColor + i, circColor2 + i, rnd(i * 20.) * 3. + 0.2 - .5, mm);
	}
	//get angle and length of the sun (uv - mouse)
	float a = atan2(uv.x - mm.x,uv.y - mm.y);
	float l = max(1.0 - fLength - 0.84, 0.0);

	float bright = 0.1;//+0.1/abs(sin(iTime/3.))/3.;//add brightness based on how the sun moves so that it is brightest
	//when it is lined up with the center

	//add the sun with the frill things
	//color += max(0.1 / pow(length(uv - mm) * 5., 5.), 0.0) * abs(sin(a * 5. + cos(a * 9.))) / 20.;
	color += max(0.1 / pow(fLength * 10., 1. / 20.), .0) + abs(sin(a * 3. + cos(a * 9.))) / 8. * (abs(sin(a * 9.))) / 1.;
	//add another sun in the middle (to make it brighter)  with the20color I want, and bright as the numerator.
	color += (max(bright / pow(fLength * 4., 1. / 2.), 0.0) * 4.) * float3(0.2, 0.21, 0.3) * 4.;
	// * (0.5+.5*sin(float3(0.4, 0.2, 0.1) + float3(a*2., 00., a*3.)+1.3));

 //multiply by the exponetial e^x ? of 1.0-length which kind of masks the brightness more so that
 //there is a sharper roll of of the light decay from the sun. 
	color *= exp(1.0 - fLength) / 5.;




	/*float3 color = float3(1.4, 1.2, 1.0) * lensflare(uv, mm);
	color = cc(color, .5, .1);*/

	Out.vColor.xyz += color;

	return Out;
}

#define SAMPLES 16
#define INTENSITY 1.
#define SCALE 2.5
#define BIAS 0.05
#define SAMPLE_RAD 0.02
#define MAX_DISTANCE 0.07

#define MOD3 float3(.1031,.11369,.13787)

float hash12(float2 p)
{
	float3 p3 = frac(p.xyx * MOD3);
	p3 += dot(p3, p3.yzx + 19.19);
	return frac((p3.x + p3.y) * p3.z);
}

float2 hash22(float2 p)
{
	float3 p3 = frac(p.xyx * MOD3);
	p3 += dot(p3, p3.yzx + 19.19);
	return frac(float2((p3.x + p3.y) * p3.z, (p3.x + p3.z) * p3.y));
}

float3 getPosition(float2 uv) {

	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, uv);

	//Shadow
	vector			vWorldPos;

	vWorldPos.x = uv.x * 2.f - 1.f;
	vWorldPos.y = uv.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	float			fViewZ = vDepthDesc.y * 1500.f;
	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
	return vWorldPos.xyz;

	/*float fl = g_DepthTexture.Sample(DefaultSampler, float2(0.f, 0.f)).x;
	float d = g_DepthTexture.Sample(DefaultSampler, uv).y;

	float2 p = uv * 2. - 1.;
	float3x3 ca = float3x3(1., 0., 0., 0., 1., 0., 0., 0., -1. / 1.5);
	ca = transpose(ca);
	float3 vTemp = mul(float3(p.x, p.y, fl), ca);
	float3 rd = normalize(vTemp);

	float3 pos = rd * d;

	return pos;*/

}

float3 getNormal(float2 uv) {
	vector			vNormalDesc = g_NormalTexture.Sample(DefaultSampler, uv);
	return (vNormalDesc.xyz * 2.f - 1.f);
}

float2 getRandom(float2 uv) {
	return normalize(hash22(uv * 126.1231) * 2. - 1.);
}


float doAmbientOcclusion(in float2 tcoord, in float2 uv, in float3 p, in float3 cnorm)
{
	float3 diff = getPosition(tcoord + uv) - p;
	float l = length(diff);
	float3 v = diff / l;
	float d = l * SCALE;
	float ao = max(0.0, dot(cnorm, v) - BIAS) * (1.0 / (1.0 + d));
	ao *= smoothstep(MAX_DISTANCE, MAX_DISTANCE * 0.5, l);
	return ao;

}

float spiralAO(float2 uv, float3 p, float3 n, float rad)
{
	float goldenAngle = 2.4;
	float ao = 0.;
	float inv = 1. / float(SAMPLES);
	float radius = 0.;

	float rotatePhase = hash12(uv * 100.) * 6.28;
	float rStep = inv * rad;
	float2 spiralUV;

	for (int i = 0; i < SAMPLES; i++) {
		spiralUV.x = sin(rotatePhase);
		spiralUV.y = cos(rotatePhase);
		radius += rStep;
		ao += doAmbientOcclusion(uv, spiralUV * radius, p, n);
		rotatePhase += goldenAngle;
	}
	ao *= inv;
	return ao;
}


PS_OUT PS_MAIN_SSAO(PS_DOWNSCALE_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_ShaderTexture.Sample(DefaultSampler, In.vTexUV);


	if (!g_bSSAO)
		return Out;

	vector			vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexUV);

	if (vDepthDesc.y > 0.5f)
		return Out;


	//Shadow
	/*vector			vWorldPos;

	vWorldPos.x = uv.x * 2.f - 1.f;
	vWorldPos.y = uv.y * -2.f + 1.f;
	vWorldPos.z = vDepthDesc.x;
	vWorldPos.w = 1.0f;

	float			fViewZ = vDepthDesc.y * 1500.f;
	vWorldPos *= fViewZ;

	vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
	vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

	vWorldPos.xyz;*/

	float3 p = getPosition(In.vTexUV);
	float3 n = getNormal(In.vTexUV);

	float ao = 0.;
	float rad = SAMPLE_RAD / p.z;

	ao = spiralAO(In.vTexUV, p, n, rad);

	ao = 1.f - ao * INTENSITY;

	//fragColor = vector(ao, ao, ao, 1.);

	float fPower = 1.6f;
	if (vDepthDesc.z > 0.99f)
		fPower = 1.f;

	Out.vColor.xyz *= pow(ao, fPower);

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

float4x4 brightnessMatrix(float brightness)
{
	return float4x4(1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		brightness, brightness, brightness, 1);
}

float4x4 contrastMatrix(float contrast)
{
	float t = (1.0 - contrast) / 2.0;

	return float4x4(contrast, 0, 0, 0,
		0, contrast, 0, 0,
		0, 0, contrast, 0,
		t, t, t, 1);

}

float4x4 saturationMatrix(float saturation)
{
	float3 luminance = float3(0.3086, 0.6094, 0.0820);

	float oneMinusSat = 1.0 - saturation;

	float3 red = luminance.x * oneMinusSat;
	red += float3(saturation, 0, 0);

	float3 green = luminance.y * oneMinusSat;
	green += float3(0, saturation, 0);

	float3 blue = luminance.z * oneMinusSat;
	blue += float3(0, 0, saturation);

	return float4x4(red, 0,
		green, 0,
		blue, 0,
		0, 0, 0, 1);
}

float brightness = 0.15;
float contrast = 1.2;
float saturation = 1.5;
bool g_bHDR = true;

PS_OUT PS_MAIN_HDR(PS_DOWNSCALE_IN In)
{
	PS_OUT		Out = (PS_OUT)0;
	Out.vColor = g_ShaderTexture.Sample(DefaultSampler, In.vTexUV);

	if (!g_bHDR)
		return Out;

	float4x4 matBrightnessContrast = mul(brightnessMatrix(brightness), contrastMatrix(contrast));
	float4x4 matBrightnessContrastSaturation = mul(matBrightnessContrast, saturationMatrix(saturation));

	Out.vColor = mul(Out.vColor, matBrightnessContrastSaturation);

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



PS_OUT	PS_MAIN_FASTBLUR(PS_DOWNSCALE_IN input)
{
	PS_OUT	Output = (PS_OUT)1;

	// Normalized pixel coordinates (from 0 to 1)
	float2 uv = input.vTexUV;
	// Pixel colour
	vector Color = g_ShaderTexture.Sample(DefaultSampler, uv);

	float Pi = 6.28318530718; // Pi*2

   // GAUSSIAN BLUR SETTINGS {{{
	float Directions = 16.0; // BLUR DIRECTIONS (Default 16.0 - More is better but slower)
	float Quality = 4.0; // BLUR QUALITY (Default 4.0 - More is better but slower)
	float Size = 20.0; // BLUR SIZE (Radius)
	// GAUSSIAN BLUR SETTINGS }}}

	float2 Radius = Size / g_vResolution;



	// Blur calculations
	for (float d = 0.0; d < Pi; d += Pi / Directions)
	{
		for (float i = 1.0 / Quality; i <= 1.0; i += 1.0 / Quality)
		{
			Color += g_ShaderTexture.Sample(DefaultSampler, uv + float2(cos(d), sin(d)) * Radius * i);
		}
	}

	// Output to screen
	Color /= Quality * Directions - 15.0;
	Output.vColor = Color;
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
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_DOWNSCALE_MAIN();
	}

	pass VERTICAL
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_VERTICAL_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_VERTICAL_MAIN();
	}

	pass HORIZONTAL
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_HORIZONTAL_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_VERTICAL_MAIN();
	}

	pass BLOOM
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLOOM_MAIN();
	}

	pass BLOOMHARD
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_BLOOMHARD_MAIN();
	}

	pass MOTIONBLUR
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MOTIONBLUR_MAIN();
	}

	pass RADIALBLUR
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_RADIALBLUR_MAIN();
	}

	pass CHROMATIC_ABERRATION
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_CHROMATIC_ABERRATION_MAIN();
	}

	pass GRAYSCALE
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_GRAYSCALE_MAIN();
	}

	pass LENSFLARE
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_LENSFLARE();
	}

	pass SSAO
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_SSAO();
	}

	pass HDR
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_HDR();
	}

	pass FASTBLUR
	{
		SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
		SetDepthStencilState(DSS_ZEnable_ZWriteEnable_false, 0);
		SetRasterizerState(RS_Default);

		VertexShader = compile vs_5_0 VS_DOWNSCALE_MAIN();
		GeometryShader = NULL; HullShader = NULL; DomainShader = NULL;
		PixelShader = compile ps_5_0 PS_MAIN_FASTBLUR();
	}

}