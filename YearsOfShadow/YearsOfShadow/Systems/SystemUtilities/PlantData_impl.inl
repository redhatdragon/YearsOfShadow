bool SystemUtilities::PlantMetaData::init(const EntityObject& obj, const char* inputFilePath) {
	filePath = inputFilePath;
	const ComponentObject* c;
	c = getComponent(obj, "harvestTime", ComponentObject::TYPE::TYPE_INT, inputFilePath);
	if (c == nullptr) return false;
	harvestTime = c->getInt();
	c = getComponent(obj, "minGrowthTime", ComponentObject::TYPE::TYPE_INT, inputFilePath);
	if (c == nullptr) return false;
	minGrowthTime = c->getInt();
	c = getComponent(obj, "maxGrowthTime", ComponentObject::TYPE::TYPE_INT, inputFilePath);
	if (c == nullptr) return false;
	maxGrowthTime = c->getInt();
	c = getComponent(obj, "lifespan", ComponentObject::TYPE::TYPE_INT, inputFilePath);
	if (c == nullptr) return false;
	lifespan = c->getInt();

	c = getComponent(obj, "requiredTool", ComponentObject::TYPE::TYPE_STRING, inputFilePath);
	if (c == nullptr) return false;
	requiredTool = c->getString();

	c = getComponent(obj, "harvestType", ComponentObject::TYPE::TYPE_DARRAY, inputFilePath);
	if (c == nullptr) return false;
	{
		DArray<uint32_t> buff = c->getDArray();
		harvestType = *(DArray<Name>*) & buff;
	}
	c = getComponent(obj, "harvestCount", ComponentObject::TYPE::TYPE_DARRAY, inputFilePath);
	if (c == nullptr) return false;
	{
		DArray<uint32_t> buff = c->getDArray();
		harvestCount.init(buff.size()/3);
		for (uint32_t i = 0; i < buff.size(); i += 3) {
			//harvestCount.init();
			//harvestCount[i / 3].min = buff[i];
			//harvestCount[i / 3].max = buff[i + 1];
			//harvestCount[i / 3].algorithm = *(Name*)&buff[i + 2];
			HarvestCountData& hcd =  harvestCount[i/3];//
			hcd.min = buff[i];
			hcd.max = buff[i + 1];
			hcd.algorithm = *(Name*)&buff[i + 2];
			//harvestCount.push_back(hcd);
		}
	}

	c = getComponent(obj, "seedlingTexture", ComponentObject::TYPE::TYPE_STRING, inputFilePath);
	if (c == nullptr) return false;
	seedlingTexturePath = c->getString();
	//c = getComponent(obj, "growthTextures", ComponentObject::TYPE::TYPE_DARRAY, inputFilePath);
	//if (c == nullptr) return false;
	//{
	//	DArray<uint32_t> buff = c->getDArray();
	//	growthTextureData.init(buff.size() / 2);
	//	for (uint32_t i = 0; i < buff.size(); i += 2) {
	//		GrowthTextureData& gtd = growthTextureData[i/2];
	//		gtd.time = buff[i];
	//		gtd.textureName = *(Name*)&buff[i + 1];
	//	}
	//}
	c = getComponent(obj, "minGrowthTexture", ComponentObject::TYPE::TYPE_STRING, inputFilePath);
	if (c == nullptr) return false;
	minGrowthTexturePath = c->getString();
	c = getComponent(obj, "maxGrowthTexture", ComponentObject::TYPE::TYPE_STRING, inputFilePath);
	if (c == nullptr) return false;
	maxGrowthTexturePath = c->getString();

	if(initAssets(inputFilePath) == false) return false;

	return true;
}

bool SystemUtilities::PlantMetaData::initAssets(const char* objName) {
	std::string totalPath = EE_getDirData();
	totalPath += seedlingTexturePath.c_str();
	seedlingTexture = TextureCodex::add(totalPath);
	if (seedlingTexture.id == 0) goto failed;
	//for (uint32_t i = 0; i < growthTextureData.size(); i++) {
	//	GrowthTextureData& gtd = growthTextureData[i];
	//	totalPath = EE_getDirData();
	//	totalPath += gtd.textureName.c_str();
	//	gtd.texture = TextureCodex::add(totalPath);
	//	if (gtd.texture.id == 0) goto failed;
	//}
	totalPath = EE_getDirData();
	totalPath += minGrowthTexturePath.c_str();
	minGrowthTexture = TextureCodex::add(totalPath);
	if (minGrowthTexture.id == 0) goto failed;
	totalPath = EE_getDirData();
	totalPath += maxGrowthTexturePath.c_str();
	maxGrowthTexture = TextureCodex::add(totalPath);
	if (maxGrowthTexture.id == 0) goto failed;
	return true;

failed:
	std::cout << "Error: failed to initialize asset " << totalPath << " for " << objName;
	std::cout << std::endl;
	return false;
}

const ComponentObject* SystemUtilities::PlantMetaData::getComponent(const EntityObject& obj,
	const char* componentName, ComponentObject::TYPE expectedType,
	const char* objName) {

	const ComponentObject* retValue = obj.getComponent(componentName);

	if (retValue == nullptr) {
		std::cout << "Error: component " << componentName << " doesn't exist "
			<< "in object " << objName;
		return nullptr;
	}
	if (retValue->type != expectedType) {
		std::cout << "Error: component " << componentName << "has incorrect type"
			<< " in object " << objName;
		return nullptr;
	}
	return retValue;
}