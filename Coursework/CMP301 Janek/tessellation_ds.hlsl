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
 
    // Determine the position of the new vertex.
	// Invert the y and Z components of uvwCoord as these coords are generated in UV space and therefore y is positive downward.
	// Alternatively you can set the output topology of the hull shader to cw instead of ccw (or vice versa).
    vertexPosition = uvwCoord.x * patch[0].position + uvwCoord.y * patch[1].position + uvwCoord.z * patch[2].position;
	vertexNormal = uvwCoord.x * patch[0].normal + uvwCoord.y * patch[1].normal + uvwCoord.z * patch[2].normal;
    vertexTex = patch[0].tex * uvwCoord.x + patch[1].tex * uvwCoord.y + patch[2].tex * uvwCoord.z;

	// Project onto unit sphere.
	float3 n =normalize(vertexPosition);
	// Project onto sphere.
    vertexPosition = radius * n;
	output.normal = vertexNormal;
	// Derive texture coordinates from spherical coordinates.
	float theta = atan2(vertexPosition.z, vertexPosition.x);
	// Put in [0, 2pi].
	if (theta < 0.0f)
		theta += 3.14159265359f;
	float phi = acos(vertexPosition.y / radius);

    vertexTex.x = theta / 3.14159265359f*2;
    vertexTex.x = phi / 3.14159265359f;
	// Partial derivative of P with respect to theta
	output.normal.x = -radius * sin(phi)*sin(theta);
	output.normal.y = 0.0f;
	output.normal.z = +radius * sin(phi)*cos(theta);
    output.normal = normalize(output.normal);


    float3 vNormal = mul(vertexNormal, (float3x3) worldMatrix);
    vNormal = normalize(vNormal);
    //vertexTex.x = phi*output.normal.x + time;
  
    //vertexTex.y = theta*output.normal.y;
		    
    	
    //vertexTex.x = asin(output.normal.x) / (2.0 * 3.14159265359f) + 0.5 + time;

    //vertexTex.y = asin(output.normal.y) / 3.14159265359f + 0.5;

    //vertexTex.x = atan2(output.normal.x, output.normal.z) / (2.0 * 3.14159265359f) + 0.5;

    //vertexTex.y = asin(output.normal.y) / 3.14159265359f + 0.5 + time;


    float4 textureColour = texture0.SampleLevel(sampler0, vertexTex, 0);
    float averageDisplacement = (textureColour.x + textureColour.y + textureColour.z) * displacement;

    vertexPosition.xyz += averageDisplacement * output.normal;

    // Calculate the position of the new vertex against the world, view, and projection matrices.
    output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.depthPosition.xyz = output.position;
    output.tex = vertexTex;

    return output;
}

