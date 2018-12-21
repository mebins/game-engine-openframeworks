#pragma once
#include "ofMain.h"
#include "box.h"
#include "ray.h"


class TreeNode {

public:
	Box box;
	vector<int> points;
	vector<TreeNode> children;
    
};

class Octree {
public:
    void create(const ofMesh & mesh, int numLevels);
    void draw_leaf();
    void draw();
private:
    void subdivide(TreeNode & node, int numLevels, int level);
    void draw(TreeNode & node, int level);
	void drawLeafNodes(TreeNode & node, int level);
	static void drawBox(const Box &box);
	static Box meshBounds(const ofMesh &);
	int getMeshPointsInBox(const ofMesh &mesh, const vector<int> & points, Box & box, vector<int> & pointsRtn);
	void subDivideBox8(const Box &b, vector<Box> & boxList);
	ofMesh mesh;
	TreeNode root;
    
};
