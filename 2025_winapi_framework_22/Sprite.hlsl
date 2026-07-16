cbuffer cbPerObject : register(b0)
{
    matrix Projection;
};

struct VS_INPUT
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD0;
    float4 Color : COLOR0;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 Tex : TEXCOORD0;
    float4 Color : COLOR0;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(float4(input.Pos, 1.0f), Projection);
    output.Tex = input.Tex;
    output.Color = input.Color;
    return output;
}

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

float4 PS(PS_INPUT input) : SV_Target
{
    float4 texColor = txDiffuse.Sample(samLinear, input.Tex);
    return texColor * input.Color;
}

float4 PS_Color(PS_INPUT input) : SV_Target
{
    return input.Color;
}
