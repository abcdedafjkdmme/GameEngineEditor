#include "ComponentFactory.h"
#include "Entity.h"
#include "EngineComponents.h"
#include "WImage.h"
#include "Component.h"
#include <string>



void Engine::ComponentFactory::RegisterAllComponents()
{
	RegisterComponent<Name>();
	RegisterComponent<Position>();
	RegisterComponent<EntityMarker>();
	RegisterComponent<WImage>();
}


Engine::ComponentFactory::ComponentFactory()
{
	RegisterAllComponents();
}


void Engine::ComponentFactory::AddComponentFromType(std::string Type, Engine::Entity& TargetEntity)
{
	CompMap.at(Type)->AddComponentToEntity(TargetEntity);
}

void Engine::ComponentFactory::AddComponentFromType(std::string Type, nlohmann::json& CompJson, Engine::Entity& TargetEntity)
{
	CompMap.at(Type)->AddComponentToEntity(TargetEntity).InitFromJson(CompJson);
}



