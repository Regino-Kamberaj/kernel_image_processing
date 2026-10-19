//
// Created by Regino Kamberaj on 18/10/2026.
//

#include "filters.h"

#include <iostream>
#include <ostream>

static inline int clamp_int(int value, int low, int high) {
    return std::max(low, std::min(value, high));
}

// Grayscale functions
void box_blur_cpu(const GrayImage& input, GrayImage& output, int kernel_size) {
    if (kernel_size % 2 == 0) {
        std::cerr << "kernel size must be even" << std::endl;
        return;
    }

    output.width = input.width;
    output.height = input.height;
    output.max_val = input.max_val;
    output.data.resize(static_cast<size_t>(input.width) * input.height);

    const int radius = kernel_size / 2;
    const int area = kernel_size * kernel_size;
    const int w = input.width;
    const int h = input.height;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int sum = 0;

            // Convolution on kernel
            for (int i = -radius; i < radius; ++i) {
                for (int j = -radius; j < radius; ++j) {
                    int py = clamp_int(y+i, 0, h-1);
                    int px = clamp_int(x+j, 0, w-1);
                    sum += input.at(py, px);
                }
            }


            int val = sum / area;
            output.at(y, x) = static_cast<uint8_t>(clamp_int(val, 0, 255));
        }
    }

    std::cout << "[box_blur_cpu] complete:" << w << "x" << h
    << " (kernel " << kernel_size << "x" << kernel_size << ")" << std::endl;
}
void gaussian_blur_cpu(const GrayImage& input, GrayImage& output) {
    output.width = input.width;
    output.height = input.height;
    output.max_val = input.max_val;
    output.data.resize(static_cast<size_t>(input.width) * input.height);

    const int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };

    const int norm = 16;
    const int w = input.width;
    const int h = input.height;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int sum = 0;

            for (int i = -1; i < 1; ++i) {
                for (int j = -1; j < 1; ++j) {
                    int py = clamp_int(y+i, 0, h-1);
                    int px = clamp_int(x+j, 0, w-1);
                    sum += kernel[i+1][j+1] * input.at(py, px);
                }
            }
            output.at(y, x) = static_cast<uint8_t>(sum / norm);
        }
    }
    std::cout << "[gaussian_blur_cpu] complete:" << w << "x" << h << std::endl;
}
void sharpen_cpu(const GrayImage& input, GrayImage& output) {
    output.width = input.width;
    output.height = input.height;
    output.max_val = input.max_val;
    output.data.resize(static_cast<size_t>(input.width) * input.height);

    const int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0},
    };

    const int w = input.width;
    const int h = input.height;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int sum = 0;

            for (int i = -1; i < 1; ++i) {
                for (int j = -1; j < 1; ++j) {
                    int py = clamp_int(y+i, 0, h-1);
                    int px = clamp_int(x+j, 0, w-1);
                    sum += kernel[i+1][j+1] * input.at(py, px);
                }
            }
            output.at(y, x) = static_cast<uint8_t>(clamp_int(sum,0,255));
        }
    }
    std::cout << "[sharpen_cpu] complete:" << w << "x" << h << std::endl;
}

// RGB functions
void box_blur_cpu_rgb(const RGBImage& input, RGBImage& output, int kernel_size){
    if (kernel_size % 2 == 0) {
        std::cerr << "kernel size must be even" << std::endl;
        return;
    }

    output.width = input.width;
    output.height = input.height;
    output.max_val = input.max_val;
    output.data.resize(static_cast<size_t>(input.width) * input.height * 3);

    const int radius = kernel_size / 2;
    const int area = kernel_size * kernel_size;
    const int w = input.width;
    const int h = input.height;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int sum_r = 0, sum_g = 0, sum_b = 0;

            // Convolution on kernel
            for (int i = -radius; i < radius; ++i) {
                for (int j = -radius; j < radius; ++j) {
                    int py = clamp_int(y+i, 0, h-1);
                    int px = clamp_int(x+j, 0, w-1);
                    sum_r += input.at(py, px, 0);
                    sum_g += input.at(py, px, 1);
                    sum_b += input.at(py, px, 2);
                }
            }


            int val_r = sum_r / area;
            int val_g = sum_g / area;
            int val_b = sum_b / area;
            output.at(y, x,0) = static_cast<uint8_t>(clamp_int(val_r, 0, 255));
            output.at(y, x,1) = static_cast<uint8_t>(clamp_int(val_g, 0, 255));
            output.at(y, x,2) = static_cast<uint8_t>(clamp_int(val_b, 0, 255));
        }
    }

    std::cout << "[box_blur_cpu_rgb] complete:" << w << "x" << h
    << " (kernel " << kernel_size << "x" << kernel_size << ")" << std::endl;
}
void gaussian_blur_cpu_rgb(const RGBImage& input, RGBImage& output) {
    output.width = input.width;
    output.height = input.height;
    output.max_val = input.max_val;
    output.data.resize(static_cast<size_t>(input.width) * input.height*3);

    const int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };

    const int norm = 16;
    const int w = input.width;
    const int h = input.height;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int sum_r = 0, sum_g = 0, sum_b = 0;

            for (int i = -1; i < 1; ++i) {
                for (int j = -1; j < 1; ++j) {
                    int py = clamp_int(y+i, 0, h-1);
                    int px = clamp_int(x+j, 0, w-1);
                    int k = kernel[i+1][j+1];
                    sum_r += k * input.at(py, px,0);
                    sum_g += k * input.at(py, px,1);
                    sum_b += k * input.at(py, px,2);
                }
            }
            output.at(y, x,0) = static_cast<uint8_t>(sum_r / norm);
            output.at(y, x,1) = static_cast<uint8_t>(sum_g / norm);
            output.at(y, x,2) = static_cast<uint8_t>(sum_b / norm);
        }
    }
    std::cout << "[gaussian_blur_cpu_rgb] complete:" << w << "x" << h << std::endl;
}
void sharpen_cpu_rgb(const RGBImage& input, RGBImage& output) {
    output.width = input.width;
    output.height = input.height;
    output.max_val = input.max_val;
    output.data.resize(static_cast<size_t>(input.width) * input.height*3);

    const int kernel[3][3] = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0},
    };

    const int w = input.width;
    const int h = input.height;

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int sum_r = 0, sum_g = 0, sum_b = 0;

            for (int i = -1; i < 1; ++i) {
                for (int j = -1; j < 1; ++j) {
                    int py = clamp_int(y+i, 0, h-1);
                    int px = clamp_int(x+j, 0, w-1);
                    int k = kernel[i+1][j+1];
                    sum_r += k * input.at(py, px,0);
                    sum_g += k * input.at(py, px,1);
                    sum_b += k * input.at(py, px,2);
                }
            }
            output.at(y, x,0) = static_cast<uint8_t>(clamp_int(sum_r,0,255));
            output.at(y, x,1) = static_cast<uint8_t>(clamp_int(sum_g,0,255));
            output.at(y, x,2) = static_cast<uint8_t>(clamp_int(sum_b,0,255));
        }
    }
    std::cout << "[sharpen_cpu_rgb] complete:" << w << "x" << h << std::endl;
}
