// Light pixel shader
// Calculate diffuse lighting for a single directional light (also texturing)

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ColourBuffer : register(b0)
{
    float red;
    float blue;
    float green;
    float invert;
};


struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
	float4 depthPosition : TEXCOORD2;
};

float4 main(InputType input) : SV_TARGET
{
    
    float4 textureColour = texture0.Sample(sampler0, input.tex);
    if (invert == 1)
    {
        textureColour = float4(red - textureColour.x, green - textureColour.y, blue - textureColour.z, textureColour.w);
    }
    else
    {
        textureColour = float4(red + textureColour.x, green + textureColour.y, blue + textureColour.z, textureColour.w);
    }
    return textureColour;
}



