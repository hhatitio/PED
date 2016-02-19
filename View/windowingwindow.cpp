#include "windowingwindow.h"

#include <QFormLayout>
#include <QPushButton>

WindowingWindow::WindowingWindow(QWidget *parent)
{
    // Initialisation de la boîte de dialogue du fenêtrage
    QFormLayout *layout = new QFormLayout(this);
    QPushButton *reset = new QPushButton("Réinitialiser", this);
    layout->addRow("Centre", &centerWindowing);
    layout->addRow("Largeur", &widthWindowing);
    layout->addWidget(reset);

    // Connexion pour la validation et le changement de valeurs
    QObject::connect(&centerWindowing, SIGNAL(valueChanged(int)), this, SLOT(changeWindowing()));
    QObject::connect(&centerWindowing, SIGNAL(valueChanged(int)), parent, SLOT(drawSlice()));
    QObject::connect(&widthWindowing, SIGNAL(valueChanged(int)), this, SLOT(changeWindowing()));
    QObject::connect(&widthWindowing, SIGNAL(valueChanged(int)), parent, SLOT(drawSlice()));
    QObject::connect(reset, SIGNAL(clicked()), this, SLOT(resetWindowing()));
    QObject::connect(reset, SIGNAL(clicked()), parent, SLOT(drawSlice()));

    setWindowTitle("OS3D - Fenêtrage");
    setMinimumWidth(WINDOWING_MINIMUM_WIDTH);
}

WindowingWindow::~WindowingWindow() { }

void WindowingWindow::buildWindowing(Interval<PixelType> *interval)
{
    // Construction de la fenêtre en fonction de l'interval d'intensité
    intervalIntensity = *interval;
    currentIntervalIntensity = interval;
    centerWindowing.setMinimum(intervalIntensity.min());
    centerWindowing.setMaximum(intervalIntensity.max());
    centerWindowing.setValue(intervalIntensity.mid());
    widthWindowing.setMaximum(intervalIntensity.max());
    widthWindowing.setValue(intervalIntensity.width());
}

void WindowingWindow::changeWindowing()
{
    // Mise à jour du fenêtrage
    currentIntervalIntensity->setMin(centerWindowing.value()-(widthWindowing.value()/2));
    currentIntervalIntensity->setMax(centerWindowing.value()+(widthWindowing.value()/2));
}

void WindowingWindow::resetWindowing()
{
    // Réinitialisation du fenêtrage
    PixelType min = intervalIntensity.min();
    PixelType max = intervalIntensity.max();
    centerWindowing.setValue((min+max)/2);
    widthWindowing.setValue(max-min);
    currentIntervalIntensity->setMin(min);
    currentIntervalIntensity->setMax(max);
}
