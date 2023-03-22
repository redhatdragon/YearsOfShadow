#pragma once
#include <UnigineNode.h>
#include <UniginePhysics.h>
#include <UnigineComponentSystem.h>
#include "Players/VRPlayer.h"

class VRPlayer;

class VRInteractable : public Unigine::ComponentBase
{
public:
	COMPONENT(VRInteractable, Unigine::ComponentBase);

	// interact methods
	virtual void grabIt(VRPlayer* player, int hand_num) {}
	virtual void holdIt(VRPlayer* player, int hand_num) {}
	virtual void useIt(VRPlayer* player, int hand_num) {}
	virtual void throwIt(VRPlayer* player, int hand_num) {}
};