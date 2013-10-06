#pragma once
#include "ofMain.h"


enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE
};

class demoParticle{
    
public:
    demoParticle();
    
    void setup();
    void setMode(particleMode newMode);
    void setAttractPoints( vector <ofPoint> * attract );
    
    void reset();
    void update(ofPoint centerOfFace, vector <ofPoint> attractPoints, float _scale, float _drag);
    void draw();
    void chaos();
    
    ofPoint pos;
    ofPoint vel;
    ofPoint frc;
    float posVal = 0;
    float timeVal = 0;
    float frcVar = 0;
    ofFloatColor color;
    
    float drag;
    float uniqueVal;
    float scale;
    
    particleMode mode;
    
    vector <ofPoint> * attractPoints;
    
};
