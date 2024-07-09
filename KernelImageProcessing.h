//
// Created by sensi on 02/07/2024.
//

#ifndef LABPROG_KERNELIMAGEPROCESSING_H
#define LABPROG_KERNELIMAGEPROCESSING_H


#include <vector>
#include <map>
#include "Image.h"

class KernelImageProcessing {
public:
    const static  std::map<std::string, std::vector<std::vector<float>>> defaultMatrices;
    explicit KernelImageProcessing(BaseImage * img) noexcept(false) {
        //checking if the given image is valid
        if(!img){
            throw std::logic_error("Image must be initialised");
        }
        for(int i = 0; i < img->getHeight(); i++){
            for(int j = 0; j < img->getWidth(); j++){
                if(img->getPixel(i, j)){
                    for(int k = 0; k < img->getPixel(i, j)->getDim(); k++){
                        if(img->getPixel(i, j)->getChannel(k) < 0){
                            throw std::logic_error("Each pixel of the image must be initialised");
                        }
                    }
                }else{
                    throw std::logic_error("Each pixel of the image must be initialised");
                }
            }
        }
        image = img;
    }
    BaseImage * identity() { return applyMethod("identity");}
    BaseImage * ridge() { return applyMethod("ridge"); }
    BaseImage * sharpen() { return applyMethod("sharpen");}
    BaseImage * boxBlur() { return applyMethod("boxblur");}
    BaseImage * gaussianBlur3x3() { return applyMethod("gaussianblur3");}
    BaseImage * gaussianBlur5x5() { return applyMethod("gaussianblur5");}
    BaseImage * unsharpMasking() { return applyMethod("unsharpmasking");}
    BaseImage * applyMethod(std::string method_name);
private:
    int convolution(std::vector<std::vector<float>> kernel_matrix, int i, int j, int c);
    BaseImage * image;
};


#endif //LABPROG_KERNELIMAGEPROCESSING_H
