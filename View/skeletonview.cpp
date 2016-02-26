#include "skeletonview.h"

SkeletonView::SkeletonView(QWidget *parent) {
    skeletonModel = new SkeletonModel();

    // Initialisation de la boîte de dialogue du fenêtrage
    QFormLayout *layout = new QFormLayout(this);
    QPushButton *button = new QPushButton("Compute", this);
    layout->addRow("Dist Charges", &distCharges);
    layout->addRow("Field Strenght", &fieldStrenght);
    layout->addRow("Perc HDP", &percHDPts);
    layout->addWidget(button);

    // Connexion pour la validation et le changement de valeurs
    QObject::connect(button, SIGNAL(clicked()), parent, SLOT(openSkeletonization()));

    setWindowTitle("OS3D - Squelettisation");
    setMinimumWidth(200);
}

SkeletonView::~SkeletonView() {}

void SkeletonView::setFilename(QString filename) {
    this->filename = filename;
}

void SkeletonView::setImage(Image *image) {
    this->image = image;
}

void SkeletonView::skeletonization() {
    int dc = distCharges.value();
    int fs = fieldStrenght.value();
    int p = percHDPts.value();
    skeletonModel->compute(image, filename, dc, fs, p);
}
