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


#pragma once

#include <UnigineMathLib.h>
#include <UnigineImage.h>
#include <UnigineMap.h>
#include <UnigineVector.h>
#include <UnigineEngine.h>

namespace Unigine
{
namespace Plugins
{
class LeapMotion
{
public:
	UNIGINE_INLINE static LeapMotion *get() { return Unigine::Engine::get()->getPlugin<LeapMotion>("LeapMotion"); }

	struct Bone
	{
		enum
		{
			TYPE_METACARPAL = 0,	// Bone connected to the wrist inside the palm
			TYPE_PROXIMAL = 1,		// Bone connecting to the palm 
			TYPE_INTERMEDIATE = 2,	// Bone between the tip and the base
			TYPE_DISTAL = 3,		// Bone at the tip of the finger 
			BONE_NUM_TYPES
		};

		int type;
		float length;
		float width;
		int is_valid;
		Unigine::Math::vec3 prev_joint;
		Unigine::Math::vec3 next_joint;
		Unigine::Math::vec3 center;
		Unigine::Math::vec3 direction;
		Unigine::Math::mat4 basis;
	};

	struct Finger
	{
		enum
		{
			TYPE_THUMB = 0,	 // The thumb 
			TYPE_INDEX = 1,	 // The index or fore-finger 
			TYPE_MIDDLE = 2, // The middle finger 
			TYPE_RING = 3,	 // The ring finger 
			TYPE_PINKY = 4,	 // The pinky or little finger 
			FINGER_NUM_TYPES,
		};

		int id;
		int type;
		float length;
		float width;
		int is_extended;
		int is_valid;
		Unigine::Math::vec3 tip_position;
		Unigine::Math::vec3 tip_velocity;
		Unigine::Math::vec3 direction;
		Unigine::Math::vec3 stabilized_tip_position;

		Bone bones[Bone::BONE_NUM_TYPES];
	};

	struct Arm
	{
		float width;
		int is_valid;
		Unigine::Math::vec3 direction;
		Unigine::Math::vec3 elbow_position;
		Unigine::Math::vec3 wrist_position;
		Unigine::Math::vec3 center;
		Unigine::Math::mat4 basis;
	};

	struct Hand
	{
		int id;
		// 0 - left, 1 - right
		int type;
		int is_valid;
		float palm_width;
		Unigine::Math::vec3 palm_position;
		Unigine::Math::vec3 palm_stabilized_position;
		Unigine::Math::vec3 palm_velocity;
		Unigine::Math::vec3 palm_normal;
		Unigine::Math::vec3 direction;
		Unigine::Math::vec3 wrist_position;
		Unigine::Math::mat4 basis;

		Arm arm;
		Unigine::Map<int, Finger> fingers;
	};

	enum POLICY
	{
		POLICY_DEFAULT = 0, // The default policy.
		POLICY_BACKGROUND_FRAMES = (1 << 0), // Receive background frames.
		POLICY_IMAGES = (1 << 1), // Receive images from sensor cameras.
		POLICY_OPTIMIZE_HMD = (1 << 2), // Optimize the tracking for head-mounted device.
		POLICY_ALLOW_PAUSE_RESUME = (1 << 3), // Allow pausing and unpausing of the Leap Motion service.
		POLICY_RAW_IMAGES = (1 << 6), // Receive raw images.
	};

	LeapMotion() = default;
	virtual ~LeapMotion() = default;

	virtual bool isConnected() const = 0;
	virtual bool isServiceConnected() const = 0;
	virtual bool isSmudged() const = 0;
	virtual bool isLightingBad() const = 0;

	virtual bool hasFocus() const = 0;

	virtual void setPolicy(POLICY policy) = 0;
	virtual bool isPolicySet(POLICY policy) const = 0;
	virtual void clearPolicy(POLICY policy) = 0;

	virtual void setPaused(bool pause) = 0;
	virtual bool isPaused() const = 0;

	virtual float getHorizontalViewAngle() const = 0;
	virtual float getVerticalViewAngle() const = 0;
	virtual float getRange() const = 0;
	virtual float getBaseline() const = 0;
	virtual float getFPS() const = 0;
	virtual float getDistanceToBoundary(const Unigine::Math::vec3 &position) const = 0;

	// R8 images from infrared cameras
	virtual const Unigine::Vector<Unigine::ImagePtr> &getImages() const = 0;
	virtual int getNumImages() const = 0;
	virtual Unigine::ImagePtr getImage(int num) const = 0;

	virtual const Unigine::Map<int, Hand> &getHands() const = 0;
	virtual void saveHandsToStream(const Unigine::StreamPtr &dest_stream) const = 0;
};
} // namespace Plugins
} // namespace Unigine