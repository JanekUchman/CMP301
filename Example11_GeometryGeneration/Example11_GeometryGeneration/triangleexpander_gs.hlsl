
cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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
};

void SetPoint(point InputType input[1], inout TriangleStream<OutputType> triStream, OutputType output, float4 position)
{
	// Move the vertex away from the point position
	output.position = input[0].position + position;
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input[0].tex;
	output.normal = mul(input[0].normal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);
}

void ExpandPoint(triangle InputType input, inout TriangleStream<OutputType> triStream, OutputType output, int multiplier)
{
	output.position = (float4)((input.position.x * multiplier), (input.position.x * multiplier), input.position.z, input.position.w);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = input.tex;
	output.normal = mul(input.normal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);
}

[maxvertexcount(3)]
void main(triangle InputType input[3], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	float4 g_positions[4] =
	{
	float4(-1, 1, 0, 0),
	float4(-1, -1, 0, 0),
	float4(1, 1, 0, 0),
	float4(1, -1, 0, 0)
	};
	
	for (int i = 0; i < 3; i++)
	{
		SetPoint(input[i], triStream, output, 1);
	}

	triStream.RestartStrip();
}