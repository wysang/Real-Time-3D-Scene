#include "GL_Texture.h"

bool GL_Texture::MakeTextureBind(char *filename, GLuint &texture)
{
	AUX_RGBImageRec *pImage = NULL;
	pImage = auxDIBImageLoadA(filename);
	if(pImage == NULL)		return false;
	glGenTextures(1, &texture);	
	glBindTexture    (GL_TEXTURE_2D,texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	//创建mipmap图
	gluBuild2DMipmaps(GL_TEXTURE_2D,3, pImage->sizeX, 
					  pImage->sizeY,GL_RGB, GL_UNSIGNED_BYTE,pImage->data);
	free(pImage->data);
	free(pImage);
	return true;
}

bool GL_Texture::MakeAlphaTextureBind(char *filename, char *alphafilename,GLuint &texture,unsigned char compare)
{								
	AUX_RGBImageRec *Image=NULL;					
	unsigned char *alpha=NULL;

	////////////////////////
	if (Image=auxDIBImageLoadA(filename)) 
	{							
		alpha=new unsigned char[4*Image->sizeX*Image->sizeY];		
		for (int a=0; a<Image->sizeX*Image->sizeY; a++)
		{
			alpha[4*a]=Image->data[a*3];					// R
			alpha[4*a+1]=Image->data[a*3+1];				// G
			alpha[4*a+2]=Image->data[a*3+2];				// B
		}
		////////////make alpha chanal
        if(alphafilename==NULL)return false;
	    FILE* file;
        if((file= fopen(alphafilename, "rb"))==NULL)
	      	return false;
		// 54字节之后为位图颜色转换矩阵部分,前面的部分为位图的一些信息如存储方式等等
		fseek(file,54,SEEK_SET);
		unsigned char temp[3];
		for (int a=0; a<Image->sizeX*Image->sizeY; a++)
		{
			fread(temp,sizeof(unsigned char),3,file);
            alpha[4*a+3]=(temp[0]>temp[1])?temp[0]:temp[1];
            if(temp[2]>alpha[4*a+3])alpha[4*a+3]=temp[2];
			if(alpha[4*a+3]>compare)
				alpha[4*a+3]=255;    //不透明
			else
				alpha[4*a+3]=0;      //透明的
		}
		fclose(file);
		////////////////////////////////		
		
		glGenTextures(1, &texture);	

		// Create Linear Filtered RGBA8-Texture
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA8, Image->sizeX, Image->sizeY, 
			              GL_RGBA, GL_UNSIGNED_BYTE, alpha);
		delete []alpha;
		free(Image->data);
		free(Image);
	}
	else
		return false;
		
	////////////////////////
    return true;
}

void Readstr(FILE *f,char *string)
{
	do
	{
		fgets(string, 255, f);
	} while ((string[0] == '/') || (string[0] == '\n'));
}
