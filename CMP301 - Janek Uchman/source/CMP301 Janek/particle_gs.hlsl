
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

[maxvertexcount(4)]
void main(point InputType input[1], inout TriangleStream<OutputType> triStream)
{
	OutputType output;

	float2 g_tex[4] =
	{
		float2(0, 0),
		float2(0, 1),
		float2(1, 0),
		float2(1, 1)
	};

    //The forward vector of the particle
    float3 planeNormal = input[0].position - cameraPos;
    planeNormal = normalize(planeNormal);

    //Set it straight up so we get x/z rotation
    float3 upVector = float3(0.0f, 1.0f, 0.0f);
    float3 rightVector = normalize(cross(planeNormal, upVector));

    rightVector = rightVector * 0.5f;
    //Set it back so the sprite renders properly
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