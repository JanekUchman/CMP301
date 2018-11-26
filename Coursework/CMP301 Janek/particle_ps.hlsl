Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambient;
	float4 diffuse;
	float4 direction;
};

cbuffer ColourBuffer : register(b1)
{
    float3 colourTint;
    float invertColour;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(InputType input) : SV_TARGET
{
    float4 textureColour = texture0.Sample(Sampler0, input.tex);
    float4 finalColour = textureColour;
    if (invertColour == 1)
    {
        finalColour.xyz = textureColour.xyz + colourTint;
    }
    else
    {
        finalColour.xyz = textureColour.xyz - colourTint;
    }
    return finalColour;
}