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
        channels = new T[dim];
        //setting all the channles to negative numbers
        initialized = false;
    }
    Pixel(const Pixel<T>& p){
        dim = p.getDim();
        channels = new T[dim];
        if(p.isInitialized()){
            for(int i = 0; i < dim; i++){
                channels[i] = p.getChannel(i);
            }
        }
        initialized = p.isInitialized();
    }
    ~Pixel(){//cleaning memory used
        delete[] channels;
    }
    T getChannel(int i) const noexcept(false){//this method throws an exception if the given index isnt valid
        if(i >= 0 && i < dim)
            if(!initialized) throw std::logic_error("Channel isn't initialized");
            else return channels[i];
        else throw std::out_of_range("Index of channel out of range");
    }
    void setChannel(int i, T value) noexcept(false){//this method throws an exception if the given index isnt valid
        if(i < 0 || i >= dim ){
            throw std::out_of_range("Index of channel out of range");
        }
        initialized = true;
        channels[i] = value;
    }
    int getDim() const{
        return dim;
    }

    bool isInitialized() const {return initialized;}

private:
    T* channels;
    int dim;
    bool initialized;
};

#endif //LABPROG_PIXEL_H
