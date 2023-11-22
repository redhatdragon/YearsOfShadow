#pragma once
#include <fstream>
#include <unordered_map>

namespace SystemUtilities {
	class SerialEntity {
		struct Component {
			Name name;
			uint32_t size;
			uint8_t data[1];  //Not allowed to do 0 length array gaaaaaaaaaaaay
		};
		uint8_t components[1];
		Component* getRootComponent() {
			return (Component*)components;
		}
	public:
		void construct(EntityID entity) {
			static FlatBuffer<ComponentID, 32> idBuff;
			idBuff.clear();
			ecs.getEntityComponentIDs(entity, idBuff);
			uint32_t idCount = idBuff.count;
			for (uint32_t i = 0; i < idCount; i++) {
				uint32_t componentSize = ecs.getComponentSize(idBuff[i]);

			}
		}
	};

	struct Serialize {
		
	};
}