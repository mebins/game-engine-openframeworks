//
//  voyager.cpp
//  final-project
//
//  Created by Mebin Skaria on 12/11/18.
//

#include "voyager.hpp"

voyage::voyage(const ofxAssimpModelLoader &model)
{
    up = false;
    down = false;
    left = false;
    right = false;
    this->model = model; // expects one mesh.
    this->model.setRotation(0, 180, 1, 0, 0);
    transform.position = this->model.getPosition();
    thruster_one = new ParticleEmitter();
    thruster_two = new ParticleEmitter();
    transform_particle_system = new ParticleSystem();
    transform.lifespan = -1;
    transform.color = ofColor::red;
    transform.radius = 20;
    transform_particle_system->add(transform);
    transform = transform_particle_system->particles[0];
    int particle_mass = 1.0f;
    int particle_lifespan = 1.0f;
    int particle_speed = 1.0f;
    init_thruster(thruster_one, particle_mass, particle_lifespan, particle_speed);
    init_thruster(thruster_two, particle_mass, particle_lifespan, particle_speed);
    gravity_force = new GravityForce(ofVec3f(0,-0.4,0.0)); // should make it go up?
    transform_particle_system->addForce(gravity_force);
    momentum = new ThrusterForce(dir,speed);
    transform_particle_system->addForce(momentum);
    cout << transform_particle_system->particles.size() << endl;
    transform =transform_particle_system->particles[0];
    
    
    //thruster + sound
    shoot_thrusters = false;
    thruster_sound.load("sounds/voyager/thrusters.wav"); //from the voyager folder. Thruster Sound.
    thruster_sound.setLoop(true);
    //thruster_sound.setVolume;
}

ofVec3f voyage::get_position()
{
    return transform.position;
}

void voyage::init_thruster(ParticleEmitter *thruster, float mass, float lifespan, float speed)
{
    thruster->setMass(mass);
    thruster->setEmitterType(DiscEmitter);
    thruster->setLifespan(lifespan);
    thruster->setParticleRadius(.1);
    thruster->setGroupSize(1);
    thruster->setRate(2);
    thruster->setVelocity(ofVec3f(0,speed,speed));
}
void voyage::toggle_thrusters()
{
    if(shoot_thrusters)
    {
        thruster_one->start();
        thruster_two->start();
        thruster_sound.play();
    }
    else
    {
        thruster_one->stop();
        thruster_two->stop();
        thruster_sound.stop();
    }
}
void voyage::keyReleased(int key) {
    switch (key) {
            
        //Thruster Logic
        case OF_KEY_UP:
            if(up)
            {
                up = false;
                dir -= ofVec3f(1,0,0);
            }
        case OF_KEY_DOWN:
            if(down)
            {
                down=false;
                dir -= ofVec3f(-1,0,0);
            }
        case OF_KEY_LEFT:
            if(left)
            {
                left=false;
                dir -= ofVec3f(0,0,-1);
            }
        case OF_KEY_RIGHT:
            if(right)
            {
                right=false;
                dir -= ofVec3f(0,0,1);
            }
            if(!(left||right||up||down)) {shoot_thrusters = false; toggle_thrusters();}
        default:
            break;
            
    }
}
void voyage::key_pressed(int key)
{
    switch(key){
        case ',':
            rotation += 10;
            break;
        case '.':
            rotation -= 10;
            break;
        case 'm':
            dir += ofVec3f(0,.1,0);
            break;
        case 'n':
            dir = ofVec3f(0,0,0);
            break;
            //a waterfall pattern, by mebin. : movement?
        case OF_KEY_UP:
            if(!up)
            {
                up = true;
                dir += ofVec3f(1,0,0);
            }
        case OF_KEY_DOWN:
            if(!down)
            {
                down=true;
                dir += ofVec3f(-1,0,0);
            }
        case OF_KEY_LEFT:
            if(!left)
            {
                left=true;
                dir += ofVec3f(0,0,-1);
            }
        case OF_KEY_RIGHT:
            if(!right)
            {
                right=true;
                dir += ofVec3f(0,0,1);
            }
            if(left||right||up||down) {shoot_thrusters = true; toggle_thrusters();}
        default:
            break;
                    //version 3
    }
}
void voyage::set_rotation(float degree, float x, float y, float z){
    model.setRotation(1,degree,x,y,z);
    
}
void voyage::draw_thrusters()
{
    if(shoot_thrusters)
    {
        ofSetColor(ofColor::red);
        thruster_one->draw();
        ofSetColor(ofColor::white);
        thruster_two->draw();
    }
}
void voyage::update_thrusters()
{
    if(shoot_thrusters)
    {
        thruster_one->setPosition(ofVec3f(transform.position.x-6,transform.position.y+1.8,transform.position.z-.9)); // moves with ship
        thruster_two->setPosition(ofVec3f(transform.position.x-6, transform.position.y+1.8, transform.position.z+.9));
        thruster_one->update();
        thruster_two->update();
    }
}
void voyage::draw()
{
    model.drawFaces();
    draw_thrusters();
    ofSetColor(ofColor::red);
    // transform_particle_system->draw(); //draws transform particle
}
void voyage::draw_wireframe()
{
    ofSetColor(ofColor::hotPink);
    model.drawWireframe();
    draw_thrusters();
}
void voyage::update()
{
    transform_particle_system->update(); // should call the update on gravity force.
    //checking if the transform moves because of gravity
    //cout << "age: " << transform_particle_system->particles[0].age() << "position: " << transform_particle_system->particles[0].position << endl;
    transform = transform_particle_system->particles[0];
    momentum->setDir(dir);
    setRotation(rotation);
    set_rotation(rotation, 0, 1, 0);
    //updates model position.
    model.setPosition(transform.position.x, transform.position.y, transform.position.z);
    update_thrusters();
}
