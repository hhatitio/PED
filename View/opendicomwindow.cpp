#include "View/opendicomwindow.h"

#include "Model/itktools.h"
#include "Model/image3D.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>

const std::vector<std::string> OpenDicomWindow::InfoDicomKeys = {
    "0008|0008",
    "00010|00010",
    "0018|0015",
    "0018|1170",
    "0028|0008",
    "0028|0010",
    "0028|0011",
    "0028|0010",
};

OpenDicomWindow::OpenDicomWindow(const QString &folderName, QWidget *parent) : QDialog(parent)
{
    // Récupération des séries DICOM du répertoire
    seriesFileNames = ITKTools<PixelType>::getSeriesFileNames(folderName.toStdString());
    if (isDicomFolder())
    {
        // Initialisation du formulaire de choix de série
        QGroupBox *groupSeries = new QGroupBox("Choix de la série", this);
        QVBoxLayout *layoutSeries = new QVBoxLayout(groupSeries);
        std::vector< std::vector<std::string> >::iterator beginSeries = seriesFileNames.begin();
        std::vector< std::vector<std::string> >::iterator endSeries = seriesFileNames.end();
        for (std::vector< std::vector<std::string> >::iterator itSeries=beginSeries; itSeries!=endSeries; ++itSeries)
        {
            QString infos = QString::fromStdString(ITKTools<PixelType>::getSerieInfos(*itSeries, InfoDicomKeys));
            QRadioButton *buttonSerie = new QRadioButton(infos);
            QObject::connect(buttonSerie, SIGNAL(clicked()), this, SLOT(changeImage()));
            buttonDICOMSeries.push_back(buttonSerie);
            layoutSeries->addWidget(buttonSerie);
        }
        QVBoxLayout *layoutFormSeries = new QVBoxLayout();
        QPushButton *buttonValidSerie = new QPushButton("Valider", this);
        layoutFormSeries->addWidget(groupSeries);
        layoutFormSeries->addWidget(buttonValidSerie);

        // On check la première série
        buttonDICOMSeries[0]->setChecked(true);

        // Initialisation de l'image servant de prévisualisation (1er image de la 1er série)
        QVBoxLayout *layoutImage = new QVBoxLayout();
        layoutImage->addWidget(&imageScroll);
        Image *imageSerie = ITKTools<PixelType>::loadDicomImage(seriesFileNames[0]);
        Interval<PixelType> intervalImage = Interval<PixelType>(imageSerie->min(), imageSerie->max());
        imageScroll.updateImage(imageSerie, 0, intervalImage);
        imageScroll.updateLabel();

        QHBoxLayout *layoutWindow = new QHBoxLayout(this);
        layoutWindow->addLayout(layoutFormSeries);
        layoutWindow->addLayout(layoutImage);

        // Connexion pour ouvrir la série DICOM
        QObject::connect(buttonValidSerie, SIGNAL(clicked()), this, SLOT(validDicom()));

        setWindowTitle(QString("OS3D - %1 - Choix série").arg(folderName.section(QDir::separator(),-1)));
        setMinimumWidth(OPENDICOM_WINDOW_MINIMUM_WIDTH);
        setMinimumHeight(OPENDICOM_WINDOW_MINIMUM_HEIGHT);
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Veuillez choisir un dossier DICOM.");
    }
}

bool OpenDicomWindow::isDicomFolder()
{
    return !seriesFileNames.empty();
}

void OpenDicomWindow::validDicom()
{
    int numSerie = 0;
    // On recherche la série checker pour la charger
    std::vector<QRadioButton*>::iterator beginButtonSerie = buttonDICOMSeries.begin();
    std::vector<QRadioButton*>::iterator endButtonSerie = buttonDICOMSeries.end();
    for (std::vector<QRadioButton*>::iterator itButtonSerie=beginButtonSerie; itButtonSerie!=endButtonSerie; ++itButtonSerie, ++numSerie)
    {
        if ((*itButtonSerie)->isChecked())
        {
            emit choosenDicom(seriesFileNames[numSerie]);
            this->close();
            return;
        }
    }
}

void OpenDicomWindow::changeImage()
{
    // On recherche la série checker
    int numSerie = 0;
    std::vector<QRadioButton*>::iterator beginButtonSerie = buttonDICOMSeries.begin();
    std::vector<QRadioButton*>::iterator endButtonSerie = buttonDICOMSeries.end();
    for (std::vector<QRadioButton*>::iterator itButtonSerie=beginButtonSerie; itButtonSerie!=endButtonSerie; ++itButtonSerie, ++numSerie)
    {
        if ((*itButtonSerie)->isChecked())
        {
            // Mise à jour de l'image pour la série sélectionner
            Image *imageSerie = ITKTools<PixelType>::loadDicomImage(seriesFileNames[numSerie]);
            Interval<PixelType> intervalImage = Interval<PixelType>(imageSerie->min(), imageSerie->max());
            imageScroll.updateImage(imageSerie, 0, intervalImage);
            imageScroll.updateLabel();
        }
    }
}
