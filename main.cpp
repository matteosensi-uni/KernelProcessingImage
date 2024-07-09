#include <iostream>

#include "ImageFileHandler.h"
#include "ImageReaderWriter.h"
#include "KernelImageProcessing.h"
int main() {
    try {
        std::string input;
        std::cout<<"Inserisci il path dell'immagine da modificare:"<<std::endl;
        std::cin>>input;
        ImageFileHandler file = ImageFileHandler(input);
        BaseImage *image = createImage(file);
        if (image != nullptr) {
            KernelImageProcessing kip(image);
            std::cout<<"Scegli il metodo con cui modificare l'immagine: \n"
                       "(identity) (sharpen) (boxblur) (ridge) (gaussianblur3) (gaussianblur5) (unsharpmasking): "<<std::endl;
            std::cin>>input;
            BaseImage *image1 = kip.applyMethod(input);
            std::cout<<"Inserisci il path dell'immagine da sovrascrivere:"<<std::endl;
            std::cin>>input;
            ImageFileHandler file1 = ImageFileHandler(input);
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
