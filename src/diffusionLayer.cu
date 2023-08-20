#include <vector>
#include <cuda_runtime.h>
#include "diffusionLayer.h"


/**
 * Updates the diffusion model on an m x n grid.
 *
 * The discretized diffusion equation is given by:
 * u_{i,j}^{n+1} = u_{i,j}^n + D * (u_{i-1,j}^n + u_{i+1,j}^n + u_{i,j-1}^n + u_{i,j+1}^n - 4 * u_{i,j}^n) + s_{i,j}
 *
 * where:
 * u_{i,j}^n represents the value at grid point (i, j) at time n,
 * D is the diffusion coefficient,
 * s_{i,j} is a source term at (i, j).
 *
 * @param u         Pointer to the current state of the grid
 * @param u_new     Pointer to the updated state of the grid
 * @param sources   Pointer to the sources grid
 * @param m         Number of rows in the grid
 * @param n         Number of columns in the grid
 * @param D         Diffusion coefficient
 */
__global__ void update_diffusion(float* u, float* u_new, const float* sources, int m, int n, float D) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if (i > 0 && i < m-1 && j > 0 && j < n-1) {
        float diff = D * (u[(i-1)*n + j] + u[(i+1)*n + j] + u[i*n + (j-1)] + u[i*n + (j+1)] - 4 * u[i*n + j]);
        u_new[i*n + j] = u[i*n + j] + diff + sources[i*n + j];
    }
}
