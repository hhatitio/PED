#------------------------------------------------
#
# Project created by QtCreator 2015-11-23T15:45:50
#
#               Configuration OS3D
#
#------------------------------------------------

# Version d'ITK installée

ITK_VERSION = itk4
ITK_NUMBER = 4.8

# Configuration

TEMPLATE = app

QT += widgets gui printsupport opengl xml

CONFIG += qt opengl warn_on release
CONFIG += $${ITK_VERSION} $${ITK_NUMBER}

# Répertoires

TARGET = OS3D
MOC_DIR = moc
OBJECTS_DIR = obj

# Fichiers

SOURCES += main.cpp\
	Model/zoomer.cpp \
        Model/Layer/imagelayer.cpp \
        Model/Algorithm/algorithmexception.cpp \
	View/mainwindow.cpp \
        View/imagescroll.cpp \
        View/opendicomwindow.cpp \
        View/histogramwindow.cpp \
        View/windowingwindow.cpp \
        View/Layer/imagelayerswindow.cpp \
        View/Layer/layersthresholdwindow.cpp \
        View/Layer/imagelayerstoolswindow.cpp \
        View/Layer/imagelayersviewer3dwindow.cpp \
        View/Algorithm/algorithmswindow.cpp \
        View/Customplot/qcustomplot.cpp \
        View/Spanslider/qxtspanslider.cpp \
    View/viewer3d.cpp \
    View/skeletonwindow.cpp

HEADERS  += deftypes.h \
	Model/zoomer.h \
	Model/image3D.h \
	Model/itktools.h \
        Model/dgtaltools.h \
	Model/interval.h \
        Model/Layer/imagelayer.h \
        Model/Algorithm/algorithmexception.h \
	Model/Algorithm/algorithm.h \
        Model/Algorithm/ITK/algorithmitk.h \
        Model/Algorithm/ITK/algorithmitkdiscretegaussian.h \
        Model/Algorithm/ITK/algorithmitkbinarythreshold.h \
        Model/Algorithm/ITK/algorithmitkthreshold.h \
        Model/Algorithm/ITK/algorithmitkconnectedcomponent.h \
        Model/Algorithm/ITK/algorithmitklabelshapekeepnobjects.h \
        Model/Algorithm/ITK/algorithmitkbinarymorphologicalclosing.h \
        Model/Algorithm/ITK/algorithmitkbinarythinningimagefilter.h \
        Model/Algorithm/Adrien/algorithmconnectedcomponentextraction.h \
        Model/Algorithm/DGtal/algorithmdgtal.h \
        Model/Algorithm/DGtal/algorithmdgtaldistancetransformation.h \
        View/mainwindow.h \
        View/imagescroll.h \
        View/opendicomwindow.h \
        View/histogramwindow.h \
        View/windowingwindow.h \
        View/Layer/imagelayerswindow.h \
        View/Layer/layersthresholdwindow.h \
        View/Layer/imagelayerstoolswindow.h \
        View/Layer/imagelayersviewer3dwindow.h \
        View/Algorithm/algorithmswindow.h \
	View/Algorithm/algorithmview.h \
        View/Algorithm/ITK/algorithmviewitkdiscretegaussian.h \
        View/Algorithm/ITK/algorithmviewitkbinarythreshold.h \
        View/Algorithm/ITK/algorithmviewitkthreshold.h \
        View/Algorithm/ITK/algorithmviewitkconnectedcomponent.h \
        View/Algorithm/ITK/algorithmviewitklabelshapekeepnobjects.h \
        View/Algorithm/ITK/algorithmviewitkbinarymorphologicalclosing.h \
        View/Algorithm/ITK/algorithmviewitkbinarythinningimagefilter.h \
        View/Algorithm/Adrien/algorithmviewconnectedcomponentextraction.h \
        View/Algorithm/DGtal/algorithmviewdgtaldistancetransformation.h \
        View/Customplot/qcustomplot.h \
        View/Spanslider/qxtglobal.h \
        View/Spanslider/qxtspanslider.h \
        View/Spanslider/qxtspanslider_p.h \
    View/viewer3d.h \
    View/skeletonwindow.h \
    Model/Algorithm/ITK/algorithmitkbinarythinning3d.h \
    View/Algorithm/ITK/algorithmviewitkbinarythinning3d.h

# Directives compilateur

QMAKE_CXXFLAGS += -std=c++11

# Librairies externes

INCLUDEPATH += /usr/include/ /usr/local/include/
DEPENDPATH += /usr/local/include
QMAKE_LIBDIR += /usr/local/lib/

# Armadillo

DEFINES	+= ARMA_DONT_USE_WRAPPER
LIBS += -larmadillo

# ITK

ITK_PATH = /usr/local/include/ITK-$${ITK_NUMBER}/
INCLUDEPATH += $${ITK_PATH}/

LIBS += -lITKIOGDCM-$${ITK_NUMBER} \
		-litkgdcmDICT-$${ITK_NUMBER} \
		-litkgdcmMSFF-$${ITK_NUMBER} \
		-litkgdcmIOD-$${ITK_NUMBER} \
		-litkgdcmDSED-$${ITK_NUMBER} \
		-litkzlib-$${ITK_NUMBER} \
		-litkgdcmCommon-$${ITK_NUMBER} \
		-litkgdcmuuid-$${ITK_NUMBER} \

4.8  { LIBS += -litkgdcmcharls-$${ITK_NUMBER} -litkgdcmopenjpeg-$${ITK_NUMBER} }
else { LIBS += -litkopenjpeg-$${ITK_NUMBER} }

LIBS +=	-litkgdcmjpeg12-$${ITK_NUMBER} \
		-litkgdcmjpeg16-$${ITK_NUMBER} \
		-litkgdcmjpeg8-$${ITK_NUMBER} \
		-lITKIOImageBase-$${ITK_NUMBER} \
		-lITKCommon-$${ITK_NUMBER} \
		-litksys-$${ITK_NUMBER} \
		-litkvnl_algo-$${ITK_NUMBER} \
		-litkv3p_netlib-$${ITK_NUMBER} \
		-litkvnl-$${ITK_NUMBER} \
		-lITKIOTIFF-$${ITK_NUMBER} \
		-litktiff-$${ITK_NUMBER} \
		-litkjpeg-$${ITK_NUMBER} \
		-lITKVNLInstantiation-$${ITK_NUMBER} \
		-lITKLabelMap-$${ITK_NUMBER} \
		-lITKStatistics-$${ITK_NUMBER} \
		-litkdouble-conversion-$${ITK_NUMBER} \
		-ldl \
		-lexpat \

# DGtal

LIBS += -lDGtal

# QGLViewer

DEFINES += WITH_VISU3D_QGLVIEWER
LIBS += -lQGLViewer -lGLU

# Lemon

LIBS += -lemon

# pfSkel

SOURCES += pfSkel/common.cpp \
            pfSkel/pfSkel.cpp \
            pfSkel/CritPts/CritPts.cpp \
            pfSkel/ExpandVol/expandVol.cpp \
            pfSkel/HighDiverg/HighDiverg.cpp \
            pfSkel/MakeSolidVol/makeSolidVol.cpp \
            pfSkel/PotField/potVect.cpp \
            pfSkel/StreamLn/StreamLn.cpp

HEADERS += pfSkel/common.h \
            pfSkel/pfSkel.h \
            pfSkel/CritPts/CritPts.h \
            pfSkel/ExpandVol/expandVol.h \
            pfSkel/HighDiverg/HighDiverg.h \
            pfSkel/JAMA/jama_cholesky.h \
            pfSkel/JAMA/jama_eig.h \
            pfSkel/JAMA/jama_lu.h \
            pfSkel/JAMA/jama_qr.h \
            pfSkel/JAMA/jama_svd.h \
            pfSkel/MakeSolidVol/makeSolidVol.h \
            pfSkel/PotField/potVect.h \
            pfSkel/StreamLn/StreamLn.h \
            pfSkel/TNT/tnt.h \
            pfSkel/TNT/tnt_array1d.h \
            pfSkel/TNT/tnt_array1d_utils.h \
            pfSkel/TNT/tnt_array2d.h \
            pfSkel/TNT/tnt_array2d_utils.h \
            pfSkel/TNT/tnt_array3d.h \
            pfSkel/TNT/tnt_array3d_utils.h \
            pfSkel/TNT/tnt_cmat.h \
            pfSkel/TNT/tnt_fortran_array1d.h \
            pfSkel/TNT/tnt_fortran_array1d_utils.h \
            pfSkel/TNT/tnt_fortran_array2d.h \
            pfSkel/TNT/tnt_fortran_array2d_utils.h \
            pfSkel/TNT/tnt_fortran_array3d.h \
            pfSkel/TNT/tnt_fortran_array3d_utils.h \
            pfSkel/TNT/tnt_i_refvec.h \
            pfSkel/TNT/tnt_math_utils.h \
            pfSkel/TNT/tnt_sparse_matrix_csr.h \
            pfSkel/TNT/tnt_stopwatch.h \
            pfSkel/TNT/tnt_subscript.h \
            pfSkel/TNT/tnt_vec.h \
            pfSkel/TNT/tnt_version.h
