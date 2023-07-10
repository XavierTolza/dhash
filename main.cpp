#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <filesystem>
#include <vector>
#include <string>

using namespace cv;

// Fonction pour calculer le dHash d'une image
std::uint32_t calculateDHash(const Mat& image) {
    // Convertir l'image en niveaux de gris
    Mat grayImage;
    cvtColor(image, grayImage, COLOR_BGR2GRAY);

    // Redimensionner l'image à une taille fixe
    Mat resizedImage;
    resize(grayImage, resizedImage, Size(9, 8));

    std::string dhash;
    std::uint64_t dhash_int=0;

    // Calculer la différence entre les pixels adjacents
    size_t i=0;
    for (int row = 0; row < resizedImage.rows; row++) {
        for (int col = 0; col < resizedImage.cols - 1; col++) {
            uchar leftPixel = resizedImage.at<uchar>(row, col);
            uchar rightPixel = resizedImage.at<uchar>(row, col + 1);
            dhash += (leftPixel > rightPixel) ? "1" : "0";
            dhash_int += ((uint64_t)(leftPixel > rightPixel))<<(63-i++);
        }
    }

    return dhash_int;
}

void ListFilesRecursive(const std::filesystem::path& path)
{
    if (!std::filesystem::exists(path)) {
        std::cout << "Invalid path: " << path << std::endl;
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_directory(entry)) {
            // Recursive call for subdirectories
            ListFilesRecursive(entry.path());
        } else if (std::filesystem::is_regular_file(entry)) {
            Mat image = imread(entry.path().string());
            if (!image.empty()) {
                // Calculer le dHash de l'image
                auto dhash = calculateDHash(image);
                // Print the file path
                std::cout << dhash << ";" << entry.path().string() << std::endl;
            }

        }
    }
}



struct Config {
    std::string inputFolder;
    std::vector<std::string> fileExtensions;
    std::string cacheFolder;
    int maxDistance;
};

Config ParseArgs(int argc, char* argv[])
{
    Config config;
    config.fileExtensions = {"jpg", "png", "jpeg"};
    config.cacheFolder = "/tmp/.dhash_cache";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-i") {
            if (i + 1 < argc) {
                config.inputFolder = argv[i + 1];
                ++i;
            } else {
                std::cout << "Missing argument for -i" << std::endl;
            }
        } else if (arg == "-e") {
            if (i + 1 < argc) {
                std::string extensionsStr = argv[i + 1];
                size_t pos = 0;
                std::string delimiter = ",";
                while ((pos = extensionsStr.find(delimiter)) != std::string::npos) {
                    std::string extension = extensionsStr.substr(0, pos);
                    config.fileExtensions.push_back(extension);
                    extensionsStr.erase(0, pos + delimiter.length());
                }
                config.fileExtensions.push_back(extensionsStr);
                ++i;
            } else {
                std::cout << "Missing argument for -e" << std::endl;
            }
        } else if (arg == "-c") {
            if (i + 1 < argc) {
                config.cacheFolder = argv[i + 1];
                ++i;
            } else {
                std::cout << "Missing argument for -c" << std::endl;
            }
        } else if (arg == "-d") {
            if (i + 1 < argc) {
                config.maxDistance = std::stoi(argv[i + 1]);
                ++i;
            } else {
                std::cout << "Missing argument for -d" << std::endl;
            }
        }
    }

    return config;
}

int main(int argc, char* argv[])
{
    Config config = ParseArgs(argc, argv);

    ListFilesRecursive(config.inputFolder);
    // // Charger l'image à partir d'un fichier
    Mat image = imread("/media/xavier/DATA/Souvenirs");

    

    // printf("dHash de l'image : %s\n", dhash.c_str());

    return 0;
}