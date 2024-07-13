//
// Created by sensi on 12/07/2024.
//

#include "gtest/gtest.h"
#include "../Image.h"


TEST(ImageTest, ReadingEmptyFile){
    EXPECT_THROW(Image<int>::createImage("../../test/fileVuoto.pgm"), std::logic_error); //empty file
    EXPECT_NO_THROW(Image<int>::createImage("../../test/validFile.ppm")); //valid file
}

TEST(ImageTest, InvalidPath){
    EXPECT_THROW(Image<int>::createImage("pippo"), std::logic_error); //invalid path
    auto img = Image<int>::createImage("../../test/validFile.ppm");
    EXPECT_THROW(img.writeImage("pippo"), std::logic_error); //invalid path
}

TEST(ImageTest, WritingOnInvalidFile){
    auto img = Image<int>::createImage("../../test/validFile.ppm");
    EXPECT_THROW(img.writeImage("../../test/fileVuoto.pgm"), std::logic_error); //different files format
}

