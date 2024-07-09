//
// Created by sensi on 01/07/2024.
//

#ifndef LABPROG_IMAGEFILEHANDLER_H
#define LABPROG_IMAGEFILEHANDLER_H

#include <string>
#include <fstream>
#include <iostream>
#include <exception>
#include <memory>

class ImageFileHandler {
public:
    explicit ImageFileHandler(const std::string& path) noexcept(false);
    std::string getLine() const;
    ~ImageFileHandler();
    void resetIndex() const;
    bool eof() const;
    void operator<<(const std::string& line){
        (*file)<<line;
    }
    void write(const std::string& string){ (*file)<<string; }
    void clear(){
        std::ofstream ofs;
        ofs.open(path, std::ofstream::out | std::ofstream::trunc);
        ofs.close();
    }
private:
    std::fstream* file;
    std::string path;
};


#endif //LABPROG_IMAGEFILEHANDLER_H
