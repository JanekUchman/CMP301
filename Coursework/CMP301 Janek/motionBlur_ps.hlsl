Texture2D sceneTexture : register(t0);
Texture2D depthTexture : register(t1);

SamplerState Sampler0 : register(s0);

cbuffer ScreenSizeBuffer : register(b0)
{
    float numSamples;
    float3 padding;
};

cbuffer MatrixBuffer : register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;

    matrix prevViewMatrix;
    matrix prevProjectionMatrix;
};

struct InputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

//float4 main(InputType input) : SV_TARGET
//{
//    float weight0, weight1, weight2;
//    float4 colour;

//	// Create the weights that each neighbor pixel will contribute to the blur.
//	weight0 = 0.4062f;
//    weight1 = 0.2442f;
//    weight2 = 0.0545f;

//	// Initialize the colour to black.
//    colour = float4(0.0f, 0.0f, 0.0f, 0.0f);

//    float texelSize = 1.0f / screenWidth;
//    // Add the horizontal pixels to the colour by the specific weight of each.
//    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -2.0f, 0.0f)) * weight2;
//    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * -1.0f, 0.0f)) * weight1;
//    colour += shaderTexture.Sample(SampleType, input.tex) * weight0;
//    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 1.0f, 0.0f)) * weight1;
//    colour += shaderTexture.Sample(SampleType, input.tex + float2(texelSize * 2.0f, 0.0f)) * weight2;

//	// Set the alpha channel to one.
//    colour.a = 1.0f;

//    return colour;
//}

float4 main(InputType input) : SV_TARGET
{

    matrix viewProjectionMatrix = (mul(viewMatrix, projectionMatrix));
    matrix prevViewProjectionMatrix = (mul(prevViewMatrix, prevProjectionMatrix));

    //https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch27.html
     // Get the depth buffer value at this pixel.
    float zOverW = depthTexture.Sample(Sampler0, input.tex).w;
    //range -1 to 1.
    float4 viewportPos = float4(input.tex.x * 2 - 1, (1 - input.tex.y) * 2 - 1, zOverW, 1);
    // Transform by the view-projection inverse.
    float4 viewProjTransform = mul(viewportPos, viewProjectionMatrix);
    // Divide by w to get the world position.
    float4 worldPos = viewProjTransform / viewProjTransform.w;

    float4 currentPos = viewProjTransform;
    // Use the world position, and transform by the previous view-
    // projection matrix.
    float4 previousPos = mul(worldPos, prevViewProjectionMatrix);
    // Convert to nonhomogeneous points [-1,1] by dividing by w.
    previousPos /= previousPos.w;
    // Use this frame's position and last frame's to compute the pixel
   // velocity.
    float2 velocity = (currentPos - previousPos) *2.f;

    // Get the initial color at this pixel.
    float4 colour = sceneTexture.Sample(Sampler0, input.tex);
    input.tex += velocity;
    for (int i = 1; i < numSamples; ++i)
    {
        input.tex += velocity;
        // Sample the color buffer along the velocity vector.
        float4 currentColour = sceneTexture.Sample(Sampler0, input.tex);
        // Add the current color to our color sum.
        colour += currentColour;
    }
    // Average all of the samples to get the final blur color.
    float4 finalColour = colour / numSamples;

    return finalColour;
}
