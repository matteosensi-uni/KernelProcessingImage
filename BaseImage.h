//
// Created by sensi on 02/07/2024.
//

#ifndef LABPROG_BASEIMAGE_H
#define LABPROG_BASEIMAGE_H

#include "BasePixel.h"
#include <string>

class BaseImage {
public:
    virtual void setPixel(BasePixel* p, int i, int j) = 0;
    virtual  BasePixel* getPixel(int i, int j) const = 0;
    virtual int getHeight() const = 0;
    virtual void setHeight(int height) = 0;
    virtual int getWidth() const = 0;
    virtual void setWidth(int width) = 0;
    virtual int getMaxValue() const = 0;
    virtual void setMaxValue(int maxValue) = 0;
    virtual std::string getFormat() = 0;
    virtual void setFormat(const std::string& value) = 0;
    virtual bool compare(BaseImage * p) = 0;
    virtual ~BaseImage() {}
};


#endif //LABPROG_BASEIMAGE_H
