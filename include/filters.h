//
// Created by Regino Kamberaj on 18/10/2026.
//

#ifndef KERNEL_IMG_PROCESSING_FILTERS_H
#define KERNEL_IMG_PROCESSING_FILTERS_H
#include "image_io.h"

// Grayscale functions
void box_blur_cpu(const GrayImage& input, GrayImage& output, int kernel_size=3);
void gaussian_blur_cpu(const GrayImage& input, GrayImage& output);
void sharpen_cpu(const GrayImage& input, GrayImage& output);

// RGB functions
void box_blur_cpu_rgb(const RGBImage& input, RGBImage& output, int kernel_size=3);
void gaussian_blur_cpu_rgb(const RGBImage& input, RGBImage& output);
void sharpen_cpu_rgb(const RGBImage& input, RGBImage& output);

#endif //KERNEL_IMG_PROCESSING_FILTERS_H