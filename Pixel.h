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
        //setting all the channles to negative numbers
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
    bool compare(BasePixel * p) override{ //checks if two pixels are equal
        if(this->dim == p->getDim()){
            for(int i = 0; i < dim; i++){
                if(p->getChannel(i) != this->getChannel(i)) return false;
            }
            return true;
        }
        return false;
    }

    ~Pixel() override{//cleaning memory used
        delete[] channels;
    }

    int getChannel(int i) const noexcept(false) override{//this method throws an exception if the given index isnt valid
        if(i >= 0 && i < dim)
            return channels[i];
        else throw std::out_of_range("Index of channel out of range");
    }

    void setChannel(int i, int value) noexcept(false) override{//this method throws an exception if the given index isnt valid
        if(i < 0 || i >= dim ){
            throw std::out_of_range("Index of channel out of range");
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
