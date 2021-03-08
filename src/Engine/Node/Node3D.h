#pragma once
 
#include "Node.h"

#include "../Editor/Editor.h"
#include "../Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/matrix_decompose.hpp"
#include "glm/gtx/euler_angles.hpp"


struct Node3D : Node
{
	Transform local_transform;

	v3 rotation = v3(0,0,0);

	void Edit(Editor& editor) override
	{
		Node::Edit(editor);

		ImGui::Separator();

		auto r = rotation;

		r = glm::degrees(r);

		ImGui::DragFloat3("origin", glm::value_ptr(local_transform.origin));

		
		if (ImGui::DragFloat3("rotation", glm::value_ptr(r)))
		{
			rotation = glm::radians(r);

			local_transform.basis = glm::eulerAngleXYZ(rotation.x, rotation.y, rotation.z);
		}

		ImGui::DragFloat3("basis:x", glm::value_ptr(local_transform.basis[0]));
		ImGui::DragFloat3("basis:y", glm::value_ptr(local_transform.basis[1]));
		ImGui::DragFloat3("basis:z", glm::value_ptr(local_transform.basis[2]));
		
	}

	Node3D() : Node("Node3D") {};
	Node3D(const Transform& trans) : Node("Node3D"), local_transform(trans) {};
	virtual ~Node3D() = default;

	NODEDESC(Node3D, Node);
};