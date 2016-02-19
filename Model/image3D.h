#ifndef IMAGE_H
#define IMAGE_H

#include <armadillo>
#include <map>

template <typename PixelType> class Image3D : public arma::Cube<PixelType>
{

public:

    static const unsigned int Dimension = 3;

    static const std::vector<std::string> MetaDataDictionaryKeys;

    Image3D();
    Image3D(int size);
    Image3D(int row, int col, int slice);
    Image3D(PixelType *data, int row, int col, int slice, bool copy);
    Image3D(const Image3D &image);

    void setMetaDataDictionary(const std::map<std::string, std::string> &metaDataDictionary);
    std::map<std::string, std::string> getMetaDataDictionary();

    void fillWithImage(const Image3D &image);

private:

    std::map<std::string, std::string> metaDataDictionary;

};

template <typename PixelType> const std::vector<std::string> Image3D<PixelType>::MetaDataDictionaryKeys = {
    "0008|0008",
    "00010|00010",
    "0018|0015",
    "0018|1170",
    "0028|0008",
    "0028|0010",
    "0028|0011",
    "0028|0010",
};

template <typename PixelType> Image3D<PixelType>::Image3D() : arma::Cube<PixelType> () { }

template <typename PixelType> Image3D<PixelType>::Image3D(int size) : arma::Cube<PixelType> (size, size, size) { }

template <typename PixelType> Image3D<PixelType>::Image3D(int row, int col, int slice) : arma::Cube<PixelType> (row, col, slice) { }

template <typename PixelType> Image3D<PixelType>::Image3D(PixelType *data, int row, int col, int slice, bool copy) : arma::Cube<PixelType> (data, row, col, slice, copy) { }

template <typename PixelType> Image3D<PixelType>::Image3D(const Image3D &image) : arma::Cube<PixelType> (image) { }

template <typename PixelType> void Image3D<PixelType>::setMetaDataDictionary(const std::map<std::string, std::string> &metaDataDictionary)
{
    this->metaDataDictionary = metaDataDictionary;
}

template <typename PixelType> std::map<std::string, std::string> Image3D<PixelType>::getMetaDataDictionary()
{
    return metaDataDictionary;
}

template <typename PixelType> void Image3D<PixelType>::fillWithImage(const Image3D &image)
{
    // On remplit notre image avec les valeurs de l'image en paramètre
    if (this->n_rows == image.n_rows && this->n_cols == image.n_cols && this->n_slices == image.n_slices)
    {
        typename Image3D::iterator thisIterator = this->begin();
        for (typename Image3D::const_iterator it=image.begin(); it!=image.end(); ++it, ++thisIterator)
        {
            *thisIterator = *it;
        }
    }
}

#endif // IMAGE_H
