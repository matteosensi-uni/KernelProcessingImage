//
// Created by sensi on 02/07/2024.
//

#include "KernelImageProcessing.h"

//initialising all the default matrices for modifying the image
const std::map<std::string, std::vector<std::vector<float>>> KernelImageProcessing::defaultMatrices = {
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

//method that calculates the result pixel based on the matrix selected
int KernelImageProcessing::convolution(std::vector<std::vector<float>> kernel_matrix, int i , int j, int c){
    int sum = 0;
    int dim = (int)kernel_matrix.size();
    int w = image->getWidth(), h = image->getHeight();
    float el;
    int halfDim = (int)(dim/2);
    for(int k = 0; k < dim; k++) {
        for (int s = 0; s < dim; s++) {
            if(i+k-halfDim < 0){
                if(j+s-halfDim < 0) el = (float)(image->getPixel(0,0)->getChannel(c));
                else if(j + s -halfDim >= w) el = (float)(image->getPixel(0,w-1)->getChannel(c));
                else el = (float )(image->getPixel(0,j+s-halfDim)->getChannel(c));
            }else if(i+k-halfDim >= h){
                if(j+s-halfDim < 0) el = (float)(image->getPixel(h-1,0)->getChannel(c));
                else if(j + s -halfDim >= w) el = (float)(image->getPixel(h-1,w-1)->getChannel(c));
                else el = (float)(image->getPixel(h-1,j+s-halfDim)->getChannel(c));
            }else if(j+s-halfDim < 0){
                el = (float)(image->getPixel(i+k-halfDim,0)->getChannel(c));
            }else if(j+s-halfDim >= w){
                el = (float)(image->getPixel(i+k-halfDim,w-1)->getChannel(c));
            }else{
                el = (float)(image->getPixel(i+k-halfDim,j+s-halfDim)->getChannel(c));
            }
            sum += (int)(el * kernel_matrix[k][s]);
        }
    }
    return sum;
}

BaseImage * KernelImageProcessing::applyMethod(std::string method_name){
    int max;
    int min;
    std::vector<std::vector<float>> matrix_selected;
    for(auto& c: method_name){
        c = (char)tolower(c);
    }
    if(defaultMatrices.find(method_name) != defaultMatrices.end()) { //cechs if the method selected is correct
        matrix_selected = (static_cast<std::map<std::string, std::vector<std::vector<float>>>>(defaultMatrices))[method_name];
        BaseImage *image1, *tmp;
        if (image->getFormat() == "P3" && method_name == "ridge") {
            image1 = Image<3>::rgbtogray(dynamic_cast<Image<3> *>(image));
            tmp = image;
            image = new Image<1>(image1);
        } else {
            if (image->getFormat() == "P2") {
                image1 = new Image<1>(image);
            } else {
                image1 = new Image<3>(image);
            }
        }
        for (int i = 0; i < image1->getHeight(); i++) {
            for (int j = 0; j < image1->getWidth(); j++) {
                for (int k = 0; k < image1->getPixel(i, j)->getDim(); k++) {
                    int val = convolution(matrix_selected, i, j, k);
                    image1->getPixel(i, j)->setChannel(k, val);
                }
            }
        }
        //checking if there are negative pixels
        max = image1->getPixel(0, 0)->getChannel(0);
        min = image1->getPixel(0, 0)->getChannel(0);
        for (int i = 0; i < image1->getHeight(); i++) {
            for (int j = 0; j < image1->getWidth(); j++) {
                for (int k = 0; k < image1->getPixel(i, j)->getDim(); k++) {
                    int val = image1->getPixel(i, j)->getChannel(k);
                    if (max < val) max = val;
                    if (min > val) min = val;
                }
            }
        }
        if (min < 0) { //if there are some negative pixels all the pixels will be proportionally set positive
                       //also check if the max is over the limit of the format, in that case all the pixels will be formatted properly
            max = max - min;
            for (int i = 0; i < image1->getHeight(); i++) {
                for (int j = 0; j < image1->getWidth(); j++) {
                    for (int k = 0; k < image1->getPixel(i, j)->getDim(); k++) {
                        int val = image1->getPixel(i, j)->getChannel(k);
                        if (image1->getFormat() == "P2") {
                            val = (val < 0) ? 0 : (max > 65.535) ? (int) (((1.0 * (val - min)) / max) * 65.535) : val;
                        } else {
                            val = (val < 0) ? 0 : (max > 255) ? ((val - min) * 255) / max : val;
                        }
                        image1->getPixel(i, j)->setChannel(k, val);
                    }
                }

            }
            //resetting the maxval of the image
            if (image1->getFormat() == "P2") {
                if (max > 65535) max = 65535;
            } else {
                if (max > 255) max = 255;
            }
            image1->setMaxValue(max);
        }

        if (image->getFormat() == "P3" && method_name == "ridge") {
            delete image;
            image = tmp;
        }
        return image1;
    }else{
        return nullptr;
    }
}