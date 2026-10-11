//
// Created by Regino Kamberaj on 27/09/2026.
//
#include <fstream>
#include "image_io.h"

#include <iostream>

static void skipWhitespaceAndComments(std::istream & file) {
    while (true) {
        int c = file.peek();

        if (c == '#') {
            std::string line;
            std::getline(file, line);
        }else if (c == '\n' || c == '\r' || c == ' ' || c == '\t') {
            file.get();
        }else
            break;
    }
}

bool load_gray_image(const std::string &filename, GrayImage &image) {
    std::cout<<"******** Starting load_gray_image function *********"<<std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return false;
    }

    std::string format;
    file >> format;
    if (format != "P5") {
        std::cerr << "Could not parse file: " << filename
        << "\n format not supported: " << format  << std::endl;
        return false;
    }

    skipWhitespaceAndComments(file);

    int w = 0,h = 0,max_val = 0;
    file >> w >> h >> max_val;
    if (!file || w <= 0 || h <= 0 || max_val <= 0 || max_val > 255) {
        std::cerr << "Could not parse file: " << filename
        << "\n Header not valid " << std::endl;
        return false;
    }
    file.get();

    image.width = w;
    image.height = h;
    image.max_val = max_val;
    image.data.resize(static_cast<size_t>(w) * h);

    file.read(reinterpret_cast<char *>(image.data.data()), image.data.size());
    if (!file) {
        std::cerr << "Could not parse file: " << filename
        << "\n Reading stopped" << std::endl;
        return false;
    }

    return true;
}

bool save_gray_image(const GrayImage &image, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not save file: " << filename << std::endl;
        return false;
    }

    file << "P5\n" << image.width << " " << image.height << "\n" << image.max_val << "\n";
    file.write(reinterpret_cast<const char *>(image.data.data()), image.data.size());
    return static_cast<bool>(file);
}

bool load_RGB_image(const std::string &filename, RGBImage &image) {
    std::cout<<"******** Starting load_RGB_image function *********"<<std::endl;
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return false;
    }

    std::string format;
    file >> format;
    if (format != "P6") {
        std::cerr << "Could not parse file: " << filename
        << "\n format not supported: " << format  << std::endl;
        return false;
    }

    skipWhitespaceAndComments(file);

    int w = 0,h = 0,max_val = 0;
    file >> w >> h >> max_val;
    if (!file || w <= 0 || h <= 0 || max_val <= 0 || max_val > 255) {
        std::cerr << "Could not parse file: " << filename
        << "\n Header not valid " << std::endl;
        return false;
    }
    file.get();

    image.width = w;
    image.height = h;
    image.max_val = max_val;
    image.data.resize(static_cast<size_t>(w) * h * 3);

    file.read(reinterpret_cast<char *>(image.data.data()), image.data.size());
    if (!file) {
        std::cerr << "Could not parse file: " << filename
        << "\n Reading stopped" << std::endl;
        return false;
    }

    return true;
}

bool save_RGB_image(const RGBImage &image, const std::string &filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not save file: " << filename << std::endl;
        return false;
    }

    file << "P6\n" << image.width << " " << image.height << "\n" << image.max_val << "\n";
    file.write(reinterpret_cast<const char *>(image.data.data()), image.data.size());
    return static_cast<bool>(file);
}


