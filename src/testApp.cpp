#include "testApp.h"

using namespace ofxCv;

void testApp::setup() {

    viewMode = 0;
    
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);
	cam.initGrabber(640, 480);
    
	tracker.setup();
	tracker.setRescale(.5);
    
	int num = 10000;
	p.assign(num, demoParticle());
	currentMode = PARTICLE_MODE_NEAREST_POINTS;
	resetParticles();
    
	centerOfFace.set(0,0,0);
	centerOfRightEye.set(0,0,0);
	centerOfLeftEye.set(0,0,0);
    centerOfJaw.set(0,0,0);
    
//--------
    //serial setup
    bSendSerialMessage = false;
	ofSetLogLevel(OF_LOG_VERBOSE);
    
	serial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = serial.getDeviceList();
	
	// this should be set to whatever com port your serial device is connected to.
	// (ie, COM4 on a pc, /dev/tty.... on linux, /dev/tty... on a mac)
	// arduino users check in arduino app....
	//serial.setup(5, 9600); //open the first device
	//serial.setup("COM4"); // windows example
	serial.setup("/dev/tty.usbmodemfd131",9600); // mac osx example
	//serial.setup("/dev/ttyUSB0", 9600); //linux example
	
	nTimesRead = 0;
	nBytesRead = 0;
	readTime = 0;
	memset(bytesReadString, 0, 4);
    
    transAtt = 0;
    transMed = 0;
//-----------
    
    ofBackground(255);
    
}

void testApp::resetParticles(){
    
	//these are the attraction points used in the forth demo
	attractPoints.clear();
	for(int i = 0; i < 4; i++){
		attractPoints.push_back( ofPoint(0,0) );
	}
    
	attractPointsWithMovement = attractPoints;
    
	for(int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].setAttractPoints(&attractPointsWithMovement);;
		p[i].reset();
    }
}

void testApp::update() {
    int howMany = serial.available();
    
    //cout << howMany << endl;
    
    if (howMany > 0){
        
        unsigned char bytes[howMany];
        serial.readBytes(bytes, howMany);
        //cout << "numBytes recvd: " << ofToString(howMany) << endl;
        
        for (int i = 0; i < howMany; i++){
            message += (bytes[i]);
        }
        
        //cout << message << endl;
        int foundHash = 0;
        foundHash = message.find("#");
        
        if (foundHash > 0){
            cout << message << endl;
            parsedMsg = ofSplitString(message, ",");
            
            sig = ofToFloat(parsedMsg[0]);
            att = ofToFloat(parsedMsg[1]);
            med = ofToFloat(parsedMsg[2]);
            
            cout << "sig: "<<sig<< "  att: "<< att << "  med: " << med << endl;
            
            //            cout << "Signal: " << parsedMsg[0] << endl;
            //            cout << "Attention: " << parsedMsg[1] << endl;
            //            cout << "Meditation: " << parsedMsg[2] << endl;
            //            cout << "Delta: " << parsedMsg[3] << endl;
            //            cout << "Theta: " << parsedMsg[4] << endl;
            //            cout << "Low Alpha: " << parsedMsg[5] << endl;
            //            cout << "High Alpha: " << parsedMsg[6] << endl;
            //            cout << "Low Beta: " << parsedMsg[7] << endl;
            //            cout << "High Beta: " << parsedMsg[8] << endl;
            //            cout << "Low Gamma: " << parsedMsg[9] << endl;
            //            cout << "High Gamma: " << parsedMsg[10] << endl;
            //
            //            cout << "---------" << endl;
            message.clear();
            
        }
        
        howMany = 0;
    }

    
	cam.update();
	if(cam.isFrameNew()) {
		tracker.update(toCv(cam));
		ofTexture _text;
		_text = cam.getTextureReference() ;
		_text.readToPixels(pixels);
        
	}
    
    for(int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].update(centerOfFace, attractPoints, transAtt, transMed);
		p[i].color.set( pixels.getColor(ofMap(p[i].pos.x, 0, ofGetWindowWidth(), 0, 640), ofMap(p[i].pos.y, 0, ofGetWindowHeight(), 0, 480) ));
	}
    
    // ATTENTION VALUES
    if(att >= 0 && att <= 10){
        transAtt = ofRandom(1,2.5);         //changes size of particles
    }
    if(att >=11 && att <= 20){
        transAtt = ofRandom(0.6,0.8);  
    }
    if(att >=21 && att <= 30){
        transAtt = ofRandom(0.9,1);
    }
    if(att >=31 && att <= 40){
        transAtt = ofRandom(1,1.2);
    }
    if(att >=41 && att <= 50){
        transAtt = ofRandom(1.2,1.4);
    }
    if(att >= 51 && att <= 60){
        transAtt = ofRandom(1.4,1.6);
    }
    if(att >= 61 && att <=70){
        transAtt = ofRandom(1.6,1.8);
    }
    if(att >= 71 && att <=80){
        transAtt = ofRandom(1.8,1.9);
    }
    if(att >= 81 && att <= 90){
        transAtt = ofRandom(1.9,2.0);
    }
    if(att >=91 && att <= 100){
        transAtt = ofRandom(2.0,2.5);
    }
    
    // MEDITATION VALUES
    if(med >= 0 && med <= 10){
        transMed = ofRandom(0.90,0.98);         // changes drag value
        particles.frcVar = 5;
        for(int i = 0; i < p.size(); i++){
            p[i].chaos();
        }
    }
    if(med >=11 && med <= 20){
        transMed = ofRandom(0.91,0.98);
        particles.frcVar = 4;
        for(int i = 0; i < p.size(); i++){
            p[i].chaos();
        }
    }
    if(med >=21 && med <= 30){
        transMed = ofRandom(0.92,0.98);
        particles.frcVar = 3;
        for(int i = 0; i < p.size(); i++){
            p[i].chaos();
        }
    }
    if(med >=31 && med <= 40){
        transMed = ofRandom(0.93,0.98);
        particles.frcVar = 2;
        for(int i = 0; i < p.size(); i++){
            p[i].chaos();
        }
    }
    if(med >=41 && med <= 50){
        transMed = ofRandom(0.94,0.98);
        particles.frcVar = 1;
        for(int i = 0; i < p.size(); i++){
            p[i].chaos();
        }
    }
    if(med >= 51 && med <= 60){
        transMed = ofRandom(0.97,0.98);
        particles.frcVar = 0.1;
        for(int i = 0; i < p.size(); i++){
            p[i].chaos();
        }

    }
    if(med >= 61 && med <=70){
        transMed = ofRandom(0.96,0.98);
    }
    if(med >= 71 && med <=80){
        transMed = ofRandom(0.97,0.98);
    }
    if(med >= 81 && med <= 90){
        transMed = ofRandom(0.975,0.98);
    }
    if(med >=91 && med <= 100){
        transMed = ofRandom(0.95,0.98);
    }

}
void testApp::draw() {
	ofSetColor(255);


//	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    
    ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
    ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
    ofPolyline faceOutline = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
    ofPolyline jaw = tracker.getImageFeature(ofxFaceTracker::JAW);
    
    
    
    // mapping image to screen dimension
    if(faceOutline.size()>0){
        centerOfFace.set(faceOutline.getCentroid2D() );
        centerOfRightEye.set(rightEye.getCentroid2D() );
        centerOfLeftEye.set(leftEye.getCentroid2D() );
        centerOfJaw.set(jaw.getCentroid2D() );
        attractPoints[0].x = ofMap(centerOfFace.x, 0, 640, 0, ofGetWindowWidth() );
        attractPoints[0].y = ofMap(centerOfFace.y, 0, 480, 0, ofGetWindowHeight() );
        attractPoints[1].x = ofMap(centerOfRightEye.x, 0, 640, 0, ofGetWindowWidth() );
        attractPoints[1].y = ofMap(centerOfRightEye.y, 0, 480, 0, ofGetWindowHeight() );
        attractPoints[2].x = ofMap(centerOfLeftEye.x, 0, 640, 0, ofGetWindowWidth() );
        attractPoints[2].y = ofMap(centerOfLeftEye.y, 0, 480, 0, ofGetWindowHeight() );
        attractPoints[3].x = ofMap(centerOfJaw.x, 0, 640, 0, ofGetWindowWidth() );
        attractPoints[3].y = ofMap(centerOfJaw.y, 0, 480, 0, ofGetWindowHeight() );
        
    }
    
    for(int i = 0; i < p.size(); i++){
		p[i].draw();
	}
    
    ofSetColor(190);
    //needed only for nearest point mode
	if( currentMode == PARTICLE_MODE_NEAREST_POINTS ){
		for(int i = 0; i < attractPoints.size(); i++){
			ofNoFill();
			ofCircle(attractPointsWithMovement[i], 10);
			ofFill();
			ofCircle(attractPointsWithMovement[i], 4);
		}
	}
    

    if(viewMode > 0){
        //draw fps
        ofSetColor(255);
        ofDrawBitmapString("fps: "+ofToString(ofGetFrameRate()), 20, 20);
        
        //draw face tracking
        ofSetLineWidth(2);
        ofSetColor(ofColor::red);
        leftEye.draw();
        ofSetColor(ofColor::green);
        rightEye.draw();
        ofSetColor(ofColor::blue);
        faceOutline.draw();

        ofSetLineWidth(1);
        ofSetColor(255);
        tracker.draw();
    }
    
    if(viewMode > 1){
        //draw meditation bar
        ofSetColor(200, 100, 50, 20);
        ofRect(ofGetWidth()-75, 0, 75, med*4);
        
        ofSetColor(50, 200, 100, 20);
        ofRect(ofGetWidth()-150, 0, 75, att*4);
        
        ofSetColor(255);
        ofDrawBitmapString("att:"+ofToString(att), ofGetWidth()-150, 75);
        ofDrawBitmapString("med:"+ofToString(med), ofGetWidth()-75, 75);
        
        if (sig > 0) {
            ofSetColor(255, 0, 30);
            ofCircle(ofGetWidth()-200, 30, 30);
        }
        
    }
    
    if(viewMode > 2){
        
        cam.draw(50, ofGetHeight()-600);
    }
        
}

void testApp::keyPressed(int key) {
	if(key == 'r') {
		tracker.reset();
	}
    if(key == ' '){
        
        viewMode ++;
        
        if(viewMode > 3) viewMode = 0;
    }
    
    if(key == OF_KEY_SHIFT) viewMode = 3;
    
    if(key == 'f') ofToggleFullscreen();
}
