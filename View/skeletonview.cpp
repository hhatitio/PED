#include "skeletonview.h"

SkeletonView::SkeletonView() {
    skeletonModel = new SkeletonModel();
}

void SkeletonView::show(Image *image) {
    skeletonModel->compute(image, filename);
}

void SkeletonView::setFilename(QString filename) {
    this->filename = filename;
}
