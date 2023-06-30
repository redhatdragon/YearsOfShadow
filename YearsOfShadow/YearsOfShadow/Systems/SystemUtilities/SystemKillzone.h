#pragma once

struct Killzone {

};

class SystemKillzone : public System {
	ComponentID bodyComponentID = ecs.registerComponent("body", sizeof(BodyID));
public:
	virtual void init() {

	}
	virtual void run() {
		
	}
	virtual const char* getName() {
		return "SystemPhysics";
	}
};