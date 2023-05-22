#pragma once

namespace SystemUtilities {
	struct EnemyAI {
		EntityID target = -1;
		EntityHandle targetHandle;
		uint8_t attackRate = 20;
		uint64_t lastAttack = 0;
		MoveToLocation moveTo;
		inline bool hasValidTarget() const {
			if (target == -1) return false;
			return ecs.entityHandleValid(target, targetHandle);
		}
		inline void setTarget(EntityID entity) {
			target = entity;
			targetHandle = ecs.entityGetHandle(entity);
		}
		inline bool canAttack(uint64_t currentGameTick) {
			if (lastAttack + attackRate >= currentGameTick)
				return true;
			return false;
		}
		inline void resetAttackTimer(uint64_t currentGameTick) {
			lastAttack = currentGameTick;
		}
	};
}