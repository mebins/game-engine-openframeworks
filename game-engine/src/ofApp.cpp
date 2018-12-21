#include "ofApp.h"
#include "Util.h"

/*--------------------------------------------------------------
//  Mebin Skaria
//  December 10th 2018
//  This is an openFrameworks 3D scene that includes an EasyCam
//  and example 3D geometry which I retrieved from https://free3d.com/3d-model/mountains-scene-41819.html
--------------------------------------------------------------*/
void ofApp::setup(){
    bWireframe = false;
    bDisplayPoints = false;
    bAltKeyDown = false;
    bCtrlKeyDown = false;
    bTerrainSelected = true;
    //    ofSetWindowShape(1024, 768);
    
    //CAMERAS
    //easy
    theCam = &cam;
    cam.setDistance(10);
    cam.setNearClip(.1);
    cam.setFov(65.5);   // approx equivalent to 28mm in 35mm format
    ofSetVerticalSync(true);
    cam.disableMouseInput();
   
    //track
    trackCam.setNearClip(.1);
    trackCam.setFov(65.5);
    track_camera_offset = ofVec3f(0,100,0);
    trackCam.rotateDeg(-90, 1,0,0);
    trackCam.setPosition(0,200,0);
    
    //tail
    tailCam.setNearClip(.1);
    tailCam.setFov(65.5);
    tail_camera_offset = ofVec3f(-10,20,0);
    tailCam.rotateDeg(-90,1,0,0);
    tailCam.rotateDeg(-90,0,1,0);
    tailCam.rotateDeg(90,0,0,1);
    tailCam.setPosition(0,200,0);
    ofEnableSmoothing();
    ofEnableDepthTest();
    
    //down
    downCam.setNearClip(.1);
    downCam.setFov(65.5);
    down_camera_offset = ofVec3f(0,-10,0);
    downCam.rotateDeg(-90, 1,0,0);
    downCam.setPosition(0,200,0);
    

    // setup rudimentary lighting
    //
    initLightingAndMaterials();
    
    // Environment Model
    //
    env.loadModel("geo/city-block-mesh-v3.obj");
    env.setScaleNormalization(false);
    //rotates mesh to proper orientation
    env.setRotation(0,180,1,0,0);
    env.setRotation(1,180,0,1,0);
    env.setPosition(0, 0, 0);
    cout << env.getMeshCount() << endl;
    env_collision = env.getMesh(0);
    // Shuttle Model
    //
    shuttle.loadModel("transport/transport-shuttle-model.obj");
    shuttle_model_loaded = true;
    shuttle.setScaleNormalization(false);
    shuttle.setPosition(0,100,0);
    shuttle.setRotation(0,180,1,0,0);
    voyager = new voyage(shuttle);
    tree.create(env_collision,0);
}

//

//--------------------------------------------------------------
void ofApp::update(){
    voyager->update();
    camera_position_updater();
}
/*
 Updates Track Cam : transform.
 */
void ofApp::camera_position_updater()
{
     ofVec3f pos = voyager->get_position();
     ofVec3f track_cam_transform = pos + track_camera_offset;
     trackCam.setPosition(track_cam_transform.x, track_cam_transform.y, track_cam_transform.z);
     ofVec3f tail_cam_transform = pos + tail_camera_offset;
     tailCam.setPosition(tail_cam_transform.x, tail_cam_transform.y,tail_cam_transform.z);
     tailCam.rotateAroundDeg((voyager->getRotation()), ofVec3f(0,1,0), ofVec3f(pos.x, pos.y, pos.z));
     tailCam.lookAt(pos, ofVec3f(0,1,0));
     ofVec3f down_cam_transform = pos + down_camera_offset;
     downCam.setPosition(down_cam_transform.x,down_cam_transform.y,down_cam_transform.z);
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor::green, ofColor::blue);   // pick your own backgroujnd
    ofBackground(ofColor::blue);
    theCam->begin();
    ofPushMatrix();
    if (bWireframe) {                    // wireframe mode  (include axis)
        ofDisableLighting();
        ofSetColor(ofColor::slateGray);
        env.drawWireframe();
        ofSetColor(ofColor::green);
        env_collision.drawWireframe();
        
        ofSetColor(ofColor::slateGray);
        if (shuttle_model_loaded) {
            voyager->draw_wireframe();
            if (!bTerrainSelected) drawAxis(shuttle.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    else {
        ofEnableLighting();              // shaded mode
        env.drawFaces();
        ofSetColor(ofColor::green);
        if (shuttle_model_loaded) {
            voyager->draw();
            if (!bTerrainSelected) drawAxis(shuttle.getPosition());
        }
        if (bTerrainSelected) drawAxis(ofVec3f(0, 0, 0));
    }
    if (bDisplayPoints) {                // display points as an option
        glPointSize(3);
        ofSetColor(ofColor::green);
        env.drawVertices();
    }
    ofSetColor(ofColor::red);
   // tree.draw_leaf();
    ofNoFill();
    ofSetColor(ofColor::white);
    
    ofPopMatrix();
    theCam->end();
}
//

// Draw an XYZ axis in RGB at world (0,0,0) for reference.
//
void ofApp::drawAxis(ofVec3f location) {
    
    ofPushMatrix();
    ofTranslate(location);
    
    ofSetLineWidth(1.0);
    
    // X Axis
    ofSetColor(ofColor(255, 0, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(1, 0, 0));
    
    
    // Y Axis
    ofSetColor(ofColor(0, 255, 0));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 1, 0));
    
    // Z Axis
    ofSetColor(ofColor(0, 0, 255));
    ofDrawLine(ofPoint(0, 0, 0), ofPoint(0, 0, 1));
    
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'C':
        case 'c':
            if (cam.getMouseInputEnabled()) cam.disableMouseInput();
            else cam.enableMouseInput();
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            break;
        case 'r':
            cam.reset();
            break;
        case 's':
            savePicture();
            break;
        case 't':
            setCameraTarget();
            break;
        case 'u':
            break;
        case 'v':
            togglePointsDisplay();
            break;
        case 'V':
            break;
        case 'w':
            toggleWireframeMode();
            break;
        case OF_KEY_ALT:
            cam.enableMouseInput();
            bAltKeyDown = true;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = true;
            break;
        case OF_KEY_SHIFT:
            break;
        case OF_KEY_DEL:
            break;
        case '1':
            theCam = &cam;
            break;
        case '2':
            theCam = &trackCam;
            break;
        case '3':
            theCam = &tailCam;
            break;
        case '4':
            theCam = &downCam;
            break;
        case OF_KEY_F8:
            cout << theCam->getPosition() << endl;
            break;
        case 'e':
            voyager->toggle_thrusters();
            break;
        default:
            break;
    }
    //OBJECT KEY BINDING
    voyager->key_pressed(key);
}


void ofApp::toggleWireframeMode() {
    bWireframe = !bWireframe;
}

void ofApp::toggleSelectTerrain() {
    bTerrainSelected = !bTerrainSelected;
}

void ofApp::togglePointsDisplay() {
    bDisplayPoints = !bDisplayPoints;
}

void ofApp::keyReleased(int key) {
    
    switch (key) {
            
        case OF_KEY_ALT:
            cam.disableMouseInput();
            bAltKeyDown = false;
            break;
        case OF_KEY_CONTROL:
            bCtrlKeyDown = false;
            break;
        case OF_KEY_SHIFT:
            break;
        default:
            break;
    }
    voyager->keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

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
//--------------------------------------------------------------
//
// support drag-and-drop of model (.obj) file loading.  when
// model is dropped in viewport, place origin under cursor
//
void ofApp::dragEvent(ofDragInfo dragInfo) {
}


// Set the camera to use the selected point as it's new target
//
void ofApp::setCameraTarget() {
    
}

//--------------------------------------------------------------
// setup basic ambient lighting in GL  (for now, enable just 1 light)
//
void ofApp::initLightingAndMaterials() {
    
    static float ambient[] =
    { 1.0f, 0.0f, 0.0f, 1.0f };
    static float diffuse[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float position[] =
    {5.0, 5.0, 5.0, 0.0 };
    
    static float lmodel_ambient[] =
    { 1.0f, 1.0f, 1.0f, 1.0f };
    
    static float lmodel_twoside[] =
    { GL_TRUE };
    
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
    
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE, lmodel_twoside);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glShadeModel(GL_SMOOTH);
}

void ofApp::savePicture() {
    ofImage picture;
    picture.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
    picture.save("screenshot.png");
    cout << "picture saved" << endl;
}

