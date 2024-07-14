//
// Created by sensi on 01/07/2024.
//

#ifndef LABPROG_IMAGE_H
#define LABPROG_IMAGE_H

#include <string>
#include <vector>
#include "Pixel.h"
#include <fstream>

template<typename  T>
class Image{
public:
    Image(const std::string& path){
        loadImage(path);
    }
    Image(int n, int w, int h, T max, const std::string& format): format(format){
        setWidth(w);
        setMaxValue(max);
        setHeight(h);
        if(n > 0 && n <= 4) channels = n;
        else throw std::logic_error("Number of channels must be >0 and <=4");
        for(int i = 0; i < h; i++){
            image.push_back(std::vector<Pixel<T>>());
            auto p = Pixel<T>(channels);
            for(int j = 0; j < width; j++) {
                for (int k = 0; k < channels; k++) {
                    p.setChannel(k, max_value);
                }
                image[i].push_back(p);
            }
        }

    }
    //copy constructor
    Image(const Image<T>& img): height(img.getHeight()), width(img.getWidth()), max_value(img.getMaxValue()), format(img.getFormat()), channels(img.getChannels()){
        for(int i = 0; i < height; i++){
            image.push_back(std::vector<Pixel<T>>());
            for(int j = 0; j < width; j++){
                auto p = Pixel<T>(channels);
                for(int k =0; k < channels; k++){
                    p.setChannel(k, img.getPixel(i, j).getChannel(k));
                }
                image[i].push_back(p);
            }
        }
    }

    void setPixel(Pixel<T>& p, int i, int j) noexcept(false){
        if(i >= height || i < 0 || j < 0 || j >= width){ //if given indexes are wrong this method throws an exception
            throw std::out_of_range("Pixel out of range");
        }else{
            image[i][j] = p;
        }
    }

    Pixel<T>& getPixel(int i, int j) const noexcept(false){//if given indexes are wrong this method throws an exception
        if(i >= height || i < 0 || j < 0 || j >= width) {
            throw std::out_of_range("Pixel out of range");
        }
        return const_cast<Pixel<T>&>(this->image[i][j]);
    }

    int getHeight() const {
        return height;
    }

    void setHeight(int h) noexcept(false) {
        if(h > 0) height = h;
        else throw std::logic_error("Height can't be negative or zero"); //if given height is wrong this method throws an exception
    }

    int getWidth() const {
        return width;
    }

    void setWidth(int w) noexcept(false) {
        if(w> 0) width = w;
        else throw std::logic_error("Width can't be negative"); //if given width is wrong this method throws an exception
    }

    T getMaxValue() const {
        return max_value;
    }

    void setMaxValue(T maxValue) noexcept(false) {
        if(maxValue> 0) max_value = maxValue;
        else{
            throw std::logic_error("Max value can't be negative"); //if given MaxVal is wrong this method throws an exception
        }
    }

    const std::string& getFormat() const{
        return format;
    }
    void setFormat(const std::string& value){
        format = value;
    }

    int getChannels() const {
        return channels;
    }

    static Image<T> * rgbtogray(Image<T>& img){ //method that transform a rgb image (3 channels) to a gray scale image (1 channel) based on the NTSC formula
        std::vector<float> rgb2gray = {0.299, 0.587, 0.114};
        if(img.isInitialized()) {
            auto *image1 = new Image<T>(1, img.getWidth(), img.getHeight(), img.getMaxValue(), "P2");
            for (int i = 0; i < img.getHeight(); i++) {
                for (int j = 0; j < img.getWidth(); j++) {
                    float resultGray = 0;
                    auto * p = new Pixel<T>(1);
                    image1->setPixel(*p, i, j);
                    for (int k = 0; k < image1->getPixel(i, j).getDim(); ++k) {
                        resultGray += (img.getPixel(i, j).getChannel(k)) * rgb2gray[k];
                    }
                    image1->getPixel(i, j).setChannel(0, static_cast<T>(resultGray));
                }
            }
            return image1;
        }else return nullptr;
    }

    void writeImage(const std::string& path){
        //checks if either the format of the file modified and image are equal
        if(format == "P2" && split(path, ".").back() != "pgm")
            throw std::logic_error("Writing file must be a pgm file");
        if(format == "P3" && split(path, ".").back() != "ppm")
            throw std::logic_error("Writing file must be a ppm file");
        std::ofstream file;
        file.open(path, std::ofstream::out | std::ofstream::trunc);
        if(!file){
            throw std::logic_error("Path non valido per la scrittura");
        }
        file<<format + "\n" + std::to_string(width)+" "+std::to_string(height)+"\n"+std::to_string(max_value)+"\n";
        int lineLength = 0;
        if(!isInitialized()){
            throw std::logic_error("Given image isn't valid, some pixel aren't initialized");
        }
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                for(int k = 0; k < channels; k++) {
                    std::string value = std::to_string(image[i][j].getChannel(k)) + " ";
                    if(lineLength + static_cast<int>(value.length()) > 70){
                        file<<"\n";
                        lineLength = static_cast<int>(value.length());
                    }else{
                        lineLength += static_cast<int>(value.length());
                    }
                    file<<value;
                }
            }
        }
        file.close();
    }

    void loadImage(const std::string& path){
        int h, w;
        std::string line;
        std::vector<std::string> splitString;
        std::fstream file;
        file.open(path);
        if(!file) throw std::logic_error("Path non valido per la lettura");
        do{//reads the format
            std::getline(file, line);
        }while(line != "P2" && line != "P3" && !file.eof());
        if(!file.eof()){
            format = line;
            do {
                std::getline(file, line);
            } while (line[0] == '#' || line.empty() || line == " " && !file.eof());//skips comments and empty lines
            if(!file.eof()) {
                splitString = split(line, " ");
                //reads width and height
                for (int i = 0, j = 0; i < splitString.size() && j < 2; i++) {
                    if (!splitString[i].empty() && splitString[i] != " " && j == 0) {
                        w = std::stoi(splitString[i]);
                        j++;
                    } else if (!splitString[i].empty() && splitString[i] != " " && j == 1) {
                        h = std::stoi(splitString[i]);
                        j++;
                    }
                }
                do {
                    std::getline(file, line);
                } while (line[0] == '#' || line.empty() || line == " " && !file.eof());//skips comments and empty lines
                if (!file.eof()) {
                    max_value = static_cast<T>(std::stof(line)); //reads maxval
                    if (format == "P2") channels = 1;
                    else channels = 3;
                    setHeight(h);
                    setWidth(w);
                    int i = 0, j = 0;
                    std::vector<T> pixels;
                    //reading pixels
                    image.clear();
                    for(int k = 0; k < h; k++){
                        image.push_back(std::vector<Pixel<T>>());
                        for(int s = 0; s < w; s++){
                            image[k].push_back(Pixel<T>(channels));
                        }
                    }
                    while (!file.eof()) {
                        std::getline(file, line);
                        if (!line.empty() && line[0] != '#') {
                            splitString = split(line, " ");
                            for (auto &el: splitString) {
                                if (!el.empty() && el != " ") {
                                    pixels.push_back(static_cast<T>(std::stof(el)));
                                    j++;
                                }
                                if (j == channels) {
                                    auto * pixel = new Pixel<T>(channels);
                                    for (int k = 0; k < j; k++) {
                                        pixel->setChannel(k, pixels[k]);
                                    }
                                    setPixel(*pixel, i / w, i % w);
                                    i++;
                                    j = 0;
                                    pixels.clear();
                                }
                            }
                        }
                    }
                    file.seekg(0);
                    //checks if all the pixels are initialised
                    if(!isInitialized()){
                        file.close();
                        throw std::logic_error("Pixel arent initialized");
                    }
                    file.close();
                }else{
                    file.close();
                    throw std::logic_error("Image file not valid");
                }
            }else{
                file.close();
                throw std::logic_error("Image file not valid");
            }
        }else{
            file.close();
            throw std::logic_error("Image file not valid");
        }
    }

private:
    std::vector<std::vector<Pixel<T>>> image;
    int channels;
    int height;
    int width;
    T max_value;
    std::string format;
    //method that split a string into a vector of substrings based on a delimiter
    std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
        size_t start = 0, end, delim_len = delimiter.length();
        std::string token; //element of the vector
        std::vector<std::string> res; // output vector

        while ((end = s.find(delimiter, start)) != std::string::npos) {
            token = s.substr (start, end - start);
            start = end + delim_len;
            res.push_back (token);
        }

        res.push_back (s.substr (start));
        return res;
    }
    bool isInitialized(){
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                if(!image[i][j].isInitialized()) return false;
            }
        }
        return true;
    }
};


#endif //LABPROG_IMAGE_H
