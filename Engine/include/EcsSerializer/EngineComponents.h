#pragma once

#include <string>
#include "nlohmann/json.hpp"
#include "Component.h"
#include "Entity.h"


namespace Engine {

	class EntityMarker : public Component {
	public:
		EntityMarker() : Component("EntityMarker") {};

		Component& AddComponentToEntity(Entity& Target) override {
			return Target.AddComponent<EntityMarker>();
		};
		void RemoveThisComponentFromEntity(Entity& Target) override {
			Target.RemoveComponent<EntityMarker>();
		};

		nlohmann::json ToJsonC() override {
			return nlohmann::json{
				{"ComponentType",ComponentType}
			};
		};

		void InitFromJson(nlohmann::json& _json) override { };

	};


	class Name : public Component {

	public:
		std::string StrName{};

		/*Name() : Component("Name") {
			ExportedProperties.push_back(Reflect::Var{ &StrName, Reflect::Type::StringType, "name" });
		};*/

		Name(std::string _Name = "unnamed") : Component("Name"), StrName(_Name) {
			ExportedProperties.push_back(Reflect::Var{ &StrName, Reflect::Type::StringType, "name" });
			CanBeKilledInEditor = false;
		};

		Component& AddComponentToEntity(Entity& Target) override {
			return Target.AddComponent<Name>();
		};
		void RemoveThisComponentFromEntity(Entity& Target) override {
			Target.RemoveComponent<Name>();
		}

		nlohmann::json ToJsonC() override {
			return nlohmann::json{
				{"ComponentType",ComponentType},
				{"Name", StrName} };
		};

		void InitFromJson(nlohmann::json& _json) override {
			StrName = _json.at("Name");
		};

	};



	class Position : public Engine::Component {
	public:

		Position(double _X, double _Y) : Component("Position"), X(_X), Y(_Y) {
			ExportedProperties.push_back(Reflect::Var{ &X, Reflect::Type::DoubleType, "x" });
			ExportedProperties.push_back(Reflect::Var{ &Y, Reflect::Type::DoubleType, "y" });
		};

		Position() : Component("Position") {
			ExportedProperties.push_back(Reflect::Var{ &X, Reflect::Type::DoubleType, "x" });
			ExportedProperties.push_back(Reflect::Var{ &Y, Reflect::Type::DoubleType, "y" });
		};

		double X{};
		double Y{};


		Component& AddComponentToEntity(Entity& Target) override {
			return Target.AddComponent<Position>();
		};
		void RemoveThisComponentFromEntity(Entity& Target) override {
			Target.RemoveComponent<Position>();
		}


		nlohmann::json ToJsonC() override {
			return nlohmann::json{
				{"ComponentType",ComponentType},
				{"X", X},
				{"Y",Y}
			};
		};

		void InitFromJson(nlohmann::json& _json) override {
			X = _json.at("X");
			Y = _json.at("Y");
		};
	};
	;

}