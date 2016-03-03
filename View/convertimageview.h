#ifndef CONVERTIMAGEVIEW_H
#define CONVERTIMAGEVIEW_H

#include <QString>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>

class ConvertImageView : public QDialog
{
public:
    ConvertImageView(QWidget *parent);
    int getX();
    int getY();
    int getZ();

private:
    QLineEdit x, y, z;
};

#endif // CONVERTIMAGEVIEW_H
