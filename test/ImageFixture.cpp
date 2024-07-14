#include "gtest/gtest.h"

#include "../Image.h"


//classe Fixture per testare la classe Image
class ImageFixture : public ::testing::Test {

protected:
    Image<int> c = Image<int>(1, 100, 100, 255, "P2");

};

TEST_F(ImageFixture, SettingHeight){
    EXPECT_EQ(c.getHeight(), 100);
    EXPECT_THROW(c.setHeight(-100), std::logic_error);
}

TEST_F(ImageFixture, SettingWidth){
    EXPECT_EQ(c.getWidth(), 100);
    EXPECT_THROW(c.setWidth(-100), std::logic_error);
}

TEST_F(ImageFixture, SettingPixel){
    EXPECT_NO_THROW(c.getPixel(0,0));
    EXPECT_THROW(c.getPixel(100, 100), std::out_of_range);
    auto * p = new Pixel<int>(1);
    EXPECT_THROW(c.setPixel(*p, 10, -1), std::out_of_range);
}

TEST_F(ImageFixture, SettingMaxVal){
    EXPECT_EQ(c.getMaxValue(), 255);
    EXPECT_THROW(c.setMaxValue(-1), std::logic_error);
}