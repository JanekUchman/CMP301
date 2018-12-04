// Light vertex shader
// Standard issue vertex shader, apply matrices, pass info to pixel shader
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

struct InputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
    float4 depthPosition : TEXCOORD0;

	float2 tex : TEXCOORD2;
	float3 normal : NORMAL;
	float3 worldPosition : TEXCOORD1;
};

OutputType main(InputType input)
{
	OutputType output;
   // input.tex.y += time;

     // Calculate the normal vector against the world matrix only and normalise.
    output.normal = mul(input.normal, (float3x3) worldMatrix);
    output.normal = normalize(output.normal);

    //Map the texture around the sphere, repeats it twice
    input.tex.x = asin(output.normal.x) / (2.0 * 3.14159265359f) + 0.5;
    input.tex.y = asin(output.normal.y) / 3.14159265359f + 0.5 + time;

    //Get the displacement based on the light value of the texture
	float4 textureColour = texture0.SampleLevel(sampler0, input.tex, 0);
	float averageDisplacement = (textureColour.x + textureColour.y + textureColour.z ) *displacement;
    //Move the vertex displacement along the normal, to help map properly to sphere
    input.position.xyz += averageDisplacement * output.normal;


	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

    //Store the depth position for the depth shader to use when rendering this shadow
	output.depthPosition = output.position;

	output.worldPosition = mul(input.position, worldMatrix).xyz;

	return output;
}