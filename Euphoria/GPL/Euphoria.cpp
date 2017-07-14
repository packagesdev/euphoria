/*
 * Copyright (C) 2000-2015  Terence M. Welsh
 *
 * This file is part of Euphoria.
 *
 * Euphoria is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as 
 * published by the Free Software Foundation.
 *
 * Euphoria is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


// Euphoria screensaver

#if defined(__GNUC__)
#if defined(__GNUC_PATCHLEVEL__)
#define __GNUC_VERSION__ (__GNUC__ * 10000 \
                            + __GNUC_MINOR__ * 100 \
                            + __GNUC_PATCHLEVEL__)
#else
#define __GNUC_VERSION__ (__GNUC__ * 10000 \
                            + __GNUC_MINOR__ * 100)
#endif

#if __GNUC_VERSION__ < 40000

#define cosf cos
#define acosf acos
#define sinf sin

#endif

#endif

#include "Euphoria.h"


#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "rsMath.h"
#include "rgbhsl.h"
#include "texture.h"

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#define PIx2 6.28318530718f

scene::scene()
{
	srand((unsigned)time(NULL));
	rand(); rand(); rand(); rand(); rand();
	rand(); rand(); rand(); rand(); rand();
	rand(); rand(); rand(); rand(); rand();
	rand(); rand(); rand(); rand(); rand();
	
	_lastRefresh=0;
	
	feedbackmap=NULL;
	wisps=NULL;
	backgroundLayers=NULL;
	
	
	textureID=0;
	feedbacktex=0;
}

scene::~scene()
{
	if (feedbackmap!=NULL)
	{
		delete feedbackmap;
		feedbackmap=NULL;
	}
	
	if (wisps!=NULL)
	{
		delete[] wisps;
		wisps=NULL;
	}
	
	if (backgroundLayers!=NULL)
	{
		delete[] backgroundLayers;
		backgroundLayers=NULL;
	}
}

#pragma mark -

void scene::resize(int inWidth,int inHeight)
{
	viewport[0] = 0;
	viewport[1] = 0;
	viewport[2] = inWidth;
	viewport[3] = inHeight;
	
	glViewport(0,0, inWidth,inHeight);
	aspectRatio = inWidth / inHeight;
	
	// setup regular drawing area just in case feedback isn't used
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(20.0, aspectRatio, 0.01, 20);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void scene::create()
{
	
	fr[0] = 0.0f;
	fr[1] = 0.0f;
	fr[2] = 0.0f;
	fr[3] = 0.0f;
	
	lr[0] = 0.0f;
	lr[1] = 0.0f;
	lr[2] = 0.0f;
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glLineWidth(2.0f);
	// Commented out because smooth lines and textures don't mix on my TNT.
	// It's like it rendering in software mode
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	
	if (glIsTexture(textureID))
	{
		glDeleteTextures(1, (GLuint *) &textureID);
		textureID=0;
	}
	
	if (textureType || feedback)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	
	if(textureType>0)
	{
		int whichtex = textureType;
		if(whichtex == 4)  // random texture
			whichtex = rsRandi(3) + 1;
		
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		// Initialize texture
		glGenTextures(1, (GLuint *) &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		switch(whichtex)
		{
			case 1:
				gluBuild2DMipmaps(GL_TEXTURE_2D, 1, TEXSIZE, TEXSIZE, GL_LUMINANCE, GL_UNSIGNED_BYTE, plasmamap);
				break;
			case 2:
				gluBuild2DMipmaps(GL_TEXTURE_2D, 1, TEXSIZE, TEXSIZE, GL_LUMINANCE, GL_UNSIGNED_BYTE, stringymap);
				break;
			case 3:
				gluBuild2DMipmaps(GL_TEXTURE_2D, 1, TEXSIZE, TEXSIZE, GL_LUMINANCE, GL_UNSIGNED_BYTE, linesmap);
		}
	}
	
	
	
	if (feedback>0)
	{
		feedbacktexsize = int(pow(2, feedbacksize));
		
		while(feedbacktexsize > viewport[2] || feedbacktexsize > viewport[3])
		{
			feedbacktexsize=feedbacktexsize/2;
		}
		
		// feedback texture setup
		
		feedbackmap = new unsigned char[feedbacktexsize*feedbacktexsize*3];
		
		if (glIsTexture(feedbacktex))
		{
			glDeleteTextures(1, (GLuint *) &feedbacktex);
			feedbacktex=0;
		}
		
		glGenTextures(1, (GLuint *) &feedbacktex);
		glBindTexture(GL_TEXTURE_2D, feedbacktex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, feedbacktexsize, feedbacktexsize, 0, GL_RGB, GL_UNSIGNED_BYTE, feedbackmap);
		
		// feedback velocity variable setup
		fv[0] = float(feedbackspeed) * (rsRandf(0.025f) + 0.025f);
		fv[1] = float(feedbackspeed) * (rsRandf(0.05f) + 0.05f);
		fv[2] = float(feedbackspeed) * (rsRandf(0.05f) + 0.05f);
		fv[3] = float(feedbackspeed) * (rsRandf(0.1f) + 0.1f);
		lv[0] = float(feedbackspeed) * (rsRandf(0.0025f) + 0.0025f);
		lv[1] = float(feedbackspeed) * (rsRandf(0.0025f) + 0.0025f);
		lv[2] = float(feedbackspeed) * (rsRandf(0.0025f) + 0.0025f);
	}
	
	// Initialize wisps
	
	wisps = new wisp[wispsCount];
	
	for(int i=0;i<wispsCount;i++)
	{
		wisps[i].initWithScene(this);
	}
	
	if (backgroundLayersCount>0)
	{
		backgroundLayers = new wisp[backgroundLayersCount];
		
		for(int i=0;i<backgroundLayersCount;i++)
		{
			backgroundLayers[i].initWithScene(this);
		}
	}
	else
	{
		backgroundLayers=NULL;
	}
	
	struct timeval tTime;
	gettimeofday(&tTime, NULL);
	
	_lastRefresh=(tTime.tv_sec*1000+tTime.tv_usec/1000);
}

#pragma mark -

void scene::draw()
{
	struct timeval tTime;
	unsigned long long tCurentTime;
	
	gettimeofday(&tTime, NULL);
	
	tCurentTime=(tTime.tv_sec*1000+tTime.tv_usec/1000);
	
	float tElapsedTime=(tCurentTime-_lastRefresh)*0.001;
	_lastRefresh=tCurentTime;
	
	// Update wisps
	for(int i=0; i<wispsCount; i++)
		wisps[i].update(visibility,tElapsedTime);
	
	for(int i=0; i<backgroundLayersCount; i++)
		backgroundLayers[i].update(visibility,tElapsedTime);
	
	
	
	if(feedback)
	{
		float feedbackIntensity = feedback / 101.0f;
		
		// update feedback variables
		for(int i=0; i<4; i++)
		{
			fr[i] += tElapsedTime * fv[i];
			if(fr[i] > PIx2)
				fr[i] -= PIx2;
		}
		
		f[0] = 30.0f * cos((double)fr[0]);
		f[1] = 0.2f * cos((double)fr[1]);
		f[2] = 0.2f * cos((double)fr[2]);
		f[3] = 0.8f * cos((double)fr[3]);
		
		for(int i=0; i<3; i++)
		{
			lr[i] += tElapsedTime * lv[i];
			if(lr[i] > PIx2)
				lr[i] -= PIx2;
			
			l[i] = cos(lr[i]);
			
			l[i] = l[i] * l[i];
		}
		
		// Create drawing area for feedback texture
		glViewport(0, 0, feedbacktexsize, feedbacktexsize);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(30.0, aspectRatio, 0.01f, 20.0f);
		glMatrixMode(GL_MODELVIEW);
		
		// Draw
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(feedbackIntensity, feedbackIntensity, feedbackIntensity);
		glBindTexture(GL_TEXTURE_2D, feedbacktex);
		glPushMatrix();
		glTranslatef(f[1] * l[1], f[2] * l[1], f[3] * l[2]);
		glRotatef(f[0] * l[0], 0, 0, 1);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(-0.5f, -0.5f);
		glVertex3f(-aspectRatio*2.0f, -2.0f, 1.25f);
		glTexCoord2f(1.5f, -0.5f);
		glVertex3f(aspectRatio*2.0f, -2.0f, 1.25f);
		glTexCoord2f(-0.5f, 1.5f);
		glVertex3f(-aspectRatio*2.0f, 2.0f, 1.25f);
		glTexCoord2f(1.5f, 1.5f);
		glVertex3f(aspectRatio*2.0f, 2.0f, 1.25f);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, textureID);
		
		for(int i=0; i<backgroundLayersCount; i++)
			backgroundLayers[i].drawAsBackground(wireframe);
		
		for(int i=0; i<wispsCount; i++)
			wisps[i].draw(wireframe);
		
		// readback feedback texture
		
		glReadBuffer(GL_BACK);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, feedbacktexsize);
		glBindTexture(GL_TEXTURE_2D, feedbacktex);
		//glReadPixels(0, 0, feedbacktexsize, feedbacktexsize, GL_RGB, GL_UNSIGNED_BYTE, feedbackmap);
		//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, feedbacktexsize, feedbacktexsize, GL_RGB, GL_UNSIGNED_BYTE, feedbackmap);
		glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, feedbacktexsize, feedbacktexsize);
		
		// create regular drawing area
		glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(20.0, aspectRatio, 0.01f, 20.0f);
		glMatrixMode(GL_MODELVIEW);
		
		// Draw again
		glClear(GL_COLOR_BUFFER_BIT);
		glColor3f(feedbackIntensity, feedbackIntensity, feedbackIntensity);
		glPushMatrix();
		glTranslatef(f[1] * l[1], f[2] * l[1], f[3] * l[2]);
		glRotatef(f[0] * l[0], 0, 0, 1);
		glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(-0.5f, -0.5f);
		glVertex3f(-aspectRatio*2.0f, -2.0f, 1.25f);
		glTexCoord2f(1.5f, -0.5f);
		glVertex3f(aspectRatio*2.0f, -2.0f, 1.25f);
		glTexCoord2f(-0.5f, 1.5f);
		glVertex3f(-aspectRatio*2.0f, 2.0f, 1.25f);
		glTexCoord2f(1.5f, 1.5f);
		glVertex3f(aspectRatio*2.0f, 2.0f, 1.25f);
		glEnd();
		glPopMatrix();
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
	
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	//
	for(int i=0; i<backgroundLayersCount; i++)
		backgroundLayers[i].drawAsBackground(wireframe);
	
	for(int i=0; i<wispsCount; i++)
		wisps[i].draw(wireframe);
	
	glFinish();
}

#pragma mark -


wisp::wisp()
{
	vertices=NULL;
}

wisp::~wisp()
{
    if (vertices!=NULL)
	{
		for(int i=0; i<=meshDensity; i++)
		{
			for(int j=0; j<=meshDensity; j++)
			{
				delete[] vertices[i][j];
			}
			
			delete[] vertices[i];
		}
	
		delete[] vertices;
		
		vertices=NULL;
	}
}

#pragma mark -

void wisp::initWithScene(scene *inScene)
{
	float recHalfDens = 1.0f / (inScene->meshDensity * 0.5f);
	
	meshDensity=inScene->meshDensity;
	
	vertices = new float**[meshDensity+1];
	
	for(int i=0; i<=meshDensity; i++)
	{
		vertices[i] = new float*[meshDensity+1];
		
		for(int j=0; j<=meshDensity; j++)
		{
			float * pointer=vertices[i][j] = new float[7];
			pointer[3] = float(i) * recHalfDens - 1.0f;  // x position on grid
			pointer[4] = float(j) * recHalfDens - 1.0f;  // y position on grid
			// distance squared from the center
			pointer[5] = pointer[3] * pointer[3]
			+ pointer[4] * pointer[4];
			pointer[6] = 0.0f;  // intensity
		}
	}
	
	// initialize constants
	for(int i=0; i<NUMCONSTS; i++)
	{
		c[i] = rsRandf(2.0f) - 1.0f;
		cr[i] = rsRandf(PIx2);
		cv[i] = rsRandf(inScene->speed * 0.03f) + (inScene->speed * 0.001f);
	}
	
	// pick color
	hsl[0] = rsRandf(1.0f);
	hsl[1] = 0.1f + rsRandf(0.9f);
	hsl[2] = 1.0f;
	hueSpeed = rsRandf(0.1f) - 0.05f;
	saturationSpeed = rsRandf(0.04f) + 0.001f;
}

void wisp::update(int inVisibility,float inElapsedTime)
{
    rsVec up, right, crossvec;
    // visibility constants
    float viscon1 = inVisibility * 0.01f;
    float viscon2 = 1.0f / viscon1;
    
    // update constants
    for(int i=0; i<NUMCONSTS; i++)
    {
        cr[i] += cv[i] * inElapsedTime;
        if(cr[i] > PIx2)
            cr[i] -= PIx2;

		c[i] = cosf((double) cr[i]);
    }

    // update vertex positions
    for(int i=0; i<=meshDensity; i++)
    {
        for(int j=0; j<=meshDensity; j++)
        {
            float * pointer=vertices[i][j];
            
            pointer[0] = pointer[3] * pointer[3] * pointer[4] * c[0]
				+ pointer[5] * c[1] + 0.5f * c[2];
            pointer[1] = pointer[4] * pointer[4] * pointer[5] * c[3]
				+ pointer[3] * c[4] + 0.5f * c[5];
            pointer[2] = pointer[5] * pointer[5] * pointer[3] * c[6]
				+ pointer[4] * c[7] + c[8];
        }
    }

    // update vertex normals for most of mesh
    for(int i=1; i<meshDensity; i++)
    {
        for(int j=1; j<meshDensity; j++)
        {
            up.set(vertices[i][j+1][0] - vertices[i][j-1][0],
                    vertices[i][j+1][1] - vertices[i][j-1][1],
                    vertices[i][j+1][2] - vertices[i][j-1][2]);
            right.set(vertices[i+1][j][0] - vertices[i-1][j][0],
                    vertices[i+1][j][1] - vertices[i-1][j][1],
                    vertices[i+1][j][2] - vertices[i-1][j][2]);
    
            up.normalize();
            right.normalize();
            crossvec.cross(right, up);
            
            // Use depth component of normal to compute intensity
            // This way only edges of wisp are bright

            if(crossvec[2] < 0.0f)
                    crossvec[2] *= -1.0f;
			
            vertices[i][j][6] = viscon2 * (viscon1 - crossvec[2]);
            
            if(vertices[i][j][6] > 1.0f)
            {
                vertices[i][j][6] = 1.0f;
            }
            else
            {
                if(vertices[i][j][6] < 0.0f)
                {
                    vertices[i][j][6] = 0.0f;
                }
            }
        }
    }

    // update color
    
    hsl[0] += hueSpeed * inElapsedTime;
    
    if(hsl[0] < 0.0f)
    {
        hsl[0] += 1.0f;
    }
    else
    {
        if(hsl[0] > 1.0f)
        {
            hsl[0] -= 1.0f;
        }
    }
    
    hsl[1] += saturationSpeed * inElapsedTime;
    
    if(hsl[1] <= 0.1f)
    {
        hsl[1] = 0.1f;
        saturationSpeed = -saturationSpeed;
    }
    
    if(hsl[1] >= 1.0f)
    {
        hsl[1] = 1.0f;
        saturationSpeed = -saturationSpeed;
    }
    
    hsl2rgb(hsl[0], hsl[1], hsl[2], rgb[0], rgb[1], rgb[2]);
}

void wisp::draw(bool inShowWireframe)
{
    glPushMatrix();

    if(inShowWireframe==true)
    {
        for(int i=1; i<meshDensity; i++)
        {
            glBegin(GL_LINE_STRIP);
            
            for(int j=0; j<=meshDensity; j++)
            {
                float * pointer=vertices[i][j];
                
                glColor3f(rgb[0] + pointer[6] - 1.0f, rgb[1] + pointer[6] - 1.0f, rgb[2] + pointer[6] - 1.0f);
                glTexCoord2d(pointer[3] - pointer[0], pointer[4] - pointer[1]);
                glVertex3fv(pointer);
            }
            glEnd();
        }
        
        for(int j=1; j<meshDensity; j++)
        {
            glBegin(GL_LINE_STRIP);
            
            for(int i=0; i<=meshDensity; i++)
            {
                float * pointer=vertices[i][j];
                
                glColor3f(rgb[0] + pointer[6] - 1.0f, rgb[1] + pointer[6] - 1.0f, rgb[2] + pointer[6] - 1.0f);
                glTexCoord2d(pointer[3] - pointer[0], pointer[4] - pointer[1]);
                glVertex3fv(pointer);
            }
            glEnd();
        }
    }
    else
    {
        for(int i=0; i<meshDensity; i++)
        {
            glBegin(GL_TRIANGLE_STRIP);
            
            for(int j=0; j<=meshDensity; j++)
            {
                float * pointer=vertices[i][j];
                    
                glColor3f(rgb[0] + vertices[i+1][j][6] - 1.0f, rgb[1] + vertices[i+1][j][6] - 1.0f, rgb[2] + vertices[i+1][j][6] - 1.0f);
                glTexCoord2d(vertices[i+1][j][3] - vertices[i+1][j][0], vertices[i+1][j][4] - vertices[i+1][j][1]);
                glVertex3fv(vertices[i+1][j]);
                glColor3f(rgb[0] + pointer[6] - 1.0f, rgb[1] + pointer[6] - 1.0f, rgb[2] + pointer[6] - 1.0f);
                glTexCoord2d(pointer[3] - pointer[0], pointer[4] - pointer[1]);
                glVertex3fv(pointer);
            }
                
            glEnd();
        }
    }

    glPopMatrix();
}

void wisp::drawAsBackground(bool inShowWireframe)
{
    glPushMatrix();
    glTranslatef(c[0] * 0.2f, c[1] * 0.2f, 1.6f);

    if(inShowWireframe==true)
    {
        for(int i=1; i<meshDensity; i++)
        {
            glBegin(GL_LINE_STRIP);
            
            for(int j=0; j<=meshDensity; j++)
            {
                float * pointer=vertices[i][j];
                
                glColor3f(rgb[0] + pointer[6] - 1.0f, rgb[1] + pointer[6] - 1.0f, rgb[2] + pointer[6] - 1.0f);
                glTexCoord2d(pointer[3] - pointer[0], pointer[4] - pointer[1]);
                glVertex3f(pointer[3], pointer[4], pointer[6]);
            }
            glEnd();
        }
        
        for(int j=1; j<meshDensity; j++)
        {
            glBegin(GL_LINE_STRIP);
            
            for(int i=0; i<=meshDensity; i++)
            {
                float * pointer=vertices[i][j];
                
                glColor3f(rgb[0] + pointer[6] - 1.0f, rgb[1] + pointer[6] - 1.0f, rgb[2] + pointer[6] - 1.0f);
                glTexCoord2d(pointer[3] - pointer[0], pointer[4] - pointer[1]);
                glVertex3f(pointer[3], pointer[4], pointer[6]);
            }
            glEnd();
        }
    }
    else
    {
        for(int i=0; i<meshDensity; i++)
        {
            glBegin(GL_TRIANGLE_STRIP);
            
            for(int j=0; j<=meshDensity; j++)
            {
                float * pointer=vertices[i][j];
                    
                glColor3f(rgb[0] + vertices[i+1][j][6] - 1.0f, rgb[1] + vertices[i+1][j][6] - 1.0f, rgb[2] + vertices[i+1][j][6] - 1.0f);
                glTexCoord2d(vertices[i+1][j][3] - vertices[i+1][j][0], vertices[i+1][j][4] - vertices[i+1][j][1]);
                glVertex3f(vertices[i+1][j][3], vertices[i+1][j][4], vertices[i+1][j][6]);
                glColor3f(rgb[0] + pointer[6] - 1.0f, rgb[1] + pointer[6] - 1.0f, rgb[2] + pointer[6] - 1.0f);
                glTexCoord2d(pointer[3] - pointer[0], pointer[4] - pointer[1]);
                glVertex3f(pointer[3], pointer[4], pointer[6]);
            }
                
            glEnd();
        }
    }

    glPopMatrix();
}
