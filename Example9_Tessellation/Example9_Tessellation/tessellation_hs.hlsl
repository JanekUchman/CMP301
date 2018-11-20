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

    float3 diff = inputPatch[0].position - cameraPos;
    float distance = sqrt(dot(diff, diff));

    float newTessEdge = tessEdge / distance * 50;
    float newTessInside = tessInside / distance * 50;

    // Set the tessellation factors for the three edges of the triangle.
	output.edges[0] = newTessEdge;// tessellationFactor;
    output.edges[1] = newTessEdge; //tessellationFactor;
    output.edges[2] = newTessEdge; // tessellationFactor;
    output.edges[3] = newTessEdge;

    // Set the tessellation factor for tessallating inside the triangle.
    output.inside[0] = newTessInside;
    output.inside[1] = newTessInside;

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