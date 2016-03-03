#include "convertimage.h"

ConvertImage::ConvertImage()
{

}

void ConvertImage::setFilename(QString filename) {
    this->filename = filename;
}

QString ConvertImage::rawToVol(int x, int y, int z) {
    int i = this->filename.lastIndexOf(".");
    std::cout << "i = " << i << std::endl;
    QString output = this->filename;
    output.replace(i, 4, ".vol");

    QString cmdRaw2Vol("raw2vol");
    cmdRaw2Vol += QString (" --x %1").arg(x);
    cmdRaw2Vol += QString (" --y %1").arg(y);
    cmdRaw2Vol += QString (" --z %1").arg(z);
    cmdRaw2Vol += " --input "+this->filename+" --output "+output;
    std::cout << cmdRaw2Vol.toStdString() << std::endl;

    QProcess::execute(cmdRaw2Vol);

    return output;
}
