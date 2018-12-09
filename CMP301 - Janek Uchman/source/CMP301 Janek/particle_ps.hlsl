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
    //Convert the sprite to greyscale so we can colour it how we want
	float grey = (textureColour.x + textureColour.y + textureColour.z) / 3;
    float4 finalColour = float4(grey, grey, grey, textureColour.w);
    //Don't colour the  texture if it's past a certain alpha
	if (finalColour.w < 0.5f)
	{
		finalColour = float4(0,0,0,0);
	}
    else if (invertColour == 1)
    {
		finalColour.xyz = finalColour.xyz + colourTint;
    }
    else
    {
        finalColour.xyz = 1- (finalColour.xyz - colourTint);
    }
    return finalColour;
}