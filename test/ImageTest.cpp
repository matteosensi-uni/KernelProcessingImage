//
// Created by sensi on 12/07/2024.
//

#include "gtest/gtest.h"
#include "../Image.h"


TEST(ImageTest, ReadingEmptyFile){
    Image<int> * img = Image<int>::createImage("../../test/fileVuoto.pgm");
    EXPECT_FALSE(img); //empty file
    img = Image<int>::createImage("../../test/ValidFile.ppm");
    EXPECT_TRUE(img);//valid file
    delete img;
}

TEST(ImageTest, WritingOnInvalidFile){
    auto * img = Image<int>::createImage("../../test/validFile.ppm");
    EXPECT_THROW(img->writeImage("../../test/fileVuoto.pgm"), std::logic_error); //different files format
    delete img;
}

