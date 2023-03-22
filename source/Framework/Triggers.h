#pragma once
#include <UnigineNode.h>
#include <UnigineVector.h>

using namespace Unigine;
using namespace Math;

struct TriggerInfo
{
	enum Type { Box, Sphere, Capsule, Cylinder };

	NodePtr node;
	Type type;
};

class Triggers
{
public:
	void init(const NodePtr &node, float extra_scale = 0);
	void addTrigger(const NodePtr &node);

	bool isInside(const Vec3 &pos);
	float getDepth();

	void renderVisualizer();

private:
	Scalar depth = 0;
	float extra_scale = 0;
	NodePtr root;
	Vector<TriggerInfo> trigger;

	void init_children(const NodePtr &node);
	
	bool is_inside_box(int i, const vec3 &p, const Vec3 &pw);
	bool is_inside_sphere(int i, const vec3 &p, const Vec3 &pw);
	bool is_inside_capsule(int i, const vec3 &p, const Vec3 &pw);
	bool is_inside_cylinder(int i, const vec3 &p, const Vec3 &pw);

	void get_depth_box(int i, const vec3 &p, const Vec3 &pw);
	void get_depth_sphere(int i, const vec3 &p, const Vec3 &pw);
};