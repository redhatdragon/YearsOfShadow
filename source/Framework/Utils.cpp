#include "Utils.h"

#include <limits>

enum Axis { X, Y, Z, UP };

vec3 projectOntoPlane(const vec3 &vec, const vec3 &planeNormal)
{
	return vec - planeNormal * dot(vec, planeNormal);
}

bool isPointUnderPlane(const vec3 &point, const vec3 &plane_point, const vec3 &plane_normal)
{
	if (dot(plane_normal, normalize(point - plane_point)) < 0)
		return true;
	return false;
}

vec3 rayPlaneIntersection(const vec3 &point, const vec3 &point_direction, const vec3 &plane_point, const vec3 &plane_normal)
{
	float d = dot(plane_normal, point_direction);
	if (d == 0) return plane_point;

	float t = (dot(plane_normal, (plane_point - point))) / d;
	return point + (point_direction * t);
}

ivec2 getScreenPosition(const NodePtr &node, const CameraPtr &cam)
{
	return getScreenPosition(node->getWorldPosition(), cam);
}

ivec2 getScreenPosition(const Vec3 &point, const CameraPtr &cam)
{
	EngineWindowPtr main_window = WindowManager::getMainWindow();
	ivec2 main_size = ivec2_one;
	if (main_window)
		main_size = main_window->getSize();

	float width =  static_cast<float>(main_size.x);
	float height = static_cast<float>(main_size.y);

	mat4 projection = cam->getProjection();
	mat4 modelview = mat4(cam->getModelview());
	projection.m00 *= height / width;

	vec4 p = projection * vec4(modelview * vec4(vec3(point), 1));
	if (p.w > 0)
	{
		return ivec2(
			ftoi(width * (0.5f + p.x * 0.5f / p.w)),
			ftoi(height - height * (0.5f + p.y * 0.5f / p.w)));
	}
	return ivec2(std::numeric_limits<int>::min(), std::numeric_limits<int>::min());
}

Vec3 getCameraLookPoint(const PlayerPtr &player, float maxDist)
{
	Vec3 p0 = player->getWorldPosition();
	Vec3 p1 = p0 + Vec3(player->getWorldDirection()) * maxDist;
	
	WorldIntersectionPtr intersection = WorldIntersection::create();
	ObjectPtr hitObj = World::getIntersection(p0, p1, 1, intersection);
	if (hitObj)
		return intersection->getPoint();
	return p1;
}

NodePtr getRootNode(const NodePtr &node)
{
	NodePtr result = node;
	NodePtr test = node;
	while (true)
	{
		test = test->getParent();
		if (!test)
		{
			return result;
		}
		result = test;
	}
}

NodePtr getChildNodeRecursively(const NodePtr &node, const std::string &name)
{
	int num = node->findChild(name.c_str());
	if (num != -1)
	{
		return node->getChild(num);
	}
	else
	{
		// go deeper
		for (int i = 0; i<node->getNumChildren(); ++i)
		{
			NodePtr r = getChildNodeRecursively(node->getChild(i), name);
			if (r)
			{
				return r;
			}
		}
	}
	throw;
}

// setParent + parent-relative position, scale and rotation 
// are modified such that the object keeps the same world space 
// position, rotation and scale as before.
void setParentKeepPosition(const NodePtr &child, const NodePtr &parent)
{
	if (parent)
	{
		Mat4 t = parent->getIWorldTransform() * child->getWorldTransform();
		child->setTransform(t);
		child->setParent(parent);
	}
	else if (child->getParent())
	{
		Mat4 t = child->getParent()->getWorldTransform() * child->getTransform();
		child->setTransform(t);
		child->setParent(parent);
	}
}

void unParentKeepPosition(const NodePtr &child)
{
	Mat4 t = child->getWorldTransform();
	NodePtr null_node;
	child->setParent(null_node);
	child->setWorldTransform(t);
}

float moveTowards(float current, float target, float maxDelta)
{
	if (Math::abs(target - current) <= maxDelta)
	{
		return target;
	}
	return current + Math::sign(target - current) * maxDelta;
}

vec3 moveTowards(const vec3 &current, const vec3 &target, float maxDelta)
{
	return vec3(
		moveTowards(current.x, target.x, maxDelta),
		moveTowards(current.y, target.y, maxDelta),
		moveTowards(current.z, target.z, maxDelta));
}

// values - the y-axis values
// return - next value after values
float linearRegression(const Vector<float> &values)
{
	if (values.size() <= 0)
		return 0;

	float sumOfX = 0;
	float sumOfY = 0;
	float sumOfXSq = 0;
	float sumOfYSq = 0;
	float ssX = 0;
	float sumCodeviates = 0;
	float sCo = 0;
	float count = itof(values.size());

	for (int ctr = 0; ctr < count; ctr++)
	{
		float x = itof(ctr);
		float y = values[ctr];
		sumCodeviates += x * y;
		sumOfX += x;
		sumOfY += y;
		sumOfXSq += x * x;
		sumOfYSq += y * y;
	}
	ssX = sumOfXSq - ((sumOfX * sumOfX) / count);
	sCo = sumCodeviates - ((sumOfX * sumOfY) / count);

	float meanX = sumOfX / count;
	float meanY = sumOfY / count;

	float yintercept = meanY - ((sCo / ssX) * meanX);
	float slope = sCo / ssX;
	
	return slope * values.size() + yintercept;
}

vec3 linearRegression(const Vector<vec3> &values)
{
	if (values.size() <= 0)
		return vec3_zero;

	float sumOfX = 0;
	vec3 sumOfY;
	float sumOfXSq = 0;
	vec3 sumOfYSq;
	vec3 sumCodeviates;
	float count = itof(values.size());

	for (int ctr = 0; ctr < count; ctr++)
	{
		float x = itof(ctr);
		vec3 y = values[ctr];
		sumCodeviates += y * x;
		sumOfX += x;
		sumOfY += y;
		sumOfXSq += x * x;
		sumOfYSq += y * y;
	}

	float ssX = sumOfXSq - ((sumOfX * sumOfX) / count);
	vec3 ssY = sumOfYSq - ((sumOfY * sumOfY) / count);
	vec3 sCo = sumCodeviates - ((sumOfY * sumOfX) / count);

	float meanX = sumOfX / count;
	vec3 meanY = sumOfY / count;

	vec3 yintercept = meanY - ((sCo / ssX) * meanX);
	vec3 slope = sCo / ssX;

	return slope * itof(values.size()) + yintercept;
}

vec3 getDirectionX(const NodePtr &node)
{
	mat4 mat = mat4(node->getTransform());
	return vec3(mat.m00, mat.m10, mat.m20); // column3(0)
}

vec3 getDirectionY(const NodePtr &node)
{
	mat4 mat = mat4(node->getTransform());
	return vec3(mat.m01, mat.m11, mat.m21); // column3(1)
}

vec3 getDirectionZ(const NodePtr &node)
{
	mat4 mat = mat4(node->getTransform());
	return vec3(mat.m02, mat.m12, mat.m22); // column3(2)
}

vec3 getDirectionNX(const NodePtr &node) { return -getDirectionX(node); }
vec3 getDirectionNY(const NodePtr &node) { return -getDirectionY(node); }
vec3 getDirectionNZ(const NodePtr &node) { return -getDirectionZ(node); }

vec3 getWorldDirectionX(const NodePtr &node)
{
	mat4 mat = mat4(node->getWorldTransform());
	return vec3(mat.m00, mat.m10, mat.m20); // column3(0)
}

vec3 getWorldDirectionY(const NodePtr &node)
{
	mat4 mat = mat4(node->getWorldTransform());
	return vec3(mat.m01, mat.m11, mat.m21); // column3(1)
}

vec3 getWorldDirectionZ(const NodePtr &node)
{
	mat4 mat = mat4(node->getWorldTransform());
	return vec3(mat.m02, mat.m12, mat.m22); // column3(2)
}

vec3 getWorldDirectionNX(const NodePtr &node) { return -getWorldDirectionX(node); }
vec3 getWorldDirectionNY(const NodePtr &node) { return -getWorldDirectionY(node); }
vec3 getWorldDirectionNZ(const NodePtr &node) { return -getWorldDirectionZ(node); }

void setDirection(const NodePtr &node, bool local, const vec3 &dir, const vec3 &up, int ret0, int v00, int v01, int ret1, int v10, int v11)
{
	mat3 rotate, scale;
	mat3 rotation = mat3(local ? node->getTransform() : node->getWorldTransform());
	orthonormalize(rotate, rotation);
	mul(scale, transpose(rotate), rotation);
	vec3 x, y, z = normalize(dir); 
	x = y = z;
	cross(ret0 == X ? x : (ret0 == Y ? y : z), v00 == X ? x : (v00 == Y ? y : (v00 == Z ? z : up)), v01 == X ? x : (v01 == Y ? y : (v01 == Z ? z : up))).normalize();
	cross(ret1 == X ? x : (ret1 == Y ? y : z), v10 == X ? x : (v10 == Y ? y : (v10 == Z ? z : up)), v11 == X ? x : (v11 == Y ? y : (v11 == Z ? z : up))).normalize();
	rotate.setColumn(0, x);
	rotate.setColumn(1, y);
	rotate.setColumn(2, z);
	if (vec3(scale.m00, scale.m11, scale.m22) == vec3_one)
		scale = mat3_identity;
	if (local)
		node->setTransform(Mat4(mul(rotation, rotate, scale), node->getTransform().getColumn3(3)));
	else
		node->setWorldTransform(Mat4(mul(rotation, rotate, scale), node->getWorldTransform().getColumn3(3)));
}

void setDirectionX(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, true, dir, up, Y, UP, X, Z, X, Y); }
void setDirectionY(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, true, dir, up, X, Y, UP, Z, X, Y); }
void setDirectionZ(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, true, dir, up, X, UP, Z, Y, Z, X); }
void setDirectionNX(const NodePtr &node, const vec3 &dir, const vec3 &up) {	setDirection(node, true, -dir, up, Y, UP, X, Z, X, Y); }
void setDirectionNY(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, true, -dir, up, X, Y, UP, Z, X, Y); }
void setDirectionNZ(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, true, -dir, up, X, UP, Z, Y, Z, X); }
void setWorldDirectionX(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, false, dir, up, Y, UP, X, Z, X, Y); }
void setWorldDirectionY(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, false, dir, up, X, Y, UP, Z, X, Y); }
void setWorldDirectionZ(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, false, dir, up, X, UP, Z, Y, Z, X); }
void setWorldDirectionNX(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, false, -dir, up, Y, UP, X, Z, X, Y); }
void setWorldDirectionNY(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, false, -dir, up, X, Y, UP, Z, X, Y); }
void setWorldDirectionNZ(const NodePtr &node, const vec3 &dir, const vec3 &up) { setDirection(node, false, -dir, up, X, UP, Z, Y, Z, X); }

void visualizeAxis(const Mat4 &mat, float scale)
{
	if (Console::getInt("show_visualizer") == 0)
		Console::run("show_visualizer 1");

	Vec3 p = mat.getTranslate();
	Visualizer::renderLine3D(p, p + Vec3(mat.m00, mat.m10, mat.m20) * scale, vec4(1, 0, 0, 1));
	Visualizer::renderLine3D(p, p + Vec3(mat.m01, mat.m11, mat.m21) * scale, vec4(0, 1, 0, 1));
	Visualizer::renderLine3D(p, p + Vec3(mat.m02, mat.m12, mat.m22) * scale, vec4(0, 0, 1, 1));
}

float loop01(float val)
{
	if (val >= 0)
		return mod(val, 1);
	else
		return 1 - mod(-val, 1);
}

float lerp_clamped(float v0, float v1, float k)
{
	return lerp(v0, v1, clamp(k, 0.0f, 1.0f));
}

float resize(float v, float from0, float from1, float to0, float to1)
{
	v = (v - from0) / (from1 - from0); // normalize
	v = to0 + (to1 - to0) * v; // resize!
	return v;
}

float getAngle(const quat &source, const quat &target)
{
	quat res = target * inverse(source);
	float angle = Math::acos(res.w) * 2.0f * Math::Consts::RAD2DEG;
	if (angle > 180) angle = 360 - angle;
	if (isnan(angle)) angle = 0;
	return angle;
}

float getAngle(float source_angle, float target_angle)
{
	return mod(target_angle - source_angle + 180.0f + 360.0f, 360.0f) - 180.0f;
}

float rotateTowards01(float a, float b, float k)
{
	if (Math::abs(b - a) < k) return b;
	if ((a > b) && (a - b > 0.5f))
	{
		float c = a + sign(b + 1.0f - a) * k;
		if (c >= 1.0f) return c - 1.0f;
		return c;
	}
	if ((b > a) && (b - a > 0.5f))
	{
		float c = a + sign(b - 1.0f - a) * k;
		if (c < 0.0f) return c + 1.0f;
		return c;
	}
	return a + sign(b - a) * k;
}

float rotateTowards(float a, float b, float k)
{
	if (Math::abs(b - a) < k) return b;
	if ((a > b) && (a - b > 180.0f))
	{
		float c = a + sign(b + 360.0f - a) * k;
		if (c >= 360.0f) return c - 360.0f;
		return c;
	}
	if ((b > a) && (b - a > 180.0f))
	{
		float c = a + sign(b - 360.0f - a) * k;
		if (c < 0.0f) return c + 360.0f;
		return c;
	}
	return a + sign(b - a) * k;
}

vec3 reflectPlane(const vec3 &vector, const vec3 &normal)
{
	return vector - (vector * normal) * 2.0f * normal;
}

void logShowBones(ObjectMeshSkinnedPtr &skin)
{
	int bonesCount = skin->getNumBones();
	Log::message("OBJECT \"%s\", BONES COUNT: %d\n", skin->getName(), bonesCount);
	for (int i = 0; i < bonesCount; i++)
	{
		Log::message("%d) %s\n", i, skin->getBoneName(i));
	}
}

void visualizeBones(ObjectMeshSkinnedPtr &skin, float scale)
{
	if (Console::getInt("show_visualizer") == 0)
		Console::run("show_visualizer 1");

	for (int i = 0; i < skin->getNumBones(); i++)
	{
		Mat4 mat = skin->getBoneWorldTransform(i);
		Vec3 p = mat.getTranslate();
		Visualizer::renderLine3D(p, p + Vec3(mat.m00, mat.m10, mat.m20) * scale, vec4(1, 0, 0, 1));
		Visualizer::renderLine3D(p, p + Vec3(mat.m01, mat.m11, mat.m21) * scale, vec4(0, 1, 0, 1));
		Visualizer::renderLine3D(p, p + Vec3(mat.m02, mat.m12, mat.m22) * scale, vec4(0, 0, 1, 1));
		Visualizer::renderMessage3D(p, vec3(0, 0, 0), skin->getBoneName(i), vec4(1, 1, 1, 1), 1);
	}
}

void resetRotation(mat4 &transform)
{
	transform.setColumn3(0, vec3(1, 0, 0));
	transform.setColumn3(1, vec3(0, 1, 0));
	transform.setColumn3(2, vec3(0, 0, 1));
}

void setTransformPosition(Mat4 &transform, const Vec3 &pos)
{
	transform.setColumn3(3, pos);
}

void setTransformRotation(Mat4 &transform, const quat &rot)
{
	mat3 rotMat = rot.getMat3();
	transform.setColumn3(0, Vec3(rotMat.getColumn(0)));
	transform.setColumn3(1, Vec3(rotMat.getColumn(1)));
	transform.setColumn3(2, Vec3(rotMat.getColumn(2)));
}

vec3 getBonePosition(ObjectMeshSkinnedPtr &skin, int boneNum)
{
	return skin->getBoneTransform(boneNum).getTranslate();
}

void setBonePosition(ObjectMeshSkinnedPtr &skin, int boneNum, const vec3 &pos)
{
	mat4 transform = skin->getBoneTransform(boneNum);
	transform.setColumn3(3, pos);
	skin->setBoneTransformWithChildren(boneNum, transform);
}

quat getBoneRotation(ObjectMeshSkinnedPtr &skin, int boneNum)
{
	return skin->getBoneTransform(boneNum).getRotate();
}

void setBoneRotation(ObjectMeshSkinnedPtr &skin, int boneNum, const quat &rot)
{
	mat4 transform = skin->getBoneTransform(boneNum);
	mat3 rotMat = rot.getMat3();
	transform.setColumn3(0, rotMat.getColumn(0));
	transform.setColumn3(1, rotMat.getColumn(1));
	transform.setColumn3(2, rotMat.getColumn(2));
	skin->setBoneTransformWithChildren(boneNum, transform);
}

void setBoneRotation(ObjectMeshSkinnedPtr &skin, int boneNum, const quat &rot, const vec3 &scale)
{
	mat4 transform = skin->getBoneTransform(boneNum);
	mat3 rotMat = rot.getMat3();
	transform.setColumn3(0, rotMat.getColumn(0));
	transform.setColumn3(1, rotMat.getColumn(1));
	transform.setColumn3(2, rotMat.getColumn(2));
	transform *= Math::scale(scale);
	skin->setBoneTransformWithChildren(boneNum, transform);
}

void addLineSegment(const ObjectMeshDynamicPtr &mesh, const vec3 &from, const vec3 &to, const vec3 &from_right, const vec3 &to_right, float width)
{
	mesh->addTriangleQuads(1);
	vec3 p0 = from - from_right * width * 0.5f;	// 0, 0
	vec3 p1 = from + from_right * width * 0.5f;  // 1, 0
	vec3 p2 = to + to_right * width * 0.5f;	// 1, 1
	vec3 p3 = to - to_right * width * 0.5f;	// 0, 1
	mesh->addVertex(p0); mesh->addTexCoord(vec4(0, 0, 0, 0));
	mesh->addVertex(p1); mesh->addTexCoord(vec4(1, 0, 0, 0));
	mesh->addVertex(p2); mesh->addTexCoord(vec4(1, 1, 0, 0));
	mesh->addVertex(p3); mesh->addTexCoord(vec4(0, 1, 0, 0));
}

void addLineSegment(const ObjectMeshDynamicPtr &mesh, const vec3 &from, const vec3 &to, const vec3 &from_forward, float width)
{
	vec3 up = vec3_up;
	vec3 to_forward = (to - from).normalize();
	vec3 to_right = normalize(cross(to_forward, up));
	vec3 from_right = normalize(cross(from_forward, up));
	addLineSegment(mesh, from, to, from_right, to_right, width);
}

void addLineSegment(const ObjectMeshDynamicPtr &mesh, const vec3 &from, const vec3 &to, float width)
{
	vec3 from_forward = (to - from).normalize();
	addLineSegment(mesh, from, to, from_forward, width);
}

Vec3 getHermiteSpline(const Vec3 &p0, const Vec3 &p1, const Vec3 &p2, const Vec3 &p3, float t)
{
	float t2 = t * t;
	float t3 = t2 * t;

	float tension = 0.5f;	// 0.5 equivale a catmull-rom

	Vec3 pp1 = (p2 - p0) * tension;
	Vec3 pp2 = (p3 - p1) * tension;

	float blend1 = 2.0f * t3 - 3.0f * t2 + 1.0f;
	float blend2 = -2.0f * t3 + 3.0f * t2;
	float blend3 = t3 - 2.0f * t2 + t;
	float blend4 = t3 - t2;

	return p1 * blend1 + p2 * blend2 + pp1 * blend3 + pp2 * blend4;
}

bool isInsideBoundBox(const vec3 &point, const vec3 &bound_min, const vec3 &bound_max)
{
	if (point.x < bound_min.x || point.y < bound_min.y || point.z < bound_min.z ||
		point.x > bound_max.x || point.y > bound_max.y || point.z > bound_max.z)
		return false;
	else
		return true;
}

vec3 getOffsetToBoundBox(const vec3 &point, const vec3 &bound_min, const vec3 &bound_max)
{
	vec3 result = vec3_zero;
	if (point.x > bound_max.x) result.x = bound_max.x - point.x;
	else if (point.x < bound_min.x) result.x = bound_min.x - point.x;

	if (point.y > bound_max.y) result.y = bound_max.y - point.y;
	else if (point.y < bound_min.y) result.y = bound_min.y - point.y;

	if (point.z > bound_max.z) result.z = bound_max.z - point.z;
	else if (point.z < bound_min.z) result.z = bound_min.z - point.z;
	
	return result;
}

vec3 getDirectionX(const mat4 &mat)
{
	return vec3(mat.m00, mat.m10, mat.m20); // column3(0)
}

vec3 getDirectionY(const mat4 &mat)
{
	return vec3(mat.m01, mat.m11, mat.m21); // column3(1)
}

vec3 getDirectionZ(const mat4 &mat)
{
	return vec3(mat.m02, mat.m12, mat.m22); // column3(2)
}

vec3 getDirectionNX(const mat4 &mat) { return -getDirectionX(mat); }
vec3 getDirectionNY(const mat4 &mat) { return -getDirectionY(mat); }
vec3 getDirectionNZ(const mat4 &mat) { return -getDirectionZ(mat); }

void setDirection(mat4 &mat, const vec3 &dir, const vec3 &up, int ret0, int v00, int v01, int ret1, int v10, int v11)
{
	mat3 rotate, scale;
	mat3 rotation = mat3(mat);
	orthonormalize(rotate, rotation);
	mul(scale, transpose(rotate), rotation);
	vec3 x, y, z = normalize(dir);
	x = y = z;
	cross(ret0 == X ? x : (ret0 == Y ? y : z), v00 == X ? x : (v00 == Y ? y : (v00 == Z ? z : up)), v01 == X ? x : (v01 == Y ? y : (v01 == Z ? z : up))).normalize();
	cross(ret1 == X ? x : (ret1 == Y ? y : z), v10 == X ? x : (v10 == Y ? y : (v10 == Z ? z : up)), v11 == X ? x : (v11 == Y ? y : (v11 == Z ? z : up))).normalize();
	rotate.setColumn(0, x);
	rotate.setColumn(1, y);
	rotate.setColumn(2, z);
	if (vec3(scale.m00, scale.m11, scale.m22) == vec3_one)
		scale = mat3_identity;

	mat = mat4(mul(rotation, rotate, scale), mat.getColumn3(3));
}

void setDirectionX(mat4 &mat, const vec3 &dir, const vec3 &up) { setDirection(mat, dir, up, Y, UP, X, Z, X, Y); }
void setDirectionY(mat4 &mat, const vec3 &dir, const vec3 &up) { setDirection(mat, dir, up, X, Y, UP, Z, X, Y); }
void setDirectionZ(mat4 &mat, const vec3 &dir, const vec3 &up) { setDirection(mat, dir, up, X, UP, Z, Y, Z, X); }
void setDirectionNX(mat4 &mat, const vec3 &dir, const vec3 &up) { setDirection(mat, -dir, up, Y, UP, X, Z, X, Y); }
void setDirectionNY(mat4 &mat, const vec3 &dir, const vec3 &up) { setDirection(mat, -dir, up, X, Y, UP, Z, X, Y); }
void setDirectionNZ(mat4 &mat, const vec3 &dir, const vec3 &up) { setDirection(mat, -dir, up, X, UP, Z, Y, Z, X); }