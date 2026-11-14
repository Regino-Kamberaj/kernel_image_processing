#include <iomanip>
#include <iostream>

#include "filters.h"
#include "../include/image_io.h"

static const std::string DEFAULT_INPUT =  "../test_images/pbmlib.pgm";
static const std::string DEFAULT_FILTER=  "box";
static const int NUM_ITERATIONS = 100;

static void execute_filter_pgm(std::string &filter, GrayImage &img, GrayImage& output) {
    if (filter == "box") {
        box_blur_cpu(img, output);
    }else if (filter == "gaussian") {
        gaussian_blur_cpu(img, output);
    }else if (filter == "sharpen") {
        sharpen_cpu(img, output);
    }else {
        std::cerr << "Unsupported filter." << std::endl;
        //return 1;
    }
}
static void execute_filter_ppm(std::string &filter, RGBImage &img, RGBImage& output) {
    if (filter == "box") {
        box_blur_cpu_rgb(img, output);
    }else if (filter == "gaussian") {
        gaussian_blur_cpu_rgb(img, output);
    }else if (filter == "sharpen") {
        sharpen_cpu_rgb(img, output);
    }else {
        std::cerr << "Unsupported filter." << std::endl;
        //return 1;
    }
}


int main(int argc, char** argv) {
    std::string filename = DEFAULT_INPUT;
    std::string filter = DEFAULT_FILTER;
    int iterations = NUM_ITERATIONS;

    if (argc >= 4) {
        filename = argv[1]; // se l'utente passa un file un file usa quello
        filter = argv[2];
        iterations = std::stoi(argv[3]);
    }

    std::cout << "Input: " << filename
    << "\tFilter: " << filter << std::endl;

    // Determino l'estensione
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos) {
        std::cerr << "Input without extension." << std::endl;
        return 1;
    }
    std::string ext = filename.substr(dot_pos);

    if (ext == ".pgm") {
        GrayImage img;
        if (!load_gray_image(filename, img)) {
            return 1;
        }

        std::cout << " PGM image loaded: " << img.width << "x" << img.height
        << " (max_val=" << img.max_val << ")" << std::endl;

        GrayImage output;

        execute_filter_pgm(filter, img, output);

        auto t0 = std::chrono::steady_clock::now();
        for (int i = 0; i < iterations; ++i) {
            execute_filter_pgm(filter, img, output);
        }

        auto t1 = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(t1 - t0).count();
        double avg = elapsed / iterations;

        std::cout << " Elapsed cpu time: " << std::fixed << std::setprecision(3) << avg
        << " ms over " << iterations  << " iterations" << std::endl;

        std::string outname = "test_output_" + filter + ext;
        if (!save_gray_image(output, outname))
            return 1;

        std::cout << "Saved " << outname << std::endl;
    } else if (ext == ".ppm") {
        RGBImage img;
        if (!load_RGB_image(filename, img)) {
            return 1;
        }

        std::cout << " PPM image loaded: " << img.width << "x" << img.height
        << " (max_val=" << img.max_val << ")" << std::endl;

        RGBImage output;
        auto t0 = std::chrono::steady_clock::now();
        for (int i = 0; i < iterations; ++i) {
            execute_filter_ppm(filter, img, output);
        }

        auto t1 = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double, std::milli>(t1 - t0).count();
        double avg = elapsed / iterations;

        std::cout << " Elapsed cpu time: " << std::fixed << std::setprecision(3) << avg
        << " ms over " << iterations  << " iterations" << std::endl;

        std::string outname = "test_output_" + filter + ext;
        if (!save_RGB_image(output, outname))
            return 1;

        std::cout << "Saved " << outname << std::endl;
    } else {
        std::cerr << "Input extension not supported."<<std::endl;
        return 1;
    }
}