#ifndef ALGORITHMSWINDOW_H
#define ALGORITHMSWINDOW_H

#include "deftypes.h"

#include "Model/interval.h"
#include "Model/Algorithm/algorithm.h"

#include "View/imagescroll.h"
#include "View/Algorithm/algorithmview.h"

#include <QDialog>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QListWidget>

#define ALGORITHMS_WINDOW_MINIMUM_HEIGHT                700
#define ALGORITHMS_WINDOW_MINIMUM_WIDTH                 1000

#define ALGORITHMS_EDIT_LOGFILE_WINDOW_MINIMUM_WIDTH    500

#define LABEL_INDEX_ALGORITHM_LOGFILE_PROPERTY "indexAlgorithmLogfile"
#define LABEL_PARAMETERS_ALGORITHM_LOGFILE_PROPERTY "parametersAlgorithmLogfile"

class AlgorithmsWindow : public QDialog
{
    Q_OBJECT

public:

    AlgorithmsWindow(Image *image, const Interval<PixelType> &intervalIntensity, QWidget *parent);
    ~AlgorithmsWindow();

    void buildAlgorithms();
    void clearLayout(QLayout *layout);

public slots:

    void changeAlgorithm(int indexAlgorithm);
    void previewAlgorithm();
    void validPreviewAlgorithm();
    void saveImageLayer();
    void applyLogfileAlgorithm();
    void openEditDialogLogfileAlgorithm(QListWidgetItem *item);
    void editLogfileAlgorithm();
    void moveLogfileAlgorithm(const QModelIndex &srcParent, int srcStart, int srcEnd, const QModelIndex &destParent, int destStart);
    void deleteLogfileAlgorithm();
    void validAlgorithms();

signals:

    void validatedAlgorithms(const std::vector<Algorithm<Image, Image>*> &AlgorithmLogfile, bool sliceBySlice);
    void savedImageLayer(const QString &name, const std::vector<Algorithm<Image, Image>*> &AlgorithmLogfile);

private:

    Image *image;
    Image *copyImage;

    Interval<PixelType> intervalIntensity;

    std::map<AlgorithmView*, Algorithm<Image, Image>*> algorithms;
    std::vector<Algorithm<Image, Image>*> algorithmLogfile;

    QDialog editDialogLogfileAlgorithm;
    QListWidget listWidgetLogfile;

    ImageScroll imageScroll;

    int currentAlgorithm = -1;
};

#endif // ALGORITHMSWINDOW_H
