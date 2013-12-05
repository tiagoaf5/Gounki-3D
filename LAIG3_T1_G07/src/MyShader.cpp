#include <stdlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include "MyShader.h"


MyShader::MyShader(const char *vsFile, const char *fsFile, const char * tex,  const char * height)
{
	init(vsFile, fsFile);
	reset = true;
	scale = 1;
	speed = 0.002;
	delta = 0.0;
	CGFshader::bind();


	// Initialize parameter in memory
	//normScale=0.0;

	// Store Id for the uniform "normScale", new value will be stored on bind()
	scaleLoc = glGetUniformLocation(id(), "scale");
	//printf("->scaleLoc: %d\n",scaleLoc);

	timeLoc = glGetUniformLocation(id(), "time");
	glUniform1f(timeLoc, 0.0);

	/*speedLoc = glGetUniformLocation(id(), "speed");*/

	texturemap = new CGFtexture(tex);
	heightmap = new CGFtexture(height);



	// get the uniform location for the sampler
	texturemapLoc = glGetUniformLocation(id(), "texturemap");

	// set the texture id for that sampler to match the GL_TEXTUREn that you 
	// will use later e.g. if using GL_TEXTURE0, set the uniform to 0
	glUniform1i(texturemapLoc, 0);

	// repeat if you use more textures in your shader(s)
	heightmapLoc = glGetUniformLocation(id(), "heightmap");
	glUniform1i(heightmapLoc, 1);

	CGFshader::unbind();
	//printf("texturemapLoc: %d\nheightmapLoc: %d\ntimeLoc: %d \nspeedLoc: %d\n",texturemapLoc, heightmapLoc,timeLoc,speedLoc);
}

void MyShader::bind(void)
{
	CGFshader::bind();

	// update uniforms
	glUniform1f(scaleLoc, scale);

	// update speed
	//glUniform1f(speedLoc, speed);

	glUniform1f(timeLoc, delta);
	// make sure the correct texture unit is active
	glActiveTexture(GL_TEXTURE0);

	// apply/activate the texture you want, so that it is bound to GL_TEXTURE0
	texturemap->apply();

	// do the same for other textures
	glActiveTexture(GL_TEXTURE1);

	heightmap->apply();

	glActiveTexture(GL_TEXTURE0);
}

void MyShader::update() 
{
	update(CGFapplication::getTime());
}

void MyShader::update(float time) 
{
	if(reset)
	{
		//printf("%f\n", time);
		startTime = time;
		reset = false;
	}
	else
	{
		if (timeLoc != -1){
			//printf("%f  ",(time-startTime)/1000);
			delta = (time-startTime)/1000;
			
		}
	}
}
