#version 430 core

layout (local_size_x = 10, local_size_y = 10, local_size_z = 1) in;

// uniforms
layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout (location = 1) uniform int textureSize; 

void main() {
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

  // Read current state
  vec4 current = imageLoad(imgOutput, texelCoord);

  // Count alive neighbors
  int aliveNeighbors = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) continue; // Skip the center cell
      ivec2 neighborCoord = (texelCoord + ivec2(dx, dy) + textureSize) % textureSize; // Wrap around
      vec4 neighbor = imageLoad(imgOutput, neighborCoord);
      aliveNeighbors += int(neighbor.x > 0.5);
    }
  }

  // Compute next state
  float nextState = float((aliveNeighbors == 3) || (aliveNeighbors == 2 && current.x > 0.5));

  // Write next state
  imageStore(imgOutput, texelCoord, vec4(nextState));
}
