#ifndef SLICEZOOMER_H
#define SLICEZOOMER_H

#include <QObject>
#include <QPoint>

#define ZOOM_COEF_IN	1.25
#define ZOOM_COEF_OUT	 .8
#define ZOOM_OUT_ON_IN	 .64

#define ZOOM_MAX_FACTOR	10.
#define ZOOM_MIN_FACTOR	 .1

class Zoomer : public QObject
{
    Q_OBJECT

public:
    explicit Zoomer(QObject *parent = 0);
    virtual ~Zoomer();

    qreal getFactor() const;
    qreal getCoefficient() const;

protected:
    bool eventFilter(QObject *obj,QEvent *event);

signals:
    void isMovedFrom(const QPoint movementVector);
    void zoomFactorChanged(const qreal newZoomFactor, const qreal newZoomCoefficient);

public slots:
    void resetZoom();

private:
    bool    isDraging;
    qreal   factor;
    qreal   coefficient;
    QPoint  pointStartDrag;
};

#endif // ZOOMER_H
