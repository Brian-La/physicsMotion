//--------------------------------------------------------------
//
//  CS 134 - Fall 2020 - Midterm  Take-Home Starter File
//
//  Animate the Triangle "Ship" with physics
//   
//  (c) Kevin M. Smith - 2020 

#include "ofApp.h"


void Triangle::draw() {
	ofPushMatrix();
	ofMultMatrix(getMatrix());
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	ofPopMatrix();
}

// Determine if point p is inside the triangle shape. Method is the simplest 
	// way but not necessarily the most efficent.  We look at the sign of the angles
	// between the vectors connecting p and the three triangle vertices. They each
	// have to be ( < 0 ) for the point to be inside.  Note: glm::orientedAngle
	// will provide a signed angle if you supply an orientation vector (in this case Z axis).
	//
bool Triangle::inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	glm::vec3 v1 = glm::normalize(p1 - p);
	glm::vec3 v2 = glm::normalize(p2 - p);
	glm::vec3 v3 = glm::normalize(p3 - p);
	float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
	float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
	float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
	if (a1 < 0 && a2 < 0 && a3 < 0) return true;
	else return false;
}




//--------------------------------------------------------------
void ofApp::setup(){
	
	tri.pos = glm::vec3(ofGetWidth()/2.0, ofGetHeight()/2.0, 0);

	// Exam;ple on how to add elements to a vector<>
	// Store the vertices to the triangle relative to origin (0, 0, 0);
	// The triangle is moveable in that we add these vertices to it's
	// position to get screen space coordinates (for drawing).
	//
	tri.verts.push_back(glm::vec3(-20, -20, 0));
	tri.verts.push_back(glm::vec3(0, 40, 0));
	tri.verts.push_back(glm::vec3(20, -20, 0));

	ofSetBackgroundColor(ofColor::black);
    
}

//--------------------------------------------------------------
void ofApp::update() {

	//at each frame, update each particle
	if (startAnim) {
        
		//compute new position based on velocity and time step
		tri.pos += tri.veloc * (1 / ofGetFrameRate());		//position = position + velocity * dt
		tri.rotation += tri.degVeloc * (1 / ofGetFrameRate());	//repeat for angle via one-dimension
		
		//compute acceleration from forces on particle using a = (1/m) * f
		//add to existing particle acceleration
		tri.accel = (1 / tri.mass) * tri.force;		
		tri.degAccel = (1 / tri.mass) * tri.degForce;

		//compute new velocity based on total acceleration and time step
			//Velocity = velocity + acceleration * dt
		tri.veloc += tri.accel * (1 / ofGetFrameRate());
		tri.degVeloc += tri.degAccel * (1 / ofGetFrameRate());

		//damp velocity
			//Velocity = velocity * damping
		tri.veloc *= tri.damp;
		tri.degVeloc *= tri.damp;
		

		//set forces on particle to zero upon key release
		
	}


}



void ofApp::draw() {

    //color change to indicate thrust
    //
    if(startAnim && (ofGetKeyPressed(OF_KEY_RIGHT) ||
                     ofGetKeyPressed(OF_KEY_LEFT)  ||
                     ofGetKeyPressed(OF_KEY_DOWN)  ||
                     ofGetKeyPressed(OF_KEY_UP)))
        ofSetColor(ofColor::white);
	else ofSetColor(ofColor::blue);
	
	// draw triangle in screen space (we need to add the position to get screen coordinates)
	//

	tri.draw();
}

	
	

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch (key) {
	case 'f':
		bFullscreen = !bFullscreen;
		ofSetFullscreen(bFullscreen);
		break;
	case OF_KEY_CONTROL:
		bCtrlKeyDown = true;
		break;
	case OF_KEY_LEFT:   // turn left
		tri.degForce = 50;
		break;
	case OF_KEY_RIGHT:  // turn right
		tri.degForce = -50;
		break;
	case OF_KEY_UP:     // go forward
		tri.degVeloc = 0;		//freeze rotation if move
		tri.force = ofVec3f(-sin(tri.rotation * PI/180) * 50, 
			cos(tri.rotation * PI/180) * 50, 0);		//convert rotation to deg to calculate..
														//..force direction
		break;
	case OF_KEY_DOWN:   // go backward
		tri.degVeloc = 0;		
		tri.force = ofVec3f(sin(tri.rotation * PI / 180) * 50, 
			-cos(tri.rotation * PI / 180) * 50, 0);	//inverse
		break;
	case ' ':
		startAnim = !startAnim;
		cout << "Physics Toggled" << endl;
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	switch (key) {
	case OF_KEY_CONTROL:
		bCtrlKeyDown = false;
		break;
	case OF_KEY_LEFT:   // turn left
		tri.degForce = 0;
		break;
	case OF_KEY_RIGHT:  // turn right
		tri.degForce = 0;
		break;
	case OF_KEY_UP:     // go forward
		tri.force = ofVec3f(0, 0, 0);
		break;
	case OF_KEY_DOWN:   // go backward
		tri.force = ofVec3f(0, 0, 0);
		break;
	default:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	if (!bDrag) return;

	glm::vec3 mousePoint(x, y, 0);

	if (bCtrlKeyDown) {
		tri.rotation += x - lastMouse.x;
	}
	else {
		tri.pos += mousePoint - lastMouse;
	}
	lastMouse = mousePoint;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {


	// transform mouse point into object space
	//
	glm::mat4 T = tri.getMatrix();
	glm::vec3 p = glm::inverse(T) * glm::vec4(x, y, 0, 1);

	// check if mouse clicked inside
	//
	if (tri.inside(p, tri.verts[0], tri.verts[1], tri.verts[2])) {
		bDrag = true;
		lastMouse = glm::vec3(x, y, 0);
	}

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bDrag = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
