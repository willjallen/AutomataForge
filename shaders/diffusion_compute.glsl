#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// uniforms
layout(rgba32f, binding = 0) uniform image2D imgOutput;
layout (location = 1) uniform ivec2 textureSize; // textureSize.x is width, textureSize.y is height

void main() {
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

  // Read current state
  vec4 current = imageLoad(imgOutput, texelCoord);

  // Count alive neighbors
  int aliveNeighbors = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) continue; // Skip the center cell
      int neighborX = (texelCoord.x + dx + textureSize.x) % textureSize.x;
      int neighborY = (texelCoord.y + dy + textureSize.y) % textureSize.y;
      ivec2 neighborCoord = ivec2(neighborX, neighborY);
      vec4 neighbor = imageLoad(imgOutput, neighborCoord);
      aliveNeighbors += int(neighbor.x > 0.5);
    }
  }

  // Compute next state
  float nextState = float((aliveNeighbors == 3) || (aliveNeighbors == 2 && current.x > 0.5));

  // Write next state
  imageStore(imgOutput, texelCoord, vec4(nextState));
}