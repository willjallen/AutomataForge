#version 430 core

layout (local_size_x = 8, local_size_y = 8, local_size_z = 1) in;

// uniforms
layout(rgba32f, binding = 0) uniform image2D imgInput;
layout(rgba32f, binding = 1) uniform image2D imgOutput;
layout(rgba32f, binding = 2) uniform image2D imgEnergy;
layout (location = 1) uniform ivec2 textureSize; // textureSize.x is width, textureSize.y is height
layout (location = 2) uniform float birthEnergyFlux; 
layout (location = 3) uniform float deathEnergyFlux; 
// width,height+2 for borders
shared float cellState[10][10];

void main() {
  ivec2 texelCoord = ivec2(gl_GlobalInvocationID.xy);

  // Read current state
  int localX = int(gl_LocalInvocationID.x) + 1;
  int localY = int(gl_LocalInvocationID.y) + 1;
  cellState[localX][localY] = imageLoad(imgInput, texelCoord).x;

  // Handle border conditions
  // TODO: Check this logic for < 0
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
  float localEnergy = imageLoad(imgEnergy, texelCoord).r;
  // // Miraculous spawn
  // // If energy is >= energyFlux, spawn a new cell regardless of the rules
  // if(localEnergy >= energyFlux){
  //   // Remove energy
  //   imageStore(imgEnergy, texelCoord, vec4(imageLoad(imgEnergy, texelCoord).r - energyFlux, 0.0, 0.0, 1.0)); 
  //   // Write next state
  //   imageStore(imgOutput, texelCoord, vec4(1.0));
  //   return;
  // }

  // Count alive neighbors
  int aliveNeighbors = 0;
  for (int dx = -1; dx <= 1; ++dx) {
    for (int dy = -1; dy <= 1; ++dy) {
      if (dx == 0 && dy == 0) continue; // Skip the center cell
      aliveNeighbors += int(cellState[localX + dx][localY + dy] > 0.5);
    }
  }

  // Compute next state
  float currentCellState = cellState[localX][localY];
  float nextCellState = 0.0;
  
  // GOL B3/S23
  float nextState = float((aliveNeighbors == 3) || (aliveNeighbors == 2 && cellState[localX][localY] > 0.5));
  // // Birth
  // if(currentCellState > 0.5 && aliveNeighbors == 3){
  //   nextCellState = 1.0;
  // }

  // // Survive
  // if(currentCellState < 0.5 && (aliveNeighbors == 2 || aliveNeighbors == 3)){
  //   nextCellState = 1.0;
  // }

  // imageStore(imgOutput, texelCoord, vec4(nextState));

  // // Birth
  // if(nextState > currentCellState){
  //   imageStore(imgEnergy, texelCoord, vec4(imageLoad(imgEnergy, texelCoord).r - energyFlux, 0.0, 0.0, 1.0)); 
  // }

  // // Death
  // if(nextState < currentCellState){
  //   imageStore(imgEnergy, texelCoord, vec4(imageLoad(imgEnergy, texelCoord).r + energyFlux, 0.0, 0.0, 1.0)); 
  // }

  // If cell should die
  if(currentCellState > 0.5 && nextState < 0.5){
    // Add energy
    imageStore(imgEnergy, texelCoord, vec4(imageLoad(imgEnergy, texelCoord).r + deathEnergyFlux, 0.0, 0.0, 1.0)); 
    // Write next state
    imageStore(imgOutput, texelCoord, vec4(nextState));
  }

  // If cell is currently dead and should be born, required at least energyFlux units of energy
  else if(currentCellState < 0.5 && nextState > 0.5){
    // Check if there is enough energy
    
    if(localEnergy >= birthEnergyFlux){
      // Remove energy
      imageStore(imgEnergy, texelCoord, vec4(imageLoad(imgEnergy, texelCoord).r - birthEnergyFlux, 0.0, 0.0, 1.0)); 
    
      // Write next state
      imageStore(imgOutput, texelCoord, vec4(nextState));
    } 
    
  }else{
    // If cell is dead and should stay dead or cell is alive and should stay alive, do nothing
    imageStore(imgOutput, texelCoord, vec4(nextState));
  }
  

}

