#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// Uniforms
layout(rgba32f, binding = 0) uniform image2D inputGrid;    // Current state of the grid (u)
layout(rgba32f, binding = 1) uniform image2D outputGrid;   // Updated state of the grid (u_new)
layout(rgba32f, binding = 2) uniform image2D sourceGrid;   // Source term at each grid point (s_{i,j})
layout (location = 1) uniform ivec2 gridSize; // gridSize.x is width (n), gridSize.y is height (m)
layout (location = 2) uniform float diffusionCoefficient; // Diffusion coefficient

void main() {
    ivec2 gridCoord = ivec2(gl_GlobalInvocationID.xy);

    // Check for boundary conditions
    if (gridCoord.x > 0 && gridCoord.x < gridSize.y - 1 && gridCoord.y > 0 && gridCoord.y < gridSize.x - 1) {
        // Load current state
        float currentState = imageLoad(inputGrid, gridCoord).r;

        // Calculate diffusion using the red channel of the neighboring cells
        float diffusion = diffusionCoefficient * (
            imageLoad(inputGrid, gridCoord + ivec2(-1, 0)).r +
            imageLoad(inputGrid, gridCoord + ivec2(1, 0)).r +
            imageLoad(inputGrid, gridCoord + ivec2(0, -1)).r +
            imageLoad(inputGrid, gridCoord + ivec2(0, 1)).r -
            4.0 * currentState
        );

        // Load source term
        // float sourceTerm = imageLoad(sourceGrid, gridCoord).r;

        // Update the state
        float newState = currentState + diffusion; // + sourceTerm;

        // Store the updated state in the red channel of the output image
        imageStore(outputGrid, gridCoord, vec4(newState, 0.0, 0.0, 1.0));
    }
}