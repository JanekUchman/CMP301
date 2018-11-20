
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer LightBuffer : register(b1)
{
    float3 cameraPos;
    float padding;
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

void SetPoint(point InputType input[1], inout TriangleStream<OutputType> triStream, OutputType output, float4 position, float2 texCoord, float3 normal)
{
	// Move the vertex away from the point position
	output.position = position;
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	output.tex = texCoord;
	float3 lineOne = output.position;
	output.normal = mul(normal, worldMatrix);
	output.normal = normalize(output.normal);
	triStream.Append(output);
}

float3 GetLine(float3 pointOne, float3 pointTwo)
{
	float3 lineVector = float3(0, 0, 0);
	float3 a = pointOne.x - pointTwo.x;
	float3 b = pointOne.y - pointTwo.y;
	a = a * a;
	b = b * b;
	lineVector = sqrt(a + b);
}

float3 CalculateNormal(float3 lineOne, float3 lineTwo, float3 lineThree)
{
	float3 U = (lineTwo - lineOne);
	float3 V = (lineThree - lineOne);

	float3 normal = float3(0, 0, 0);
	normal.x = (U.y*V.z) - (U.z * V.y);
	normal.y = (U.z*V.x) - (U.x * V.z);
	normal.z = (U.x*V.y) - (U.y * V.x);

	return normal;
}

[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;
	
	//float4 g_positions[4] =
	//{
	//	float4(-1, 1, 0, 0),
	//	float4(-1, -1, 0, 0),
	//	float4(1, 1, 0, 0),
	//	float4(1, -1, 0, 0)
	//};

	float2 g_tex[4] =
	{
		float2(0, 0),
		float2(0, 1),
		float2(1, 0),
		float2(1, 1)
	};

    float3 planeNormal = input[0].position - cameraPos;
    planeNormal.y = 0.0f;
    planeNormal = normalize(planeNormal);

    float3 upVector = float3(0.0f, 1.0f, 0.0f);
    float3 rightVector = normalize(cross(planeNormal, upVector));

    rightVector = rightVector * 0.5f;
    upVector = float3(0, -1.f, 0);


    float3 positions[4] =
    {
        input[0].position - rightVector,
		input[0].position + rightVector,
		input[0].position - rightVector + upVector,
		input[0].position + rightVector + upVector
    };

    float4 g_positions[4] =
    {
        float4(positions[0].x, positions[0].y, positions[0].z, input[0].position.w),
		float4(positions[1].x, positions[1].y, positions[1].z, input[0].position.w),
		float4(positions[2].x, positions[2].y, positions[2].z, input[0].position.w),
	    float4(positions[3].x, positions[3].y, positions[3].z, input[0].position.w)
    };


	for (int i = 0; i < 4; i++)
	{
		SetPoint(input, triStream, output, g_positions[i], g_tex[i], -planeNormal);
	}
	
    triStream.RestartStrip();
}