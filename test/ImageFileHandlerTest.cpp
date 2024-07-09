//
// Created by sensi on 09/07/2024.
//
#include "gtest/gtest.h"

#include "gtest/gtest.h"
#include "../ImageFileHandler.h"

TEST(ImageFileHandler, OpeningFile){
    EXPECT_THROW(ImageFileHandler file("prova"), std::logic_error);
    auto * file = new ImageFileHandler(R"(C:\Users\sensi\Downloads\pbmlib.ascii.ppm)");
    EXPECT_TRUE(file);
    delete file;
}