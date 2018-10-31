#ifndef GL_TEXTURE_H
#define GL_TEXTURE_H

#include<windows.h>
#include<gl\gl.h>				 
#include<gl\glu.h>
#include<glaux.h>
#include <stdio.h>
class GL_Texture
{
public:
	bool MakeTextureBind(char *filename, GLuint &texture);  //生成一个纹理，并用texture邦定
	bool MakeAlphaTextureBind(char *filename,char *alphafilename, GLuint &texture,unsigned char compare);
};
void Readstr(FILE *f,char *string);   //读取文件中的数据
#endif   //GL_TEXTURE_H