#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// Uniforms
layout(rgba32f, binding = 2) uniform image2D inputGridAB;    // Current state of A (red channel) and B (green channel)
layout(rgba32f, binding = 3) uniform image2D outputGridAB;   // Updated state of A (red channel) and B (green channel)

layout (location = 6) uniform float diffusionRateA; // Diffusion rate for type A
layout (location = 7) uniform float diffusionRateB; // Diffusion rate for type B

layout (location = 8) uniform float feedRate; // Feed rate
layout (location = 9) uniform float killRate; // Kill rate 

layout (location = 10) uniform float timestep; // The timestep

// 3x3 convolution
layout (location = 11) uniform float centerKernelWeight; // Weight given to the center center tile
layout (location = 12) uniform float adjKernelWeight; // Weight given to adjacent (cardinal) tiles
layout (location = 13) uniform float diagKernelWeight; // Weight given to diagonal tiles

/*
    R-D Equation is given as:
    A' = A + (D_A * laplace(A) - AB^2 + f(1-A)) dt
    B' = B + (D_B * laplace(B) + AB^2 - k(k+f)B) dt
*/
void main() {
    ivec2 gridCoord = ivec2(gl_GlobalInvocationID.xy);

    // Load current state
    float A = imageLoad(inputGridAB, gridCoord).r;
    float B = imageLoad(inputGridAB, gridCoord).g;
    
    vec4 centerPixel = imageLoad(inputGridAB, gridCoord + ivec2(0, 0));

    vec4  adjPixel1 = imageLoad(inputGridAB, gridCoord + ivec2(1, 0));
    vec4  adjPixel2 = imageLoad(inputGridAB, gridCoord + ivec2(0, 1));
    vec4  adjPixel3 = imageLoad(inputGridAB, gridCoord + ivec2(-1, 0));
    vec4  adjPixel4 = imageLoad(inputGridAB, gridCoord + ivec2(0, -1));

    vec4 diagPixel1 = imageLoad(inputGridAB, gridCoord + ivec2(1, 1));
    vec4 diagPixel2 = imageLoad(inputGridAB, gridCoord + ivec2(1, -1));
    vec4 diagPixel3 = imageLoad(inputGridAB, gridCoord + ivec2(-1, 1));
    vec4 diagPixel4 = imageLoad(inputGridAB, gridCoord + ivec2(-1, -1));


    // Calculate lapacian of A
    float laplacianA = (
        centerKernelWeight * centerPixel.r +
        adjKernelWeight * adjPixel1.r +
        adjKernelWeight * adjPixel2.r +
        adjKernelWeight * adjPixel3.r +
        adjKernelWeight * adjPixel4.r +
        diagKernelWeight * diagPixel1.r + 
        diagKernelWeight * diagPixel2.r + 
        diagKernelWeight * diagPixel3.r + 
        diagKernelWeight * diagPixel4.r 
    );

    // Calculate lapacian of B
    float laplacianB = (
        centerKernelWeight * centerPixel.g +
        adjKernelWeight * adjPixel1.g +
        adjKernelWeight * adjPixel2.g +
        adjKernelWeight * adjPixel3.g +
        adjKernelWeight * adjPixel4.g +
        diagKernelWeight * diagPixel1.g + 
        diagKernelWeight * diagPixel2.g + 
        diagKernelWeight * diagPixel3.g + 
        diagKernelWeight * diagPixel4.g 
    );


    float newA = A + (diffusionRateA * laplacianA - A * (B * B) + feedRate * (1 - A)) * timestep;
    float newB = B + (diffusionRateB * laplacianB + A * (B * B) - (killRate + feedRate) * B) * timestep;

    // Store the updated state in the red channel of the output image
    imageStore(outputGridAB, gridCoord, vec4(newA, newB, 0.0, 1.0));
    
}
