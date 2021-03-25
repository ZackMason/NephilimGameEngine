#pragma once

#include "../Camera.h"
#include "Node3D.h"
#include "../Input.h"
#include "../AssetDB.h"
#include "../../Graphics/Material.h"
#include "StaticMeshNode.h"

enum class eCameraMode {
	SPHERICAL, FPS
};

struct CameraNode : Node3D
{
	Camera camera;

	bool active = false;

	v3 target = v3(0.0f);

	eCameraMode mode = eCameraMode::SPHERICAL;

	AABB bounding_box = { {-0.5,-0.5,-0.75} , {0.5,0.5,0.75} };

	v3 SphereToCart(const v3& r)
	{
		return v3{
			r.z * std::sinf(r.y) * std::cosf(r.x),
			r.z * std::cosf(r.y),
			r.z * std::sinf(r.y) * std::sinf(r.x),
		};
	}

	void OnEvent(Event& e) override
	{
		if (!active) return;
		EventDispatcher d(e);
		d.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(CameraNode::OnMouseMoved));
		d.Dispatch<MouseScrolledEvent>(BIND_EVENT_FN(CameraNode::OnMouseScroll));
		d.Dispatch<MouseButtonPressedEvent>(BIND_EVENT_FN(CameraNode::OnMousePressed));
		d.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(CameraNode::OnMouseReleased));
		d.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(CameraNode::OnResize));
	}

	bool OnMouseScroll(MouseScrolledEvent& e)
	{
		local_transform.origin.z -= e.GetYOffset();
		return true;
	}

	v2 last_mouse = v2(0);
	bool clicked = false;

	bool OnResize(WindowResizeEvent& e)
	{
		camera.width = e.GetWidth();
		camera.height = e.GetHeight();
		camera.MakeProjection();
		return true;
	}

	bool OnMousePressed(MouseButtonPressedEvent& e)
	{
		//auto root = GetRoot();
		//
		//auto ray = camera.GetScreenRay(last_mouse.x, last_mouse.y);
		//
		//if (auto widget = dynamic_cast<StaticMeshNode*>(GetChild("Editor Widget")); widget)
		//{
		//	if (auto hit_data = widget->IntersectRay(ray); hit_data.intersect)
		//	{
		//		std::cout << "Clicked Widget\n";
		//	}
		//	else if (auto hit_data = root->IntersectRay(ray); hit_data.intersect)
		//	{
		//		std::cout << hit_data << "\n";
		//
		//		if (auto n = dynamic_cast<CameraNode*>(hit_data.node); n)
		//		{
		//			widget->local_transform.origin = n->camera.position;
		//		}
		//		else if (auto n = dynamic_cast<Node3D*>(hit_data.node); n)
		//		{
		//			widget->local_transform.origin = n->local_transform.origin;
		//		}
		//		else
		//		{
		//			widget->local_transform.origin = hit_data.position;
		//		}
		//	}
		//}
		return clicked = true;
	}

	bool OnMouseReleased(MouseButtonReleasedEvent& e)
	{
		return clicked = false;
	}

	bool OnMouseMoved(MouseMovedEvent& e)
	{
		v2 mouse_delta = { e.GetX() - last_mouse.x, e.GetY() - last_mouse.y };
		last_mouse = { e.GetX(), e.GetY()};

		if (!clicked) return false;

		local_transform.origin.x += (mouse_delta.x / camera.width) * 2.0f;
		local_transform.origin.y -= (mouse_delta.y / camera.height) * 2.0f;
		return true; 
	}

	void OnDraw(Graphics& gfx) override
	{
		if(!active)
		{
			auto box = Transform(glm::inverse(camera.view)).xform(bounding_box);
			auto box_mesh = AssetDB::ResolveStaticMesh("res/MODELS/cube.obj");
			Transform box_t;
			box_t.Scale((box.max - box.min) / 2.0f);
			box_t.origin = (box.max + box.min) / 2.0f;
			PBRMaterial material;
			material.SetTransform(box_t.ToMatrix());
			material.Bind(gfx);

			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			box_mesh->Draw(gfx);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			auto camera_mesh = AssetDB::ResolveStaticMesh("res/MODELS/camera.obj");

			material.SetTransform(glm::inverse(camera.view));
			material.Bind(gfx);
			camera_mesh->Draw(gfx);
		}

		if (!active) return;
		gfx.SetCamera(uCamera{
			camera.view, camera.projection, v4(camera.GetPosition(),0.0)
		});
	}

	void OnUpdate(f32 dt, InputActions& actions) override
	{
		if (!active) return;
		switch (mode)
		{
		case eCameraMode::SPHERICAL: {
			local_transform.origin.y = std::clamp(local_transform.origin.y, glm::radians(1.0f), glm::radians(179.0f));
			camera.position = target + SphereToCart(local_transform.origin);
			camera.LookAt(target);

			if (actions.is_action_pressed("W"))
			{
				target += camera.GetForward() * v3{ 1, 0, 1 };
			}
			if (actions.is_action_pressed("S"))
			{
				target += -camera.GetForward() * v3 { 1, 0, 1 };
			}
			if (actions.is_action_pressed("A"))
			{
				target += camera.GetRight() * v3 { 1, 0, 1 };
			}
			if (actions.is_action_pressed("D"))
			{
				target += -camera.GetRight() * v3 { 1, 0, 1 };
			}
		}
			break;
		case eCameraMode::FPS:
			camera.position = local_transform.origin;
			camera.LookAt(local_transform.origin + local_transform.basis[2]);
			break;
		}
	}

	CameraNode(const std::string& node_name, const Camera& cam) : camera(cam)
	{
		name = node_name; 
		local_transform.origin.y = glm::radians(30.0f);
		local_transform.origin.z = 10.0;

		local_transform.origin.y = std::clamp(local_transform.origin.y, glm::radians(1.0f), glm::radians(179.0f));
		camera.position = target + SphereToCart(local_transform.origin);
		camera.LookAt(target);
	}

	IntersectData IntersectRay(const Ray& ray, std::vector<IntersectData>& hit_list = std::vector<IntersectData>{}) override
	{
		if (auto hit_data = Transform(glm::inverse(camera.view)).xform(bounding_box).IntersectRay(ray, this); !active && hit_data.intersect)
		{
			hit_list.push_back(hit_data);
		}
		{
			return Node::IntersectRay(ray, hit_list);
		}
	}

	virtual ~CameraNode() = default;

	NODEDESC(CameraNode, Node3D);
};


