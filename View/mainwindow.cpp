#include "View/mainwindow.h"

#include "Model/itktools.h"
#include "Model/dgtaltools.h"
#include "Model/Algorithm/algorithmexception.h"
#include "View/Customplot/qcustomplot.h"
#include "View/myopenglwidget.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QAction>
#include <QPushButton>
#include <QFileInfo>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QColorDialog>
#include <QList>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), image(0), sliderSlice(Qt::Horizontal),
    imageLayersWindow(this), imageLayersToolsWindow(this), imageLayersViewer3DWindow(this),
    layersThresholdWindow(this), windowingWindow(this)//, convertImageView(this)
{
    buildMenus();

    // Initialisation de la fenêtre principale

    setWindowTitle("OS3D");
    setMinimumHeight(MAINWINDOW_MINIMUM_HEIGHT);
    setMinimumWidth(MAINWINDOW_MINIMUM_WIDTH);
    int centerX = (qApp->desktop()->width()-MAINWINDOW_MINIMUM_WIDTH)/2;
    int centerY = (qApp->desktop()->height()-MAINWINDOW_MINIMUM_HEIGHT)/2;
    move(centerX, centerY);

    intervalIntensity = new Interval<PixelType>();

    imageScroll.installLabelEventFilter(this);
    QObject::connect(&sliderSlice, SIGNAL(valueChanged(int)), this, SLOT(drawSlice()));
    QWidget *centralZone = new QWidget(this);
    QVBoxLayout *layoutMain = new QVBoxLayout(centralZone);
    layoutMain->addWidget(&imageScroll);
    layoutMain->addWidget(&sliderSlice);
    setCentralWidget(centralZone);

    // Initialisation des fenêtres utilisant les images calques

    imageLayers = new std::vector<ImageLayer>();
    imageLayersWindow.setImageLayers(imageLayers);
    imageLayersToolsWindow.setImageLayers(imageLayers);
    imageLayersViewer3DWindow.setImageLayers(imageLayers);
    QObject::connect(&imageLayersWindow, SIGNAL(componentColorChanged()), this, SLOT(drawSlice()));
    QObject::connect(&imageLayersWindow, SIGNAL(imageLayerRemoved(int)), this, SLOT(removeActionImageLayer(int)));
    QObject::connect(&imageLayersWindow, SIGNAL(imageLayerRemoved(int)), &imageLayersToolsWindow, SLOT(removeViewLayer(int)));
    QObject::connect(&imageLayersWindow, SIGNAL(imageLayerRemoved(int)), &imageLayersViewer3DWindow, SLOT(removeViewLayer(int)));
    QObject::connect(&imageLayersToolsWindow, SIGNAL(rejected()), this, SLOT(drawSlice()));
}

MainWindow::~MainWindow()
{
    delete image;
    delete imageLayerMenu;
    delete imageLayers;
    delete intervalIntensity;
}

void MainWindow::buildMenus()
{
    // Menu fichier
    QMenu *menuFile = menuBar()->addMenu("&Fichier");
    QAction *actionOpenDicom = new QAction("&Ouvrir DICOM", this);
    QAction *actionOpenImage3D = new QAction("&Ouvrir image 3D", this);
    QAction *actionOpenOthersImage3D = new QAction("&Ouvrir image 3D (.tif, .skel)", this);
    QAction *actionSave = new QAction("&Sauvegarder", this);
    QAction *actionClose = new QAction("&Fermer", this);
    QAction *actionQuit = new QAction("&Quitter", this);
    menuFile->addAction(actionOpenDicom);
    menuFile->addAction(actionOpenImage3D);
    menuFile->addAction(actionOpenOthersImage3D);
    menuFile->addAction(actionSave);
    menuFile->addAction(actionClose);
    menuFile->addAction(actionQuit);

    // Menu Outils
    QMenu *menuTools = menuBar()->addMenu("&Outils");
    QAction *actionChooseAlgorithms = new QAction("&Filtres", this);
    QAction *actionChangeWindowing = new QAction(ACTION_WINDOWING_TEXT, this);
    QAction *actionHistogram = new QAction(ACTION_HISTOGRAM_TEXT, this);
    QAction *actionSkeletonization = new QAction(ACTION_SKELETONIZATION_TEXT, this);
    //QAction *actionConvert = new QAction(ACTION_CONVERT_TEXT, this);
    QAction *actionGenerateGraph = new QAction(ACTION_GRAPH_TEXT, this);
    menuTools->addAction(actionChooseAlgorithms);
    menuTools->addAction(actionChangeWindowing);
    menuTools->addAction(actionHistogram);
    menuTools->addAction(actionSkeletonization);
    //menuTools->addAction(actionConvert);
    menuTools->addAction(actionGenerateGraph);

    // Menu Calque
    QMenu *menuToolsLayer = menuBar()->addMenu("&Calques");
    imageLayerMenu = menuToolsLayer->addMenu("&Images calques");
    QAction *emptyImageLayer = new QAction(ACTION_EMPTY_IMAGE_LAYER, this);
    emptyImageLayer->setEnabled(false);
    imageLayerMenu->addAction(emptyImageLayer);
    QAction *actionChooseThresholdLayers = new QAction(ACTION_LAYER_THRESHOLD_TEXT, this);
    QAction *actionToolsImageLayers = new QAction(ACTION_TOOLS_IMAGE_LAYER, this);
    QAction *actionManageImageLayers = new QAction(ACTION_MANAGE_IMAGE_LAYER, this);
    QAction *actionViewer3DImageLayers = new QAction(ACTION_VIEWER3D_IMAGE_LAYER, this);
    menuToolsLayer->addAction(actionChooseThresholdLayers);
    menuToolsLayer->addAction(actionToolsImageLayers);
    menuToolsLayer->addAction(actionManageImageLayers);
    menuToolsLayer->addAction(actionViewer3DImageLayers);

    // Gestion des raccourcis clavier
    actionOpenDicom->setShortcut(Qt::CTRL + Qt::Key_O);
    actionOpenImage3D->setShortcut(Qt::CTRL + Qt::ALT + Qt::Key_O);
    actionClose->setShortcut(Qt::CTRL + Qt::Key_W);
    actionSave->setShortcut(Qt::CTRL + Qt::Key_S);
    actionQuit->setShortcut(Qt::CTRL + Qt::Key_Q);

    // Gestion des connexions
    QObject::connect(actionOpenDicom, SIGNAL(triggered()), this, SLOT(chooseDicom()));
    QObject::connect(actionOpenImage3D, SIGNAL(triggered()), this, SLOT(openImage3D()));
    QObject::connect(actionOpenOthersImage3D, SIGNAL(triggered()), this, SLOT(openOthersImage3D()));
    QObject::connect(actionSave, SIGNAL(triggered()), this, SLOT(saveImage()));
    QObject::connect(actionClose, SIGNAL(triggered()), this, SLOT(closeImage()));
    QObject::connect(actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    QObject::connect(actionChooseAlgorithms, SIGNAL(triggered()), this, SLOT(openAlgorithms()));
    QObject::connect(actionChangeWindowing, SIGNAL(triggered()), this, SLOT(openSecondaryWindow()));
    QObject::connect(actionHistogram, SIGNAL(triggered()), this, SLOT(openSecondaryWindow()));
    QObject::connect(actionChooseThresholdLayers, SIGNAL(triggered()), this, SLOT(openSecondaryWindow()));
    QObject::connect(actionToolsImageLayers, SIGNAL(triggered()), this, SLOT(openSecondaryWindow()));
    QObject::connect(actionManageImageLayers, SIGNAL(triggered()), this, SLOT(openSecondaryWindow()));
    QObject::connect(actionViewer3DImageLayers, SIGNAL(triggered()), this, SLOT(openSecondaryWindow()));
    QObject::connect(actionSkeletonization, SIGNAL(triggered()), this, SLOT(skeletonization()));
    //QObject::connect(actionConvert, SIGNAL(triggered()), this, SLOT(openSecondaryWindow()));
    QObject::connect(actionGenerateGraph, SIGNAL(triggered()), this, SLOT(getGraph()));
}

void MainWindow::updateImageComponents()
{
    // Mise à jour du slider et de l'interval d'intensité
    int nbSlice = image->n_slices;
    sliderSlice.setRange(0, nbSlice-1);
    sliderSlice.setValue(nbSlice/2);
    intervalIntensity->setBounds(image->min(), image->max());

    // Mise à jour des fenêtres secondaires
    histogramWindow.setHistogramData(image);
    layersThresholdWindow.setDataParameters(image->min(), image->max());
    windowingWindow.buildWindowing(intervalIntensity);
}

void MainWindow::removeLayers()
{
    while (!imageLayers->empty())
        imageLayersWindow.removeLayer();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (object == &(imageScroll.getLabel()) && currentImageType != ImageType::Empty)
    {
        // Application du crayon ou de la gomme après click droit et sélection préalable de l'outil
        if ((event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseMove))
        {
            const QMouseEvent* mouseEvent = static_cast<const QMouseEvent*>(event);
            if (mouseEvent->buttons() & Qt::RightButton && imageLayersToolsWindow.isVisible())
            {
                QPoint pos = mouseEvent->pos()/imageScroll.getZoomer().getFactor();
                imageLayersToolsWindow.apply(pos, currentSlice);
                drawSlice();
            }
        }
        // Affichage de la valeur de l'image après click droit
        else if (event->type() == QEvent::MouseButtonRelease && !imageLayersToolsWindow.isVisible())
        {
            const QMouseEvent* mouseEvent = static_cast<const QMouseEvent*>(event);
            if (mouseEvent->button() == Qt::RightButton)
            {
                QPoint pos = mouseEvent->pos()/imageScroll.getZoomer().getFactor();
                // Cordonnées inverse pour l'image
                int x = std::abs((int)(pos.x()-image->n_rows));
                int y = std::abs((int)(pos.y()-image->n_cols));
                qDebug() << "Position (" << pos.x() << ", " << pos.y() << ") = " << image->at(x, y, currentSlice);
            }
        }
    }

    return QMainWindow::eventFilter(object, event);
}

void MainWindow::chooseDicom()
{
    // Déclaration de la fenêtre d'ouverture de DICOM
    OpenDicomWindow *openDicomWindow;

    do {
        // Choix du répertoire dicom par l'utilisateur
        QString folderName = QFileDialog::getExistingDirectory(this, "Sélection du répertoire DICOM", QDir::homePath(), QFileDialog::ShowDirsOnly);
        if (folderName.isEmpty()) return;
        // Chargement des images DICOM en fonction du répertoire choisi
        openDicomWindow = new OpenDicomWindow(folderName, this);
        QObject::connect(openDicomWindow, SIGNAL(choosenDicom(std::vector<std::string>)), this, SLOT(openDicom(std::vector<std::string>)));
        setWindowTitle(QString("OS3D - %1").arg(folderName.section(QDir::separator(),-1)));
        if (openDicomWindow->isDicomFolder())
            openDicomWindow->exec();
    } while (!openDicomWindow->isDicomFolder());
}

void MainWindow::openDicom(const std::vector<std::string> &serieFileNames)
{
    // Fermeture de l'image courante
    if (!closeImage())
        return;
    // Suppression des calques
    removeLayers();
    // Mise à jour de l'image et de la fenêtre principale
    image = ITKTools<PixelType>::loadDicomImage(serieFileNames);
    currentImageType = ImageType::Dicom;
    updateImageComponents();
    drawSlice();
}

void MainWindow::openImage3D()
{
    // Mise à jour de l'image et de la fenêtre principale
    QString filename = QFileDialog::getOpenFileName(this, "Sélection de l'image segmentée", QDir::homePath(), "Image3D (*.vol *.pgm3d)");
    if (filename.isEmpty()) return;
    // Fermeture de l'image courante
    if (!closeImage())
        return;
    // Suppression des calques
    removeLayers();
    // Mise à jour de l'image et de la fenêtre principale
    image = DGtalTools<PixelType>::loadImage3D(filename.toStdString());
    currentImageType = ImageType::Image3D;
    skeletonModel.setSkeleton3DIm(image);
    updateImageComponents();
    drawSlice();
}

void MainWindow::openOthersImage3D()
{
    // Mise à jour de l'image et de la fenêtre principale
    QString filename = QFileDialog::getOpenFileName(this, "Sélection de l'image segmentée", QDir::homePath(), "Image3D (*.tif *.skel)");
    if (filename.isEmpty()) return;
    // Fermeture de l'image courante
    if (!closeImage())
        return;
    // Suppression des calques
    removeLayers();
    currentImageType = ImageType::Image3D;
    skeletonModel.setFilename(filename);
    //convertImage.setFilename(filename);
    if (filename.contains(".tif")) {
        skeletonModel.loadTIFFFile();
        image = skeletonModel.getSkeleton3DIm();
        currentImageType = ImageType::Image3D;
        updateImageComponents();
        drawSlice();
    }
    if (filename.contains(".skel")) {
        skeletonModel.setDataFromFile();
        skeletonModel.generate3DImFromData();
        image = skeletonModel.getSkeleton3DIm();
        currentImageType = ImageType::Image3D;
        updateImageComponents();
        drawSlice();
    }
}

void MainWindow::saveImage()
{
    if (image)
    {
        switch (currentImageType)
        {
            case ImageType::Dicom :
            {
                // Récupération du chemin et sauvegarde de la série en .dcm
                QString filename = QFileDialog::getSaveFileName(this, "Sauvegarde du DICOM", QDir::homePath(), "DICOM Files (*.dcm)");
                if (filename.isEmpty()) return;
                if (filename.section('.', -1) != "dcm")
                    filename += ".dcm";
                ITKTools<PixelType>::saveImage(filename.toStdString(), image);
                break;
            }
            case ImageType::Image3D :
            {
                QString filename = QFileDialog::getSaveFileName(this, "Sauvegarde de l'image 3D", QDir::homePath(), "Image3D (*.vol)");
                if (filename.isEmpty()) return;
                if (filename.section('.', -1) != "vol")
                    filename += ".vol";
                DGtalTools<PixelType>::saveImage3D(filename.toStdString(), image);
                break;
            }
            case ImageType::Empty : break;
        }
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Veuillez ouvrir un dossier DICOM.");
    }
}

bool MainWindow::closeImage()
{
    if (currentImageType != ImageType::Empty)
    {
        // Vérification de la fermeture de l'image
        QMessageBox::StandardButton reply;
        QString title = "Confirmation fermeture image";
        QString question = "Êtes-vous sûr de vouloir fermer l'image courante ? Les calques associés seront supprimés.";
        reply = QMessageBox::question(this, title, question, QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::No)
            return false;

        delete image;
        image = 0;
    }

    // On met à jour une image vide
    currentImageType = ImageType::Empty;
    imageScroll.resizeBlackImage(1, 1);
    drawSlice();
    setWindowTitle("OS3D");
    return true;
}

void MainWindow::drawSlice()
{
    if (currentImageType != ImageType::Empty)
    {
        // Mise à jour de l'image
        currentSlice = sliderSlice.value();
        if (layersThresholdWindow.isVisible())
        {
            QImage *qImage = layersThresholdWindow.getLayer(image, currentSlice);
            imageScroll.updateImage(qImage);
        }
        else
        {
            imageScroll.updateImage(image, currentSlice, *intervalIntensity);
            applyImageLayers();
        }
    }
    // Mise à jour de l'image via son label
    imageScroll.updateLabel();
}

void MainWindow::openAlgorithms()
{
    if (currentImageType != ImageType::Empty)
    {
        // Chargement de l'image courante et ouverture de la fenêtre filtre
        Image *currentImage = new Image(image->slice(currentSlice).memptr(), image->n_rows, image->n_cols, 1, true);
        AlgorithmsWindow *algorithmsWindow = new AlgorithmsWindow(currentImage, *intervalIntensity, this);
        QObject::connect(algorithmsWindow, SIGNAL(validatedAlgorithms(std::vector<Algorithm<Image, Image>*>, bool)),
                         this, SLOT(applyAlgorithms(std::vector<Algorithm<Image, Image>*>, bool)));
        QObject::connect(algorithmsWindow, SIGNAL(savedImageLayer(QString, std::vector<Algorithm<Image, Image>*>)),
                         this, SLOT(addImageLayer(QString, std::vector<Algorithm<Image, Image>*>)));
        algorithmsWindow->show();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Veuillez ouvrir un dossier DICOM.");
    }
}

void MainWindow::applyAlgorithms(const std::vector<Algorithm<Image, Image>*> &algorithmLogfile, bool sliceBySlice)
{
    // Application des différents filtres contenus dans appliedAlgorithms
    std::vector<Algorithm<Image, Image>*>::const_iterator beginAlgorithms = algorithmLogfile.begin();
    std::vector<Algorithm<Image, Image>*>::const_iterator endAlgorithms = algorithmLogfile.end();
    for (std::vector<Algorithm<Image, Image>*>::const_iterator itAlgorithms = beginAlgorithms; itAlgorithms != endAlgorithms; ++itAlgorithms)
    {
        try // Application de l'algorithme
        {
            if (sliceBySlice)
            {
                (*itAlgorithms)->executeSliceBySliceInPlace(image);
            }
            else
            {
                (*itAlgorithms)->executeInPlace(image);
            }
        }
        catch (const AlgorithmException &exception)
        {
            exception.manage(*itAlgorithms, image, this);
        }
    };
    // Mise à jour de l'image
    updateImageComponents();
    imageScroll.updateImage(image, currentSlice, *intervalIntensity);
    imageScroll.updateLabel();
}

void MainWindow::addImageLayer(const QString &name, const std::vector<Algorithm<Image, Image>*> &algorithmLogfile)
{
    // Application des différents filtres contenus dans appliedAlgorithms
    Image *imageLayer = new Image(*image);
    std::vector<Algorithm<Image, Image>*>::const_iterator beginAlgorithms = algorithmLogfile.begin();
    std::vector<Algorithm<Image, Image>*>::const_iterator endAlgorithms = algorithmLogfile.end();
    for (std::vector<Algorithm<Image, Image>*>::const_iterator itAlgorithms = beginAlgorithms; itAlgorithms != endAlgorithms; ++itAlgorithms)
    {
        try // Application de l'algorithme
        {
            (*itAlgorithms)->executeInPlace(imageLayer);
        }
        catch (const AlgorithmException &exception)
        {
            QMessageBox::critical(this, QString("Erreur algorithme"), QString(exception.what()));
        }
    }

    // Si l'utilisateur valide le nouveau calque image
    if (ImageLayer::checkSizeComponents(imageLayer))
    {
        // Si premier calque à ajouter on supprime l'action emptyImageLayer
        QAction *firstImageLayer = imageLayerMenu->actions()[0];
        if (firstImageLayer->text() == ACTION_EMPTY_IMAGE_LAYER)
            imageLayerMenu->removeAction(firstImageLayer);

        // Ajout de l'action au menu ajouter calque
        QAction *addImageLayer = new QAction(name, this);
        addImageLayer->setCheckable(true);
        imageLayerMenu->addAction(addImageLayer);
        QObject::connect(addImageLayer, SIGNAL(triggered()), this, SLOT(applyImageLayers()));
        QObject::connect(addImageLayer, SIGNAL(triggered()), this, SLOT(drawSlice()));

        // On ajoute le nouveau calque image et sa vue
        ImageLayer layer (name, imageLayer);
        imageLayers->push_back(layer);
        imageLayersWindow.addViewLayer(layer);
        imageLayersToolsWindow.addViewLayer(layer);
        imageLayersViewer3DWindow.addViewLayer(layer);
        imageLayersWindow.addApplyActionLayer(imageLayers->size()-1, addImageLayer);
    }
}

void MainWindow::applyImageLayers()
{
    if (imageLayersToolsWindow.isVisible() && !imageLayers->empty())
    {
        ImageLayer imageLayer = imageLayersToolsWindow.getCurrentImageLayer();
        imageScroll.addImageLayer(imageLayer, currentSlice);
        imageScroll.updateLabel();

    }
    else
    {
        QList<QAction*> actions = imageLayerMenu->actions();

        // Ajout des calques sélectionnés par l'utilisateur sur l'image courante
        int indexLayer = 0;
        for (QList<QAction*>::Iterator action=actions.begin(); action!=actions.end(); ++action, ++indexLayer)
        {
            if ((*action)->isChecked())
            {
                ImageLayer imageLayer = (*imageLayers)[indexLayer];
                imageScroll.addImageLayer(imageLayer, currentSlice);
                imageScroll.updateLabel();
            }
        }
    }
}

void MainWindow::removeActionImageLayer(int indexLayer)
{
    // Suppression de l'action
    imageLayerMenu->removeAction(imageLayerMenu->actions()[indexLayer]);

    if (imageLayerMenu->isEmpty())
    {
        // S'il n'y a plus d'image calque on le signale
        QAction *emptyImageLayer = new QAction(ACTION_EMPTY_IMAGE_LAYER, this);
        emptyImageLayer->setEnabled(false);
        imageLayerMenu->addAction(emptyImageLayer);
    }

    // On met à jour l'affichage
    drawSlice();
}

void MainWindow::openSecondaryWindow()
{
    if (currentImageType != ImageType::Empty)
    {
        // Ouverture d'une fenêtre secondaire en fonction de l'action choisie
        QAction *action = qobject_cast<QAction*>(this->sender());
        if (action->text() == ACTION_LAYER_THRESHOLD_TEXT)
            layersThresholdWindow.show();
        if (action->text() == ACTION_TOOLS_IMAGE_LAYER)
        {
            imageLayersToolsWindow.show();
            drawSlice();
        }
        if (action->text() == ACTION_MANAGE_IMAGE_LAYER)
            imageLayersWindow.show();
        if (action->text() == ACTION_VIEWER3D_IMAGE_LAYER)
            imageLayersViewer3DWindow.show();
        if (action->text() == ACTION_WINDOWING_TEXT)
            windowingWindow.show();
        if (action->text() == ACTION_HISTOGRAM_TEXT)
            histogramWindow.show();
        //if (action->text() == ACTION_CONVERT_TEXT)
        //    convertImageView.show();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Veuillez ouvrir un dossier DICOM.");
    }
}

void MainWindow::skeletonization() {
    skeletonModel.compute();
    image = skeletonModel.getSkeleton3DIm();
    currentImageType = ImageType::Image3D;
    updateImageComponents();
    drawSlice();
}

/*void MainWindow::convertImageRawToVol() {
    int x = convertImageView.getX();
    int y = convertImageView.getY();
    int z = convertImageView.getZ();
    QString filename = convertImage.rawToVol(x, y, z);
    image = DGtalTools<PixelType>::loadImage3D(filename.toStdString());
    skeletonModel.setSkeleton3DIm(image);
    updateImageComponents();
    drawSlice();
}*/

void MainWindow::getGraph(){
    image = skeletonModel.getSkeleton3DIm();
    skeletonGraph.setGraph(image);
    skeletonGraph.compute();
    skeletonGraph.exportGraph("graph.eps");
    //graphTest.setGraph(image);
    //graphTest.initGraph();
    //graphTest.compute();
    //graphTest.compute();
    // Mise à jour de l'image et de la fenêtre principale
    //QString filename = QFileDialog::getOpenFileName(this, "Sélection de l'image segmentée", QDir::homePath(), "Image3D (*.vol *.pgm3d)");
    //if (filename.isEmpty()) return;

    // Mise à jour de l'image et de la fenêtre principale
    image = skeletonGraph.getGraphImage3D();
    //image = skeletonGraph.getSkeleton3DIm();
    //image = graphTest.getSkeleton3DIm();
    currentImageType = ImageType::Image3D;
    updateImageComponents();
    drawSlice();
    DGtalTools<PixelType>::saveImage3D("graphImage3D.vol", image);

    QDialog *openglDialog = new QDialog(this);
    
    //openglDialog->setMinimumHeight(600);
    //openglDialog->setMinimumWidth(800);
    
    openglDialog->setObjectName(QString("3D Visualizer"));
    openglDialog->resize(500, 534);
    openglDialog->setMinimumSize(QSize(500, 534));
    QWidget *centralWidget = new QWidget(openglDialog);
    centralWidget->setObjectName(QString("centralWidget"));
    centralWidget->setMinimumSize(QSize(500, 500));
    QGridLayout *gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(6);
    gridLayout->setContentsMargins(11, 11, 11, 11);
    gridLayout->setObjectName(QString("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);
    MyOpenGLWidget *openGLWidget = new MyOpenGLWidget(centralWidget, image);
    openGLWidget->setObjectName(QString("openGLWidget"));
    openGLWidget->setMinimumSize(QSize(500, 500));
    
    gridLayout->addWidget(openGLWidget, 0, 0, 1, 1);
    
    //openglDialog->set
    
    //QWidget *widget = new MyOpenGLWidget(openglDialog);
    
    openglDialog->show();
    //widget->show();

    // Fermeture de l'image courante
    /*if (!closeImage())
        return;
    // Suppression des calques
    removeLayers();*/
    //skeletonView.setFilename(filename);
}
