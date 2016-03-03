#include "convertimageview.h"

ConvertImageView::ConvertImageView(QWidget *parent)
{
    // Initialisation de la boîte de dialogue du fenêtrage
    QFormLayout *layout = new QFormLayout(this);
    QPushButton *button = new QPushButton("Convert", this);
    layout->addRow("x = ", &x);
    layout->addRow("y = ", &y);
    layout->addRow("z = ", &z);
    layout->addWidget(button);

    // Connexion pour la validation et le changement de valeurs
    QObject::connect(button, SIGNAL(clicked()), parent, SLOT(convertImageRawToVol()));

    setWindowTitle("OS3D - Convert Image");
    setMinimumWidth(200);
}

int ConvertImageView::getX() {
    return x.text().toInt();
}

int ConvertImageView::getY() {
    return y.text().toInt();
}

int ConvertImageView::getZ() {
    return z.text().toInt();
}

