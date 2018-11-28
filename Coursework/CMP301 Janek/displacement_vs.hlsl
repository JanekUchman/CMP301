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
    float3 padding;
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
	float2 tex : TEXCOORD0;
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


    input.tex.x = asin(output.normal.x) / (2.0 * 3.14159265359f) + 0.5;

    input.tex.y = asin(output.normal.y) / 3.14159265359f + 0.5 + time;

   /* input.tex.x = atan2(output.normal.x, output.normal.z) / (2.0 * 3.14159265359f) + 0.5;

    input.tex.y = asin(output.normal.y) / 3.14159265359f + 0.5 + time;
*/




	float4 textureColour = texture0.SampleLevel(sampler0, input.tex, 0);
	float averageDisplacement = (textureColour.x + textureColour.y + textureColour.z ) /10;

   

    input.position.xyz += averageDisplacement * output.normal;


	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	

	output.worldPosition = mul(input.position, worldMatrix).xyz;

	return output;
}