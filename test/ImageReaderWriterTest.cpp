//
// Created by sensi on 09/07/2024.
//

#include "gtest/gtest.h"
#include "../ImageFileHandler.h"
#include "../ImageReaderWriter.h"

TEST(ImageFileReaderWriter, ReadingEmptyFile){
    ImageFileHandler file(R"(C:\Users\sensi\CLionProjects\LabProg\test\fileVuoto.pgm)");
    BaseImage * img = createImage(file);
    EXPECT_FALSE(img); //empty file
    ImageFileHandler file1(R"(C:\Users\sensi\CLionProjects\LabProg\test\validFile.ppm)");
    img = createImage(file1);
    EXPECT_TRUE(img);//valid file
    delete img;
}

TEST(ImageFileReaderWriter, WritingOnInvalidFile){
    ImageFileHandler file(R"(C:\Users\sensi\CLionProjects\LabProg\test\fileVuoto.pgm)");
    ImageFileHandler file1(R"(C:\Users\sensi\CLionProjects\LabProg\test\validFile.ppm)");
    BaseImage * img = createImage(file1);
    EXPECT_THROW(writeImagefile(img, file), std::logic_error); //different files format
    delete img;
}