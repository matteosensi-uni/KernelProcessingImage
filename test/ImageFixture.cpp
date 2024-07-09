#include "gtest/gtest.h"

#include "../Image.h"


class ImageSuite : public ::testing::Test {

protected:
    virtual void SetUp() {
        c = new Image<1>(100, 100, 255, "P2");

    }

    BaseImage* c = nullptr;  // XXX C++11 standard initialization
};

