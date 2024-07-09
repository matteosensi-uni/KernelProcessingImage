#include <iostream>

#include "ImageFileHandler.h"
#include "ImageReaderWriter.h"
#include "KernelImageProcessing.h"
int main() {
    try {
        ImageFileHandler file = ImageFileHandler("C:\\Users\\sensi\\Downloads\\pbmlib.ascii.ppm");
        BaseImage *image = createImage(file);
        if (image != nullptr) {
            KernelImageProcessing kip(image);
            BaseImage *image1 = kip.applyMethod("gaussianblur5");
            ImageFileHandler file1 = ImageFileHandler("C:\\Users\\sensi\\Downloads\\prova3.ppm");
            file1.clear();
            writeImagefile(image1, file1);
            std::cout<<"File sovrascritto correttamente"<<std::endl;
        } else {
            std::cout << "Errore nella lettura del file";
        }
    }catch (std::logic_error e){
        std::cout<<e.what()<<std::endl;
    }catch (std::out_of_range e1){
        std::cout<<e1.what()<<std::endl;
    }
    return 0;
}
