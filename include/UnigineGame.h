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

#include "UniginePlayers.h"
#include "UniginePathFinding.h"

namespace Unigine
{

class Obstacle;

class UNIGINE_API GameIntersection : public APIInterface
{
public:
	static Ptr<GameIntersection> create();
	void setPoint(const Math::Vec3 &point);
	Math::Vec3 getPoint() const;
};
typedef Ptr<GameIntersection> GameIntersectionPtr;

//////////////////////////////////////////////////////////////////////////

class UNIGINE_API Game
{
protected:
	

public:
	static int isInitialized(); 
	static void setEnabled(bool enabled);
	static bool isEnabled();
	static void setData(const char *data);
	static const char *getData();
	static void setFrame(int frame);
	static int getFrame();
	static void setIFps(float ifps);
	static float getIFps();
	static void setScale(float scale);
	static float getScale();
	static void setTime(float time);
	static float getTime();
	static void setSeed(int seed);
	static int getSeed();
	static void setPlayer(const Ptr<Player> &player);
	static Ptr<Player> getPlayer();
	static void setPlayerListener(const Ptr<Player> &listener);
	static Ptr<Player> getPlayerListener();
	static void getMainPlayers(Vector<Ptr<Player>> &players);
	static void getListeners(Vector<Ptr<Player>> &players);
	static unsigned int getRandom();
	static Math::vec4 getRandomColor();
	static int getRandomInt(int from, int to);
	static float getRandomFloat(float from, float to);
	static double getRandomDouble(double from, double to);
	static float getNoise1(float pos, float size, int frequency);
	static float getNoise2(const Math::vec2 &pos, const Math::vec2 &size, int frequency);
	static float getNoise3(const Math::vec3 &pos, const Math::vec3 &size, int frequency);
	static Ptr<Obstacle> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, float radius, int mask, const Vector<Ptr<Node>> &exclude, Math::Vec3 *intersection);
	static Ptr<Obstacle> getIntersection(const Math::Vec3 &p0, const Math::Vec3 &p1, float radius, int mask, const Ptr<GameIntersection> &intersection);
};

} // namespace Unigine
