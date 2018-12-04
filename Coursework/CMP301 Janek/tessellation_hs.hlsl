// Tessellation Hull Shader
// Prepares control points for tessellation
cbuffer TessellationBuffer : register(b0)
{
    float tessellationFactor;
	float3 padding;
};

struct InputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

struct ConstantOutputType
{
    float edges[3] : SV_TessFactor;
    float inside : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD0;
};

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 3> inputPatch, uint patchId : SV_PrimitiveID)
{
    ConstantOutputType output;

    // Set the tessellation factors for the three edges of the triangle.
    output.edges[0] = tessellationFactor; 
    output.edges[1] = tessellationFactor; 
	output.edges[2] = tessellationFactor; 

    // Set the tessellation factor for tessallating inside the triangle.
    output.inside = tessellationFactor; 

    return output;
}


[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 3> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;


    output.tex= patch[patchId].tex;

	output.normal = patch[pointId].normal;
    // Set the position for this control point as the output position.
    output.position = patch[pointId].position;


    return output;
}