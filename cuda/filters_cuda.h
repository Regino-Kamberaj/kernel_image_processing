//
// Created by Regino Kamberaj on 14/11/2026.
//

#ifndef KERNEL_IMG_PROCESSING_FILTERS_CUDA_H
#define KERNEL_IMG_PROCESSING_FILTERS_CUDA_H

#include "../include/image_io.h"

void box_blur_cuda_naive(const GrayImage& input, GrayImage& output, int kernel_size = 3);
void box_blur_cuda_tiled(const GrayImage& input, GrayImage& output, int kernel_size = 3);

void gaussian_blur_cuda_naive(const GrayImage& input, GrayImage& output);
void gaussian_blur_cuda_tiled(const GrayImage& input, GrayImage& output);

void sharpen_cuda_naive(const GrayImage& input, GrayImage& output);
void sharpen_cuda_tiled(const GrayImage& input, GrayImage& output);

#endif //KERNEL_IMG_PROCESSING_FILTERS_CUDA_H