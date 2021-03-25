#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>

#include "../Core.h"
#include "../Ray.h"
#include "../IntersectData.h"
#include "../Event.h"

#include "ImGUI/imgui.h"

struct Graphics;
struct InputActions;
struct Editor;
// FROM GODOT ENGINE
#define NODEDESC(m_class, m_inherits)	\
	virtual std::string get_class() const {		\
		return std::string(#m_class);	\
	}									\
	virtual std::string get_inherits() const {		\
		return std::string(#m_inherits);	\
	}									\
	static inline std::string get_class_static() { \
		return std::string(#m_class);		\
	}										\
	static std::string inherits_static() {       \
		return std::string(#m_inherits);         \
	}  \
	static std::string get_parent_class_static() {\
		return m_inherits::get_class_static();	\
	}\
	virtual bool is_class(const std::string& _Class) const { return (_Class == (#m_class)) ? true : m_inherits::is_class(_Class); } \
// end of godot code

struct Node : std::enable_shared_from_this<Node>
{
	std::string name = "Node";
	bool destroyed = false;
	bool visible = true;
	Node* parent{ nullptr };
	std::unordered_map<std::string, Node*> children;

	Node* GetRoot()
	{
		auto c = this;
		while (c->parent)
		{
			c = c->parent;
		}
		return c;
	}

	Node* GetChild(const std::string& node)
	{
		if (auto itr = children.find(node); itr != children.end())
		{
			return itr->second;
		}
		return nullptr;
	}
	void RemoveNode(const std::string& node_name)
	{
		if (auto it = children.find(node_name); it != children.end())
		{
			children.erase(node_name);
		}
	}

	void CleanTree()
	{
		std::vector<std::string> names;
		for (const auto& [k, child] : children)
		{
			child->CleanTree();
			if (child->destroyed)
			{
				child->OnDestroy();
				names.push_back(child->name);
				delete child;
			}
		}
		for (const auto& n : names)
			RemoveNode(n);
	}

	Node* GetNode(const std::string& path)
	{
		size_t start_path = path.find('/');
		if (start_path != std::string::npos)
		{
			std::string token = std::string(path.begin(), path.begin() + start_path);
			std::string rest = path.substr(start_path + 1, path.size() - start_path);
			if (token == "~") return GetRoot()->GetNode(rest);
			if (token == "..")
			{
				assert(parent);
				return parent->GetNode(rest);
			}
			if (auto node = GetChild(token); node)
			{
				return node->GetNode(rest);
			}
		}
		if (path == "~") return GetRoot();
		if (path == "..")
		{
			assert(parent);
			return parent;
		}
		return GetChild(path);
	}

	virtual Node* Copy()
	{
		Node* node = new Node(name);
		node->visible = visible;

		for (auto& c : children)
		{
			node->AddChild(c.second->Copy());
		}
		return node;
	}

	Node* AddChild(Node* node)
	{
		static int dup_count = 0;
		node->parent = (this);
		std::string key = node->name;
		if (children.find(key) != children.end()) key = key + std::to_string(dup_count++);
		node->name = key;
		children[key] = node;
		OnAddNode(node);
		return (this);
	}

	void Draw(Graphics& gfx)
	{
		if (!visible) return;
		if (user_draw) user_draw(gfx);
		OnDraw(gfx);
		for (const auto& [k, child] : children)
		{
			child->Draw(gfx);
		}
	}

	void Update(float dt, InputActions& actions)
	{
		if (!visible) return;
		if (user_update) user_update(dt, actions);
		OnUpdate(dt, actions);
		for (const auto& [k, child] : children)
		{
			child->Update(dt, actions);
		}
	}

	void AppEvent(Event& e)
	{
		if (!visible) return;
		if (user_event) user_event(e);
		OnEvent(e);
		for (const auto& [k, child] : children)
		{
			child->AppEvent(e);
		}
	}

	using UserUpdate = std::function<void(float, InputActions&)>;
	using UserDraw = std::function<void(Graphics& gfx)>;
	using UserInput = std::function<void(void)>;
	using UserEvent = std::function<void(Event&)>;
	using UserDestroy = std::function<void(void)>;
	using UserAdd = std::function<void(void)>;

	UserUpdate user_update;
	UserDraw user_draw;
	UserInput user_input;
	UserEvent user_event;
	UserDestroy user_destroy;
	UserAdd user_add;

	virtual void OnAddNode(Node* node) {};
	virtual void OnDestroy() {};
	virtual void OnUpdate(float dt, InputActions& actions) {};
	virtual void OnDraw(Graphics& gfx) {};
	virtual void OnInput() {};
	virtual void OnEvent(Event& e) { };

	virtual void Edit(Editor& editor);

	virtual IntersectData IntersectRay(const Ray& ray, std::vector<IntersectData>& hit_list = std::vector<IntersectData>{})
	{
		//if (is_class("StaticMeshNode") == false)
		{
			for (const auto& [k, child] : children)
			{
				if (auto hit_data = child->IntersectRay(ray, hit_list); hit_data.intersect)
				{
					hit_list.push_back(hit_data);
				}
			}
		}
		if (hit_list.empty())
			return { false, 0.0, nullptr, 0 };
		else
		{
			return *std::min_element(hit_list.begin(), hit_list.end(), [](auto a, auto b) {return a.distance < b.distance; });
		}
	}

	Node() {};
	Node(const std::string& n) : name(n) {};
	virtual ~Node() = default;

	virtual bool is_class(const std::string& _Class) const
	{
		return (_Class == "Node");
	}
	virtual std::string get_class() const {
		return std::string("Node");
	}
	static inline std::string get_class_static() {
		return std::string("Node");
	}
	virtual std::string get_inherits() const {
		return "None";
	}
};

