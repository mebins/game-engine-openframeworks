//
//  Octree.cpp
//  final-project
//
//  Created by Mebin Skaria on 12/10/18.
//  Kevin M. Smith - Basic Octree Class - CS134/235 4/18/18

#include "Octree.h"

void Octree::draw()
{
    draw(root, 0);
}
void Octree::draw_leaf()
{
    drawLeafNodes(root, 0);
}

void Octree::draw(TreeNode & node, int level) {
    drawBox(node.box);
    level++;
    for (int i = 0; i < node.children.size(); i++) {
        draw(node.children[i], level);
    }
}

// draw only leaf Nodes
//
void Octree::drawLeafNodes(TreeNode & node, int level) {
    if(node.children.empty()) {
        ofSetColor(ofColor::white);
        drawBox(node.box);
    }
    else {
        for(int i = 0; i < node.children.size(); i++) {
            drawLeafNodes(node.children[i],level+1);
        }
    }
}

//draw a box from a "Box" class
//
void Octree::drawBox(const Box &box) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    ofVec3f p = ofVec3f(center.x(), center.y(), center.z());
    float w = size.x();
    float h = size.y();
    float d = size.z();
    ofDrawBox(p, w, h, d);
}

// return a Mesh Bounding Box for the entire Mesh
//
Box Octree::meshBounds(const ofMesh & mesh) {
    int n = mesh.getNumVertices();
    ofVec3f v = mesh.getVertex(0);
    ofVec3f max = v;
    ofVec3f min = v;
    for (int i = 1; i < n; i++) {
        ofVec3f v = mesh.getVertex(i);
        
        if (v.x > max.x) max.x = v.x;
        else if (v.x < min.x) min.x = v.x;
        
        if (v.y > max.y) max.y = v.y;
        else if (v.y < min.y) min.y = v.y;
        
        if (v.z > max.z) max.z = v.z;
        else if (v.z < min.z) min.z = v.z;
    }
    cout << "vertices: " << n << endl;
    //    cout << "min: " << min << "max: " << max << endl;
    return Box(Vector3(min.x, min.y, min.z), Vector3(max.x, max.y, max.z));
}

// getMeshPointsInBox:  return an array of indices to points in mesh that are contained
//                      inside the Box.  Return count of points found;
//
int Octree::getMeshPointsInBox(const ofMesh & mesh, const vector<int>& points,
                               Box & box, vector<int> & pointsRtn)
{
    int counter = 0;
    for (int i = 0; i < points.size(); i++) {
        ofVec3f p = mesh.getVertex(points[i]);
        Vector3 point = Vector3(p.x, p.y, p.z);
        if(box.inside(point)){
            pointsRtn.push_back(i);
            counter++;
        }
    }
    return counter;
}

//  Subdivide a Box into eight(8) equal size boxes, return them in boxList;
//
void Octree::subDivideBox8(const Box &box, vector<Box> & boxList) {
    Vector3 min = box.parameters[0];
    Vector3 max = box.parameters[1];
    Vector3 size = max - min;
    Vector3 center = size / 2 + min;
    float xdist = (max.x() - min.x()) / 2;
    float ydist = (max.y() - min.y()) / 2;
    float zdist = (max.z() - min.z()) / 2;
    Vector3 h = Vector3(0, ydist, 0);
    
    //  generate ground floor
    //
    Box b[8];
    b[0] = Box(min, center);
    b[1] = Box(b[0].min() + Vector3(xdist, 0, 0), b[0].max() + Vector3(xdist, 0, 0));
    b[2] = Box(b[1].min() + Vector3(0, 0, zdist), b[1].max() + Vector3(0, 0, zdist));
    b[3] = Box(b[2].min() + Vector3(-xdist, 0, 0), b[2].max() + Vector3(-xdist, 0, 0));
    
    boxList.clear();
    for (int i = 0; i < 4; i++)
        boxList.push_back(b[i]);
    
    // generate second story
    //
    for (int i = 4; i < 8; i++) {
        b[i] = Box(b[i - 4].min() + h, b[i - 4].max() + h);
        boxList.push_back(b[i]);
    }
}

void Octree::create(const ofMesh & geo, int numLevels) {
    // initialize octree structure
    mesh = geo;
    root.box = meshBounds(geo); // level 0e
    int n = mesh.getNumVertices();
    for(int i = 0; i < n; i++)
    {
        root.points.push_back(i);
    }
    subdivide(root, numLevels, 1);
    cout << "subdivide process done" << endl;
}

void Octree::subdivide(TreeNode & node, int numLevels, int level) {
    if(level >= numLevels) {
        return;
    }
    vector<Box> boxlist;
    subDivideBox8(node.box, boxlist);
    for(int i = 0; i < 8; i++) {
        TreeNode new_child;
        int count = getMeshPointsInBox(mesh, node.points,boxlist[i],new_child.points);
        if(count > 0) {
            if(count > 1)
            {
                new_child.box = boxlist[i];
                subdivide(new_child, numLevels, level+1);
            }
            node.children.push_back(new_child);
        }
    }
}





