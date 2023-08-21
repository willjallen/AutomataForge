#version 330 core

uniform sampler2D uTexture; // The current state of the Game of Life
out vec4 LFragment;

in vec2 vTexCoord;

void main() {
  // Define the offsets for the 8 neighbors
  vec2 offsets[8];
  float size = 1.0 / 512.0; // Assuming a 512x512 texture
  offsets[0] = vec2(-size,  size);
  offsets[1] = vec2( 0.0,  size);
  offsets[2] = vec2( size,  size);
  offsets[3] = vec2(-size,  0.0);
  offsets[4] = vec2( size,  0.0);
  offsets[5] = vec2(-size, -size);
  offsets[6] = vec2( 0.0, -size);
  offsets[7] = vec2( size, -size);

  // Count living neighbors
  int aliveNeighbors = 0;
  for (int i = 0; i < 8; i++) {
    if (texture(uTexture, vTexCoord + offsets[i]).r > 0.5) {
      aliveNeighbors++;
    }
  }

  // Apply the rules of the Game of Life
  bool isAlive = texture(uTexture, vTexCoord).r > 0.5;
  if (isAlive && (aliveNeighbors < 2 || aliveNeighbors > 3)) {
    LFragment = vec4(0.0, 0.0, 0.0, 1.0); // Die
  } else if (!isAlive && aliveNeighbors == 3) {
    LFragment = vec4(1.0, 1.0, 1.0, 1.0); // Born
  } else {
    LFragment = isAlive ? vec4(1.0, 1.0, 1.0, 1.0) : vec4(0.0, 0.0, 0.0, 1.0); // Remain the same
  }
}
