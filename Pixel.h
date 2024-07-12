//
// Created by sensi on 02/07/2024.
//

#ifndef LABPROG_PIXEL_H
#define LABPROG_PIXEL_H

#include <stdexcept>

template<typename T>
class Pixel{
public:
    Pixel(int n){
        dim = n;
        channels = new int[dim];
        //setting all the channles to negative numbers
        for(int i = 0; i < dim; i++){
            channels[i] = -1;
        }
    }
    Pixel(const Pixel<T>& p){
        dim = p.getDim();
        channels = new int[dim];
        for(int i = 0; i < dim; i++){
            channels[i] = p.getChannel(i);
        }
    }
    ~Pixel(){//cleaning memory used
        delete[] channels;
    }
    T getChannel(int i) const noexcept(false){//this method throws an exception if the given index isnt valid
        if(i >= 0 && i < dim)
            return channels[i];
        else throw std::out_of_range("Index of channel out of range");
    }
    void setChannel(int i, T value) noexcept(false){//this method throws an exception if the given index isnt valid
        if(i < 0 || i >= dim ){
            throw std::out_of_range("Index of channel out of range");
        }
        channels[i] = value;
    }
    int getDim() const{
        return dim;
    }

private:
    T* channels;
    int dim;
};



#endif //LABPROG_PIXEL_H
