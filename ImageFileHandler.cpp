//
// Created by sensi on 01/07/2024.
//

#include "ImageFileHandler.h"
#include <stdexcept>

ImageFileHandler::ImageFileHandler(const std::string &path) {
    file = new std::fstream();
    file->open(path);
    if(!(*file)){
        throw std::logic_error("Path non valido");
    }
    this->path = path;
}

std::string ImageFileHandler::getLine() const{
    std::string line;
    std::getline(*file, line);
    return line;
}

void ImageFileHandler::resetIndex() const {
    file->seekg(0);
}

ImageFileHandler::~ImageFileHandler(){
    file->close();
}

bool ImageFileHandler::eof() const{
    return file->eof();
}