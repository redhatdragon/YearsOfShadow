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

#include <UnigineCamera.h>
#include <UnigineEngine.h>
#include <UnigineTextures.h>

namespace Unigine
{
namespace Plugins
{


class UltraleapArm;
class UltraleapHand;
class UltraleapFinger;

class UltraleapBone
{
protected:
	virtual ~UltraleapBone(){}
public:

	enum TYPE
	{
		TYPE_METACARPAL = 0,	// Bone connected to the wrist inside the palm
		TYPE_PROXIMAL,	// Bone connected to the palm
		TYPE_INTERMEDIATE,	// Bone between the tip and the base
		TYPE_DISTAL,	// Bone at the tip of the finger
		NUM_TYPES,
	};
	virtual UltraleapFinger * getFinger() const = 0;
	virtual UltraleapBone::TYPE getType() const = 0;
	virtual double getLength() const = 0;
	virtual double getWidth() const = 0;
	virtual Math::Vec3 getJointBeginPosition() const = 0;
	virtual Math::Vec3 getJointEndPosition() const = 0;
	virtual Math::Vec3 getCenter() const = 0;
	virtual Math::vec3 getDirection() const = 0;
};


class UltraleapFinger
{
protected:
	virtual ~UltraleapFinger(){}
public:

	enum TYPE
	{
		TYPE_THUMB = 0,
		TYPE_INDEX,
		TYPE_MIDDLE,
		TYPE_RING,
		TYPE_PINKY,
		NUM_TYPES,
	};
	virtual UltraleapFinger::TYPE getType() const = 0;
	virtual bool isExtended() const = 0;
	virtual double getLength() const = 0;
	virtual UltraleapHand * getHand() const = 0;
	virtual UltraleapBone * getBoneMetacarpal() const = 0;
	virtual UltraleapBone * getBoneProximal() const = 0;
	virtual UltraleapBone * getBoneIntermediate() const = 0;
	virtual UltraleapBone * getBoneDistal() const = 0;
};


class UltraleapHand
{
protected:
	virtual ~UltraleapHand(){}
public:

	enum TYPE
	{
		TYPE_LEFT = 0,
		TYPE_RIGHT,
		NUM_TYPES,
	};
	virtual UltraleapHand::TYPE getType() const = 0;
	virtual bool isVisible() const = 0;
	virtual long long getVisibleTime() const = 0;
	virtual float getPinchDistance() const = 0;
	virtual float getPinchStrength() const = 0;
	virtual float getGrabStrength() const = 0;
	virtual float getGrabAngle() const = 0;
	virtual double getPalmWidth() const = 0;
	virtual Math::Vec3 getPalmPosition() const = 0;
	virtual Math::Vec3 getPalmStabilizedPosition() const = 0;
	virtual Math::dvec3 getPalmVelocity() const = 0;
	virtual Math::vec3 getPalmNormal() const = 0;
	virtual Math::vec3 getDirection() const = 0;
	virtual UltraleapArm * getArm() const = 0;
	virtual UltraleapFinger * getFingerThumb() const = 0;
	virtual UltraleapFinger * getFingerIndex() const = 0;
	virtual UltraleapFinger * getFingerMiddle() const = 0;
	virtual UltraleapFinger * getFingerRing() const = 0;
	virtual UltraleapFinger * getFingerPinky() const = 0;
};


class UltraleapArm
{
protected:
	virtual ~UltraleapArm(){}
public:
	virtual double getWidth() const = 0;
	virtual Math::vec3 getDirection() const = 0;
	virtual Math::Vec3 getPositionElbow() const = 0;
	virtual Math::Vec3 getPositionWrist() const = 0;
	virtual Math::Vec3 getCenter() const = 0;
	virtual UltraleapHand * getHand() const = 0;
};


class UltraleapDevice
{
protected:
	virtual ~UltraleapDevice(){}
public:

	enum HARDWARE_TYPE
	{
		HARDWARE_TYPE_UNKNOWN = 0x0000,
		HARDWARE_TYPE_PERIPHERAL = 0x0003,
		HARDWARE_TYPE_DRAGONFLY = 0x1102,
		HARDWARE_TYPE_NIGHTCRAWLER = 0x1201,
		HARDWARE_TYPE_RIGEL = 0x1202,
		HARDWARE_TYPE_SIR170 = 0x1203,
		HARDWARE_TYPE__3DI = 0x1204,
		HARDWARE_TYPE_INVALID = -1,
	};

	enum TRANSFORM_MODE
	{
		TRANSFORM_MODE_HMD_VARJO,
		TRANSFORM_MODE_MANUAL,
	};
	virtual bool isStatusConnected() const = 0;
	virtual bool isStatusStreaming() const = 0;
	virtual bool isStatusPaused() const = 0;
	virtual bool isStatusRobust() const = 0;
	virtual bool isStatusSmudged() const = 0;
	virtual bool isStatusLowResource() const = 0;
	virtual bool isStatusUnknownFailure() const = 0;
	virtual bool isStatusBadCalibration() const = 0;
	virtual bool isStatusBadFirmware() const = 0;
	virtual bool isStatusBadTransport() const = 0;
	virtual bool isStatusBadControl() const = 0;
	virtual String getSerial() const = 0;
	virtual float getHFov() const = 0;
	virtual float getVFov() const = 0;
	virtual double getRange() const = 0;
	virtual double getDistanceBetweenCameras() const = 0;
	virtual UltraleapDevice::HARDWARE_TYPE getHardwareType() const = 0;
	virtual unsigned int getLeapID() const = 0;
	virtual bool isSupportedColorImages() const = 0;
	virtual bool isSupportedAccelerometer() const = 0;
	virtual bool isSupportedGyroscope() const = 0;
	virtual bool isSupportedTemperature() const = 0;
	virtual Math::vec3 getAccelerometer() const = 0;
	virtual Math::vec3 getGyroscope() const = 0;
	virtual float getTemperature() const = 0;
	virtual void setTrackingOffset(const Math::Vec3 &offset) = 0;
	virtual Math::Vec3 getTrackingOffset() const = 0;
	virtual float getTrackingFPS() const = 0;
	virtual void setTransform(const Math::Mat4 &transform) = 0;
	virtual Math::Mat4 getTransform() const = 0;
	virtual void setTransformMode(UltraleapDevice::TRANSFORM_MODE mode) = 0;
	virtual UltraleapDevice::TRANSFORM_MODE getTransformMode() const = 0;
	virtual UltraleapHand * getLeftHand() const = 0;
	virtual UltraleapHand * getRightHand() const = 0;
	virtual bool isLeftDistortionReceived() const = 0;
	virtual Ptr<Image> getLeftDistortionImage() const = 0;
	virtual bool isLeftColorReceived() const = 0;
	virtual Ptr<Image> getLeftColorImage() const = 0;
	virtual bool isRightDistortionReceived() const = 0;
	virtual Ptr<Image> getRightDistortionImage() const = 0;
	virtual bool isRightColorReceived() const = 0;
	virtual Ptr<Image> getRightColorImage() const = 0;
};


class Ultraleap
{
protected:
	virtual ~Ultraleap(){}
public:
	UNIGINE_INLINE static Ultraleap *get() { return Unigine::Engine::get()->getPlugin<Ultraleap>("Ultraleap"); }

	enum TRACKING_MODE
	{
		TRACKING_MODE_DESKTOP = 0,
		TRACKING_MODE_HMD = 1,
		TRACKING_MODE_SCREEN_TOP = 2,
		TRACKING_MODE_UNKNOWN = 3,
	};

	enum CONNECTION_STATUS
	{
		CONNECTION_STATUS_NOT_CONNECTED = 0,
		CONNECTION_STATUS_CONNECTED = 1,
		CONNECTION_STATUS_HANDSHAKE_INCOMPLETE = 2,
		CONNECTION_STATUS_NOT_RUNNING = -1,
	};

	enum OPTIMIZE_MODE
	{
		OPTIMIZE_MODE_DISABLED = 0,
		OPTIMIZE_MODE_AUTO = 1,
		OPTIMIZE_MODE_HMD = 2,
		OPTIMIZE_MODE_SCREEN_TOP = 3,
	};
	virtual void setBackgroundUpdate(bool update) = 0;
	virtual bool isBackgroundUpdate() const = 0;
	virtual void setStreamImages(bool images) = 0;
	virtual bool isStreamImages() const = 0;
	virtual void setPaused(bool paused) = 0;
	virtual bool isPaused() const = 0;
	virtual void setAllowPauseResume(bool resume) = 0;
	virtual bool isAllowPauseResume() const = 0;
	virtual void setOptimizeMode(Ultraleap::OPTIMIZE_MODE mode) = 0;
	virtual Ultraleap::OPTIMIZE_MODE getOptimizeMode() const = 0;
	virtual void setTrackingMode(Ultraleap::TRACKING_MODE mode) = 0;
	virtual Ultraleap::TRACKING_MODE getTrackingMode() const = 0;
	virtual void setTrackingInterpolation(bool interpolation) = 0;
	virtual bool isTrackingInterpolation() const = 0;
	virtual Ultraleap::CONNECTION_STATUS getConnectionStatus() const = 0;
	virtual bool isStatusLowFPSDetected() const = 0;
	virtual bool isStatusPoorPerformancePause() const = 0;
	virtual bool isStatusTrackingErrorUnknown() const = 0;
	virtual void setTrackingOffsetDefault(const Math::Vec3 &val) = 0;
	virtual Math::Vec3 getTrackingOffsetDefault() const = 0;
	virtual void setTransformModeOffsetDefault(UltraleapDevice::TRANSFORM_MODE val) = 0;
	virtual UltraleapDevice::TRANSFORM_MODE getTransformModeOffsetDefault() const = 0;
	virtual int getNumDevices() const = 0;
	virtual UltraleapDevice * getDevice(int device_id) const = 0;
	virtual int getNumDevicesConnected() const = 0;
	virtual UltraleapDevice * getDeviceConnected(int device_id) const = 0;
	virtual long long getLeapNow() const = 0;
	virtual void synchronize() const = 0;
};

} // namespace Plugins
} // namespace Unigine
