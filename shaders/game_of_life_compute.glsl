#version 430 core

layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

// uniforms
layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout (location = 0) uniform float t;
layout (location = 1) uniform int textureSize; 

// Simulate Game of Life
void main() {
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);
  
  // Read current state
  vec4 current = imageLoad(imgOutput, texelCoord);
  
  // Check neighbors
  int aliveNeighbors = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) continue; // Skip the center cell
      ivec2 neighborCoord = (texelCoord + ivec2(dx, dy) + textureSize) % textureSize; // Wrap around
      vec4 neighbor = imageLoad(imgOutput, neighborCoord);
      if (neighbor.x > 0.5) aliveNeighbors++;
    }
  }

  // Determine next state
  vec4 value;
  if (current.x > 0.5) { // Currently alive
    value = (aliveNeighbors == 2 || aliveNeighbors == 3) ? vec4(1.0) : vec4(0.0);
  } else { // Currently dead
    value = (aliveNeighbors == 3) ? vec4(1.0) : vec4(0.0);
  }

  // Write next state
  imageStore(imgOutput, texelCoord, value);
}
