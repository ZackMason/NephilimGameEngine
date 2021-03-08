#pragma once

#include <ImGUI/imgui.h>
#include <ImGUI/imgui_impl_glfw.h>
#include <ImGUI/imgui_impl_opengl3.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <functional>

#include "Engine/Node/Node.h"
#include "Engine/Node/CameraNode.h"

struct Editor
{
    static void ShowExampleMenuFile()
    {
        ImGui::MenuItem("(demo menu)", NULL, false, false);
        if (ImGui::MenuItem("New")) {}
        if (ImGui::MenuItem("Open", "Ctrl+O")) {}
        if (ImGui::BeginMenu("Open Recent"))
        {
            ImGui::MenuItem("fish_hat.c");
            ImGui::MenuItem("fish_hat.inl");
            ImGui::MenuItem("fish_hat.h");
            if (ImGui::BeginMenu("More.."))
            {
                ImGui::MenuItem("Hello");
                ImGui::MenuItem("Sailor");
                if (ImGui::BeginMenu("Recurse.."))
                {
                    ShowExampleMenuFile();
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::MenuItem("Save", "Ctrl+S")) {}
        if (ImGui::MenuItem("Save As..")) {}

        ImGui::Separator();
        if (ImGui::BeginMenu("Options"))
        {
            static bool enabled = true;
            ImGui::MenuItem("Enabled", "", &enabled);
            ImGui::BeginChild("child", ImVec2(0, 60), true);
            for (int i = 0; i < 10; i++)
                ImGui::Text("Scrolling Text %d", i);
            ImGui::EndChild();
            static float f = 0.5f;
            static int n = 0;
            ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
            ImGui::InputFloat("Input", &f, 0.1f);
            ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Colors"))
        {
            float sz = ImGui::GetTextLineHeight();
            for (int i = 0; i < ImGuiCol_COUNT; i++)
            {
                const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
                ImVec2 p = ImGui::GetCursorScreenPos();
                ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
                ImGui::Dummy(ImVec2(sz, sz));
                ImGui::SameLine();
                ImGui::MenuItem(name);
            }
            ImGui::EndMenu();
        }

        // Here we demonstrate appending again to the "Options" menu (which we already created above)
        // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
        // In a real code-base using it would make senses to use this feature from very different code locations.
        if (ImGui::BeginMenu("Options")) // <-- Append!
        {
            static bool b = true;
            ImGui::Checkbox("SomeOption", &b);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Disabled", false)) // Disabled
        {
            IM_ASSERT(0);
        }
        if (ImGui::MenuItem("Checked", NULL, true)) {}
        if (ImGui::MenuItem("Quit", "Alt+F4")) {}
    }

    Node* root = nullptr;
    CameraNode* editor_camera = nullptr;
    Node* selected_node = nullptr;

    v2 mouse_pos;

    bool OnMouseMove(MouseMovedEvent& e)
    {
        mouse_pos = { e.GetX(), e.GetY() };
        return true;
    }

    bool OnClick(MouseButtonPressedEvent& e)
    {
        Ray ray = editor_camera->camera.GetScreenRay(mouse_pos.x, mouse_pos.y);
        if (auto hit_data = root->IntersectRay(ray); hit_data.intersect)
        {
            std::cout << hit_data << "\n";
            selected_node = hit_data.node;
        }
        return true;
    }


    void TopMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                ShowExampleMenuFile();
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

    }

    void NodeEditPanel()
    {
        ImGui::Begin("NodeEditor");

        if (selected_node)
        {
            selected_node->Edit(*this);
        }

        ImGui::End();
    }

    void NodeProbe(Node* root)
    {
        if (ImGui::TreeNode(root->name.c_str()))
        {

           // root->Editor();

            if (ImGui::IsItemClicked())
                selected_node = root;

            for (auto& c : root->children)
            {
                NodeProbe(c.second);
            }

            ImGui::TreePop();
        }
    }

    void NodePanel(Node* root)
    {
        ImGui::Begin("NodePanel");

        NodeProbe(root);

        ImGui::End();
    }

    void SceneHierarchyPanel()
    {
    }

    void Scene(u32 tbo, const ImVec2& size, CameraNode* camera)
    {
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        bool opt_fullscreen = true;
        if (opt_fullscreen)
        {
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            //ImGui::SetNextWindowViewport(viewport->ID);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;
        ImGuiIO& io = ImGui::GetIO(); (void)io;


        ImGui::Begin("Scene", 0, window_flags);
        
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::Begin("test_window");
        ImGui::Text("test");
        ImGui::End();
        
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        auto render_target = AssetDB::ResolveFramebuffer("lambert_pass");
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        if ((viewportPanelSize.x > 0 && viewportPanelSize.y > 0) && 
            (viewportPanelSize.x != render_target->width || viewportPanelSize.y != render_target->height))
        {
            std::cout << "Resizing viewport " << viewportPanelSize.x << " " << viewportPanelSize.y << "\n";
            AssetDB::ResizeFramebuffers(viewportPanelSize.x, viewportPanelSize.y);
            camera->camera.width = viewportPanelSize.x;
            camera->camera.height = viewportPanelSize.y;
            camera->camera.MakeProjection();
        }
        render_target = AssetDB::ResolveFramebuffer("lambert_pass");
        ImGui::Image((ImTextureID)render_target->m_tbo, ImVec2 { (f32)render_target->width, (f32)render_target->height }, { 0,1 }, { 1,0 });
        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    void NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void EndFrame()
    {
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::Render();
        int display_w, display_h;
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Update and Render additional Platform Windows
        // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
        //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

	Editor(GLFWwindow* window)
	{
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 130");

	}

};