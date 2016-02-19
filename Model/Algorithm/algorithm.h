#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <QString>
#include <QVariant>

template <typename InputImageType, typename OutputImageType> class Algorithm
{

public:

    // Dimension de l'image en entrée

    static const unsigned int Dimension = InputImageType::Dimension;

    // Défintion des types d'images

    typedef typename InputImageType::elem_type      InputPixelType;
    typedef typename OutputImageType::elem_type     OutputPixelType;
    typedef InputImageType *                        InputImagePointer;
    typedef OutputImageType *                       OutputImagePointer;

    // Fonctions d'algorithmes

    Algorithm(const std::string &name) { this->name = name; }

    virtual ~Algorithm() { }

    void setName(const std::string &name) { this->name = name; }

    std::string getName() const { return name; }

    virtual Algorithm * copy() = 0;

    virtual void executeInPlace(InputImagePointer image) = 0;
    virtual void executeSliceBySliceInPlace(InputImagePointer image) = 0;

    virtual OutputImagePointer executeOutPlace(InputImagePointer image) = 0;
    virtual OutputImagePointer executeSliceBySliceOutPlace(InputImagePointer image) = 0;

private:

    std::string name;

};

#endif // ALGORITHM_H
