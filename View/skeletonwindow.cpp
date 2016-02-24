#include "skeletonwindow.h"

SkeletonWindow::SkeletonWindow()
{
    std::cout << "SkeletonWindow - Constructeur" << std::endl;
}

void SkeletonWindow::compute() {
    std::cout << "SkeletonWindow - show" << std::endl;
    Skeleton *Skel;
    Skel = NULL;
    AllocateSkeleton(&Skel, 50000, 5000);
}
