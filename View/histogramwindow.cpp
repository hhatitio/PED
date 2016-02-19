#include "histogramwindow.h"

HistogramWindow::HistogramWindow()
{
	// Creation du graphe
	histogramPlot.setMinimumHeight(HISTOGRAM_WINDOW_MINIMUM_HEIGHT);
	histogramPlot.setMinimumWidth(HISTOGRAM_WINDOW_MINIMUM_WIDTH);
	histogramPlot.setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    barsHisto = new QCPBars(histogramPlot.xAxis, histogramPlot.yAxis);
    histogramPlot.addPlottable(barsHisto);
}

void HistogramWindow::setHistogramData(Image *image)
{
	// Calcul des données de l'histogramme
	int minValue = image->min();
	int maxValue = image->max();

	QVector<double> ticks(maxValue-minValue+1, 0.);
	QVector<double> histogramData(maxValue-minValue+1, 0.);
	double maxCount = 0.;
	for (Image::iterator it = image->begin(); it != image->end(); ++it)
	{
		histogramData[*it-minValue] += 1;
        maxCount = qMax(maxCount, histogramData[*it-minValue]);
	}

	int numTick = minValue;
	for( QVector<double>::iterator it = ticks.begin() ; it != ticks.end() ; ++it, ++numTick )
	{
		*it = numTick;
	}

	// Initialisation des axes X et Y
	histogramPlot.yAxis->setRange(0, maxCount);
	histogramPlot.xAxis->setRange(minValue, maxValue);

	// Ajout des données
    barsHisto->setData(ticks, histogramData);

	histogramPlot.replot();
}

void HistogramWindow::show()
{
	histogramPlot.show();
}
