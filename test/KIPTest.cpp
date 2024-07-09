//
// Created by sensi on 09/07/2024.
//
#include "gtest/gtest.h"
#include "../KernelImageProcessing.h"

TEST(KernelImageProcessing, ImageNotValid){
    BaseImage * img = new Image<1>(100, 100, 255, "P2");
    EXPECT_THROW(KernelImageProcessing kip(img), std::logic_error);
}