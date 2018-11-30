
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	matrix lightViewMatrix[2];
	matrix lightProjectionMatrix[2];
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
    float4 lightViewPos[2] : TEXCOORD1;
    float3 worldPos : TEXCOORD3;
};


OutputType main(InputType input)
{
    OutputType output;

	// Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    
	// Calculate the position of the vertice as viewed by the light source.
    output.lightViewPos[0] = mul(input.position, worldMatrix);
    output.lightViewPos[0] = mul(output.lightViewPos[0], lightViewMatrix[0]);
    output.lightViewPos[0] = mul(output.lightViewPos[0], lightProjectionMatrix[0]);

    // Calculate the position of the vertice as viewed by the light source.
    output.lightViewPos[1] = mul(input.position, worldMatrix);
    output.lightViewPos[1] = mul(output.lightViewPos[1], lightViewMatrix[1]);
    output.lightViewPos[1] = mul(output.lightViewPos[1], lightProjectionMatrix[1]);

    output.worldPos = mul(input.position, worldMatrix).xyz;

    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	return output;
}