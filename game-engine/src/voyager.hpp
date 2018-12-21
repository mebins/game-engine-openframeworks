//
//  voyager.hpp
//  final-project
//
//  Created by Mebin Skaria on 12/11/18.
//

#ifndef voyager_hpp
#define voyager_hpp

#include <stdio.h>
#include "ofMain.h"
#include "ParticleEmitter.h"
#include "TransformObject.h"
#include "ofxAssimpModelLoader.h"

class voyage : public TransformObject{
public:
    voyage(const ofxAssimpModelLoader &model);
    void draw();
    void update();
    void draw_wireframe();
    void update_thrusters();
    void draw_thrusters();
    void toggle_thrusters();
    void key_pressed(int key);
    void keyReleased(int key);
    void set_rotation(float degree, float x, float y, float z);
    ofVec3f get_position();
private:
    void init_thruster(ParticleEmitter *thruster, float mass, float lifespan, float speed);
    Particle transform;
    ParticleEmitter *thruster_one;
    ParticleEmitter *thruster_two;
    ParticleSystem *transform_particle_system;
    ofVec3f dir;
    bool up,down,left,right;
    float speed = 2.0f;
    float rotation = 0.0f;
    ofxAssimpModelLoader model;
    GravityForce *gravity_force;
    ThrusterForce *momentum;
    bool shoot_thrusters = false;
    //Sound
    ofSoundPlayer thruster_sound;
};

#endif /* voyager_hpp */
