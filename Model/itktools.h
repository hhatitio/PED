#ifndef ITKTOOLS_H
#define ITKTOOLS_H

#include "deftypes.h"

#include <iostream>
#include <cstdlib>
#include <string>

#include <itkImage.h>
#include <itkRawImageIO.h>
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImageFileWriter.h>
#include <itkImageDuplicator.h>
#include <itkImportImageFilter.h>
#include <itkImportImageContainer.h>

template <typename PixelType> class ITKTools
{
    // Définition des type de l'image

    typedef typename itk::ImportImageFilter<PixelType, Dimension>   	ImportFilterType;
    typedef typename itk::ImportImageFilter<PixelType, 2>               ImportFilterType2D;
    typedef itk::ImageDuplicator<ITKImage>                              DuplicatorType;
    typedef itk::ImageFileWriter<ITKImage>                              WriterType;
    typedef itk::ImageSeriesReader <ITKImage>							SeriesReaderType;
    typedef itk::GDCMImageIO											GDCMImageIOType;
    typedef itk::GDCMSeriesFileNames									NamesGeneratorType;
    typedef itk::MetaDataDictionary                                     DictionaryType;
    typedef itk::MetaDataObject<std::string>                            MetaDataStringType;

public:

    static std::vector< std::vector<std::string> > getSeriesFileNames(const std::string &directory);
    static std::map<std::string, std::string> getSerieDictionary(const std::vector<std::string> &serieFileNames, const std::vector<std::string> &dictionaryKeys);
    static std::string getSerieInfos(const std::vector<std::string> &serieFileNames, const std::vector<std::string> &infoDicomKeys);

    static typename ITKImage::Pointer loadDicomITKImage(const std::string &directory);
    static Image * loadDicomImage(const std::string &directory);

    static typename ITKImage::Pointer loadDicomITKImage(const std::vector<std::string> &serieFileNames);
    static Image * loadDicomImage(const std::vector<std::string> &serieFileNames);

    static int saveITKImage(const std::string &filename, const typename ITKImage::Pointer &itkImage);
    static int saveImage(const std::string &filename, Image *image);

    static typename ITKImage::Pointer convertInItkImage(Image *image, bool copy);
    static typename ITKImage2D::Pointer convertInItkImage2D(Image *image, int slice, bool copy);
    static Image * convertInImage(const typename ITKImage::Pointer &itkImage, bool copy);

    static typename ITKImage::Pointer copyITKImage(const typename ITKImage::Pointer &itkImage);

};

template <typename PixelType> std::vector< std::vector<std::string> > ITKTools<PixelType>::getSeriesFileNames(const std::string &directory)
{
    NamesGeneratorType::Pointer namesGenerator = NamesGeneratorType::New();
    namesGenerator->SetDirectory(directory);
    namesGenerator->GlobalWarningDisplayOff();

    std::vector< std::vector<std::string> > seriesFileNames;
    std::vector<std::string> seriesUIDs = namesGenerator->GetSeriesUIDs();
    std::vector<std::string>::iterator beginUIDs = seriesUIDs.begin();
    std::vector<std::string>::iterator endUIDs = seriesUIDs.end();
    for (std::vector<std::string>::iterator itUIDs=beginUIDs; itUIDs!=endUIDs; ++itUIDs) {
        seriesFileNames.push_back(namesGenerator->GetFileNames(*itUIDs));
    }

    return seriesFileNames;
}

template <typename PixelType>
std::map<std::string, std::string> ITKTools<PixelType>::getSerieDictionary(const std::vector<std::string> &serieFileNames, const std::vector<std::string> &dictionaryKeys)
{
    std::map<std::string, std::string> metaDataDictionary;

    // Initialisation du lecteur DICOM
    typename SeriesReaderType::Pointer reader = SeriesReaderType::New();
    GDCMImageIOType::Pointer dicomIO = GDCMImageIOType::New();
    reader->SetImageIO(dicomIO);

    // Essaie de lecture
    try {
        reader->SetFileNames(serieFileNames);
        reader->Update();
    } catch (itk::ExceptionObject &exp) {
        std::cerr << exp << std::endl;
        return metaDataDictionary;
    }

    // Récupération des données du DICOM
    const  DictionaryType &dictionary = dicomIO->GetMetaDataDictionary();
    for (DictionaryType::ConstIterator it=dictionary.Begin(); it!=dictionary.End(); ++it)
    {
        if (std::find(dictionaryKeys.begin(), dictionaryKeys.end(), it->first) != dictionaryKeys.end())
        {
            itk::MetaDataObjectBase::Pointer entry = it->second;
            MetaDataStringType::Pointer entryValue = dynamic_cast<MetaDataStringType*>(entry.GetPointer());
            if (entryValue)
            {
                std::string label;
                std::string key = it->first;
                std::string value = entryValue->GetMetaDataObjectValue();
                if (itk::GDCMImageIO::GetLabelFromTag(key, label))
                {
                    std::pair<std::string, std::string> data (key, value);
                    metaDataDictionary.insert(data);
                }
            }
        }
    }

    return metaDataDictionary;
}

template <typename PixelType>
std::string ITKTools<PixelType>::getSerieInfos(const std::vector<std::string> &serieFileNames, const std::vector<std::string> &infoDicomKeys)
{
    std::string infos = "Infos DICOM : ";

    // Initialisation du lecteur DICOM
    typename SeriesReaderType::Pointer reader = SeriesReaderType::New();
    GDCMImageIOType::Pointer dicomIO = GDCMImageIOType::New();
    reader->SetImageIO(dicomIO);

    // Essaie de lecture
    try {
        reader->SetFileNames(serieFileNames);
        reader->Update();
    } catch (itk::ExceptionObject &exp) {
        std::cerr << exp << std::endl;
        return infos;
    }

    // Récupération des données du DICOM
    const  DictionaryType &dictionary = dicomIO->GetMetaDataDictionary();
    for (DictionaryType::ConstIterator it=dictionary.Begin(); it!=dictionary.End(); ++it)
    {
        if (std::find(infoDicomKeys.begin(), infoDicomKeys.end(), it->first) != infoDicomKeys.end())
        {
            itk::MetaDataObjectBase::Pointer entry = it->second;
            MetaDataStringType::Pointer entryValue = dynamic_cast<MetaDataStringType*>(entry.GetPointer());
            if (entryValue)
            {
                std::string label;
                std::string key = it->first;
                std::string value = entryValue->GetMetaDataObjectValue();
                if (itk::GDCMImageIO::GetLabelFromTag(key, label))
                {
                    infos += "\n" + label + " : " + value;
                }
            }
        }
    }

    return infos;
}

template <typename PixelType> typename ITKImage::Pointer ITKTools<PixelType>::loadDicomITKImage(const std::string &directory)
{
    std::vector< std::vector<std::string> > seriesFileNames = getSeriesFileNames(directory);

    if (seriesFileNames.empty())
        return nullptr;

    return loadDicomITKImage(*seriesFileNames.begin());
}

template <typename PixelType> Image * ITKTools<PixelType>::loadDicomImage(const std::string &directory)
{
    Image *image = convertInImage(loadDicomITKImage(directory), false);
    std::vector< std::vector<std::string> > seriesFileNames = getSeriesFileNames(directory);
    std::map<std::string, std::string> dictionary = ITKTools<PixelType>::getSerieDictionary(*seriesFileNames.begin(), Image::MetaDataDictionaryKeys);
    image->setMetaDataDictionary(dictionary);
    return image;
}

template <typename PixelType> typename ITKImage::Pointer ITKTools<PixelType>::loadDicomITKImage(const std::vector<std::string> &serieFileNames)
{
    // Initialisation du lecteur DICOM
    typename SeriesReaderType::Pointer reader = SeriesReaderType::New();
    GDCMImageIOType::Pointer dicomIO = GDCMImageIOType::New();
    reader->SetImageIO(dicomIO);

    // Essaie de lecture
    try {
        reader->SetFileNames(serieFileNames);
        reader->Update();
    } catch (itk::ExceptionObject &exp) {
        std::cerr << exp << std::endl;
        return nullptr;
    }

    return reader->GetOutput();
}

template <typename PixelType> Image * ITKTools<PixelType>::loadDicomImage(const std::vector<std::string> &serieFileNames)
{
    ITKImage::Pointer itkImage = loadDicomITKImage(serieFileNames);
    Image *image = convertInImage(itkImage, false);
    std::map<std::string, std::string> dictionary = ITKTools<PixelType>::getSerieDictionary(serieFileNames, Image::MetaDataDictionaryKeys);
    image->setMetaDataDictionary(dictionary);
    return image;
}

template <typename PixelType> int ITKTools<PixelType>::saveITKImage(const std::string &filename, const typename ITKImage::Pointer &itkImage)
{
    // Initialisation de l'enregisteur DICOM
    typename WriterType::Pointer writer = WriterType::New();
    GDCMImageIOType::Pointer dicomIO = GDCMImageIOType::New();
    writer->SetFileName(filename);
    writer->SetImageIO(dicomIO);
    writer->SetInput(itkImage);

    // Essaie d'enregistrement
    try {
        writer->Update();
    } catch(itk::ExceptionObject & exp) {
        std::cerr << exp << std::endl;
    }

    return EXIT_SUCCESS;
}

template <typename PixelType> int ITKTools<PixelType>::saveImage(const std::string &filename, Image *image)
{
    // Conversion vers une image ITK
    typename ITKImage::Pointer itkImage = convertInItkImage(image, false);

    return saveITKImage(filename, itkImage);
}

template <typename PixelType> typename ITKImage::Pointer ITKTools<PixelType>::convertInItkImage(Image *image, bool copy)
{
    // Initialisation de l'importation vers ITK
    typename ImportFilterType::Pointer importFilter = ImportFilterType::New();
    typename ImportFilterType::SizeType size;
    size[0]  = image->n_rows;
    size[1]  = image->n_cols;
    size[2]  = image->n_slices;
    typename ImportFilterType::IndexType start;
    start.Fill(0);
    typename ImportFilterType::RegionType region;
    region.SetIndex(start);
    region.SetSize(size);
    importFilter->SetRegion(region);
    const itk::SpacePrecisionType origin[Dimension] = {0.0, 0.0, 0.0};
    importFilter->SetOrigin(origin);
    const itk::SpacePrecisionType  spacing[Dimension] =  {1.0, 1.0, 1.0};
    importFilter->SetSpacing(spacing);

    // Conversion d'Armadillo en image ITK
    const bool importImageFilterWillOwnTheBuffer = false;
    const unsigned int numberOfPixels =  size[0]*size[1]*size[2];
    importFilter->SetImportPointer(image->memptr(), numberOfPixels, importImageFilterWillOwnTheBuffer);
    importFilter->Update();

    if (copy)
        return ITKTools<PixelType>::copyITKImage(importFilter->GetOutput());
    else
        return importFilter->GetOutput();
}

template <typename PixelType> typename ITKImage2D::Pointer ITKTools<PixelType>::convertInItkImage2D(Image *image, int slice, bool copy)
{
    // Initialisation de l'importation vers ITK
    typename ImportFilterType2D::Pointer importFilter = ImportFilterType2D::New();
    typename ImportFilterType2D::SizeType size;
    size[0]  = image->n_rows;
    size[1]  = image->n_cols;
    typename ImportFilterType2D::IndexType start;
    start.Fill(0);
    typename ImportFilterType2D::RegionType region;
    region.SetIndex(start);
    region.SetSize(size);
    importFilter->SetRegion(region);
    const itk::SpacePrecisionType origin[Dimension] = {0.0, 0.0, 0.0};
    importFilter->SetOrigin(origin);
    const itk::SpacePrecisionType  spacing[Dimension] =  {1.0, 1.0, 1.0};
    importFilter->SetSpacing(spacing);

    // Conversion d'Armadillo en image ITK
    const bool importImageFilterWillOwnTheBuffer = false;
    const unsigned int numberOfPixels =  size[0]*size[1];
    importFilter->SetImportPointer(image->slice(slice).memptr(), numberOfPixels, importImageFilterWillOwnTheBuffer);
    importFilter->Update();

    if (copy)
        return ITKTools<PixelType>::copyITKImage(importFilter->GetOutput());
    else
        return importFilter->GetOutput();
}

template <typename PixelType> Image * ITKTools<PixelType>::convertInImage(const typename ITKImage::Pointer &itkImage, bool copy)
{
    if (itkImage == nullptr)
        return nullptr;

    // Increment smart pointer
    if (!copy)
        itkImage->Register();

    // Remplissage de la structure image Armadillo en fonction des données de l'image ITK
    Image *image = new Image (itkImage->GetBufferPointer(), itkImage->GetLargestPossibleRegion().GetSize()[0],
        itkImage->GetLargestPossibleRegion().GetSize()[1], itkImage->GetLargestPossibleRegion().GetSize()[2], copy);

    return image;
}

template <typename PixelType> typename ITKImage::Pointer ITKTools<PixelType>::copyITKImage(const typename ITKImage::Pointer &itkImage)
{
    typename DuplicatorType::Pointer duplicator = DuplicatorType::New();
    duplicator->SetInputImage(itkImage);
    duplicator->Update();

    return duplicator->GetModifiableOutput();
}

#endif // ITKTOOLS_H
