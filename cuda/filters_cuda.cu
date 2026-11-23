#include "filters_cuda.h"
#include <iostream>
#include <cstdlib>

#ifdef __CUDACC__
#include <cuda_runtime.h>

#define CHECK_CUDA(call) \
    do { \
        cudaError_t err = call; \
        if (err != cudaSuccess) { \
            std::cerr << "CUDA error " << cudaGetErrorString(err) \
                 << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

// Kernel naive => 1 thread = 1 pixel
__global__ void box_blur_naive_kernel(const uint8_t* input, uint8_t* output,
    int width, int height, int kernel_size) {
    int x = blockIdx.x * blockDim.x + threadIdx.x;
    int y = blockIdx.y * blockDim.y + threadIdx.y;

    if (x >= width || y >= height)
       return;

    int radius = kernel_size /2;
    int area = kernel_size * kernel_size;
    int sum = 0;

    for(int ky = -radius; ky <= radius; ky++) {
        for(int kx = -radius; kx <= radius; kx++) {
            int py = min(max(y+ky, 0), height - 1);
            int px = min(max(x+kx, 0), width - 1);
            sum += input[py * width + px];
        }
    }

    output[y * width + x] = static_cast<uint8_t>(sum / area);
}

void box_blur_cuda_naive(const GrayImage& input, GrayImage& output, int kernel_size){
    std::cout << "***** Starting box_blur_cuda_naive function *******"
        << std::endl;

    output.width = input.width;
    output.height = input.height;
    output.max_val = input.max_val;
    output.data.resize(input.data.size());

    size_t input_size = input.data.size() * sizeof(uint8_t);

    uint8_t* input_data = nullptr;
    CHECK_CUDA(cudaMalloc(&input_data, input_size));
    uint8_t* output_data = nullptr;
    CHECK_CUDA(cudaMalloc(&output_data, input_size));
    CHECK_CUDA(cudaMemcpy( input_data, input.data.data(),
                  input_size, cudaMemcpyHostToDevice));

    dim3 blockSize(16,16);
    dim3 gridSize((input.width + blockSize.x - 1) / blockSize.x,
     (input.height + blockSize.y - 1) / blockSize.y);

    box_blur_naive_kernel<<<gridSize, blockSize>>>(input_data, output_data, input.width, input.height, kernel_size);

    CHECK_CUDA(cudaGetLastError());
    CHECK_CUDA(cudaDeviceSynchronize());
    CHECK_CUDA(cudaMemcpy(output.data.data(), output_data,
     input_size, cudaMemcpyDeviceToHost));

    CHECK_CUDA(cudaFree(input_data));
    CHECK_CUDA(cudaFree(output_data));

    std::cout << "***** Finished box_blur_cuda_naive function: "
    << input.width << "x" << input.height << " *****" << std::endl;
}

#define TILE_SIZE 16
#define HALO 1

__global__ void box_blur_tiled_kernel(const uint8_t* __restrict__ input,
    uint8_t* output, int width, int height) {
    __shared__ uint8_t tile[TILE_SIZE + 2*HALO][TILE_SIZE + 2*HALO];

    int tx = threadIdx.x;
    int ty = threadIdx.y;
    int x = blockIdx.x*TILE_SIZE +tx;
    int y = blockIdx.y*TILE_SIZE +ty;

    // ogni thread carica solo il suo pixel e non i 9 vicini
    if (x < width && y < height) {
        tile[ty+HALO][tx+HALO] = input[y*width + x];
    }

    __syncthreads();

    int sum = 0;
    for(int dy = -HALO; dy <= HALO; dy++) {
        for(int dx = -HALO; dx <= HALO; dx++) {
            sum += tile[ty+dy+HALO][tx+dx+HALO];
        }
    }

    output[y * width + x] = static_cast<uint8_t>(sum / 9);
}

void box_blur_cuda_tiled(const GrayImage& input, GrayImage& output, int kernel_size){
    std::cout << "***** Starting box_blur_cuda_tiled function *******" << std::endl;

    output.width = input.width;
    output.height = input.height;
    output.max_val = input.max_val;
    output.data.resize(input.data.size());

    size_t input_size = input.data.size() * sizeof(uint8_t);

    uint8_t* input_data = nullptr;
    CHECK_CUDA(cudaMalloc(&input_data, input_size));
    uint8_t* output_data = nullptr;
    CHECK_CUDA(cudaMalloc(&output_data, input_size));
    CHECK_CUDA(cudaMemcpy(input_data, input.data.data(), input_size, cudaMemcpyHostToDevice));

    dim3 blockSize(TILE_SIZE,TILE_SIZE);
    dim3 gridSize((input.width + TILE_SIZE - 1) / TILE_SIZE,
     (input.height + TILE_SIZE - 1) / TILE_SIZE);

    box_blur_tiled_kernel<<<gridSize,blockSize>>>(input_data, output_data, input.width, input.height);

    CHECK_CUDA(cudaGetLastError());
    CHECK_CUDA(cudaDeviceSynchronize());
    CHECK_CUDA(cudaMemcpy(output.data.data(), output_data, input_size, cudaMemcpyDeviceToHost));

    CHECK_CUDA(cudaFree(input_data));
    CHECK_CUDA(cudaFree(output_data));

    std::cout << "***** Finished box_blur_cuda_tiled function: " << input.width << "x" << input.height << " *****" << std::endl;
}

#else
void box_blur_cuda_naive(const GrayImage& input, GrayImage& output, int kernel_size){
    std::cerr << "***** Error on starting box_blur_cuda_naived: CUDA not working ******" << std::endl;
}

void box_blur_cuda_tiled(const GrayImage& input, GrayImage& output, int kernel_size){
    std::cerr << "***** Error on starting box_blur_cuda_tiled: CUDA not working ******" << std::endl;
}

#endif