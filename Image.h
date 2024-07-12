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
    Image(int n, int w, int h, T max, const std::string& format): height(h), width(w), max_value(max), format(format){
        //Allocating space for the image and setting every pixel
        image = new Pixel<T>**[h];
        for(int i = 0; i < h; i++){
            image[i] = new Pixel<T>*[w];
            for(int j = 0; j < w; j++){
                image[i][j] = new Pixel<T>(n);
            }
        }
        channels = n;
    }

    //copy constructor
    Image(const Image<T>& img): height(img.getHeight()), width(img.getWidth()), max_value(img.getMaxValue()), format(img.getFormat()), channels(img.getChannels()){
        image = new Pixel<T>**[height];
        for(int i = 0; i < height; i++){
            image[i] = new Pixel<T>*[width];
            for(int j = 0; j < width; j++){
                if(img.getPixel(i, j) != nullptr) {
                    image[i][j] = new Pixel<T>(*img.getPixel(i, j));
                }
            }
        }
    }

    void setPixel(Pixel<T> * p, int i, int j) noexcept(false){
        if(i >= height || i < 0 || j < 0 || j >= width){ //if given indexes are wrong this method throws an exception
            throw std::out_of_range("Pixel out of range");
        }else{
            image[i][j] = p;
        }
    }

    Pixel<T> * getPixel(int i, int j) const noexcept(false){//if given indexes are wrong this method throws an exception
        if(i >= height || i < 0 || j < 0 || j >= width) {
            throw std::out_of_range("Pixel out of range");
        }
        return image[i][j];
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

    ~Image(){ // cleaning memory from used space
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                if(image[i][j]!= nullptr)
                    delete image[i][j];
            }
            delete image[i];
        }
        delete image;
    }

    static Image<T> * rgbtogray(Image<T> * img){ //method that transform a rgb image (3 channels) to a gray scale image (1 channel) based on the NTSC formula
        std::vector<float> rgb2gray = {0.299, 0.587, 0.114};
        auto * image1 = new Image<T>(1,img->getWidth(), img->getHeight(), img->getMaxValue(), "P2");
        for(int i = 0; i < img->getHeight(); i++) {
            for (int j = 0; j < img->getWidth(); j++) {
                float resultGray = 0;
                image1->setPixel(new Pixel<T>(1), i, j);
                for (int k = 0; k < image1->getPixel(i, j)->getDim(); ++k) {
                    resultGray += (img->getPixel(i, j)->getChannel(k)) * rgb2gray[k];
                }
                image1->getPixel(i, j)->setChannel(0, static_cast<T>(resultGray));
            }
        }
        return image1;
    }

    static Image<T> * createImage(const std::string& path){
        int h, w;
        T max_value;
        std::string line, format;
        Image<T> * image;
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
                    int channels;
                    if (format == "P2") channels = 1;
                    else channels = 3;
                    image = new Image<T>(channels, w, h, max_value, format);
                    int i = 0, j = 0;
                    std::vector<T> pixels;
                    //reading pixels
                    while (!file.eof()) {
                        std::getline(file, line);
                        if (!line.empty() && line[0] != '#') {
                            splitString = split(line, " ");
                            for (auto &el: splitString) { //reads 3 channels for every pixel
                                if (!el.empty() && el != " ") {
                                    pixels.push_back(static_cast<T>(std::stof(el)));
                                    j++;
                                }
                                if (j == channels) {
                                    auto *pixel = new Pixel<T>(channels);
                                    for (int k = 0; k < j; k++) {
                                        pixel->setChannel(k, pixels[k]);
                                    }
                                    image->setPixel(pixel, i / w, i % w);
                                    i++;
                                    j = 0;
                                    pixels.clear();
                                }
                            }
                        }
                    }
                    file.seekg(0);
                    //checks if all the pixels are initialised
                    for (int k = 0; i < image->getHeight(); i++) {
                        for (int s = 0; j < image->getWidth(); j++) {
                            for (int c = 0; k < image->getPixel(k, s)->getDim(); k++) {
                                if (image->getPixel(k, s)->getChannel(c) == -1) return nullptr;
                            }
                        }
                    }
                    file.close();
                    return image;
                }else{
                    file.close();
                    return nullptr;
                }
            }else{
                file.close();
                return nullptr;
            }
        }else{
            file.close();
            return nullptr;
        }
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
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                for(int k = 0; k < channels; k++) {
                    if(image[i][j] == nullptr){
                        throw std::logic_error("Given image isn't valid, some pixel aren't initialized");
                    }else if(image[i][j]->getChannel(k) == -1){
                        throw std::logic_error("Given image isn't valid, some pixel aren't initialized");
                    }
                    std::string value = std::to_string(image[i][j]->getChannel(k)) + " ";
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

    bool isInitialized(){//checks if all pixels are initialized
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                if(image[i][j]){
                    for(int k = 0; k < channels; k++){
                        if(image[i][j]->getChannel(k) < 0) return false;
                    }
                }else{
                    return false;
                }
            }
        }
        return true;
    }

private:
    Pixel<T>*** image; //matrice di pixel che compone l'immagine
    int channels;
    int height;
    int width;
    T max_value;
    std::string format;
    static std::vector<std::string> split(const std::string& s, const std::string& delimiter) {
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

};

#endif //LABPROG_IMAGE_H
