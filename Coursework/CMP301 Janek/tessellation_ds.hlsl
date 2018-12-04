// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices

Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer TimeBuffer : register(b1)
{
    float time;
    float displacement;
    float2 padding;
};
struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct InputType
{
    float3 position : POSITION;
	float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct OutputType
{
    float4 position : SV_POSITION;
	float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
    float4 depthPosition : TEXCOORD1;


};

[domain("tri")]
OutputType main(ConstantOutputType input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 3> patch)
{
    float3 vertexPosition;
	float3 vertexNormal;
    float2 vertexTex;

    OutputType output;
	float radius = 1;
 
    // Determine the position of the new vertex by interpolating from the parent coordinates
    vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
	// Project onto unit sphere.
	float3 n =normalize(vertexPosition);
	// Project onto sphere.
    output.position.xyz = radius * n;
	output.normal = n;

    //Wrap the texture around the sphere    
	output.tex.x = asin(output.normal.x) / (2.0 * 3.14159265359f) + 0.5 ;
	output.tex.y = asin(output.normal.y) / 3.14159265359f + 0.5 + time;

    //Calculate how much we shoud displace based on the darkness of the displacement map
    float4 textureColour = texture0.SampleLevel(sampler0, output.tex, 0);
    float averageDisplacement = (textureColour.x + textureColour.y + textureColour.z) * displacement;

    //Move the vertex along the normal
	output.position.xyz += averageDisplacement * output.normal;

	output.position.w = 1;
    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(output.position), worldMatrix);
	output.normal = mul(float4(output.position.xyz, 0.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    //Store the depth position for the shadow shader
    output.depthPosition.xyz = output.position;

    return output;
}

