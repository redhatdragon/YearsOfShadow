/* Copyright (C) 2005-2022, UNIGINE. All rights reserved.
 *
 * This file is a part of the UNIGINE 2 SDK.
 *
 * Your use and / or redistribution of this software in source and / or
 * binary form, with or without modification, is subject to: (i) your
 * ongoing acceptance of and compliance with the terms and conditions of
 * the UNIGINE License Agreement; and (ii) your inclusion of this notice
 * in any version of this software that you use or redistribute.
 * A copy of the UNIGINE License Agreement is available by contacting
 * UNIGINE. at http://unigine.com/
 */

// DO NOT EDIT DIRECTLY. This is an auto-generated file. Your changes will be lost.

#pragma once

#include <UnigineEngine.h>

namespace Unigine
{
namespace Plugins
{


class Kinect
{
protected:
	virtual ~Kinect(){}
public:
	UNIGINE_INLINE static Kinect *get() { return Unigine::Engine::get()->getPlugin<Kinect>("Kinect"); }

	enum STREAM
	{
		STREAM_COLOR = 1,
		STREAM_INFRARED = 2,
		STREAM_DEPTH = 8,
		STREAM_BODY = 32,
		STREAM_ALL = 43,
	};

	enum NUM
	{
		NUM_BODIES = 6,
		NUM_BONES = 25,
		NUM_FACE_POINTS = 5,
		NUM_FACE_PROPERTIES = 8,
	};

	enum TRACKING_STATE
	{
		TRACKING_STATE_NOT_TRACKED = 0,
		TRACKING_STATE_INFERRED = 1,
		TRACKING_STATE_TRACKED = 2,
	};

	enum TRACKING_CONFIDENCE
	{
		TRACKING_CONFIDENCE_LOW = 0,
		TRACKING_CONFIDENCE_HIGH = 1,
	};

	enum HAND_STATE
	{
		HAND_STATE_UNKNOWN = 0,
		HAND_STATE_NOT_TRACKED = 1,
		HAND_STATE_OPEN = 2,
		HAND_STATE_CLOSED = 3,
		HAND_STATE_LASSO = 4,
	};

	enum BONE
	{
		BONE_HEAD = 3,
		BONE_NECK = 2,
		BONE_SPINE_SHOULDER = 20,
		BONE_SPINE_MID = 1,
		BONE_SPINE_BASE = 0,
		BONE_SHOULDER_LEFT = 4,
		BONE_ELBOW_LEFT = 5,
		BONE_WRIST_LEFT = 6,
		BONE_HAND_LEFT = 7,
		BONE_HAND_TIP_LEFT = 21,
		BONE_THUMB_LEFT = 22,
		BONE_SHOULDER_RIGHT = 8,
		BONE_ELBOW_RIGHT = 9,
		BONE_WRIST_RIGHT = 10,
		BONE_HAND_RIGHT = 11,
		BONE_HAND_TIP_RIGHT = 23,
		BONE_THUMB_RIGHT = 24,
		BONE_HIP_LEFT = 12,
		BONE_KNEE_LEFT = 13,
		BONE_ANKLE_LEFT = 14,
		BONE_FOOT_LEFT = 15,
		BONE_HIP_RIGHT = 16,
		BONE_KNEE_RIGHT = 17,
		BONE_ANKLE_RIGHT = 18,
		BONE_FOOT_RIGHT = 19,
	};

	enum FACE_DETECTION_RESULT
	{
		FACE_DETECTION_RESULT_UNKNOWN = 0,
		FACE_DETECTION_RESULT_NO = 1,
		FACE_DETECTION_RESULT_MAYBE = 2,
		FACE_DETECTION_RESULT_YES = 3,
	};

	enum FACE_POINT
	{
		FACE_POINT_EYE_LEFT = 0,
		FACE_POINT_EYE_RIGHT = 1,
		FACE_POINT_NOSE = 2,
		FACE_POINT_MOUTH_CORNER_LEFT = 3,
		FACE_POINT_MOUTH_CORNER_RIGHT = 4,
	};

	enum FACE_PROPERTY
	{
		FACE_PROPERTY_HAPPY = 0,
		FACE_PROPERTY_ENGAGED = 1,
		FACE_PROPERTY_GLASSES = 2,
		FACE_PROPERTY_EYE_LEFT_CLOSED = 3,
		FACE_PROPERTY_EYE_RIGHT_CLOSED = 4,
		FACE_PROPERTY_MOUTH_OPEN = 5,
		FACE_PROPERTY_MOUTH_MOVED = 6,
		FACE_PROPERTY_LOOKING_AWAY = 7,
	};
	virtual int init(unsigned int stream_flags) = 0;
	virtual void shutdown() = 0;
	virtual Ptr<Image> getColorBuffer() const = 0;
	virtual Ptr<Image> getDepthBuffer() const = 0;
	virtual Ptr<Image> getInfraredBuffer() const = 0;
	virtual bool isBodyTracked(int body) const = 0;
	virtual Kinect::TRACKING_STATE getBodyLeanState(int body) const = 0;
	virtual Kinect::TRACKING_STATE getBoneState(int body, Kinect::BONE bone) const = 0;
	virtual Math::vec3 getBonePosition(int body, Kinect::BONE bone) const = 0;
	virtual Math::quat getBoneOrientation(int body, Kinect::BONE bone) const = 0;
	virtual Kinect::HAND_STATE getLeftHandState(int body) const = 0;
	virtual Kinect::TRACKING_CONFIDENCE getLeftHandConfidence(int body) const = 0;
	virtual Kinect::HAND_STATE getRightHandState(int body) const = 0;
	virtual Kinect::TRACKING_CONFIDENCE getRightHandConfidence(int body) const = 0;
	virtual bool isFaceTracked(int face) const = 0;
	virtual Math::quat getFaceOrientation(int face) const = 0;
	virtual Math::ivec4 getFaceBoundsInColorSpace(int face) const = 0;
	virtual Math::ivec4 getFaceBoundsInInfraredSpace(int face) const = 0;
	virtual Math::vec3 getFacePointInColorSpace(int face, Kinect::FACE_POINT point) const = 0;
	virtual Math::vec3 getFacePointInInfraredSpace(int face, Kinect::FACE_POINT point) const = 0;
	virtual Kinect::FACE_DETECTION_RESULT getFaceProperty(int face, Kinect::FACE_PROPERTY property) const = 0;
};

} // namespace Plugins
} // namespace Unigine
