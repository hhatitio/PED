#ifndef ALGORITHMEXCEPTION_H
#define ALGORITHMEXCEPTION_H

#include "algorithm.h"
#include "../../deftypes.h"

#include <iostream>
#include <exception>

#define TITLE_EXCEPTION_WINDOW "Erreur algorithme"
#define MESSAGE_NO_INPLACE_FUNCTION_EXCEPTION "Fonction inPlace non disponible."
#define MESSAGE_NO_OUTPLACE_FUNCTION_EXCEPTION "Fonction outPlace non disponible."
#define MESSAGE_NO_SLICE_BY_SLICE_INPLACE_FUNCTION_EXCEPTION "Fonction SliceBySlice inPlace non disponible."
#define MESSAGE_NO_SLICE_BY_SLICE_OUTPLACE_FUNCTION_EXCEPTION "Fonction SliceBySlice outPlace non disponible."

class AlgorithmException : public std::exception
{

public:

    AlgorithmException(const std::string &msg);

    const char * what() const throw();

    bool manage(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent = 0) const throw();
    bool manageInPlace(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent = 0) const throw();
    bool manageOutPlace(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent = 0) const throw();
    bool manageSliceBySliceInPlace(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent = 0) const throw();
    bool manageSliceBySliceOutPlace(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent = 0) const throw();

private:

    std::string msg;

};

#endif // ALGORITHMEXCEPTION_H
