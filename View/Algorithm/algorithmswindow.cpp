#include "View/Algorithm/algorithmswindow.h"

#include <itkThresholdImageFilter.h>

#include "Model/itktools.h"

#include "../../Model/Algorithm/algorithmexception.h"

#include "../../Model/Algorithm/ITK/algorithmitkthreshold.h"
#include "../../Model/Algorithm/ITK/algorithmitkbinarythreshold.h"
#include "../../Model/Algorithm/ITK/algorithmitkdiscretegaussian.h"
#include "../../Model/Algorithm/ITK/algorithmitkbinarythinningimagefilter.h"
#include "../../Model/Algorithm/Adrien/algorithmconnectedcomponentextraction.h"
#include "../../Model/Algorithm/DGtal/algorithmdgtaldistancetransformation.h"

#include "View/Algorithm/ITK/algorithmviewitkthreshold.h"
#include "View/Algorithm/ITK/algorithmviewitkbinarythreshold.h"
#include "View/Algorithm/ITK/algorithmviewitkdiscretegaussian.h"
#include "View/Algorithm/ITK/algorithmviewitkbinarythinningimagefilter.h"
#include "View/Algorithm/Adrien/algorithmviewconnectedcomponentextraction.h"
#include "View/Algorithm/DGtal/algorithmviewdgtaldistancetransformation.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>

AlgorithmsWindow::AlgorithmsWindow(Image *image2D, const Interval<PixelType> &interval, QWidget *parent = 0) : QDialog(parent),
    image(image2D), intervalIntensity(interval), editDialogLogfileAlgorithm(this)
{
    // Copie de l'image d'origine pour la réinitialisation
    copyImage = new Image(image->memptr(), image->n_rows, image->n_cols, image->n_slices, true);

    // Paramétrisation de fenêtre
    setMinimumHeight(ALGORITHMS_WINDOW_MINIMUM_HEIGHT);
    setMinimumWidth(ALGORITHMS_WINDOW_MINIMUM_WIDTH);
    setWindowTitle("OS3D - Filtrage");

    // Paramétrisation de la fenêtre d'édition de l'historique
    editDialogLogfileAlgorithm.setLayout(new QVBoxLayout());
    editDialogLogfileAlgorithm.setMinimumWidth(ALGORITHMS_EDIT_LOGFILE_WINDOW_MINIMUM_WIDTH);
    editDialogLogfileAlgorithm.setWindowTitle("OS3D - Filtrage - Édition de l'historique");

    // Construction des filtres et initialisation des iterateurs
    buildAlgorithms();
    std::map<AlgorithmView*, Algorithm<Image, Image>*>::iterator beginAlgorithms = algorithms.begin();
    std::map<AlgorithmView*, Algorithm<Image, Image>*>::iterator endAlgorithms = algorithms.end();

    // Construction du groupe Filtre
    QGroupBox *groupAlgorithmChoice = new QGroupBox("Choix du filtre", this);
    QFormLayout *layoutAlgorithmChoice = new QFormLayout(groupAlgorithmChoice);
    QComboBox *comboBoxAlgorithm = new QComboBox(groupAlgorithmChoice);
    for (std::map<AlgorithmView*, Algorithm<Image, Image>*>::iterator itAlgorithms = beginAlgorithms; itAlgorithms != endAlgorithms; ++itAlgorithms)
    {
        comboBoxAlgorithm->addItem(itAlgorithms->first->getName());
    }
    comboBoxAlgorithm->setCurrentIndex(currentAlgorithm);
    layoutAlgorithmChoice->addWidget(comboBoxAlgorithm);

    // Construction du groupe Paramètre
    QGroupBox *groupAlgorithmParameters = new QGroupBox("Paramètres", this);
    QGridLayout *layoutAlgorithmParameters = new QGridLayout(groupAlgorithmParameters);
    int numAlgorithm = 0;
    for (std::map<AlgorithmView*, Algorithm<Image, Image>*>::iterator itAlgorithms = beginAlgorithms; itAlgorithms != endAlgorithms; ++itAlgorithms, ++numAlgorithm)
    {
        layoutAlgorithmParameters->addWidget(itAlgorithms->first, numAlgorithm, 0, 1, 2);
    }
    QPushButton *buttonPreviewAlgorithm = new QPushButton("Prévisualisation", groupAlgorithmParameters);
    QPushButton *buttonValidPreviewAlgorithm = new QPushButton("Valider", groupAlgorithmParameters);
    layoutAlgorithmParameters->addWidget(buttonPreviewAlgorithm, numAlgorithm+1, 0, 1, 1);
    layoutAlgorithmParameters->addWidget(buttonValidPreviewAlgorithm, numAlgorithm+1, 1, 1, 1);

    // Construction du groupe Historique
    QGroupBox *groupAlgorithmLogfile = new QGroupBox("Historique", this);
    QGridLayout *layoutAlgorithmLogfile = new QGridLayout(groupAlgorithmLogfile);
    QPushButton *buttonApplyLogfileAlgorithm = new QPushButton("Appliquer", groupAlgorithmLogfile);
    QPushButton *buttonDeleteLogfileAlgorithm = new QPushButton("Supprimer", groupAlgorithmLogfile);
    QPushButton *buttonSaveImageAlgorithm = new QPushButton("Sauvegarder", groupAlgorithmLogfile);
    listWidgetLogfile.setSelectionMode(QAbstractItemView::SingleSelection);
    listWidgetLogfile.setDragEnabled(true);
    listWidgetLogfile.viewport()->setAcceptDrops(true);
    listWidgetLogfile.setDropIndicatorShown(true);
    listWidgetLogfile.setDragDropMode(QAbstractItemView::InternalMove);
    layoutAlgorithmLogfile->addWidget(&listWidgetLogfile, 0, 0, 1, 2);
    layoutAlgorithmLogfile->addWidget(buttonApplyLogfileAlgorithm, 1, 0, 1, 1);
    layoutAlgorithmLogfile->addWidget(buttonDeleteLogfileAlgorithm, 1, 1, 1, 1);
    layoutAlgorithmLogfile->addWidget(buttonSaveImageAlgorithm, 2, 0, 1, 2);

    // Construction de la partie gauche (application + sélection de filtres)
    QVBoxLayout *layoutAlgorithms = new QVBoxLayout();
    layoutAlgorithms->addWidget(groupAlgorithmChoice);
    layoutAlgorithms->addWidget(groupAlgorithmParameters);
    layoutAlgorithms->addWidget(groupAlgorithmLogfile);

    // Construction de la partie droite (prévisualisation + validation)
    QVBoxLayout *layoutImage = new QVBoxLayout();
    layoutImage->addWidget(&imageScroll);
    imageScroll.updateImage(image, 0, intervalIntensity);
    imageScroll.updateLabel();
    QPushButton *buttonValidAlgorithms = new QPushButton("Valider", this);
    layoutImage->addWidget(buttonValidAlgorithms);

    // Ajout du formulaire et de l'image à la fenetre
    QHBoxLayout *layoutWindow = new QHBoxLayout(this);
    layoutWindow->addLayout(layoutAlgorithms);
    layoutWindow->addLayout(layoutImage);

    // Connexion des boutons
    QObject::connect(comboBoxAlgorithm, SIGNAL(currentIndexChanged(int)), this, SLOT(changeAlgorithm(int)));
    QObject::connect(buttonPreviewAlgorithm, SIGNAL(clicked()), this, SLOT(previewAlgorithm()));
    QObject::connect(buttonValidPreviewAlgorithm, SIGNAL(clicked()), this, SLOT(validPreviewAlgorithm()));
    QObject::connect(buttonSaveImageAlgorithm, SIGNAL(clicked()), this, SLOT(saveImageLayer()));
    QObject::connect(buttonApplyLogfileAlgorithm, SIGNAL(clicked()), this, SLOT(applyLogfileAlgorithm()));
    QObject::connect(buttonDeleteLogfileAlgorithm, SIGNAL(clicked()), this, SLOT(deleteLogfileAlgorithm()));
    QObject::connect(buttonValidAlgorithms, SIGNAL(clicked()), this, SLOT(validAlgorithms()));
    QObject::connect(&listWidgetLogfile, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openEditDialogLogfileAlgorithm(QListWidgetItem*)));
    QObject::connect(listWidgetLogfile.model(), SIGNAL(rowsMoved(QModelIndex,int,int,QModelIndex,int)),
                     this, SLOT(moveLogfileAlgorithm(QModelIndex,int,int,QModelIndex,int)));
}

AlgorithmsWindow::~AlgorithmsWindow()
{
    delete image;
    delete copyImage;
}

void AlgorithmsWindow::buildAlgorithms()
{
    // Initialisation des algorithms
    Algorithm<Image, Image> *thresholdAlgorithm = new AlgorithmITKThreshold<Image>();
    Algorithm<Image, Image> *binaryThresholdAlgorithm = new AlgorithmITKBinaryThreshold<Image, Image>();
    Algorithm<Image, Image> *gaussianDiscretAlgorithm = new AlgorithmITKDiscreteGaussian<Image, Image>();
    Algorithm<Image, Image> *adrienAlgorithm = new AlgorithmConnectedComponentExtraction<Image, Image>();
    Algorithm<Image, Image> *distanceTransformationAlgorithm = new AlgorithmDGtalDistanceTransformation<Image, Image>();
    Algorithm<Image, Image> *skeletonAlgorithm = new AlgorithmITKBinaryThinningImageFilter<Image, Image>();

    // Initialisation des vues
    AlgorithmView *thresholdView = new AlgorithmViewITKThreshold(image->min(), image->max(), this);
    AlgorithmView *binaryThresholdView = new AlgorithmViewITKBinaryThreshold(image->min(), image->max(), this);
    AlgorithmView *gaussianDiscretView = new AlgorithmViewITKDiscreteGaussian(image->Dimension, this);
    AlgorithmView *adrienView = new AlgorithmViewConnectedComponentExtraction(image->min(), image->max(), this);
    AlgorithmView *distanceTransformationView = new AlgorithmViewDGtalDistanceTransformation(this);
    AlgorithmView *skeletonView = new AlgorithmViewITKBinaryThinningImageFilter(0, 10, this);

    // Création des paires Algorithm <-> Vue
    std::pair<AlgorithmView*, Algorithm<Image, Image>*> threshold (thresholdView, thresholdAlgorithm);
    std::pair<AlgorithmView*, Algorithm<Image, Image>*> binaryThreshold (binaryThresholdView, binaryThresholdAlgorithm);
    std::pair<AlgorithmView*, Algorithm<Image, Image>*> gaussianDiscret (gaussianDiscretView, gaussianDiscretAlgorithm);
    std::pair<AlgorithmView*, Algorithm<Image, Image>*> adrien (adrienView, adrienAlgorithm);
    std::pair<AlgorithmView*, Algorithm<Image, Image>*> distanceTransformation(distanceTransformationView, distanceTransformationAlgorithm);
    std::pair<AlgorithmView*, Algorithm<Image, Image>*> skeleton(skeletonView, skeletonAlgorithm);

    // Ajout des paires à la liste de filtres
    algorithms.insert(threshold);
    algorithms.insert(binaryThreshold);
    algorithms.insert(gaussianDiscret);
    algorithms.insert(adrien);
    algorithms.insert(distanceTransformation);
    algorithms.insert(skeleton);
}

void AlgorithmsWindow::clearLayout(QLayout *layout)
{
    QLayoutItem *item;
    while((item = layout->takeAt(0))) {
        if (item->layout()) {
            clearLayout(item->layout());
            delete item->layout();
        }
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
}

void AlgorithmsWindow::changeAlgorithm(int indexAlgorithm)
{
    // Si un filtre a étè précédemment choisi on le cache
    if (currentAlgorithm != -1)
    {
        std::next(algorithms.begin(), currentAlgorithm)->first->hide();
    }

    // On affiche le filtre choisi et on met à jour le filtre courant
    std::next(algorithms.begin(), indexAlgorithm)->first->show();
    currentAlgorithm = indexAlgorithm;

    // on réapplique l'historique en cours
    applyLogfileAlgorithm();
}

void AlgorithmsWindow::previewAlgorithm()
{
    if (currentAlgorithm != -1)
    {
        // Initialisation de l'algorithme
        std::pair<AlgorithmView*, Algorithm<Image, Image>*> algorithm = *(std::next(algorithms.begin(), currentAlgorithm));
        algorithm.first->updateAlgorithmParameters(algorithm.second);
        Image *previewImage;

        try // Application de l'algorithme
        {
            previewImage = algorithm.second->executeOutPlace(image);
        }
        catch(const AlgorithmException &exception)
        {
            QMessageBox::critical(this, QString(TITLE_EXCEPTION_WINDOW), QString(exception.what()));
            return;
        }

        // Mise à jour de l'image et de son interval d'intensité
        intervalIntensity.setBounds(previewImage->min(), previewImage->max());
        imageScroll.updateImage(previewImage, 0, intervalIntensity);
        imageScroll.updateLabel();

        delete previewImage;
    }
}

void AlgorithmsWindow::validPreviewAlgorithm()
{
    if (currentAlgorithm != -1)
    {
        // Initialisation de l'algorithme
        std::pair<AlgorithmView*, Algorithm<Image, Image>*> algorithm = *(std::next(algorithms.begin(), currentAlgorithm));
        algorithm.first->updateAlgorithmParameters(algorithm.second);

        try // Application de l'algorithme
        {
            algorithm.second->executeInPlace(image);
        }
        catch (const AlgorithmException &exception)
        {
            if (!exception.manage(algorithm.second, image, this))
                return;
        }

        // Ajout du filtre à l'historique
        QListWidgetItem *item =  new QListWidgetItem(algorithm.first->toString());
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
        listWidgetLogfile.addItem(item);
        algorithmLogfile.push_back(algorithm.second->copy());

        // Mise à jour de l'image et de son interval d'intensité
        intervalIntensity.setBounds(image->min(), image->max());
        imageScroll.updateImage(image, 0, intervalIntensity);
        imageScroll.updateLabel();
    }
}

void AlgorithmsWindow::saveImageLayer()
{
    // Demande à l'utilisateur du nom de l'image
    QString name = QInputDialog::getText(this, "Nom image calque", "Entrez le nom de l'image calque : ");

    if (!name.isEmpty())
    {
        // Récupération de l'ensemble des algorithmes sélectionnés dans l'historique
        std::vector<Algorithm<Image, Image>*> algorithmLogfileLayer;
        for (int index = 0; index < listWidgetLogfile.count(); ++index)
        {
            if (listWidgetLogfile.item(index)->checkState() == Qt::Checked)
            {
                algorithmLogfileLayer.push_back(algorithmLogfile[index]);
            }
        }

        // Sauvegarde de l'image calque dans MainWindow
        emit savedImageLayer(name, algorithmLogfileLayer);
    }
}

void AlgorithmsWindow::applyLogfileAlgorithm()
{
    // Récupèration de l'image d'orgine
    std::memcpy(image->memptr(), copyImage->memptr(), sizeof(PixelType)*copyImage->n_elem);

    // Application de l'ensemble des algorithmes sélectionnés dans l'historique
    for (int index = 0; index < listWidgetLogfile.count(); ++index)
    {
        if (listWidgetLogfile.item(index)->checkState() == Qt::Checked)
        {
            try // Application de l'algorithme
            {
                algorithmLogfile[index]->executeInPlace(image);
            }
            catch (const AlgorithmException &exception)
            {
                exception.manage(algorithmLogfile[index], image, this);
            }
        }
    }

    // Mise à jour de l'image et de son interval d'intensité
    intervalIntensity.setBounds(image->min(), image->max());
    imageScroll.updateImage(image, 0, intervalIntensity);
    imageScroll.updateLabel();
}

void AlgorithmsWindow::openEditDialogLogfileAlgorithm(QListWidgetItem *item)
{
    // Recherche et affichage de l'algorithme concerné
    int indexAlgorithmLogfile = listWidgetLogfile.row(item);
    Algorithm<Image, Image> *algorithm = algorithmLogfile[indexAlgorithmLogfile];
    for (std::map<AlgorithmView*, Algorithm<Image, Image>*>::iterator it = algorithms.begin(); it != algorithms.end(); ++it)
    {
        if (typeid(*algorithm) == typeid(*(it->second)))
        {
            // Création du formulaire d'édition
            clearLayout(editDialogLogfileAlgorithm.layout());
            AlgorithmView *algorithmView = it->first->copy();
            QGroupBox *groupEditAlgorithm = new QGroupBox(algorithmView->getName());
            QVBoxLayout *layoutEditAlgorithm = new QVBoxLayout(groupEditAlgorithm);
            QPushButton *buttonEditAlgorithm = new QPushButton("Modifier", groupEditAlgorithm);
            layoutEditAlgorithm->addWidget(algorithmView);
            layoutEditAlgorithm->addWidget(buttonEditAlgorithm);

            // Mise à jour des paramètres du formulaire
            buttonEditAlgorithm->setProperty(LABEL_INDEX_ALGORITHM_LOGFILE_PROPERTY, indexAlgorithmLogfile);
            algorithmView->updateParametersWithAlgorithm(algorithm);
            algorithmView->show();

            // Connexion du bouton modifier
            QObject::connect(buttonEditAlgorithm, SIGNAL(clicked()), this, SLOT(editLogfileAlgorithm()));

            // Ajout du formulaire à la boite de dialogue
            editDialogLogfileAlgorithm.layout()->addWidget(groupEditAlgorithm);
        }
    }

    // Affichage de la boite de dialogue
    editDialogLogfileAlgorithm.exec();
}

void AlgorithmsWindow::editLogfileAlgorithm()
{
    // Récupération de l'algorithme et du formulaire
    QPushButton *button = qobject_cast<QPushButton*>(this->sender());
    int indexAlgorithmLogfile = button->property(LABEL_INDEX_ALGORITHM_LOGFILE_PROPERTY).toInt();
    Algorithm<Image, Image> *algorithm = algorithmLogfile[indexAlgorithmLogfile];
    AlgorithmView *algorithmView = (AlgorithmView *) editDialogLogfileAlgorithm.layout()->itemAt(0)->widget()->layout()->itemAt(0)->widget();

    // Mise à jour de l'algorithme en fonction du formulaire
    algorithmView->updateAlgorithmParameters(algorithm);
    listWidgetLogfile.item(indexAlgorithmLogfile)->setText(algorithmView->toString());
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void AlgorithmsWindow::moveLogfileAlgorithm(const QModelIndex &srcParent, int srcStart, int srcEnd, const QModelIndex &destParent, int destStart)
{
    // Réajustement de l'index de destination
    if (srcStart < destStart)
        destStart--;

    // Echange des algorithmes de l'historique concernés
    Algorithm<Image, Image> *tmpAlgorithm = algorithmLogfile[srcStart];
    algorithmLogfile.erase(algorithmLogfile.begin()+srcStart);
    algorithmLogfile.emplace(algorithmLogfile.begin()+destStart, tmpAlgorithm);
}
#pragma GCC diagnostic pop

void AlgorithmsWindow::deleteLogfileAlgorithm()
{
    // Suppression de l'ensemble des algorithmes sélectionnés dans l'historique
    for (int index = 0; index < listWidgetLogfile.count(); ++index)
    {
        if (listWidgetLogfile.item(index)->checkState() == Qt::Checked)
        {
            listWidgetLogfile.takeItem(index);
            algorithmLogfile.erase(algorithmLogfile.begin()+index);
            index--;
        }
    }

    // on réapplique l'historique en cours
    applyLogfileAlgorithm();
}

void AlgorithmsWindow::validAlgorithms()
{
    // Choix de filtres slice by slice ou 3D
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Choix du type d'exécution", "Souhaitez vous exécutez les filtres en mode SliceBySlice ?",
                                  QMessageBox::Cancel|QMessageBox::No|QMessageBox::Yes);

    // Récupération de la réponse
    bool sliceBySlice;
    switch (reply)
    {
        case QMessageBox::Yes : sliceBySlice = true; break;
        case QMessageBox::No : sliceBySlice = false; break;
        default : return;
    }

    // Envoie des filtres à la fenêtre principale
    emit validatedAlgorithms(algorithmLogfile, sliceBySlice);
    this->close();
}
