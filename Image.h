//
// Created by sensi on 01/07/2024.
//

#ifndef LABPROG_IMAGE_H
#define LABPROG_IMAGE_H

#include <string>
#include "ImageFileHandler.h"
#include <vector>
#include "Pixel.h"
#include "BaseImage.h"

//template class image used for rapresenting different type of images based on number of channels
template<int N>
class Image: public BaseImage{
public:
    Image(int w, int h, int max, const std::string& format): height(h), width(w), max_value(max), format(format){
        //Allocating space for the image and setting every pixel
        image = new Pixel<N>**[h];
        for(int i = 0; i < h; i++){
            image[i] = new Pixel<N>*[w];
            for(int j = 0; j < w; j++){
                image[i][j] = new Pixel<N>;
            }
        }
    }

    bool compare(BaseImage * img) override{
        //method that return true if either the classes are equal
        if(img!= nullptr) {
            for (int i = 0; i < height; i++) {
                for (int j = 0; j < width; ++j) {
                    if(img->getPixel(i, j) == nullptr) return false;
                    if (!getPixel(i, j)->compare(img->getPixel(i, j))) return false;
                }
            }
            return true;
        }else return false;
    }

    //copy constructor
    explicit Image(BaseImage * img): height(img->getHeight()), width(img->getWidth()), max_value(img->getMaxValue()), format(img->getFormat()){
        image = new Pixel<N>**[height];
        for(int i = 0; i < height; i++){
            image[i] = new Pixel<N>*[width];
            for(int j = 0; j < width; j++){
                if(img->getPixel(i, j) != nullptr) {
                    image[i][j] = new Pixel<N>(img->getPixel(i, j));
                }else{
                    image[i][j] = new Pixel<N>();
                }
            }
        }
    }

    void setPixel(BasePixel* p, int i, int j) noexcept(false) override{
        if(i >= height || i < 0 || j < 0 || j >= width){ //if given indexes are wrong this method throws an exception
            throw std::out_of_range("Pixel out of range");
        }else{
            image[i][j] = dynamic_cast<Pixel<N>*>(p);
        }
    }

    BasePixel* getPixel(int i, int j) const noexcept(false) override{//if given indexes are wrong this method throws an exception
        if(i >= height || i < 0 || j < 0 || j >= width) {
            throw std::out_of_range("Pixel out of range");
        }
        return image[i][j];
    }

    int getHeight() const override {
        return height;
    }

    void setHeight(int h) noexcept(false) override {
        if(h > 0) height = h;
        else throw std::logic_error("Height can't be negative or zero"); //if given height is wrong this method throws an exception
    }

    int getWidth() const override {
        return width;
    }

    void setWidth(int w) noexcept(false) override {
        if(w> 0) width = w;
        else throw std::logic_error("Width can't be negative"); //if given width is wrong this method throws an exception
    }

    int getMaxValue() const override {
        return max_value;
    }

    void setMaxValue(int maxValue) noexcept(false) override {
        if(maxValue> 0) max_value = maxValue;
        else{
            throw std::logic_error("Max value can't be negative"); //if given MaxVal is wrong this method throws an exception
        }
    }

    std::string getFormat() override{
        return format;
    }
    void setFormat(const std::string& value) override{
        format = value;
    }

    ~Image() override{ // cleaning memory from used space
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                if(image[i][j]!= nullptr)
                    delete image[i][j];
            }
            delete image[i];
        }
        delete image;
    }

    static Image<1> * rgbtogray(Image<3> * img){ //method that transform a rgb image (3 channels) to a gray scale image (1 channel) based on the NTSC formula
        std::vector<float> rgb2gray = {0.299, 0.587, 0.114};
        auto image1 = new Image<1>(img->getWidth(), img->getHeight(), img->getMaxValue(), "P2");
        for(int i = 0; i < img->getHeight(); i++) {
            for (int j = 0; j < img->getWidth(); j++) {
                float resultGray = 0;
                image1->setPixel(new Pixel<1>, i, j);
                for (int k = 0; k < image1->getPixel(i, j)->getDim(); ++k) {
                    resultGray += (float)(img->getPixel(i, j)->getChannel(k)) * rgb2gray[k];
                }
                image1->getPixel(i, j)->setChannel(0, (int)(resultGray));
            }
        }
        return image1;
    }

private:
    Pixel<N>*** image;
    int height;
    int width;
    int max_value;
    std::string format;
};


#endif //LABPROG_IMAGE_H
