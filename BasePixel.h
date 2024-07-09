//
// Created by sensi on 02/07/2024.
//

#ifndef LABPROG_BASEPIXEL_H
#define LABPROG_BASEPIXEL_H

class BasePixel{
public:
    virtual int getChannel(int i) const = 0;
    virtual void setChannel(int i, int value) = 0;
    virtual int getDim() const = 0;
    virtual bool compare(BasePixel * p) = 0;

};
#endif //LABPROG_BASEPIXEL_H
