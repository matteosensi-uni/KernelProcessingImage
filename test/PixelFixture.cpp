//
// Created by sensi on 09/07/2024.
//
#include "gtest/gtest.h"

#include "../Pixel.h"

//classe Fixture per testare la classe Image
class PixelFixture : public ::testing::Test {

protected:
    void SetUp() override {
        c = new Pixel<1>();
    }
    void TearDown() override{
        delete c;
    }
    BasePixel *c = nullptr;

};

TEST_F(PixelFixture, SettingValue){
    EXPECT_EQ(c->getChannel(0), -1);
    EXPECT_THROW(c->getChannel(3), std::out_of_range);
    EXPECT_THROW(c->setChannel(5, 3), std::out_of_range);
}

