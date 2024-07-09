//
// Created by sensi on 02/07/2024.
//

#ifndef LABPROG_PIXEL_H
#define LABPROG_PIXEL_H

#include "BasePixel.h"


template<int N>
class Pixel : public BasePixel{
public:
    Pixel(){
        dim =N;
        channels = new int[dim];
        for(int i = 0; i < dim; i++){
            channels[i] = -1;
        }
    }

    Pixel(BasePixel* p){
        dim =N;
        channels = new int[dim];
        for(int i = 0; i < dim; i++){
            channels[i] = p->getChannel(i);
        }
    }
    bool compare(BasePixel * p) override{
        if(this->dim == p->getDim()){
            for(int i = 0; i < dim; i++){
                if(p->getChannel(i) != this->getChannel(i)) return false;
            }
            return true;
        }
        return false;
    }

    ~Pixel() override{
        delete[] channels;
    }

    int getChannel(int i) const override{
        if(i >= 0 && i < dim)
            return channels[i];
        else throw std::out_of_range("Index of channel out of range");
    }

    void setChannel(int i, int value) override{
        if(i < 0 || i >= dim || value < 0){
            if(value < 0) throw std::logic_error("Cannot set a channel with negative value");
            else throw std::out_of_range("Index of channel out of range");
        }
        channels[i] = value;
    }

    int getDim() const override {
        return dim;
    }

private:
    int* channels;
    int dim;
};


#endif //LABPROG_PIXEL_H
