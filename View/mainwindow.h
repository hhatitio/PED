#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "deftypes.h"
//#include "graphtest.h"

#include "Model/Algorithm/algorithm.h"
#include "Model/Layer/imagelayer.h"
#include "Model/skeletonmodel.h"
//#include "Model/convertimage.h"
#include "Model/skeletongraph.h"

#include "View/imagescroll.h"
#include "View/Algorithm/algorithmswindow.h"
#include "View/Layer/imagelayerswindow.h"
#include "View/Layer/imagelayerstoolswindow.h"
#include "View/Layer/layersthresholdwindow.h"
#include "View/Layer/imagelayersviewer3dwindow.h"
#include "View/opendicomwindow.h"
#include "View/histogramwindow.h"
#include "View/windowingwindow.h"
//#include "View/convertimageview.h"

#include <QMainWindow>
#include <QImage>
#include <QSlider>
#include <QRadioButton>

#define MAINWINDOW_MINIMUM_HEIGHT               400
#define MAINWINDOW_MINIMUM_WIDTH                600

#define ACTION_WINDOWING_TEXT           "&Fenêtrage"
#define ACTION_HISTOGRAM_TEXT           "&Histogramme"
#define ACTION_SKELETONIZATION_TEXT     "&Squelettisation"
//#define ACTION_CONVERT_TEXT             "&Convertir (Raw to Vol)"
#define ACTION_GRAPH_TEXT               "&Generate Graph"

#define ACTION_EMPTY_IMAGE_LAYER        "&Aucune image calque"
#define ACTION_LAYER_THRESHOLD_TEXT     "&Calques Seuillage"
#define ACTION_TOOLS_IMAGE_LAYER        "&Outils calque"
#define ACTION_MANAGE_IMAGE_LAYER       "&Gestion des images calques"
#define ACTION_VIEWER3D_IMAGE_LAYER     "&Visualisation 3D"

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum ImageType { Empty , Dicom , Image3D };

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void buildMenus();
    void updateImageComponents();
    void removeLayers();
    void removeLayer(QString name);

    bool eventFilter(QObject *object, QEvent *event);

public slots:

    // Gestion des fichiers images
    void chooseDicom();
    void openDicom(const std::vector<std::string> &serieFileNames);
    void openImage3D();
    void openOthersImage3D();
    void saveImage();
    bool closeImage();

    // Affichage de l'image
    void drawSlice();

    // Gestion des filtres
    void openAlgorithms();
    void applyAlgorithms(const std::vector<Algorithm<Image, Image>*> &algorithmLogfile, bool sliceBySlice);

    // Gestion des calques Image
    void addImageLayer(const QString &name, const std::vector<Algorithm<Image, Image>*> &algorithmLogfile);
    void autoAddImageLayer(const QString &name, Image * imageLayer);
    void autoAddImageLayer(Image * imageLayer);
    void applyImageLayers();
    void removeActionImageLayer(int indexLayer);

    // Gestion des fenêtres secondaires
    void openSecondaryWindow();

    void skeletonization();
    //void convertImageRawToVol();
    void getGraph();

private:

    Image *image;
    Image *skeletonImage;
    ImageType currentImageType = ImageType::Empty;

    QMenu *imageLayerMenu;
    std::vector<ImageLayer> *imageLayers;

    QSlider sliderSlice;
    ImageScroll imageScroll;
    ImageLayersWindow imageLayersWindow;
    ImageLayersToolsWindow imageLayersToolsWindow;
    ImageLayersViewer3DWindow imageLayersViewer3DWindow;
    LayersThresholdWindow layersThresholdWindow;
    HistogramWindow histogramWindow;
    WindowingWindow windowingWindow;
    SkeletonModel *skeletonModel;
    //ConvertImage convertImage;
    //ConvertImageView convertImageView;
    SkeletonGraph  *skeletonGraph;

    Interval<PixelType> *intervalIntensity;

    int currentSlice;
    int layersCount;
};

#endif // MAINWINDOW_H
