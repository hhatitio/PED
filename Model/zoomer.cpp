#include "Model/zoomer.h"

#include <QMouseEvent>
#include <QDebug>

Zoomer::Zoomer(QObject *parent) : QObject(parent),
    isDraging(false), factor(1), coefficient(0), pointStartDrag(0,0) { }

Zoomer::~Zoomer() { }

qreal Zoomer::getFactor() const
{
    return factor;
}

qreal Zoomer::getCoefficient() const
{
    return coefficient;
}

bool Zoomer::eventFilter(QObject *obj, QEvent *event)
{
    switch (event->type())
    {
        case QEvent::MouseButtonPress:
        {
            isDraging = true;
            pointStartDrag = static_cast<const QMouseEvent *>(event)->pos();
        }
            break;
        case QEvent::MouseMove:
        {
            const QMouseEvent* mouseEvent = static_cast<const QMouseEvent*>(event);
            if (isDraging && mouseEvent->buttons() & Qt::LeftButton)
            {
                const QMouseEvent *mouseEvent = static_cast<const QMouseEvent *>(event);
                const QPoint mousePos = mouseEvent->pos();
                const QPoint movementVector = mousePos - pointStartDrag;
                pointStartDrag = mousePos;
                emit isMovedFrom(movementVector);
            }
        }
            break;
        case QEvent::MouseButtonRelease:
        {
                isDraging = false;
        }
            break;
        case QEvent::Wheel:
        {
            const QWheelEvent *wheelEvent = static_cast<const QWheelEvent *>(event);
            const int wheelDelta = wheelEvent->delta();
            if ( wheelDelta )
            {
                const qreal newZoomCoefficient = wheelDelta > 0 ? ZOOM_COEF_IN : ZOOM_COEF_OUT;
                const qreal newZoomFactor = factor * newZoomCoefficient;
                if (newZoomFactor > ZOOM_MIN_FACTOR && newZoomFactor < ZOOM_MAX_FACTOR)
                {
                    coefficient = newZoomCoefficient;
                    factor = newZoomFactor;
                    emit zoomFactorChanged(newZoomFactor, newZoomCoefficient);
                }
            }
        }
            break;
        default:
            break;
    }
    return QObject::eventFilter(obj,event);
}

void Zoomer::resetZoom()
{
    isDraging = false;
    coefficient = 1./factor;
    factor = 1.;
    emit zoomFactorChanged(factor, coefficient);
}
