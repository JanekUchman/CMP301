// Tessellation Hull Shader
// Prepares control points for tessellation
cbuffer TessellationBuffer : register(b0)
{
	float tessellationFactor;
	float tessEdge;
	float tessInside;
    float3 cameraPos;
    float2 padding;
};


cbuffer MatrixBuffer : register(b1)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct InputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

struct ConstantOutputType
{
    float edges[4] : SV_TessFactor;
    float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
    float4 colour : COLOR;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;
    
    // Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = tessEdge;// tessellationFactor;
    output.edges[1] = tessEdge; //tessellationFactor;
    output.edges[2] = tessEdge; // tessellationFactor;
    output.edges[3] = tessEdge;

    // Set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = tessInside;
    output.inside[1] = tessInside;

    return output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;


    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;

    // Set the input colour as the output colour.
    output.colour = patch[pointId].colour;

    return output;
}