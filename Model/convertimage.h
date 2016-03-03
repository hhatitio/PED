#ifndef CONVERTIMAGE_H
#define CONVERTIMAGE_H

#include <QString>
#include <QProcess>
#include <iostream>

class ConvertImage
{
public:
    ConvertImage();
    void setFilename(QString filename);
    QString rawToVol(int x, int y, int z);

private:
    QString filename;
};

#endif // CONVERTIMAGE_H
