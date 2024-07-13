//
// Created by sensi on 09/07/2024.
//
#include "gtest/gtest.h"
#include "../KernelImageProcessing.h"


TEST(KernelImageProcessing, ImageNotValid){
    auto  img = Image<int>(1, 100, 100, 255, "P2");
    EXPECT_THROW(KernelImageProcessing kip(&img), std::logic_error); //pixels arent initialised
}

TEST(KernelImageProcessing, MethodNotValid){
    auto  img = Image<int>::createImage("../../test/ValidFile.ppm");
    KernelImageProcessing kip(&img);
    EXPECT_THROW(kip.applyMethod("pippo"), std::logic_error);
}