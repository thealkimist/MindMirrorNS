#pragma once

#include "ofMain.h"
//#include "ofxCv.h"
//#include "ofxOpenCv.h"
#include "ofxFaceTracker.h"
#include "demoParticle.h"
#include "ofxThinkGear.h"

class testApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void resetParticles();
    
    // mindwave mobile
    ofxThinkGear thinkGear;
    void attentionListener(float &param);
    void meditationListener(float &param);
    
    float atChangeTime;
    float meChangeTime;
    float attention;
    float meditation;
    float distAw;
    float prevAw;
    float currAw;
    float distMw;
    float prevMw;
    float currMw;
    
    // facetracking
	ofVideoGrabber cam;
	ofxFaceTracker tracker;
//    ofTexture mirror;
//    unsigned char *mirrorImage;
//    
//    int camWidth;
//    int camHeight;
    
    particleMode currentMode;
    string currentModeStr;
    
    vector <demoParticle> p;
    vector <ofPoint> attractPoints;
    vector <ofPoint> attractPointsWithMovement;
    
    demoParticle particles;
    
    ofPoint centerOfFace;
    ofPoint centerOfRightEye;
    ofPoint centerOfLeftEye;
    ofPoint centerOfMouth;
    ofPoint centerOfJaw;
    
    //ofxCvColorImage colorImage;
    ofPixels pixels;
    
    float sig;
    float att;
    float med;
    
//    vector <string> parsedMsg;
    float medSize;
    
    int viewMode;
    
    float transAtt, transMed;
    float a1, a2, a3, a4, a5;
    float m1, m2, m3, m4, m5;
    


    
    
};
