matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float4 g_vColor;
BlendState BS_Default
{
	BlendEnable[0] = false;
};
DepthStencilState DSS_Default
{
	DepthEnable = true;
	DepthWriteMask = all;
	DepthFunc = less_equal;
};
RasterizerState RS_Default
{
	FillMode = solid;
	CullMode = back;
	FrontCounterClockwise = false;
};
struct VS_IN
{
	float3 vPosition : POSITION;
};
struct VS_IN_Instancing
{
    float3 vPosition : POSITION;

    float3		vPointA : TEXCOORD0;
    float3		vPointB : TEXCOORD1;
    float3		vPointC : TEXCOORD2;
    float4		vColor : TEXCOORD3;
};
struct VS_OUT
{
	float4 vPosition : SV_POSITION;
};
struct VS_OUT_Instancing
{
    float3 vPosition : POSITION;

    float3		vPointA : TEXCOORD0;
    float3		vPointB : TEXCOORD1;
    float3		vPointC : TEXCOORD2;
    float4		vColor : TEXCOORD3;
};

struct GS_IN_Instancing
{
    float3 vPosition : POSITION;

    float3		vPointA : TEXCOORD0;
    float3		vPointB : TEXCOORD1;
    float3		vPointC : TEXCOORD2;
    float4		vColor : TEXCOORD3;
};

struct GS_OUT_Instancing
{
    vector		vPosition : SV_POSITION;
    float4		vColor : TEXCOORD0;
};

struct PS_IN
{
	float4 vPosition : SV_POSITION;
};
struct PS_IN_Instancing
{
	float4 vPosition : SV_POSITION;
    float4		vColor : TEXCOORD0;
};
struct PS_OUT
{
	vector vColor : SV_TARGET0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    matrix matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);

    return Out;
}

VS_OUT_Instancing VS_MAIN_Instancing(VS_IN_Instancing In)
{
    VS_OUT_Instancing Out = (VS_OUT_Instancing)In;

    return Out;
}

[maxvertexcount(3)]
void GS_MAIN_Instancing(point GS_IN_Instancing In[1], inout TriangleStream<GS_OUT_Instancing> Stream)
{
    GS_OUT_Instancing			Out[3];

    matrix		matVP;

    matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = mul(vector(In[0].vPointA, 1.f), matVP);
    Out[0].vColor = In[0].vColor;

    Out[1].vPosition = mul(vector(In[0].vPointB, 1.f), matVP);
    Out[1].vColor = In[0].vColor;

    Out[2].vPosition = mul(vector(In[0].vPointC, 1.f), matVP);
    Out[2].vColor = In[0].vColor;

    Stream.Append(Out[0]);
    Stream.Append(Out[1]);
    Stream.Append(Out[2]);
    Stream.RestartStrip();
}



PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;

	Out.vColor = g_vColor;



    return Out;
}

PS_OUT PS_MAIN_Instancing(PS_IN_Instancing In)
{
    PS_OUT Out = (PS_OUT)0;

    Out.vColor = In.vColor;



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
        GeometryShader = NULL;HullShader = NULL;DomainShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
    pass Instancing
    {
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
        SetDepthStencilState(DSS_Default, 0);
        SetRasterizerState(RS_Default);

        VertexShader = compile vs_5_0 VS_MAIN_Instancing();
        GeometryShader = compile gs_5_0 GS_MAIN_Instancing();
        PixelShader = compile ps_5_0 PS_MAIN_Instancing();
    }
}