//
// Created by sensi on 09/07/2024.
//

#include "gtest/gtest.h"
#include "../ImageFileHandler.h"
#include "../ImageReaderWriter.h"

TEST(ImageFileReaderWriter, ReadingEmptyFile){
    ImageFileHandler file(R"(C:\Users\sensi\CLionProjects\LabProg\test\fileVuoto.pgm)");
    BaseImage * img = createImage(file);
    EXPECT_FALSE(img); //img è null perché non riesce a leggere correttamente il file nei due formati
    ImageFileHandler file1(R"(C:\Users\sensi\CLionProjects\LabProg\test\validFile.ppm)");
    img = createImage(file1);
    EXPECT_TRUE(img);
}