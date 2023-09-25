struct BodyAABB {
    Vec3D<physics_fp> pos, siz, vel;
	uint8_t isSolid;

	__forceinline void simulate() {
		pos += vel;
	}
	__forceinline void reverseSimulate() {
		pos -= vel;
	}
	__forceinline bool collidesWith(const BodyAABB& other) const {
		if (pos.x + siz.x < other.pos.x || pos.x > other.pos.x + other.siz.x ||
			pos.y + siz.y < other.pos.y || pos.y > other.pos.y + other.siz.y ||
			pos.z + siz.z < other.pos.z || pos.z > other.pos.z + other.siz.z)
			return false;
		return true;
	}
	//Expects the input body to already be reverse simulated and was colliding with other before hand
	__forceinline Vec3D<physics_fp> resolveClosestSafePosition(const BodyAABB& other) {
		//const Vec3D<int32_t>& signedVel = *(Vec3D<int32_t>*)&vel;
		//Vec3D<uint32_t> futurePos = pos + vel;
		//Vec3D<uint32_t> closestPos = futurePos;
		//if (futurePos.x + siz.x > other.pos.x && futurePos.x < other.pos.x + other.siz.x) {
		//	if (signedVel.x < 0) {
		//		closestPos.x = other.pos.x + other.siz.x;
		//	} else if (signedVel.x > 0) {
		//		closestPos.x = other.pos.x - siz.x;
		//	}
		//}
		//if (futurePos.y + siz.y > other.pos.y && futurePos.y < other.pos.y + other.siz.y) {
		//	if (signedVel.y < 0) {
		//		closestPos.y = other.pos.y + other.siz.y;
		//	}
		//	else if (signedVel.y > 0) {
		//		closestPos.y = other.pos.y - siz.y;
		//	}
		//}
		//if (futurePos.z + siz.z > other.pos.z && futurePos.z < other.pos.z + other.siz.z) {
		//	if (signedVel.z < 0) {
		//		closestPos.z = other.pos.z + other.siz.z;
		//	}
		//	else if (signedVel.z > 0) {
		//		closestPos.z = other.pos.z - siz.z;
		//	}
		//}
		//return closestPos;

		Vec3D<physics_fp> savedPos = pos;
		constexpr uint32_t intervil = 100;
        Vec3D<physics_fp> miniVel = vel / intervil;
        Vec3D<physics_fp> lastPos = pos;
		for (uint32_t i = 0; i < intervil; i++) {
            pos += miniVel;
			if (collidesWith(other))
				break;
			lastPos = pos;
		}
		pos = savedPos;
		return lastPos;
	}
	__forceinline Vec3D<physics_fp> resolveClosestSafePosition(BodyAABB** others, uint32_t count) {
        Vec3D<physics_fp> closestPos = resolveClosestSafePosition(*others[0]);
		for (uint32_t i = 1; i < count; i++) {
            Vec3D<physics_fp> curPos = resolveClosestSafePosition(*others[i]);
            Vec3D<physics_fp> dist = curPos - pos;
			if (dist.getDistanceSquaredFP() < (closestPos - pos).getDistanceSquaredFP())
				closestPos = curPos;
		}
		return closestPos;
	}

	void printDebug() const {
		std::cout << ((physics_fp*)&pos.x)->getAsFloat() << ' '
			<< ((physics_fp*)&pos.y)->getAsFloat() << ' ' <<
			((physics_fp*)&pos.z)->getAsFloat() << std::endl;
		std::cout << ((physics_fp*)&siz.x)->getAsFloat() << ' '
			<< ((physics_fp*)&siz.y)->getAsFloat() << ' ' <<
			((physics_fp*)&siz.z)->getAsFloat() << std::endl;
	}
};
constexpr uint32_t sizeofBodyAABB = sizeof(BodyAABB);

#pragma pack(push, 1)
struct BodyID {
	uint32_t id;
	bool operator==(BodyID other) {
		if (id == other.id)
			return true;
		return false;
	}
	bool operator!=(BodyID other) {
		if (id != other.id)
			return true;
		return false;
	}
};
#pragma pack(pop)