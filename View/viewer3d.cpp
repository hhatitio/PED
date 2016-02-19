#include "viewer3d.h"

void Viewer3D::setImageLayer(const ImageLayer &imageLayer)
{
    this->imageLayer = imageLayer;
}

void Viewer3D::initGLScene()
{
    // Initialize values of the scene
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (GLfloat)width()/(GLfloat)height(), 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glDisable(GL_LIGHTING);
}

void Viewer3D::init()
{
    camera()->setPosition(qglviewer::Vec(0, 0, 100));
}

void Viewer3D::draw()
{
    initGLScene();

    std::vector<GLfloat*> vertices;
    std::vector<GLfloat*> colors;

    Image *image = imageLayer.getImage();
    std::map<PixelType, QColor> components = imageLayer.getComponents();

    double center[3] = {
        ((double) image->n_rows)/2,
        ((double) image->n_cols)/2,
        ((double) image->n_slices)/2
    };

    for (unsigned int row=0; row<image->n_rows; ++row)
    {
        for (unsigned int col=0; col<image->n_cols; ++col)
        {
            for (unsigned int slice=0; slice<image->n_slices; ++slice)
            {
                PixelType pixelValue = image->at(row, col, slice);

                if (pixelValue != 0)
                {
                    GLfloat x = row-center[0], y = col-center[1], z = slice-center[2];

                    GLfloat *vertexArray = new GLfloat[24];

                    vertexArray[0] = x-0.5f; vertexArray[1] = y+0.5f; vertexArray[2] = z-0.5f;
                    vertexArray[3] = x-0.5f; vertexArray[4] = y-0.5f; vertexArray[5] = z-0.5f;
                    vertexArray[6] = x-0.5f; vertexArray[7] = y+0.5f; vertexArray[8] = z+0.5f;
                    vertexArray[9] = x-0.5f; vertexArray[10] = y-0.5f; vertexArray[11] = z+0.5f;
                    vertexArray[12] = x+0.5f; vertexArray[13] = y+0.5f; vertexArray[14] = z+0.5f;
                    vertexArray[15] = x+0.5f; vertexArray[16] = y-0.5f; vertexArray[17] = z+0.5f;
                    vertexArray[18] = x+0.5f; vertexArray[19] = y+0.5f; vertexArray[20] = z-0.5f;
                    vertexArray[21] = x+0.5f; vertexArray[22] = y-0.5f; vertexArray[23] = z-0.5f;

                    GLfloat *colorArray = new GLfloat[24];
                    QColor voxelColor = components[pixelValue];
                    for (int indexColor=0; indexColor<24; indexColor+=3)
                    {
                        colorArray[indexColor] = (double) voxelColor.red()/255;
                        colorArray[indexColor+1] = (double) voxelColor.green()/255;
                        colorArray[indexColor+2] = (double) voxelColor.blue()/255;
                    }

                    vertices.push_back(vertexArray);
                    colors.push_back(colorArray);
                }
            }
        }
    }

    GLuint indiceArray[36] = {
        0,1,2,2,1,3,
        4,5,6,6,5,7,
        3,1,5,5,1,7,
        0,2,6,6,2,4,
        6,7,0,0,7,1,
        2,3,4,4,3,5
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

    std::vector<GLfloat*>::iterator itColors = colors.begin();
    for (std::vector<GLfloat*>::iterator itVertices = vertices.begin(); itVertices != vertices.end(); ++itVertices, ++itColors)
    {
        glVertexPointer(3, GL_FLOAT, 0, *itVertices);
        glColorPointer(3, GL_FLOAT, 0, *itColors);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indiceArray);
    }

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}
