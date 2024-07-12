//
// Created by sensi on 09/07/2024.
//

//classe Fixture per testare la classe Image

#include "gtest/gtest.h"

#include "../Pixel.h"

class PixelFixture : public ::testing::Test {

protected:
    void SetUp() override {
        c = new Pixel<int>(1);
    }
    void TearDown() override{
        delete c;
    }
    Pixel<int> *c = nullptr;

};

TEST_F(PixelFixture, SettingValue){
EXPECT_EQ(c->getChannel(0), -1);
EXPECT_THROW(c->getChannel(3), std::out_of_range);
EXPECT_THROW(c->setChannel(5, 3), std::out_of_range);
}