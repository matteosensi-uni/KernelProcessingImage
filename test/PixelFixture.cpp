//
// Created by sensi on 09/07/2024.
//

//classe Fixture per testare la classe Image

#include "gtest/gtest.h"

#include "../Pixel.h"

class PixelFixture : public ::testing::Test {

protected:
    void SetUp() override {
    }
    void TearDown() override{
    }
    Pixel<int> c =  Pixel<int>(1);

};

TEST_F(PixelFixture, SettingValue){
    EXPECT_FALSE(c.isInitialized());
    EXPECT_THROW(c.getChannel(0), std::logic_error); //pixel not initialized
    EXPECT_THROW(c.getChannel(3), std::out_of_range);
    EXPECT_THROW(c.setChannel(5, 3), std::out_of_range);
    c.setChannel(0, 2);
    EXPECT_TRUE(c.isInitialized());
}