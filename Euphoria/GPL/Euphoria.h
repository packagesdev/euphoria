#ifndef __EUPHORIA__
#define __EUPHORIA__

#define NUMCONSTS 9

class wisp;

class scene
{
public:
	
	float aspectRatio;
	int viewport[4];
	
	unsigned long long _lastRefresh;
	
	int wispsCount;
	wisp * wisps;
	
	int backgroundLayersCount;
	wisp * backgroundLayers;
	
	unsigned char* feedbackmap;
	
	int visibility;
	
	int feedback;
	int feedbackspeed;
	int feedbacksize;
	bool wireframe;
	int textureType;
	
	unsigned int textureID;
	
	// feedback texture object
	
	unsigned int feedbacktex;
	
	int feedbacktexsize;
	
	int meshDensity;
	
	int speed;
	
	// feedback variables
	
	float fr[4];
	float fv[4];
	float f[4];
	
	// feedback limiters
	
	float lr[3];
	float lv[3];
	float l[3];
	
	scene();
	virtual ~scene();
	
	void create();
	void resize(int inWidth,int inHeight);
	void draw();
};

class wisp
{
    public:
    
        float ***vertices;
        float c[NUMCONSTS];     // constants
        float cr[NUMCONSTS];    // constants' radial position
        float cv[NUMCONSTS];    // constants' change velocities
        float hsl[3];
        float rgb[3];
        float hueSpeed;
        float saturationSpeed;
        
        int meshDensity;
    
        wisp();
        ~wisp();
		
		void initWithScene(scene *inScene);
		void update(int inVisibility,float inElapsedTime);
	
        void draw(bool inShowWireframe);
        void drawAsBackground(bool inShowWireframe);
};

#endif