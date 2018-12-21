#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "box.h"
#include "ray.h"
#include "Octree.h"
#include "voyager.hpp"

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
        void drawAxis(ofVec3f);
        void initLightingAndMaterials();
        void savePicture();
        void toggleWireframeMode();
        void togglePointsDisplay();
        void toggleSelectTerrain();
        void setCameraTarget();
        void drawBox(const Box &box);
        void camera_position_updater();
    
        Octree tree;
    
        //Cameras
        ofEasyCam cam;
        ofCamera *theCam = NULL;
        ofCamera trackCam;
        ofVec3f track_camera_offset;
        ofCamera tailCam;
        ofVec3f tail_camera_offset;
        ofCamera downCam;
        ofVec3f down_camera_offset;
    
        //Models
        ofxAssimpModelLoader env, shuttle;
        voyage *voyager;
        ofMesh env_collision;
        //Lighting
        ofLight light;
    
        
    
    
        int  tree_levels = -1;
        bool bAltKeyDown;
        bool bCtrlKeyDown;
        bool bWireframe;
        bool bDisplayPoints;
        bool bPointSelected;
    
        bool shuttle_model_loaded;
        bool bTerrainSelected;
		
};
