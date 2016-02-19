#ifndef OPENDICOMWINDOW_H
#define OPENDICOMWINDOW_H

#include "imagescroll.h"

#include <QDialog>
#include <QString>
#include <QRadioButton>

#define OPENDICOM_WINDOW_MINIMUM_HEIGHT   400
#define OPENDICOM_WINDOW_MINIMUM_WIDTH    800

class OpenDicomWindow : public QDialog
{
    Q_OBJECT

public:

    static const std::vector<std::string> InfoDicomKeys;

    OpenDicomWindow(const QString &folderName, QWidget *parent = 0);
    bool isDicomFolder();

public slots:

    void validDicom();
    void changeImage();

signals:

    void choosenDicom(const std::vector<std::string> &serieFileNames);

private:

    std::vector<QRadioButton*> buttonDICOMSeries;
    std::vector< std::vector<std::string> > seriesFileNames;

    ImageScroll imageScroll;

};

#endif // OPENDICOMWINDOW_H
