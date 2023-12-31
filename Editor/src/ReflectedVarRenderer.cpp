#include "ReflectedVarRenderer.h"
#include <cassert>
#include "imgui.h"
#include "misc/cpp/imgui_stdlib.h"
#include <sstream>
#include "EditorTypes.h"
#include <filesystem>
#include <iostream>
#include "FilePayload.h"

std::string Editor::GetImGuiIDFromName(const Reflect::Var& _Var)
{

	std::ostringstream Temp;
	Temp << &_Var;
	return _Var.Name + "##" + Temp.str();

}

void Editor::RenderVar(Reflect::Var& _Var)
{

	switch (_Var.VarType)
	{
	case Reflect::Type::None:
	{
		assert(false && "the var type is not set.");
		break;
	}

	case Reflect::Type::IntType:
	{
		ImGui::InputInt(GetImGuiIDFromName(_Var).c_str(), (int*)_Var.Data);
		break;
	}

	case Reflect::Type::FloatType:
	{
		ImGui::DragFloat(GetImGuiIDFromName(_Var).c_str(), (float*)_Var.Data);
		break;
	}

	case Reflect::Type::DoubleType:
	{
		ImGui::DragScalar(GetImGuiIDFromName(_Var).c_str(), ImGuiDataType_Double, (double*)_Var.Data);
		break;
	}

	case Reflect::Type::StringType:
	{
		ImGui::InputText(GetImGuiIDFromName(_Var).c_str(), (std::string*)_Var.Data);
		break;
	}

	case Reflect::Type::FilePathType:
	{

		auto* VarAsFilePath = static_cast<std::filesystem::path*>(_Var.Data);
		std::string VarFilePathString = VarAsFilePath->string();


		ImGui::InputText(GetImGuiIDFromName(_Var).c_str(), &VarFilePathString);

		*VarAsFilePath = std::filesystem::path{ VarFilePathString };

		if (ImGui::BeginDragDropTarget()) {

			const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("FILE");

			if (Payload) {


				Editor::FilePayload* PayloadData = static_cast<Editor::FilePayload*>(Payload->Data);

				std::filesystem::path PayloadFilePath = std::filesystem::relative(PayloadData->_SentFile, PayloadData->_BuildDirPath);

				*VarAsFilePath = std::filesystem::path{ PayloadFilePath };
			}

			ImGui::EndDragDropTarget();
		}

	
		break;
	}

	case Reflect::Type::BoolType:
	{
		ImGui::Checkbox(GetImGuiIDFromName(_Var).c_str(), (bool*)_Var.Data);
		break;
	}

	case Reflect::Type::Color4Type:
	{
		Editor::Color4* DataAsCol4 = static_cast<Editor::Color4*>(_Var.Data);
		float Color[4] = { DataAsCol4->r, DataAsCol4->g, DataAsCol4->b ,DataAsCol4->a };
		ImGui::ColorEdit4(GetImGuiIDFromName(_Var).c_str(), Color);
		DataAsCol4->r = Color[0];
		DataAsCol4->g = Color[1];
		DataAsCol4->b = Color[2];
		DataAsCol4->a = Color[3];
		break;
	}

	case Reflect::Type::Vec2Type:
	{
		Editor::Vector2* DataAsVec2 = static_cast<Editor::Vector2*>(_Var.Data);
		float Vector[2] = { DataAsVec2->x, DataAsVec2->y };
		ImGui::DragFloat2(GetImGuiIDFromName(_Var).c_str(), Vector);
		DataAsVec2->x = Vector[0];
		DataAsVec2->y = Vector[1];
		break;
	}

	default:
	{
		assert(false && "this var type cannot be rendered");
		break;
	}
	}
}

