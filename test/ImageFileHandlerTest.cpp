//
// Created by sensi on 09/07/2024.
//
#include "gtest/gtest.h"

#include "gtest/gtest.h"
#include "../ImageFileHandler.h"

TEST(ImageFileHandler, OpeningFile){
    EXPECT_THROW(ImageFileHandler file("prova"), std::logic_error); //test for not valid file
    auto * file = new ImageFileHandler("../../test/fileVuoto.pgm");
    EXPECT_TRUE(file); //test for valid file
    delete file;
}