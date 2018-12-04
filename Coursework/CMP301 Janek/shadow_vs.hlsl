
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	
};

cbuffer ShadowBuffer: register(b1)
{
    
    matrix lightViewMatrix1;
    matrix lightProjectionMatrix1;
    matrix lightViewMatrix2;
    matrix lightProjectionMatrix2;
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
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
    float4 lightViewPosOne : TEXCOORD1;
	float4 lightViewPosTwo : TEXCOORD2;

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
	output.lightViewPosOne = mul(input.position, worldMatrix);
    output.lightViewPosOne = mul(output.lightViewPosOne, lightViewMatrix1);
    output.lightViewPosOne = mul(output.lightViewPosOne, lightProjectionMatrix1);

	// Calculate the position of the vertice as viewed by the light source.
    //TODO something  is going wrong here, but the matrices all get passed in correctly
    output.lightViewPosTwo = mul(input.position, worldMatrix);
    output.lightViewPosTwo = mul(output.lightViewPosTwo, lightViewMatrix2);
    output.lightViewPosTwo = mul(output.lightViewPosTwo, lightProjectionMatrix2);


    output.worldPos = mul(input.position, worldMatrix).xyz;

    output.tex = input.tex;
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

	return output;
}