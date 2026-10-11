#include <iostream>

#include "../include/image_io.h"

static const std::string DEFAULT =  "../test_images/pbmlib.pgm";

int main(int argc, char** argv) {
    std::string filename = DEFAULT;

    if (argc >= 2) {
        filename = argv[1]; // se l'utente passa un file un file usa quello
    }

    // Determino l'estensione
    auto dot_pos = filename.find_last_of('.');
    auto ext = filename.substr(dot_pos);
    auto slash_pos = filename.find_last_of('/');

    if (ext == ".pgm") {
        GrayImage img;
        if (!load_gray_image(filename, img)) {
            return 1;
        }

        std::cout << filename.substr(slash_pos+1) << " image loaded: " << img.width << "x" << img.height
        << " (max_val=" << img.max_val << ")" << std::endl;

        if (!save_gray_image(img, "test_output.pgm"))
            return 1;

        std::cout << "Save test_output.pgm" << std::endl;
    } else if (ext == ".ppm") {
        RGBImage img;
        if (!load_RGB_image(filename, img)) {
            return 1;
        }

         std::cout << filename.substr(slash_pos+1) << " image loaded: " << img.width << "x" << img.height
        << " (" << img.num_pixels() << " pixel RGB)" << std::endl;

        if (!save_RGB_image(img, "test_output.ppm"))
            return 1;

        std::cout << "Save test_output.ppm" << std::endl;
    } else {
        std::cerr << "Extension not supported."<<std::endl;
        return 1;
    }
}