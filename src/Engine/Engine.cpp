#include "Engine.h"

#include "Event.h"
#include "Camera.h"
#include "Random.h"

#include "Graphics/StaticMesh.h"
#include "Graphics/Shader.h"
#include "Graphics/UniformBuffer.h"
#include "Graphics/Framebuffer.h"
#include "Graphics/Screen.h"
#include "Graphics/HDRTexture.h"


#include "Node/StaticMeshNode.h"
#include "Node/CameraNode.h"

#include "Editor/Editor.h"


#include <iostream>


void Engine::InitializeLibraries()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	
}

bool Engine::OnWindowResize(WindowResizeEvent& e) {
	config["width"] = e.GetWidth();
	config["height"] = e.GetHeight();

	window->Resize(e.GetWidth(), e.GetHeight());

	window_resize_timer.Start();
	//AssetDB::ResizeFramebuffers(e.GetWidth(), e.GetHeight());

	return false;
}

void Engine::OnEvent(Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Engine::OnWindowClose));
	dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Engine::OnWindowResize));
	dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Engine::OnKeyPressed));
	dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Engine::OnKeyReleased));


	scene_root->AppEvent(e);
	if (editor->editor_camera->active)
	{
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Editor::OnClick, editor.get(), std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Editor::OnMouseMove, editor.get(), std::placeholders::_1));
		editor->editor_camera->OnEvent(e);
	}
}

void SetupInput(InputActions& ia)
{
	ia.add_action("W", GLFW_KEY_W);
	ia.add_action("A", GLFW_KEY_A);
	ia.add_action("S", GLFW_KEY_S);
	ia.add_action("D", GLFW_KEY_D);
	ia.add_action("RELOAD SHADERS", GLFW_KEY_P);
	ia.add_action("RELOAD FRAMES", GLFW_KEY_L);
}

void SetupInternalTextures()
{
	auto white = CreateRef<Texture2D>(1, 1);
	u32 white_color = 0xffffffff;
	white->SetData(&white_color, sizeof(u32));
	AssetDB::tex2D_cache["white"] = white;

	auto blue = CreateRef<Texture2D>(1, 1);
	u32 blue_color = 0xffff0000;
	blue->SetData(&blue_color, sizeof(u32));
	AssetDB::tex2D_cache["blue"] = blue;

	auto black = CreateRef<Texture2D>(1, 1);
	u32 black_color = 0x00000000;
	black->SetData(&black_color, sizeof(u32));
	AssetDB::tex2D_cache["black"] = black;
}

void Engine::Run()
{

	editor->editor_camera = new CameraNode("Editor Camera", Camera(glm::radians(45.0f), window->data.width, window->data.height, 0.01, 1000.f));
	editor->editor_camera->active = true;
	editor->editor_camera->parent = scene_root;
	editor->editor_camera->local_transform.origin.x = 1.0;
	

	f32 elsapsed_time = 0.0f;
	f32 last_time = glfwGetTime();

	SetupInput(input_actions);

	Graphics gfx;
	gfx.ClearColor(0.4, 0.4, 0.4, 0.0);
	gfx.Enable(GL_DEPTH_TEST);
	gfx.CullMode(GL_BACK);

	Random rng;
	auto n = new Node("spheres");
	scene_root->AddChild(n);
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			StaticMeshNode* mn = new StaticMeshNode();
			mn->name = "sphere";
			mn->name = mn->name + std::to_string(i) + std::string(" ") + std::to_string(j);

			mn->SetMesh("res/MODELS/Sphere.obj");

			mn->local_transform.origin = { i*3, j*3, -20 };

			Ref<PBRMaterial> mat = std::dynamic_pointer_cast<PBRMaterial>(mn->material);


			mat->color = rng.rand_color();

			mat->roughness = i / 10.0;
			mat->metallic = j / 10.0;
			n->AddChild(mn);
		}
	}


	AssetDB::CreateFramebuffer("lambert_pass_aa", config["width"], config["height"], true);
	AssetDB::CreateFramebuffer("lambert_pass", config["width"], config["height"]);

	Camera camera(glm::radians(45.0f), window->data.width, window->data.height, 0.01, 1000.f);
	auto cn = new CameraNode("camera", camera);
	scene_root->AddChild(cn);

	Screen screen;
	Ref<Shader> scr_shader = AssetDB::ResolveShader("screen");

	StaticMeshNode* mesh_node = new StaticMeshNode();
	mesh_node->name = "Cerberus";
	mesh_node->SetMesh("res/MODELS/Cerberus.obj");

	mesh_node->local_transform.Translate({ 0,-1,0 });
	
	Ref<PBRMaterial> mat = std::dynamic_pointer_cast<PBRMaterial>(mesh_node->material);
	mat->diffuse_texture = "Cerberus_A.tga";
	mat->normal_texture = "Cerberus_N.tga";
	mat->metallic_texture = "Cerberus_M.tga";
	mat->roughness_texture = "Cerberus_R.tga";

	scene_root->AddChild(mesh_node);

	mesh_node = new StaticMeshNode();
	mesh_node->name = "Teapot";
	mesh_node->SetMesh("res/MODELS/Teapot.obj");

	mesh_node->local_transform.Translate({ 0,-1,0 });

	mat = std::dynamic_pointer_cast<PBRMaterial>(mesh_node->material);
	mat->normal_texture = "brick_normal.png";

	scene_root->AddChild(mesh_node);


	StaticMeshNode* torus = new StaticMeshNode();
	torus->name = "Torus";
	torus->SetMesh("res/MODELS/floor.obj");

	torus->local_transform.SetScale({ 3.0,3.0,3.0 });
	torus->local_transform.origin.y = -4;
	mat = std::dynamic_pointer_cast<PBRMaterial>(torus->material);
	//mat->diffuse_texture = "brick_1.png";
	//mat->normal_texture = "brick_normal.png";
	scene_root->AddChild(torus);


	AssetDB::ResolveShader("pbr");
	AssetDB::ResolveShader("widget");
	AssetDB::ResolveShader("skycube");
	AssetDB::ResolveShader("iblFilter");
	AssetDB::SetShadersUniformBlock("uCamera", 1);

	std::string envir = "Etnies_Park_Center_3k.hdr";// "Tokyo_BigSight_3k.hdr";

	//HDRTexture enviroment("Etnies_Park_Center_3k.hdr");
	AssetDB::ResolveHDRTexture(envir);

	while (!exit)
	{
		editor->NewFrame();

		if (window_resize_timer.Tick())
		{
			AssetDB::ResizeFramebuffers(config["width"], config["height"]);
		}

		const f32 time = static_cast<f32>(glfwGetTime());
		const f32 delta_time = time - last_time;
		last_time = time;
		elsapsed_time += delta_time;

		if (input_actions.is_action_just_pressed("RELOAD FRAMES"))
		{
			std::cout << config["width"] << ", " << config["height"] << "\n";
			AssetDB::ResizeFramebuffers(config["width"], config["height"]);
		}
		if (input_actions.is_action_just_pressed("RELOAD SHADERS"))
		{
			std::cout << "reload\n";
			AssetDB::ReloadShaderCache();
			AssetDB::SetShadersUniformBlock("uCamera", 1);
			scr_shader = AssetDB::ResolveShader("screen");

			AssetDB::ResolveShader("pbr")->Bind();
			AssetDB::ResolveShader("pbr")->setInt("uPrefilterMap", 12);
			AssetDB::ResolveShader("pbr")->setInt("ubrdfLUT", 0);

			glActiveTexture(GL_TEXTURE0 + 12);
			glBindTexture(GL_TEXTURE_CUBE_MAP, AssetDB::ResolveHDRTexture(envir)->prefilterMap);
			glActiveTexture(GL_TEXTURE0 + 14);
			glBindTexture(GL_TEXTURE_2D, AssetDB::ResolveHDRTexture(envir)->brdfLUTTexture);
		}

		gfx.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDepthFunc(GL_LEQUAL);


		if (editor->editor_camera->active)
		{
			editor->editor_camera->OnUpdate(delta_time, input_actions);
			editor->editor_camera->OnDraw(gfx);
		}

		auto render_target = AssetDB::ResolveFramebuffer("lambert_pass_aa");

		render_target->mode = eFramebufferMode::RENDER_TARGET;
		render_target->Bind(gfx);
		render_target->Clear(gfx);

		scene_root->Update(delta_time, input_actions);
		scene_root->CleanTree();
		scene_root->Draw(gfx);

		auto skybox_shader = AssetDB::ResolveShader("skycube");
		skybox_shader->Bind();
		skybox_shader->setInt("environmentMap", 10);
		//AssetDB::ResolveHDRTexture(envir)->Bind(gfx);
		glActiveTexture(GL_TEXTURE0 + 10);
		glBindTexture(GL_TEXTURE_CUBE_MAP, AssetDB::ResolveHDRTexture(envir)->prefilterMap);

		glCullFace(GL_BACK);
		AssetDB::ResolveStaticMesh("res/MODELS/cube.obj")->Draw(gfx);
		render_target->Unbind(gfx);

		auto post_process = AssetDB::ResolveFramebuffer("lambert_pass");
		post_process->Blit(*render_target);

		post_process->mode = eFramebufferMode::TEXTURE;
		post_process->slot = 1;
		post_process->Bind(gfx);
		
		scr_shader->Bind();
		scr_shader->setInt("uTexture1", 1);
		
		screen.Bind(gfx);
		screen.Draw(gfx);

		editor->NodePanel(scene_root);
		editor->NodeEditPanel();
		editor->TopMenu();
			
		input_actions.reset_action_state();

		editor->EndFrame();

		window->Swap();
	}
	glfwTerminate();
}

Engine::Engine()
{
	InitializeLibraries();

	window = CreateRef<Window>(config["width"], config["height"], config["name"]);

	window->SetEventCallback(BIND_EVENT_FN(Engine::OnEvent));

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize GLAD\n";
	}

	std::cout << window->data.width << " x " << window->data.height << "\n";

	SetupInternalTextures();

	editor = CreateRef<Editor>(window->window);
	editor->root = scene_root;

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

Engine::~Engine()
{

}