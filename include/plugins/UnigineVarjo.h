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
#include <UnigineMathLib.h>
#include <UnigineVector.h>
#include <UniginePlayers.h>

namespace Unigine
{
namespace Plugins
{


class Varjo
{
protected:
	virtual ~Varjo(){}
public:
	UNIGINE_INLINE static Varjo *get() { return Unigine::Engine::get()->getPlugin<Varjo>("Varjo"); }

	enum HMD_TYPE
	{
		HMD_TYPE_VR_1,
		HMD_TYPE_XR_1,
		HMD_TYPE_VR_2,
		HMD_TYPE_VR_2_PRO,
		HMD_TYPE_VR_3,
		HMD_TYPE_XR_3,
		HMD_TYPE_AERO,
		HMD_TYPE_UNKNOWN,
	};
	virtual String getHMDName() const = 0;
	virtual Varjo::HMD_TYPE getHMDType() const = 0;
	virtual Math::Vec3 getHandtrackingOffset() const = 0;
	virtual void setHeadPositionLock(bool lock) = 0;
	virtual bool isHeadPositionLock() const = 0;
	virtual void setHeadRotationLock(bool lock) = 0;
	virtual bool isHeadRotationLock() const = 0;
	virtual int getMaxTrackedDeviceCount() const = 0;
	virtual int getControllerStateAxisCount() const = 0;

	enum DEVICE
	{
		DEVICE_INVALID,
		DEVICE_HMD,
		DEVICE_CONTROLLER,
		DEVICE_GENERIC_TRACKER,
		DEVICE_TRACKING,
	};
	virtual Math::mat4 getDevicePose(int device_num) = 0;
	virtual Math::vec3 getDeviceVelocity(int device_num) = 0;
	virtual Math::vec3 getDeviceAngularVelocity(int device_num) = 0;
	virtual bool isDeviceConnected(int device_num) = 0;
	virtual bool isPoseValid(int device_num) = 0;
	virtual int getTrackingResult(int device_num) = 0;

	enum AXIS
	{
		AXIS_NONE,
		AXIS_TRACKPAD,
		AXIS_JOYSTICK,
		AXIS_TRIGGER,
	};

	enum BUTTON
	{
		BUTTON_SYSTEM = 0,
		BUTTON_APPLICATIONMENU = 1,
		BUTTON_GRIP = 2,
		BUTTON_DPAD_LEFT = 3,
		BUTTON_DPAD_UP = 4,
		BUTTON_DPAD_RIGHT = 5,
		BUTTON_DPAD_DOWN = 6,
		BUTTON_A = 7,
		BUTTON_AXIS0 = 32,
		BUTTON_AXIS1 = 33,
		BUTTON_AXIS2 = 34,
		BUTTON_AXIS3 = 35,
		BUTTON_AXIS4 = 36,
		BUTTON_STEAMVR_TOUCHPAD = 32,
		BUTTON_STEAMVR_TRIGGER = 33,
		BUTTON_DASHBOARD_BACK = 2,
		BUTTON_MAX = 64,
	};
	virtual Varjo::DEVICE getDeviceType(int device_num) = 0;
	virtual Varjo::AXIS getDeviceAxisType(int device_num, int axis_num) = 0;
	virtual String getDeviceManufacturerName(int device) = 0;
	virtual String getDeviceModelNumber(int device) = 0;

	enum CONTROLLER_ROLE
	{
		CONTROLLER_ROLE_INVALID = 0,
		CONTROLLER_ROLE_LEFT_HAND = 1,
		CONTROLLER_ROLE_RIGHT_HAND = 2,
		CONTROLLER_ROLE_OPT_OUT = 3,
		CONTROLLER_ROLE_TREADMILL = 4,
		CONTROLLER_ROLE_MAX = 5,
	};
	virtual Varjo::CONTROLLER_ROLE getControllerRole(int device_num) = 0;
	virtual int getControllerPacketNum(int device_num) = 0;
	virtual bool getControllerButtonPressed(int device_num, Varjo::BUTTON button) = 0;
	virtual bool getControllerButtonTouched(int device_num, Varjo::BUTTON button) = 0;
	virtual Math::vec2 getControllerAxis(int device_num, int axis_num) = 0;
	virtual void setControllerVibration(int device_num, unsigned short duration) = 0;
	virtual void render(const Ptr<Player> &player, const Math::ivec2 &size, bool render_window) = 0;

	enum VIEWPORT
	{
		VIEWPORT_BLACK_SCREEN,
		VIEWPORT_MONO,
		VIEWPORT_STEREO,
	};
	virtual void setViewportMode(Varjo::VIEWPORT mode) = 0;
	virtual Varjo::VIEWPORT getViewportMode() const = 0;
	virtual void setFocusViewportSupersamplingFactor(float factor) = 0;
	virtual float getFocusViewportSupersamplingFactor() const = 0;
	virtual void setFoveatedRenderingEnabled(bool enabled) = 0;
	virtual bool isFoveatedRenderingEnabled() const = 0;
	virtual bool isMixedRealityAvailable() const = 0;
	virtual void setMarkerTrackingEnabled(bool enabled) = 0;
	virtual bool isMarkerTrackingEnabled() const = 0;
	virtual void setAlphaBlend(bool blend) = 0;
	virtual bool isAlphaBlend() const = 0;
	virtual void setAlphaInvert(bool invert) = 0;
	virtual bool isAlphaInvert() const = 0;
	virtual void setVideo(bool video) = 0;
	virtual bool isVideo() const = 0;
	virtual void setDepthTest(bool test) = 0;
	virtual bool isDepthTest() const = 0;
	virtual void setDepthTestRangeEnabled(bool enabled) = 0;
	virtual bool isDepthTestRangeEnabled() const = 0;

	enum ENVIRONMENT_MODE
	{
		ENVIRONMENT_MODE_PRESET = 0,
		ENVIRONMENT_MODE_OVERLAP = 1,
	};
	virtual void setStreamColorCorrectionEnabled(bool enabled) = 0;
	virtual bool isStreamColorCorrectionEnabled() const = 0;
	virtual void setStreamColorCorrectionWhiteBalanceEnabled(bool enabled) = 0;
	virtual bool isStreamColorCorrectionWhiteBalanceEnabled() const = 0;
	virtual void setStreamEnvironmentCubemapEnabled(bool enabled) = 0;
	virtual bool isStreamEnvironmentCubemapEnabled() const = 0;
	virtual void setStreamEnvironmentCubemapGGXQuality(float quality) = 0;
	virtual float getStreamEnvironmentCubemapGGXQuality() const = 0;
	virtual void setDepthTestRange(const Math::dvec2 &range) = 0;
	virtual Math::dvec2 getDepthTestRange() const = 0;
	virtual void setViewOffset(double offset) = 0;
	virtual double getViewOffset() const = 0;
	virtual void setStreamEnvironmentCubemapMode(Varjo::ENVIRONMENT_MODE mode) = 0;
	virtual Varjo::ENVIRONMENT_MODE getStreamEnvironmentCubemapMode() const = 0;
	virtual void setStreamEnvironmentCubemapPresetIndex(int index) = 0;
	virtual int getStreamEnvironmentCubemapPresetIndex() const = 0;
	virtual void setMotionPrediction(bool prediction) = 0;
	virtual bool isMotionPrediction() const = 0;
	virtual void setMotionPredictionVelocityPrecision(float precision) = 0;
	virtual float getMotionPredictionVelocityPrecision() const = 0;
	virtual void setMotionPredictionVelocityTimeDelta(float delta) = 0;
	virtual float getMotionPredictionVelocityTimeDelta() const = 0;

	enum BLEND_MASKING_MODE
	{
		BLEND_MASKING_MODE_DISABLED = 0,
		BLEND_MASKING_MODE_RESTRICT_VIDEO_TO_MASK = 1,
		BLEND_MASKING_MODE_RESTRICT_VR_TO_MASK = 2,
		BLEND_MASKING_MODE_RESTRICT_VR_TO_CHROMAKEY_REDUCED_BY_MASK = 3,
	};
	virtual void setBlendMaskingMode(Varjo::BLEND_MASKING_MODE mode) = 0;
	virtual Varjo::BLEND_MASKING_MODE getBlendMaskingMode() const = 0;
	virtual void setBlendMaskingDebugEnabled(bool enabled) = 0;
	virtual bool isBlendMaskingDebugEnabled() const = 0;

	enum MARKER_FLAGS
	{
		MARKER_FLAGS_DO_PREDICTION = 0x1,
	};

	enum MARKER_POSE_FLAGS
	{
		MARKER_POSE_FLAGS_TRACING_OK = 0x1,
		MARKER_POSE_FLAGS_TRACING_LOST = 0x2,
		MARKER_POSE_FLAGS_TRACING_DISCONNECTED = 0x4,
		MARKER_POSE_FLAGS_HAS_POSITION = 0x8,
		MARKER_POSE_FLAGS_HAS_ROTATION = 0x10,
		MARKER_POSE_FLAGS_HAS_VELOCITY = 0x20,
		MARKER_POSE_FLAGS_HAS_ANGULAR_VELOCITY = 0x40,
		MARKER_POSE_FLAGS_HAS_ACCELERATION = 0x80,
		MARKER_POSE_FLAGS_HAS_CONFIDENCE = 0x100,
	};
	virtual void setMarkerLifetime(float lifetime, Vector<unsigned long long> &marker_ids) = 0;
	virtual void setMarkerLifetime(float lifetime, unsigned long long marker_id) = 0;
	virtual void setMarkerFlags(Varjo::MARKER_FLAGS flags, Vector<unsigned long long> &marker_ids) = 0;
	virtual void setMarkerFlags(Varjo::MARKER_FLAGS flags, unsigned long long marker_id) = 0;

	struct MarkerObject
	{
		Math::Mat4 transform;
		Math::Vec3 size;
		Math::Vec3 velocity;
		Math::Vec3 angular_velocity;
		Math::Vec3 acceleration;
		int64_t timestamp; // In Nanoseconds
		double confidence;
		MARKER_POSE_FLAGS pose_flags;
		MARKER_FLAGS flags;
		int id;
	};

	virtual Varjo::MarkerObject getMarkerObject(int index) const = 0;
	virtual int getMarkerObjectNum() const = 0;

	enum CHROMAKEY_TYPE
	{
		CHROMAKEY_TYPE_DISABLED = 0,
		CHROMAKEY_TYPE_HSV = 1,
	};
	virtual void setChromaKey(bool key) = 0;
	virtual bool isChromaKey() const = 0;
	virtual int getChromaKeyConfigNum() const = 0;
	virtual void chromaKeyConfigSubmit(int index) const = 0;
	virtual Varjo::CHROMAKEY_TYPE getChromaKeyConfigType(int index) const = 0;
	virtual void setChromaKeyConfigType(int index, Varjo::CHROMAKEY_TYPE type) = 0;
	virtual Math::vec3 getChromaKeyConfigFalloff(int index) const = 0;
	virtual void setChromaKeyConfigFalloff(int index, const Math::vec3 &falloff) = 0;
	virtual Math::vec3 getChromaKeyConfigTargetColor(int index) const = 0;
	virtual void setChromaKeyConfigTargetColor(int index, const Math::vec3 &target_color) = 0;
	virtual Math::vec3 getChromaKeyConfigTolerance(int index) const = 0;
	virtual void setChromaKeyConfigTolerance(int index, const Math::vec3 &tolerance) = 0;
	virtual void setCameraExposureTime(double time) = 0;
	virtual double getCameraExposureTime() const = 0;
	virtual void setCameraExposureTimeAuto() const = 0;
	virtual void setCameraExposureTimeManual() const = 0;
	virtual bool isCameraExposureTimeAuto() const = 0;
	virtual void setCameraWhiteBalance(int balance) = 0;
	virtual int getCameraWhiteBalance() const = 0;
	virtual void setCameraWhiteBalanceAuto() const = 0;
	virtual void setCameraWhiteBalanceManual() const = 0;
	virtual bool isCameraWhiteBalanceAuto() const = 0;
	virtual void setCameraISO(int cameraiso) = 0;
	virtual int getCameraISO() const = 0;
	virtual void setCameraISOAuto() const = 0;
	virtual void setCameraISOManual() const = 0;
	virtual bool isCameraISOAuto() const = 0;
	virtual void setCameraFlickerCompensation(int compensation) = 0;
	virtual int getCameraFlickerCompensation() const = 0;
	virtual void setCameraSharpness(int sharpness) = 0;
	virtual int getCameraSharpness() const = 0;

	enum EYETRACKING_STATUS
	{
		EYETRACKING_STATUS_INVALID,
		EYETRACKING_STATUS_ADJUST,
		EYETRACKING_STATUS_VALID,
	};

	enum EYE_STATUS
	{
		EYE_STATUS_INVALID,
		EYE_STATUS_VISIBLE,
		EYE_STATUS_COMPENSATED,
		EYE_STATUS_TRACKED,
	};
	virtual bool isEyeTrackingValid() const = 0;
	virtual Math::Vec3 getFocusWorldPosition() const = 0;
	virtual void requestCalibration() = 0;
	virtual Math::Vec3 getLeftEyeWorldPosition() const = 0;
	virtual Math::Vec3 getLeftEyeWorldDirection() const = 0;
	virtual Math::Vec3 getRightEyeWorldPosition() const = 0;
	virtual Math::Vec3 getRightEyeWorldDirection() const = 0;
	virtual Math::Vec3 getGazeWorldDirection() const = 0;
	virtual double getLeftEyePupilSize() const = 0;
	virtual double getRightEyePupilSize() const = 0;
	virtual long long getCaptureTime() const = 0;
	virtual double getFocusDistance() const = 0;
	virtual double getStability() const = 0;
	virtual Varjo::EYETRACKING_STATUS getEyeTrackingStatus() const = 0;
	virtual Varjo::EYE_STATUS getLeftEyeStatus() const = 0;
	virtual Varjo::EYE_STATUS getRightEyeStatus() const = 0;
	virtual long long getFrameNum() const = 0;
	virtual void setVisualizerEnabled(bool enabled) = 0;
	virtual bool isVisualizerEnabled() const = 0;
};

} // namespace Plugins
} // namespace Unigine
