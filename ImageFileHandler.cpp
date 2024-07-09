//
// Created by sensi on 01/07/2024.
//

#include "ImageFileHandler.h"
#include <stdexcept>

ImageFileHandler::ImageFileHandler(const std::string &path) {
    file = new std::fstream();
    file->open(path);
    this->path = path;
    if(!(*file)){ //if the file wont open this method throws an exception
        throw std::logic_error("Path non valido");
    }
    closed = false;
}

std::string ImageFileHandler::getLine() const{
    std::string line;
    std::getline(*file, line);
    return line;
}

void ImageFileHandler::resetIndex() const { //this method resets the reading/writing index to the first char of the file
    file->seekg(0);
}

ImageFileHandler::~ImageFileHandler(){
    if(!closed) file->close();
    delete file;
}

bool ImageFileHandler::eof() const{ //checks if the reading/writing index is at the end of file
    return file->eof();
}