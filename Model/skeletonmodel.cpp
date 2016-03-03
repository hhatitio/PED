#include "skeletonmodel.h"

SkeletonModel::SkeletonModel() {

}

void SkeletonModel::setFilename(QString filename) {
    this->filename = filename;
}

void SkeletonModel::compute() {
    std::cout << "Execute process" << std::endl;
    QString cmdImageJ(PATH_TO_IMAGEJ);
    cmdImageJ += " \""+filename+"\""+MAKE_BINARY+SKELETONIZE+SAVE_AS;
    std::cout << "cmdImageJ = " << cmdImageJ.toStdString() << std::endl;
    QString cmdRaw2Vol("raw2vol"); /** TODO **/
    QProcess::execute(cmdImageJ);
}
