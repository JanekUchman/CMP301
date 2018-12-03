
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture1 : register(t1);
Texture2D depthMapTexture2 : register(t2);

SamplerState diffuseSampler  : register(s0);
SamplerState shadowSampler : register(s1);


cbuffer LightBuffer : register(b0)
{
	float4 ambient[3];
	float4 diffuse[3];
    float4 lightPosition[3];
	float4 lightDirection[3];

};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPos1 : TEXCOORD1;
	float4 lightViewPos2 : TEXCOORD2;

    float3 worldPos : TEXCOORD3;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateDirectionalLighting(InputType input, float3 lightDirection, float4 diffuse)
{

    float intensity = saturate(dot(input.normal, lightDirection));
    return saturate(diffuse * intensity);
}

float4 calculatePointLighting(InputType input, float4 diffuse, float3 lightPosition)
{
    float3 dist = normalize(lightPosition - input.worldPos);
    float attenuation = 1 / (0.5 + (0.125 * dist) + (0.f * (dist * dist)));
    return saturate(diffuse * attenuation);
}

float4 calculateShadow(InputType input, Texture2D depthTexture, float3 lightDirection, float4 lightViewPos, float4 ambient, float4 diffuse, float4 textureColour, inout bool  inShadow)
{
    float depthValue;
    float lightDepthValue;
    float shadowMapBias = 0.005f;
    float4 colour = float4(0.f, 0.f, 0.f, 1.f);

	// Calculate the projected texture coordinates.
    float2 pTexCoord = lightViewPos.xy / lightViewPos.w;
    pTexCoord *= float2(0.5, -0.5);
    pTexCoord += float2(0.5f, 0.5f);

    // Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
    if (pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f)
    {
        return textureColour;
    }
	
    // Sample the shadow map (get depth of geometry)
    depthValue = depthTexture.Sample(shadowSampler, pTexCoord).r;
	// Calculate the depth from the light.
    lightDepthValue = lightViewPos.z / lightViewPos.w;
    lightDepthValue -= shadowMapBias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
    if (lightDepthValue < depthValue)
    {
        colour = calculateDirectionalLighting(input, -lightDirection, diffuse);
    }
	else
	{
		inShadow = true;
	}
    return saturate(colour + ambient);
}

float4 main(InputType input) : SV_TARGET
{
	bool inShadow = false;
    float4 colour = (0.f, 0.f, 0.f, 1.f);
    float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);
    colour += calculateShadow(input, depthMapTexture1, lightDirection[0].xyz, input.lightViewPos1, ambient[0], diffuse[0], textureColour, inShadow);
    colour += calculateShadow(input, depthMapTexture2, lightDirection[1].xyz, input.lightViewPos2, ambient[1], diffuse[1], textureColour, inShadow);
    colour += calculatePointLighting(input, diffuse[2], lightPosition[2].xyz);
	if (inShadow)
	{
		colour = colour/3;
	}
	
	colour += calculatePointLighting(input, diffuse[2], lightPosition[2]);

    return (colour) *textureColour;
}