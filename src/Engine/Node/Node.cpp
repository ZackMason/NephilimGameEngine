#include "Node.h"

#include "Engine/Editor/Editor.h"

void Node::Edit(Editor& editor)
{
	ImGui::Text("Class: %s", get_class().c_str());
	ImGui::Checkbox("Destroy", &destroyed);
	if (destroyed) editor.selected_node = nullptr;
}
