#include "EditorApplication.h"
#include <string>
#include "PropertyInspectorPanel.h"
#include "HierarchyEntityPanel.h"
#include "imgui_demo.cpp"
#include "EngineComponents.h"
#include <assert.h>

void EditorApplication::Startup()
{
	Application::Startup();
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		// return -1;
	}

	// From 2.0.18: Enable native IME.
#ifdef SDL_HINT_IME_SHOW_UI
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

	// Create window with SDL_Renderer graphics context
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_MAXIMIZED);
	window = SDL_CreateWindow("Dear ImGui SDL2+SDL_Renderer example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1000, window_flags);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_Log("Error creating SDL_Renderer!");
		//  return 0;
	}

	//SDL_RendererInfo info;
	//SDL_GetRendererInfo(renderer, &info);
	//SDL_Log("Current SDL_Renderer: %s", info.name);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = &ImGui::GetIO(); (void)io;
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io->ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != nullptr);


	static int t_int = 4;
	static std::string t_str = "hed";
	static float t_fl = 3.5934;
	static double t_db = 34.5934;
	static double u_db = 34.5934;
	static Editor::Color4 t_col4{};
	static Editor::Color4 u_col4{};
	static bool t_bool{};
	static Editor::Vector2 t_vec2{};

	ReflVars.push_back(Reflect::Var{ &t_int, Reflect::Type::IntType, "my int var" });
	ReflVars.push_back(Reflect::Var{ &t_str, Reflect::Type::StringType, "my string var" });
	ReflVars.push_back(Reflect::Var{ &t_col4, Reflect::Type::Color4Type, "my string var" });
	ReflVars.push_back(Reflect::Var{ &u_col4, Reflect::Type::Color4Type, "my string var" });
	ReflVars.push_back(Reflect::Var{ &t_fl, Reflect::Type::FloatType, "my fl var" });
	ReflVars.push_back(Reflect::Var{ &t_db, Reflect::Type::DoubleType, "my db var" });
	ReflVars.push_back(Reflect::Var{ &u_db, Reflect::Type::DoubleType, "my db var" });
	ReflVars.push_back(Reflect::Var{ &t_bool, Reflect::Type::BoolType, "my db var" });
	ReflVars.push_back(Reflect::Var{ &t_vec2, Reflect::Type::Vec2Type, "my db var" });



};

void EditorApplication::Loop()
{

	Application::Loop();
	// Start the Dear ImGui frame
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{

		Editor::RenderProperties(ReflVars);
		s_EntityPanel.Render(EcsWorld, *TreeNodeRoot);
		s_AssetBrowser.Render();

		Editor::TreeNode* SelectedTreeNode = s_EntityPanel.s_TreeNodeRenderer.SelectedNode;
		if (SelectedTreeNode) {
			Editor::RenderInspectorPanel(&(SelectedTreeNode->AssociatedObject));
		}
		else {
			Editor::RenderInspectorPanel(nullptr);
		}
		
		
		ImGui::Begin("save and load scene");
		if (ImGui::Button("save scene")) {
			SaveScene("scene.json");
		}
		if (ImGui::Button("load scene")) {
			LoadScene("scene.json");
		}
		ImGui::End();
	}



	// Rendering
	ImGui::Render();
	SDL_RenderSetScale(renderer, io->DisplayFramebufferScale.x, io->DisplayFramebufferScale.y);
	SDL_SetRenderDrawColor(renderer, (Uint8)(clear_color.x * 255), (Uint8)(clear_color.y * 255), (Uint8)(clear_color.z * 255), (Uint8)(clear_color.w * 255));
	SDL_RenderClear(renderer);
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
};

void EditorApplication::InputLoop()
{
	Application::InputLoop();
	// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_QUIT)
			IsRunning = true;
		if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
			IsRunning = true;
	}
}
void EditorApplication::LoadScene(std::string InFilePath)
{
	s_EntityPanel.s_TreeNodeRenderer.SelectedNode = nullptr;

	//this order of resetting tree node root is very important.
	//first the tree node root needs to be deleted then set.
	//because the EcsWorld.RootEntity points to a different entity after scene is loaded.
	//so if you create the root entity first, the reference to EcsWorld.RootEntity becomes invalid after loading the scene

	TreeNodeRoot.reset();
	EcsWorld.LoadScene(InFilePath);
	TreeNodeRoot = std::make_unique<Editor::TreeNode>(*EcsWorld.RootEntity);

	ConvertEcsEntitiesToTreeNodes(*TreeNodeRoot, *EcsWorld.RootEntity);
	
};



void EditorApplication::ConvertEcsEntitiesToTreeNodes(Editor::TreeNode& RootTreeNode, Engine::Entity& RootEntity)
{
	for (auto& Ent : RootEntity.Children)
	{
		RootTreeNode.AddChild(std::make_unique<Editor::TreeNode>(*Ent));
		ConvertEcsEntitiesToTreeNodes(*RootTreeNode.Children.back(), *Ent); 
	};
};

void EditorApplication::SaveScene(std::string OutFilePath)
{

	/* ConvertTreeNodesToEcsNodes(*TreeNodeRoot);*/
	EcsWorld.SaveScene(OutFilePath);
};

EditorApplication::EditorApplication()
{

}

EditorApplication::~EditorApplication()
{
	Application::~Application();
	// Cleanup
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

};
