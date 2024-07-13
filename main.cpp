#include <iostream>
#include "KernelImageProcessing.h"
int main() {
    try {
        std::string input;
        std::cout<<"Inserisci il path dell'immagine da modificare:"<<std::endl;
        std::cin>>input;
        auto image = Image<unsigned int>::createImage(input);
        do {
            KernelImageProcessing<unsigned int> kip(&image);
            std::cout << "Scegli il metodo con cui modificare l'immagine: \n"
                         "(identity) (sharpen) (boxblur) (ridge) (gaussianblur3) (gaussianblur5) (unsharpmasking): "
                      << std::endl;
            std::cin >> input;
            try {
                auto image1 = kip.applyMethod(input);
                std::cout << "Inserisci il path dell'immagine da sovrascrivere:" << std::endl;
                std::cin >> input;
                image1.writeImage(input);
                std::cout << "File sovrascritto correttamente" << std::endl;
            }catch (std::logic_error& e){
                std::cout<<e.what()<<std::endl;
            }
            std::cout<<"Inserisci 1 per continuare a modificare l'immagine"<<std::endl;
            std::cin>>input;
        }while(input == "1");
    }catch (std::out_of_range& e1){
        std::cout<<e1.what()<<std::endl;
    }catch (std::logic_error& e){
        std::cout<<e.what()<<std::endl;
    }
    return 0;
}
