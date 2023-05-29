struct PhysicsFrame {
	Vec3D<uint32_t> dynamicBodyPositions[max_dynamic_bodies];
	Vec3D<uint32_t> dynamicBodyVelocities[max_dynamic_bodies];
	uint32_t dynamicBodyCount;

	struct _CommandFrame {
		enum {
			ADD_BODY,
			REMOVE_BODY
		}type;
		BodyAABB bodyData;
		uint32_t id;
		inline void setAsAddBody(const BodyAABB& body, uint32_t _id) {
			type = ADD_BODY;
			bodyData = body;
			id = _id;
		}
		inline void setAsRemoveBody(uint32_t _id) {
			type = REMOVE_BODY;
			id = _id;
		}
	};

	FlatBuffer<_CommandFrame, 10000> commandFrames;

	inline void clear() {
		bodyCount = 0;
		commandFrames.count = 0;
	}

	inline void addBody(const BodyAABB& body, uint32_t id) {
		commandFrames[commandFrames.count].setAsAddBody(body, id);
		commandFrames.count++;
	}
	inline void removeBody(uint32_t id) {
		commandFrames[commandFrames.count].setAsRemoveBody(id);
		commandFrames.count++;
	}
	inline void addNextDynamicBodyPositionAndVelocity(const Vec3D<uint32_t>& pos, const Vec3D<uint32_t>& vel) {
		dynamicBodyPositions[dynamicBodyCount] = pos;
		dynamicBodyVelocities[dynamicBodyCount] = vel;
	}
};

uint64_t sizeofPhysicsFrame = sizeof(PhysicsFrame);