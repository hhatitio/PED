#include "algorithmexception.h"

#include "../itktools.h"

#include <QMessageBox>

AlgorithmException::AlgorithmException(const std::string &msg)
{
    this->msg = msg;
}

const char * AlgorithmException::what() const throw()
{
    return this->msg.c_str();
}

bool AlgorithmException::manage(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent) const throw()
{
    bool reply = false;

    // Gestion de l'exception en fonction du message qu'il contient
    if (msg == MESSAGE_NO_INPLACE_FUNCTION_EXCEPTION)
        reply = manageInPlace(algorithm, image, parent);
    else if (msg == MESSAGE_NO_OUTPLACE_FUNCTION_EXCEPTION)
        reply = manageOutPlace(algorithm, image, parent);
    else if (msg == MESSAGE_NO_SLICE_BY_SLICE_INPLACE_FUNCTION_EXCEPTION)
        reply = manageSliceBySliceInPlace(algorithm, image, parent);
    else if (msg == MESSAGE_NO_SLICE_BY_SLICE_OUTPLACE_FUNCTION_EXCEPTION)
        reply = manageSliceBySliceOutPlace(algorithm, image, parent);

    return reply;
}

bool AlgorithmException::manageInPlace(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent) const throw()
{
    // Proposition d'une autre solution à l'utilisateur
    QString msgCritical = QString::fromStdString(algorithm->getName() + " : " + msg);
    msgCritical += QString("\n\nSouhaitez vous exécuter ce filtre avec la fonction outPlace ?");
    QMessageBox::StandardButton reply = QMessageBox::critical(parent, QString(TITLE_EXCEPTION_WINDOW), msgCritical, QMessageBox::No|QMessageBox::Yes);
    if (reply == QMessageBox::Yes)
    {
        try // Si l'utilisateur accepte on essaye la seconde possibilité
        {
            Image *result = algorithm->executeOutPlace(image);
            image->fillWithImage(*result);
            delete result;
            return true;
        }
        catch (const AlgorithmException &exception)
        {
            // Gestion de l'exception levée par la seconde possibilité
            msgCritical = QString::fromStdString(algorithm->getName() + " : ") + QString(exception.what());
            QMessageBox::critical(parent, QString(TITLE_EXCEPTION_WINDOW), msgCritical);
            return false;
        }
    }
    return false;
}

bool AlgorithmException::manageOutPlace(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent) const throw()
{
    // Proposition d'une autre solution à l'utilisateur
    QString msgCritical = QString::fromStdString(algorithm->getName() + " : " + msg);
    msgCritical += QString("\n\nSouhaitez vous exécuter ce filtre avec la fonction inPlace ?");
    QMessageBox::StandardButton reply = QMessageBox::critical(parent, QString(TITLE_EXCEPTION_WINDOW), msgCritical, QMessageBox::No|QMessageBox::Yes);
    if (reply == QMessageBox::Yes)
    {
        try // Si l'utilisateur accepte on essaye la seconde possibilité
        {
            algorithm->executeInPlace(image);
            return true;
        }
        catch (const AlgorithmException &exception)
        {
            // Gestion de l'exception levée par la seconde possibilité
            msgCritical = QString::fromStdString(algorithm->getName() + " : ") + QString(exception.what());
            QMessageBox::critical(parent, QString(TITLE_EXCEPTION_WINDOW), msgCritical);
            return false;
        }
    }
    return false;
}

bool AlgorithmException::manageSliceBySliceInPlace(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent) const throw()
{
    // Proposition d'une autre solution à l'utilisateur
    QString msgCritical = QString::fromStdString(algorithm->getName() + " : " + msg);
    msgCritical += QString("\n\nSouhaitez vous exécuter ce filtre sans le mode SliceBySlice ?");
    QMessageBox::StandardButton reply = QMessageBox::critical(parent, QString(TITLE_EXCEPTION_WINDOW), msgCritical, QMessageBox::No|QMessageBox::Yes);
    if (reply == QMessageBox::Yes)
    {
        try  // Si l'utilisateur accepte on essaye la seconde possibilité
        {
            algorithm->executeInPlace(image);
            return true;
        }
        catch (const AlgorithmException &exception)
        {
            // Gestion de l'exception levée par la seconde possibilité
            return exception.manage(algorithm, image, parent);
        }
    }
    return false;
}

bool AlgorithmException::manageSliceBySliceOutPlace(Algorithm<Image, Image> *algorithm, Image *image, QWidget *parent) const throw()
{
    // Proposition d'une autre solution à l'utilisateur
    QString msgCritical = QString::fromStdString(algorithm->getName() + " : " + msg);
    msgCritical += QString("\n\nSouhaitez vous exécuter ce filtre sans le mode SliceBySlice ?");
    QMessageBox::StandardButton reply = QMessageBox::critical(parent, QString(TITLE_EXCEPTION_WINDOW), msgCritical, QMessageBox::No|QMessageBox::Yes);
    if (reply == QMessageBox::Yes)
    {
        try  // Si l'utilisateur accepte on essaye la seconde possibilité
        {
            Image *result = algorithm->executeOutPlace(image);
            image->fillWithImage(*result);
            delete result;
            return true;
        }
        catch (const AlgorithmException &exception)
        {
            // Gestion de l'exception levée par la seconde possibilité
            return exception.manage(algorithm, image, parent);
        }
    }
    return false;
}
