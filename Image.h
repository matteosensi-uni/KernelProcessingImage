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

template<int N>
class Image: public BaseImage{
public:
    Image(int w, int h, int max, const std::string& format): height(h), width(w), max_value(max), format(format){
        image = new Pixel<N>**[h];
        for(int i = 0; i < h; i++){
            image[i] = new Pixel<N>*[w];
            for(int j = 0; j < w; j++){
                image[i][j] = nullptr;
            }
        }
    }

    bool compare(BaseImage * img) override{
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

    void setPixel(BasePixel* p, int i, int j) override{
        if(i >= height || i < 0 || j < 0 || j >= width){
            throw std::out_of_range("Pixel out of range");
        }else{
            image[i][j] = dynamic_cast<Pixel<N>*>(p);
        }
    }

    BasePixel* getPixel(int i, int j) const override{
        if(i >= height || i < 0 || j < 0 || j >= width) {
            throw std::out_of_range("Pixel out of range");
        }
        return image[i][j];
    }

    int getHeight() const override {
        return height;
    }

    void setHeight(int h) override {
        if(h > 0) height = h;
        else throw std::logic_error("Height can't be negative");
    }

    int getWidth() const override {
        return width;
    }

    void setWidth(int w) override {
        if(w> 0) width = w;
        else throw std::logic_error("Width can't be negative");
    }

    int getMaxValue() const override {
        return max_value;
    }

    void setMaxValue(int maxValue) override {
        if(maxValue> 0) max_value = maxValue;
        else throw std::logic_error("Max value can't be negative");
    }

    std::string getFormat() override{
        return format;
    }
    void setFormat(const std::string& value) override{
        format = value;
    }

    ~Image() override{
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                delete image[i][j];
            }
            delete image[i];
        }
        delete image;
    }

    static Image<1> * rgbtogray(Image<3> * img){
        std::vector<float> rgb2gray = {0.3, 0.59, 0.11};
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
