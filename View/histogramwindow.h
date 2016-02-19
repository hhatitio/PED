#ifndef HISTOGRAMWINDOW_H
#define HISTOGRAMWINDOW_H

#include "../deftypes.h"
#include "Customplot/qcustomplot.h"

#include <QVector>

#define HISTOGRAM_WINDOW_MINIMUM_HEIGHT 300
#define HISTOGRAM_WINDOW_MINIMUM_WIDTH  500

class HistogramWindow
{

public:

    HistogramWindow();

	void setHistogramData(Image *image);

	void show();

private:

	QCustomPlot histogramPlot;
    QCPBars *barsHisto;

};

#endif // HISTOGRAMWINDOW_H
