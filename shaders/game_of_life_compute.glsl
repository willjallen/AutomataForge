#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// uniforms
layout(rgba32f, binding = 0) uniform image2D imgInput;
layout(rgba32f, binding = 1) uniform image2D imgOutput;
layout (location = 1) uniform ivec2 textureSize; // textureSize.x is width, textureSize.y is height

// width,height+2 for borders
shared float cellState[10][10];

void main() {
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

  // Read current state
  int localX = int(gl_LocalInvocationID.x) + 1;
  int localY = int(gl_LocalInvocationID.y) + 1;
  cellState[localX][localY] = imageLoad(imgInput, texelCoord).x;

  // Handle border conditions
  if (localX == 1) {
    cellState[localX - 1][localY] = imageLoad(imgInput, ivec2((texelCoord.x - 1 + textureSize.x) % textureSize.x, texelCoord.y)).x;
  }
  if (localX == 8) {
    cellState[localX + 1][localY] = imageLoad(imgInput, ivec2((texelCoord.x + 1) % textureSize.x, texelCoord.y)).x;
  }
  if (localY == 1) {
    cellState[localX][localY - 1] = imageLoad(imgInput, ivec2(texelCoord.x, (texelCoord.y - 1 + textureSize.y) % textureSize.y)).x;
  }
  if (localY == 8) {
    cellState[localX][localY + 1] = imageLoad(imgInput, ivec2(texelCoord.x, (texelCoord.y + 1) % textureSize.y)).x;
  }

  // Synchronize threads within the work group
  barrier();

  // Count alive neighbors
  int aliveNeighbors = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) continue; // Skip the center cell
      aliveNeighbors += int(cellState[localX + dx][localY + dy] > 0.5);
    }
  }

  // Compute next state
  float nextState = float((aliveNeighbors == 3) || (aliveNeighbors == 2 && cellState[localX][localY] > 0.5));

  // Write next state
  imageStore(imgOutput, texelCoord, vec4(nextState));
}