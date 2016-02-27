#include "skeletongraphview.h"

SkeletonGraphView::SkeletonGraphView(QWidget *parent) {
    skeletongraph = new SkeletonGraph();

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

SkeletonGraphView::~SkeletonGraphView() {}

void SkeletonGraphView::showGraph() {
    int dc = distCharges.value();
    int fs = fieldStrenght.value();
    int p = percHDPts.value();
    skeletongraph->compute();
}
