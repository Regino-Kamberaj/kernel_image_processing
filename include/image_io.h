//
// Created by Regino Kamberaj on 27/09/2026.
//

#ifndef KERNEL_IMG_PROCESSING_IMAGE_IO_H
#define KERNEL_IMG_PROCESSING_IMAGE_IO_H
#include <vector>
#include <string>
#include <cstdint>

struct GrayImage {
    int width;
    int height;
    int max_val;
    std::vector<uint8_t> data; //dati di dimensione width * height => caratteri senza segno

    GrayImage(int width, int height, int max_val = 255) :
    width(width), height(height), max_val(max_val),
    data(static_cast<size_t>(width) * height){} // si fa il cast per evitare overflow

    GrayImage() = default;
    // Accesso in scrittura
    uint8_t& at(int y, int x) {
        return data[static_cast<size_t>(y) * width + x];
    }

    // Accesso in lettura
    const uint8_t& at(int y, int x) const {
        return data[static_cast<size_t>(y) * width + x];
    }

    size_t size() const { return data.size(); }
};

struct RGBImage {
    int width;
    int height;
    int max_val;
    std::vector<uint8_t> data; //dati di dimensione width * height * 3

    RGBImage(int width, int height, int max_val = 255) :
    width(width), height(height), max_val(max_val),
    data(static_cast<size_t>(width) * height * 3){}

    RGBImage() = default;

    // Accesso in scrittura (canale c => 0=R, 1=G, 2=B)
    uint8_t& at(int y, int x, int c) {
        return data[(static_cast<size_t>(y) * width + x) * 3 + c];
    }

    // Accesso in lettura
    const uint8_t& at(int y, int x, int c) const {
        return data[(static_cast<size_t>(y) * width + x) * 3 + c];
    }

    size_t size() const { return data.size(); }
    size_t num_pixels() const { return static_cast<size_t>(width) * height; }
};


// Carica un file PGM (PG5 binario). Torna true se ok
bool load_gray_image(const std::string& filename, GrayImage& image);
// Salva un immagine in scala di grigi come file PGM. Ritorna true se ok
bool save_gray_image(const GrayImage& image, const std::string& filename);
// Carica un file PPM (PG6 binario). Torna true se ok
bool load_RGB_image(const std::string& filename, RGBImage& image);
// Salva un immagine RGB come file PPM. Ritorna true se ok
bool save_RGB_image(const RGBImage& image, const std::string& filename);

#endif //KERNEL_IMG_PROCESSING_IMAGE_IO_H
