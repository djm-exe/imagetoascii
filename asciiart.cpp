#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <limits>
/*#include <filesystem>*/
/*#include <system_error>*/

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"



const std::string ascii_chars = "@%#*+=-:. ";

char mapPixelToAscii(int grayscale){
  int numchars = ascii_chars.size();
  return ascii_chars[numchars - 1 - (grayscale * (numchars - 1)) / 255];
}

void resizeImage(const unsigned char* src, int srcWidth, int srcHeight, unsigned char* dst, int dstWidth, int dstHeight) {
    float x_ratio = srcWidth / static_cast<float>(dstWidth);
    float y_ratio = srcHeight / static_cast<float>(dstHeight);
    float px, py;
    for (int i = 0; i < dstHeight; ++i) {
        for (int j = 0; j < dstWidth; ++j) {
            px = std::floor(j * x_ratio);
            py = std::floor(i * y_ratio);
            dst[i * dstWidth + j] = src[static_cast<int>((py * srcWidth + px))];
        }
    }
}

/*void createDirIfNotExist(const std::string& path){*/
/*  std::error_code e;*/
/*  if(!std::filesystem::exists(path)){*/
/*    std::filesystem::create_directory(path, e);*/
/*    if(e){*/
/*      std::cerr << "Error creating folder: " << e.message() << std::endl;*/
/*    }*/
/*  }*/
/*}*/

void imageToASCII(const std::string& imagePath){
  int width, height, channels;
  unsigned char* img = stbi_load(imagePath.c_str(), &width, &height, &channels, 1);
  if(!img){
    std::cerr << "Error: could not load image." << std::endl;
    return;
  }

  std::cout << "Image loaded: " << width << "x" << height << std::endl;

  int newWidth, newHeight;

  std::cout << "Enter desired width: ";
  std::cin >> newWidth;

  std::cout << "Enter desired height: ";
  std::cin >> newHeight;

  if (newWidth <= 0 || newHeight <= 0) {
    std::cerr << "Error: Width and height must be positive integers." << std::endl;
    stbi_image_free(img);
    return;
  }

  if (newWidth * newHeight > std::numeric_limits<std::size_t>::max() / sizeof(unsigned char)) {
        std::cerr << "Error: Resized image dimensions are too large!" << std::endl;
        stbi_image_free(img);
        return;
    }

  std::vector<unsigned char> resizedImage(newWidth * newHeight);

  resizeImage(img, width, height, resizedImage.data(), newWidth, newHeight);

  /*std::vector<unsigned char> outputImage(newWidth * newHeight * 3, 255);*/


  for (int y = 0; y < newHeight; ++y) {
    for (int x = 0; x < newWidth; ++x) {
      int grayscale = resizedImage[y * newWidth + x];
      char ascii = mapPixelToAscii(grayscale);


      /*int idx = (y * newWidth + x) * 3;*/
      /*if (ascii != ' '){*/
      /*  outputImage[idx] = 0;*/
      /*  outputImage[idx + 1] = 0;*/
      /*  outputImage[idx + 2] = 0;*/
      /*}*/
      std::cout << ascii;
    }
    std::cout << std::endl;
  }
  /*createDirIfNotExist("finished");*/

  /*stbi_write_png(outputPath.c_str(), newWidth, newHeight, 3, outputImage.data(), newWidth * 3);*/

  stbi_image_free(img);

}

int main(){
  std::string imagePath = "";
  std::cout << "Enter the path to the image: ";
  std::getline(std::cin, imagePath);

  /*std::string outputFileName = "";*/
  /*std::cout << "Enter the name for the output file (including extensions, e.g., output.png): ";*/
  /*std::getline(std::cin, outputFileName);*/
  /**/
  /*std::string outputPath = "finished/" + outputFileName;*/

  imageToASCII(imagePath);

  return 0;
}
