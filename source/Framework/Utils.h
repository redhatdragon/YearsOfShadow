#pragma once

#include <UnigineEditor.h>
#include <UnigineWindowManager.h>
#include <UnigineConsole.h>
#include <UnigineVisualizer.h>
#include <UnigineVector.h>
#include <functional>
#include <utility>
#include <string>
#include <vector>
#include <map>

using namespace Unigine;
using namespace Math;

/*
Math: Common
*/
float loop01(float val);
float lerp_clamped(float v0, float v1, float k);
float resize(float v, float from0, float from1, float to0, float to1);
float moveTowards(float current, float target, float maxDelta);
vec3 moveTowards(const vec3 &current, const vec3 &target, float maxDelta);
float linearRegression(const Vector<float> &values);
vec3 linearRegression(const Vector<vec3> &values);

/*
Math: Angles
*/
float rotateTowards01(float a, float b, float k); // from 0 to 1
float rotateTowards(float a, float b, float k); // from 0 to 360
float getAngle(const quat &source, const quat &target);
float getAngle(float source_angle, float target_angle);

/*
3D Math: Plane
*/
vec3 projectOntoPlane(const vec3 &vec, const vec3 &planeNormal);
bool isPointUnderPlane(const vec3 &point, const vec3 &plane_point, const vec3 &plane_normal);
vec3 rayPlaneIntersection(const vec3 &point, const vec3 &point_direction, const vec3 &plane_point, const vec3 &plane_normal);
vec3 reflectPlane(const vec3 &vector, const vec3 &normal);

/*
3D Math: Bound Box
*/
bool isInsideBoundBox(const vec3 &point, const vec3 &bound_min, const vec3 &bound_max);
vec3 getOffsetToBoundBox(const vec3 &point, const vec3 &bound_min, const vec3 &bound_max);

/*
Player: Screen/World Position
*/
ivec2 getScreenPosition(const NodePtr &node, const CameraPtr &cam);
ivec2 getScreenPosition(const Vec3 &point, const CameraPtr &cam);
Vec3 getCameraLookPoint(const PlayerPtr &player, float maxDist = 1000);

/*
Node: Child/Parent Relationships
*/
NodePtr getRootNode(const NodePtr &node);
NodePtr getChildNodeRecursively(const NodePtr &node, const std::string &name);
void setParentKeepPosition(const NodePtr &child, const NodePtr &parent);
void unParentKeepPosition(const NodePtr &child);

/*
Node: Directions
*/ 
vec3 getDirectionX(const NodePtr &node);
vec3 getDirectionY(const NodePtr &node);
vec3 getDirectionZ(const NodePtr &node);
vec3 getDirectionNX(const NodePtr &node);
vec3 getDirectionNY(const NodePtr &node);
vec3 getDirectionNZ(const NodePtr &node);
vec3 getWorldDirectionX(const NodePtr &node);
vec3 getWorldDirectionY(const NodePtr &node);
vec3 getWorldDirectionZ(const NodePtr &node);
vec3 getWorldDirectionNX(const NodePtr &node);
vec3 getWorldDirectionNY(const NodePtr &node);
vec3 getWorldDirectionNZ(const NodePtr &node);
void setDirectionX(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setDirectionY(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setDirectionZ(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setDirectionNX(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setDirectionNY(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setDirectionNZ(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setWorldDirectionX(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setWorldDirectionY(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setWorldDirectionZ(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setWorldDirectionNX(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setWorldDirectionNY(const NodePtr &node, const vec3 &dir, const vec3 &up);
void setWorldDirectionNZ(const NodePtr &node, const vec3 &dir, const vec3 &up);
void visualizeAxis(const Mat4 &mat, float scale = 1);

/*
Transform: Partial changes
*/
void resetRotation(mat4 &transform);
void setTransformPosition(Mat4 &transform, const Vec3 &pos);
void setTransformRotation(Mat4 &transform, const quat &rot);
vec3 getDirectionX(const mat4 &mat);
vec3 getDirectionY(const mat4 &mat);
vec3 getDirectionZ(const mat4 &mat);
vec3 getDirectionNX(const mat4 &mat);
vec3 getDirectionNY(const mat4 &mat);
vec3 getDirectionNZ(const mat4 &mat);
void setDirectionX(mat4 &mat, const vec3 &dir, const vec3 &up);
void setDirectionY(mat4 &mat, const vec3 &dir, const vec3 &up);
void setDirectionZ(mat4 &mat, const vec3 &dir, const vec3 &up);
void setDirectionNX(mat4 &mat, const vec3 &dir, const vec3 &up);
void setDirectionNY(mat4 &mat, const vec3 &dir, const vec3 &up);
void setDirectionNZ(mat4 &mat, const vec3 &dir, const vec3 &up);

/*
Bones
*/
void logShowBones(ObjectMeshSkinnedPtr &skin);
void visualizeBones(ObjectMeshSkinnedPtr &skin, float scale = 0.5f);
vec3 getBonePosition(ObjectMeshSkinnedPtr &skin, int boneNum);
void setBonePosition(ObjectMeshSkinnedPtr &skin, int boneNum, const vec3 &pos);
quat getBoneRotation(ObjectMeshSkinnedPtr &skin, int boneNum);
void setBoneRotation(ObjectMeshSkinnedPtr &skin, int boneNum, const quat &rot);
void setBoneRotation(ObjectMeshSkinnedPtr &skin, int boneNum, const quat &rot, const vec3 &scale);

/*
Mesh Dynamic: Procedural Generators
*/
void addLineSegment(const ObjectMeshDynamicPtr &mesh, const vec3 &from, const vec3 &to, const vec3 &from_right, const vec3 &to_right, float width);
void addLineSegment(const ObjectMeshDynamicPtr &mesh, const vec3 &from, const vec3 &to, const vec3 &from_forward, float width);
void addLineSegment(const ObjectMeshDynamicPtr &mesh, const vec3 &from, const vec3 &to, float width);

/*
Spline
*/
Vec3 getHermiteSpline(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2, const Vec3 &p3, float t);

/*
Callbacks

usage example:
	Action<void()> onAction;
	Action<void(BodyPtr, int)> onContact;

	onAction += some_static_member;

	int id = onAction.addListener(some_static_member);
	onAction.removeListener(id);

	onAction += std::bind(&ClassName::non_static_member, this, std::placeholders::_1, std::placeholders::_2);
	onAction();
*/
template<class Sig>
struct Action
{
	// add new listener
	template<typename Functor>
	void operator+=(Functor&& f)
	{
		addListener(f);
	}

	// add new listener with returning id
	template<typename Functor>
	int addListener(Functor&& f)
	{
		if (in_process)
		{
			add_id_queue.push_back(id);
			add_function_queue.push_back(std::forward<Functor>(f));
			return id++;
		}

		_funcs[id] = std::forward<Functor>(f);
		return id++;
	}

	// reserve id for some listener
	// (use addListener(int, Functor) later)
	int reserveId()
	{
		return id++;
	}

	// add listener to specific place 
	// use it when you want to return
	// removed listener (or add reserved)
	template<typename Functor>
	void addListener(int id, Functor&& f)
	{
		if (in_process)
		{
			add_id_queue.push_back(id);
			add_function_queue.push_back(std::forward<Functor>(f));
			return;
		}

		_funcs[id] = std::forward<Functor>(f);
	}

	// remove listener by id
	void removeListener(int id)
	{
		if (in_process)
		{
			remove_queue.push_back(id);
			return;
		}

		_funcs.erase(id);
	}

	// count of listeners
	size_t size() const
	{
		return _funcs.size();
	}

	// remove all listeners from Action
	void clear()
	{
		if (in_process)
		{
			need_clear = true;
			return;
		}

		_funcs.clear();

		// id doesn't set to 0 because we
		// want to safe unique ID for each new functor
	}

	// execute (call to listeners)
	template<class... Args>
	void operator()(Args&&... args)
	{
		// execute
		in_process = true;
		for (auto it = _funcs.begin(); it != _funcs.end(); ++it)
			it->second(args...);
		in_process = false;

		// check add/remove queue
		if (add_id_queue.size() > 0)
		{
			for (int i = 0; i < add_id_queue.size(); i++)
				addListener(add_id_queue[i], add_function_queue[i]);

			add_id_queue.clear();
			add_function_queue.clear();
		}

		if (remove_queue.size() > 0 || need_clear)
		{
			for (int i = 0; i < remove_queue.size(); i++)
				removeListener(remove_queue[i]);
			remove_queue.clear();
			need_clear = false;
		}
	}

private:
	std::map<int, std::function<Sig>> _funcs;
	int id = 0;

	// deferred execution of methods
	// (because we want sometimes add/remove
	// listeners inside called method)
	bool in_process = false;

	std::vector<int> add_id_queue;
	std::vector<std::function<Sig>> add_function_queue;

	std::vector<int> remove_queue;
	bool need_clear = false;
};
