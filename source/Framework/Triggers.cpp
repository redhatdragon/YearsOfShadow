#include "Triggers.h"
#include <UnigineEditor.h>
#include <UnigineVisualizer.h>
#include <UnigineNodes.h>

void Triggers::init(const NodePtr &node, float extra_scale)
{
	root = node;
	if (!root)
		return;

	this->extra_scale = extra_scale;
	trigger.clear();
	init_children(root);
}

void Triggers::init_children(const NodePtr &node)
{
	if (!node) return;

	addTrigger(node);

	// find next
	for (int i = 0; i < node->getNumChildren(); i++)
	{
		NodeReferencePtr nodeRef = checked_ptr_cast<NodeReference>(node->getChild(i));
		if (nodeRef)
			init_children(nodeRef->getReference());
		else
			init_children(node->getChild(i));
	}
}

void Triggers::addTrigger(const NodePtr &node)
{
	ObjectPtr obj = checked_ptr_cast<Object>(node);
	if (obj)
	{
		// disable all collisions (surfaces and shapes)
		int surfaces_count = obj->getNumSurfaces();
		for (int i = 0; i < surfaces_count; i++)
			obj->setCollision(false, i);

		BodyPtr obj_body = obj->getBody();
		if (obj_body)
		{
			int shapes_count = obj_body->getNumShapes();
			for (int i = 0; i < shapes_count; i++)
				obj_body->getShape(i)->setEnabled(false);
		}

		node->setScale(node->getScale() + vec3_one * extra_scale);
		node->setEnabled(0);

		for (int i = 0; i < obj->getNumSurfaces(); i++)
		{
			obj->setCollision(0, i);
			obj->setIntersection(0, i);
			obj->setCastShadow(0, i);
			obj->setCastWorldShadow(0, i);
		}

		const char *name = obj->getSurfaceName(0);

		TriggerInfo t;
		t.node = node;

		if (strcmp(name, "box") == 0)
			t.type = TriggerInfo::Box;
		else if (strcmp(name, "sphere") == 0)
			t.type = TriggerInfo::Sphere;
		else if (strcmp(name, "capsule") == 0)
			t.type = TriggerInfo::Capsule;
		else if (strcmp(name, "cylinder") == 0)
			t.type = TriggerInfo::Cylinder;

		trigger.append(t);
	}
}

bool Triggers::isInside(const Vec3 &pos)
{
	depth = 0;
	bool is_inside = false;

	for (int i = 0; i < trigger.size(); i++)
	{
		vec3 p = trigger[i].node->toLocal(pos);

		switch (trigger[i].type)
		{
		case TriggerInfo::Box:
			if (is_inside_box(i, p, pos)) is_inside = true;
			break;
		case TriggerInfo::Sphere:
			if (is_inside_sphere(i, p, pos)) is_inside = true;
			break;
		case TriggerInfo::Capsule:
			if (is_inside_capsule(i, p, pos)) is_inside = true;
			break;
		case TriggerInfo::Cylinder:
			if (is_inside_cylinder(i, p, pos)) is_inside = true;
			break;
		}
	}

	return is_inside;
}

float Triggers::getDepth()
{
	return (float)depth;
}

bool Triggers::is_inside_box(int i, const vec3 &p, const Vec3 &pw)
{
	if (p.x > -0.5f && p.x < 0.5f &&
		p.y > -0.5f && p.y < 0.5f &&
		p.z > -0.5f && p.z < 0.5f)
	{
		get_depth_box(i, p, pw);
		return true;
	}

	return false;
}

void Triggers::get_depth_box(int i, const vec3 &p, const Vec3 &pw)
{
	float x = Math::abs(-0.5f - p.x) < Math::abs(0.5f - p.x) ? -0.5f : 0.5f;
	float dx = (float)length(trigger[i].node->toWorld(vec3(x, p.y, p.z)) - pw);
	float y = Math::abs(-0.5f - p.y) < Math::abs(0.5f - p.y) ? -0.5f : 0.5f;
	float dy = (float)length(trigger[i].node->toWorld(vec3(p.x, y, p.z)) - pw);
	float z = Math::abs(-0.5f - p.z) < Math::abs(0.5f - p.z) ? -0.5f : 0.5f;
	float dz = (float)length(trigger[i].node->toWorld(vec3(p.x, p.y, z)) - pw);
	
	depth = max(depth, Scalar(min(dx, min(dy, dz))));
}

bool Triggers::is_inside_sphere(int i, const vec3 &p, const Vec3 &pw)
{
	if (length(p) < 1.0f)
	{
		get_depth_sphere(i, p, pw);
		return true;
	}

	return false;
}

void Triggers::get_depth_sphere(int i, const vec3 &p, const Vec3 &pw)
{
	vec3 pos = p;
	if (pos == vec3_zero)
	{
		vec3 size = trigger[i].node->getWorldScale();
		if (size.x > size.y && size.x > size.z) pos = vec3(1, 0, 0);
		if (size.y > size.x && size.y > size.z) pos = vec3(0, 1, 0);
		if (size.z > size.y && size.z > size.x) pos = vec3(0, 0, 1);
	}
	vec3 dir = normalize(pos);

	depth = max(depth, length(trigger[i].node->toWorld(dir) - pw));
}

bool Triggers::is_inside_capsule(int i, const vec3 &p, const Vec3 &pw)
{
	float d = clamp(dot(p, vec3_up), -0.5f, 0.5f); // height
	vec3 line_point = vec3_up * d;

	float l = length(p - line_point);
	if (l <= 1.0f) // radius
	{
		depth = max(depth, Scalar(1.0f - l));
		return true;
	}

	return false;
}

bool Triggers::is_inside_cylinder(int i, const vec3 &p, const Vec3 &pw)
{
	float d = dot(p, vec3_up);
	if (Math::abs(d) > 0.25f) // height / 2
		return false;

	vec3 line_point = vec3_up * d;

	float l = length(p - line_point);
	if (l <= 1.0f) // radius
	{
		depth = max(depth, Scalar(1.0f - l));
		return true;
	}

	return false;
}

void Triggers::renderVisualizer()
{
	Visualizer::setEnabled(1);

	vec4 color = vec4(0, 1, 0, 1);
	for (int i = 0; i < trigger.size(); i++)
	{
		float rx = 1.0f;
		float ry = 1.0f;
		float rz = 1.0f;

		if (trigger[i].type == TriggerInfo::Box)
		{
			rx = ry = rz = 0.5f;
		}
		else if (trigger[i].type == TriggerInfo::Capsule)
		{
			rz = 1.5f;
		}
		else if (trigger[i].type == TriggerInfo::Cylinder)
		{
			rz = 0.5f;
		}
		
		Vec3 p0 = Vec3(-rx, -ry, rz);
		Vec3 p1 = Vec3(rx, -ry, rz);
		Vec3 p2 = Vec3(rx, -ry, -rz);
		Vec3 p3 = Vec3(-rx, -ry, -rz);
		Vec3 p4 = Vec3(-rx, ry, rz);
		Vec3 p5 = Vec3(rx, ry, rz);
		Vec3 p6 = Vec3(rx, ry, -rz);
		Vec3 p7 = Vec3(-rx, ry, -rz);

		Node *node = trigger[i].node.get();
		p0 = node->toWorld(vec3(p0));
		p1 = node->toWorld(vec3(p1));
		p2 = node->toWorld(vec3(p2));
		p3 = node->toWorld(vec3(p3));
		p4 = node->toWorld(vec3(p4));
		p5 = node->toWorld(vec3(p5));
		p6 = node->toWorld(vec3(p6));
		p7 = node->toWorld(vec3(p7));

		Visualizer::renderLine3D(p0, p1, color);
		Visualizer::renderLine3D(p1, p2, color);
		Visualizer::renderLine3D(p2, p3, color);
		Visualizer::renderLine3D(p3, p0, color);
		Visualizer::renderLine3D(p4, p5, color);
		Visualizer::renderLine3D(p5, p6, color);
		Visualizer::renderLine3D(p6, p7, color);
		Visualizer::renderLine3D(p7, p4, color);
		Visualizer::renderLine3D(p0, p4, color);
		Visualizer::renderLine3D(p1, p5, color);
		Visualizer::renderLine3D(p2, p6, color);
		Visualizer::renderLine3D(p3, p7, color);
	}
}

