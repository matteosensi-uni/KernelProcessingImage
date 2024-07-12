//
// Created by sensi on 02/07/2024.
//

#ifndef LABPROG_KERNELIMAGEPROCESSING_H
#define LABPROG_KERNELIMAGEPROCESSING_H

#include <vector>
#include <map>
#include "Image.h"
template<typename T>
class KernelImageProcessing {
public:
    const static  std::map<std::string, std::vector<std::vector<float>>> defaultMatrices;
    explicit KernelImageProcessing(Image<T> * img) noexcept(false) {
        //checking if the given image is valid
        if(!img){
            throw std::logic_error("Image must be initialised");
        }
        if(img->isInitialized()) image = img;
        else throw std::logic_error("Image must be initialised");
    }
    Image<T> * identity() { return applyMethod("identity");}
    Image<T> * ridge() { return applyMethod("ridge"); }
    Image<T> * sharpen() { return applyMethod("sharpen");}
    Image<T> * boxBlur() { return applyMethod("boxblur");}
    Image<T> * gaussianBlur3x3() { return applyMethod("gaussianblur3");}
    Image<T> * gaussianBlur5x5() { return applyMethod("gaussianblur5");}
    Image<T> * unsharpMasking() { return applyMethod("unsharpmasking");}
    Image<T> * applyMethod(std::string method_name);
private:
    int convolution(std::vector<std::vector<float>> kernel_matrix, int i, int j, int c){ //calculates resulting pixel after modifying it
        float sum = 0;
        int dim = (int)kernel_matrix.size();
        int w = image->getWidth(), h = image->getHeight();
        float el;
        int halfDim = (int)(dim/2);
        for(int k = 0; k < dim; k++) {
            for (int s = 0; s < dim; s++) {
                if(i+k-halfDim < 0){
                    if(j+s-halfDim < 0) el = static_cast<float>(image->getPixel(0,0)->getChannel(c));
                    else if(j + s -halfDim >= w) el = static_cast<float>(image->getPixel(0,w-1)->getChannel(c));
                    else el = static_cast<float>(image->getPixel(0,j+s-halfDim)->getChannel(c));
                }else if(i+k-halfDim >= h){
                    if(j+s-halfDim < 0) el = static_cast<float>(image->getPixel(h-1,0)->getChannel(c));
                    else if(j + s -halfDim >= w) el = static_cast<float>(image->getPixel(h-1,w-1)->getChannel(c));
                    else el = static_cast<float>(image->getPixel(h-1,j+s-halfDim)->getChannel(c));
                }else if(j+s-halfDim < 0){
                    el = static_cast<float>(image->getPixel(i+k-halfDim,0)->getChannel(c));
                }else if(j+s-halfDim >= w){
                    el = static_cast<float>(image->getPixel(i+k-halfDim,w-1)->getChannel(c));
                }else{
                    el = static_cast<float>(image->getPixel(i+k-halfDim,j+s-halfDim)->getChannel(c));
                }
                sum += (el * kernel_matrix[k][s]);
            }
        }
        return static_cast<T>(sum);
    }
    Image<T> * image;
};

template<typename T>
Image<T>* KernelImageProcessing<T>::applyMethod(std::string method_name) {
    std::vector<std::vector<float>> matrix_selected;
    for(auto& c: method_name){
        c = (char)tolower(c);
    }
    if(defaultMatrices.find(method_name) != defaultMatrices.end()) { //checks if the method selected is correct
        matrix_selected = (static_cast<std::map<std::string, std::vector<std::vector<float>>>>(defaultMatrices))[method_name];
        Image<T> * image1;
        Image<T> *tmp = nullptr;
        if (image->getFormat() == "P3" && method_name == "ridge") {
            image1 = Image<T>::rgbtogray(image);
            tmp = image;
            image =  new Image<T>(*image1);
        } else {
            image1 = new Image<T>(*image);
        }
        for (int i = 0; i < image1->getHeight(); i++) {
            for (int j = 0; j < image1->getWidth(); j++) {
                for (int k = 0; k < image1->getChannels(); k++) {
                    T val = convolution(matrix_selected, i, j, k);
                    image1->getPixel(i, j)->setChannel(k, val);
                }
            }
        }
        //pixel normalization
        //calculating max and min intensity of each pixel of the modified image
        T newMax = image1->getPixel(0, 0)->getChannel(0);
        T newMin = image1->getPixel(0, 0)->getChannel(0);
        for (int i = 0; i < image1->getHeight(); i++) {
            for (int j = 0; j < image1->getWidth(); j++) {
                for (int k = 0; k < image1->getChannels(); k++) {
                    T val = image1->getPixel(i, j)->getChannel(k);
                    if (newMax < val) newMax = val;
                    if (newMin > val) newMin = val;
                }
            }
        }
        //calculating max and min intensity of each pixel of the base image
        T max = image->getPixel(0, 0)->getChannel(0);
        T min = image->getPixel(0, 0)->getChannel(0);
        for (int i = 0; i < image->getHeight(); i++) {
            for (int j = 0; j < image->getWidth(); j++) {
                for (int k = 0; k < image->getChannels(); k++) {
                    T val = image->getPixel(i, j)->getChannel(k);
                    if (max < val) max = val;
                    if (min > val) min = val;
                }
            }
        }
        //normalizing each pixel of the new image
        for (int i = 0; i < image1->getHeight(); i++) {
            for (int j = 0; j < image1->getWidth(); j++) {
                for (int k = 0; k < image1->getChannels(); k++) {
                    int val = image1->getPixel(i, j)->getChannel(k);
                    val = (T)((1.0*(val-newMin))*(1.0*(max-min)/(newMax-newMin)));
                    image1->getPixel(i, j)->setChannel(k, val);
                }
            }
        }
        image1->setMaxValue(image->getMaxValue());

        if (tmp && method_name == "ridge") {
            delete image;
            image = tmp;
        }
        return image1;
    }else{
        return nullptr;
    }
}

template <typename T>
const std::map<std::string, std::vector<std::vector<float>>> KernelImageProcessing<T>::defaultMatrices ={
        {"identity", {{0,0,0},{0,1,0},{0,0,0}}},
        {"ridge", {{0,-1,0},{-1,4,-1},{0,-1,0}}},
        {"sharpen", {{0,-1,0},{-1,5,-1},{0,-1,0}}},
        {"boxblur", {{1.0/9,1.0/9,1.0/9},
                              {1.0/9,1.0/9,1.0/9},
                             {1.0/9,1.0/9,1.0/9}
                     }},
        {"gaussianblur3", {
                      {0.0625,0.125,0.0625},
                              {0.1250,0.250,0.1250},
                             {0.0625,0.125,0.0625}
                     }},
        {"gaussianblur5", {
                      {1.0/256,4.0/256,6.0/256,4.0/256,1.0/256},
                              {4.0/256,0.0625,0.09375,0.0625,4.0/256},
                             {6.0/256,0.09375,36.0/256,0.09375,6.0/256},
                             {4.0/256,0.0625,0.09375,0.0625,4.0/256},
                             {1.0/256,4.0/256,6.0/256,4.0/256,1.0/256}
                     }},
        {"unsharpmasking", {{-1.0 / 256, -4.0 / 256, -6.0 / 256, -4.0 / 256, -1.0 / 256},
                              {-4.0 / 256, -0.0625, -0.09375, -0.0625, -4.0 / 256},
                             {-6.0 / 256, -0.09375, 476.0 / 256, -0.09375, -6.0 / 256},
                             {-4.0 / 256, -0.0625, -0.09375, -0.0625, -4.0 / 256},
                             {-1.0 / 256, -4.0 / 256, -6.0 / 256, -4.0 / 256, -1.0 / 256}
                     }}
};


#endif //LABPROG_KERNELIMAGEPROCESSING_H
