#pragma once

#include "ofMain.h"

class Shape {
public:
	Shape() {}
	virtual void draw() {}
	virtual bool inside(glm::vec3 p) { return false; }   // returns True if p is inside Shape

	glm::mat4 getMatrix() {
		glm::mat4 m = glm::translate(glm::mat4(1.0), glm::vec3(pos));
		glm::mat4 M = glm::rotate(m, glm::radians(rotation), glm::vec3(0, 0, 1));
		return M;
	}

	// data
	//
	vector<glm::vec3> verts;
	glm::vec3 pos;   // translation
	float rotation = 0;   // degrees
    
};

class Triangle: public Shape {
public:
	bool inside(glm::vec3 p, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
	void draw();


	// local data define here
    ofVec3f veloc;       //velocity vecs
    ofVec3f accel;        //acceleration vec
    ofVec3f force;        //force

	float degVeloc, degAccel, degForce;		//angle parameters
	
    float damp = 0.99;     //damping value (drag)
    float mass = 0.5;     //mass
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
	  	void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);



		// application data
	
		glm::vec3 lastMouse;   // location of where mouse moved last (when dragging)
		
		// UI control data
		//
		bool bFullscreen = false;	
		bool bCtrlKeyDown = false;
		bool bDrag = false;

		// create one triangle
		//
		Triangle tri;

		bool startAnim = false;
		 
};
