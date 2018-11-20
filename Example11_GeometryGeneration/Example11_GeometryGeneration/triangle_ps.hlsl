Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambient;
	float4 diffuse;
	float4 direction;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}


float4 main(InputType input) : SV_TARGET
{
	float4 colour = calculateLighting(-direction, input.normal, diffuse);
	float4 textureColour = texture0.Sample(Sampler0, input.tex);
	colour = saturate(colour + ambient);
	return saturate(colour) * textureColour;
}