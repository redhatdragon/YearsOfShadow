#pragma once

namespace SystemUtilities {
	struct Inventory {
		struct SlotMetaData {
			Name name;
			uint32_t maxStack;
			uint16_t maxHealth;
			const char* description;
			void* data;
		};
		struct Slot {
			uint32_t count;
			uint16_t id;
			uint16_t dmg;
			void* data;
		};
		static FlatBuffer<SlotMetaData, 256 * 256> slotMetaData;
		FlatBuffer<Slot, 8 * 8> slots;

		void init(uint32_t count) {
			memset(this, 0, sizeof(Inventory));
			slots.count = count;
		}

		inline Slot& getSlot(uint32_t slotIndex) {
			return slots[slotIndex];
		}
		inline void setSlot(Slot& slot, Slot& slotData) {
			slot = slotData;
		}
		uint32_t addSlotData(Slot& slotData) {
			uint32_t count = slots.count;
			for (uint32_t i = 0; i < count; i++) {
				Slot& slot = slots[i];
				if (slot.count == 0) {
					slot = slotData;
					return 0;
				}
				if (slot.id == slotData.id) {
					uint32_t leftover = addToSlot(slot, slotData.count);
					slotData.count = leftover;
					if (leftover == 0)
						return 0;
				}
			}
			return slotData.count;
		}
		bool mergeSlots(Slot& destSlot, Slot& otherSlot) {
			uint32_t leftover = addToSlot(destSlot, otherSlot.count);
			if (leftover) {
				otherSlot.count = leftover;
				return false;
			}
			return true;
		}
		void zeroSlot(Slot& slot) {
			slot.count = 0;
		}

	private:
		void decrementSlot(Slot& slot) {
			uint32_t& count = slot.count;
			if (count)
				count--;
		}
		void decrementSlotUnsafe(Slot& slot) {
			slot.count--;
		}
		uint32_t addToSlot(Slot& slot, uint32_t num) {
			SlotMetaData& metaData = slotMetaData[slot.id];
			uint64_t total = num + slot.count;
			int64_t result = metaData.maxStack - total;
			if (result <= 0) {
				slot.count = metaData.maxStack;
				return -result;
			}
			slot.count = (uint32_t)total;
			return 0;
		}
		uint32_t removeFromSlot(Slot& slot, uint32_t num) {
			SlotMetaData& metaData = slotMetaData[slot.id];
			int64_t total = slot.count - num;
			if (total <= 0) {
				slot.count = 0;
				return (uint32_t)-total;
			}
			slot.count -= num;
			return 0;
		}
	};

	void combineInventories(Inventory& dest, Inventory& other) {
		uint32_t count = other.slots.count;
		for (uint32_t i = 0; i < count; i++) {
			dest.addSlotData(other.slots[i]);
		}
	}
};

FlatBuffer<SystemUtilities::Inventory::SlotMetaData, 256 * 256> 
SystemUtilities::Inventory::slotMetaData = {};