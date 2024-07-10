//
// Created by sensi on 01/07/2024.
//

#ifndef LABPROG_IMAGEREADERWRITER_H
#define LABPROG_IMAGEREADERWRITER_H

#include "Image.h"

//method that split a string into a vector based on a delimiter given
/*
 * EX: input: string:"c.ia.a.a" delimiter: '.'
 *     output: std::vector<string>: ["c", "ia", "a", "a"]
 * */
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

//creates a 3-channel/1-channel image based on the content of the given file
BaseImage * createImage(ImageFileHandler& file){
    int h, w;
    int max_value;
    std::string line;
    BaseImage * image;
    std::string format;
    std::vector<std::string> splitString;
    do{//reads the format
        line = file.getLine();
    }while(line != "P2" && line != "P3" && !file.eof());
    if(!file.eof()){
        format = line;
        do {
            line = file.getLine();
        } while (line[0] == '#' || line.empty() || line == " " && !file.eof());//skips comments and empty lines
        splitString = split(line, " ");
        //reads width and height
        for(int i = 0, j = 0; i < splitString.size() && j < 2; i++){
            if(!splitString[i].empty() && splitString[i]!=" " && j == 0){
                w = std::stoi(splitString[i]);
                j++;
            }else if(!splitString[i].empty() && splitString[i]!=" " && j == 1){
                h = std::stoi(splitString[i]);
                j++;
            }
        }
        do {
            line = file.getLine();
        } while (line[0] == '#' || line.empty() || line == " " && !file.eof());//skips comments and empty lines
        max_value = std::stoi(line); //reads maxval
        if(format == "P2") image = new Image<1>(w, h, max_value, format);
        else image = new Image<3>(w, h, max_value, format);
        int i = 0, j = 0;
        std::vector<int> pixels;
        //reading pixels
        while (!file.eof()) {
            line = file.getLine();
            if (!line.empty() && line[0] != '#') {
                splitString = split(line, " ");
                if(format == "P2"){
                    for (auto &el: splitString) { //reads 1 channel for every pixel
                        if (!el.empty() && el != " "){
                            auto * pixel = new Pixel<1>();
                            pixel->setChannel(0, std::stoi(el));
                            image->setPixel(pixel, i / w, i % w);
                            i++;
                        }
                    }
                }else {
                    for (auto &el: splitString) { //reads 3 channels for every pixel
                        if (!el.empty() && el != " ") {
                            pixels.push_back(std::stoi(el));
                            j++;
                        }
                        if (j == 3) {
                            auto *pixel = new Pixel<3>();
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
        }
        file.resetIndex();
        //checks if all the pixels are initialised
        for(int k = 0; i < image->getHeight(); i++){
            for(int s = 0; j < image->getWidth(); j++){
                if(image->getPixel(k, s) == nullptr) return nullptr;
                else{
                    for (int c = 0; k < image->getPixel(k, s)->getDim(); k++) {
                        if(image->getPixel(k, s)->getChannel(c) == -1) return nullptr;
                    }
                }
            }
        }
        return image;
    }else{
        file.resetIndex();
        return nullptr;
    }
}

void writeImagefile(BaseImage * image, ImageFileHandler& fileHandler) noexcept(false){
    //checks if either the format of the file modified and image are equal
    if(image->getFormat() == "P2" && split(fileHandler.getPath(), ".").back() != "pgm")
        throw std::logic_error("Writing file must be a pgm file");
    if(image->getFormat() == "P3" && split(fileHandler.getPath(), ".").back() != "ppm")
        throw std::logic_error("Writing file must be a ppm file");
    fileHandler.write(image->getFormat() + "\n" + std::to_string(image->getWidth())+" "+std::to_string(image->getHeight())+"\n"+std::to_string(image->getMaxValue())+"\n");
    for(int i = 0; i < image->getHeight(); i++){
        for(int j = 0; j < image->getWidth(); j++){
            for (int k = 0; k < image->getPixel(i, j)->getDim(); k++) {
                if(image->getPixel(i, j) == nullptr){
                    throw std::logic_error("Given image isn't valid, some pixel aren't initialized");
                }else if(image->getPixel(i, j)->getChannel(k) == -1){
                    throw std::logic_error("Given image isn't valid, some pixel aren't initialized");
                }
                fileHandler.write(std::to_string(image->getPixel(i, j)->getChannel(k))+" ");
            }
        }
        fileHandler.write("\n");
    }
}

#endif //LABPROG_IMAGEREADERWRITER_H
