#include "demoParticle.h"

//------------------------------------------------------------------
demoParticle::demoParticle(){
	attractPoints = NULL;
	color.set(0.0);
}

//------------------------------------------------------------------
void demoParticle::setMode(particleMode newMode){
	mode = newMode;
}

//------------------------------------------------------------------
void demoParticle::setAttractPoints( vector <ofPoint> * attract ){
	attractPoints = attract;
}

//------------------------------------------------------------------
void demoParticle::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);
    
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
    
	vel.x = ofRandom(-3.9, 3.9);
	vel.y = ofRandom(-3.9, 3.9);
    
	frc   = ofPoint(0,0,0);
    
	scale = ofRandom(0.5, 1.0);
    
	if( mode == PARTICLE_MODE_NOISE ){
		drag  = ofRandom(0.97, 0.99);
		vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	}else{
		drag  = ofRandom(0.95, 0.998);
	}
}

//------------------------------------------------------------------
void demoParticle::update(ofPoint centerOfFace, vector <ofPoint> attractPoints){
    
	if( mode == PARTICLE_MODE_NEAREST_POINTS ){
        
		if( attractPoints.size() > 0 ){
            
			//1 - find closest attractPoint
			ofPoint closestPt;
			int closest = -1;
			float closestDist = 9999999;
            
			for(int i = 0; i < attractPoints.size(); i++){
				float lenSq = ( attractPoints.at(i)-pos ).lengthSquared();
				if( lenSq < closestDist ){
					closestDist = lenSq;
					closest = i;
				}
			}
            
			//2 - if we have a closest point - lets calcuate the force towards it
			if( closest != -1 ){
				closestPt = attractPoints.at(closest);
				float dist = sqrt(closestDist);
                
				//in this case we don't normalize as we want to have the force proportional to distance
				frc = closestPt - pos;
                
				vel *= drag;
                
				//lets also limit our attraction to a certain distance and don't apply if 'f' key is pressed
				if( dist < 300 && dist > 40 && !ofGetKeyPressed('f') ){
					vel += frc * 0.003;
				}else{
					//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy.
					frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
					frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
					vel += frc * 0.4;
				}
                
			}
            
		}
        
	}
    
	//2 - UPDATE OUR POSITION
	pos += vel;
    
	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN
	//we could also pass in bounds to check - or alternatively do this at the testApp level
	if( pos.x > ofGetWidth() ){
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}else if( pos.x < 0 ){
		pos.x = 0;
		vel.x *= -1.0;
	}
	if( pos.y > ofGetHeight() ){
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if( pos.y < 0 ){
		pos.y = 0;
		vel.y *= -1.0;
	}
    
}

//------------------------------------------------------------------
void demoParticle::draw(){
    
	if( mode == PARTICLE_MODE_ATTRACT ){
		ofSetColor(255, 63, 180);
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofSetColor(208, 255, 63);
	}
	else if( mode == PARTICLE_MODE_NOISE ){
		ofSetColor(99, 63, 255);
	}
	else if( mode == PARTICLE_MODE_NEAREST_POINTS ){
        ofSetColor(color);
	}
    
	ofCircle(pos.x, pos.y, scale * 4.0);
}

