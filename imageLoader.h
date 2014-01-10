#ifndef IMAGELOADER_H_INCLUDED
#define IMAGELOADER_H_INCLUDED

#endif // IMAGELOADER_H_INCLUDED

#include "soil/SOIL.h"

GLuint loadtex( string texname, int width, int height, int x, int y )
{
    GLuint tex_2d = SOIL_load_OGL_texture (
        texname.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
    );
    glBindTexture(GL_TEXTURE_2D, tex_2d);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_POLYGON);
        glColor3f(1.0f, 1.0f, 1.0f);
        glTexCoord2f(1, 0);        glVertex2f(width + x, height + y);
        glTexCoord2f(0, 0);        glVertex2f(0 + x, height + y);
        glTexCoord2f(0, 1);        glVertex2f(0 + x, 0 + y);
        glTexCoord2f(1, 1);        glVertex2f(width + x, 0 + y);
    glEnd();

    glDisable(GL_TEXTURE_2D);

    return tex_2d;
};
