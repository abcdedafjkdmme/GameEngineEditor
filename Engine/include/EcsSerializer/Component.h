#pragma once

#include "ReflectedVar.h"
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <iostream>

namespace Engine {

	class Entity;
	

	class Component {	

	public:
		Component( std::string _ComponentType) :  ComponentType(_ComponentType) { };
		virtual ~Component() {};

		const std::string ComponentType{ };
		Entity* Owner = nullptr;
		std::vector<Reflect::Var> ExportedProperties;

		bool IsAliveInEditor = true;
		bool CanBeKilledInEditor = true;
		/*bool QueuedForDeletion = false;*/

		virtual Component& AddComponentToEntity(Entity& Target) = 0;
		virtual void RemoveThisComponentFromEntity(Entity& Target) = 0;
		virtual nlohmann::json ToJsonC() = 0;
		virtual void InitFromJson(nlohmann::json& _json) = 0;
		
		
	};

	
}