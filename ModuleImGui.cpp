#include "Globals.h"
#include "Application.h"
#include "Math.h"
#include "ModuleImGui.h"
#include "imgui-1.51\imgui.h"
#include "imgui-1.51\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"


#pragma comment( lib, "Glew/libx86/glew32.lib" )

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

#define MAX_FPS_MS_COUNT 81

//Displays usefull information about that option
static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{}

//Load assets
bool ModuleImGui::Start()
{
	LOG("Loading Intro assets");
	AddLogToWindow("Loading Intro assets");
	bool ret = true;

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->GetWindow());

	return ret;
}

//PreUpdate
update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindow());
	return(UPDATE_CONTINUE);
}

//Update
update_status ModuleImGui::Update(float dt)
{
	//Main Menu Bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ESC"))
			{
				return UPDATE_STOP;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console"))
			{
				consoleActive = !consoleActive;
			}
			if (ImGui::MenuItem("Math Test Playground"))
			{
				mathPlaygroundActive = !mathPlaygroundActive;
			}
			if (ImGui::MenuItem("Configuration"))
			{
				configurationActive = !configurationActive;
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ImGui Demo"))
			{
				testWindowActive = !testWindowActive;
			}
			if (ImGui::MenuItem("Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/blob/master/README.md", NULL, NULL, SW_SHOWNORMAL); // This should be the wiki when we have one.
			}
			if (ImGui::MenuItem("Latest Release"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/releases", NULL, NULL, SW_SHOWNORMAL); 
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/adriancl1/AK-Engine/issues", NULL, NULL, SW_SHOWNORMAL); 
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	//-----

	ShowDebugWindow();

	if (menuActive)
	{
		ShowMenuWindow();
	}
	if (testWindowActive)
	{
		ImGui::ShowTestWindow();
	}
	if (consoleActive)
	{
		ShowConsoleWindow();
	}
	if (mathPlaygroundActive)
	{
		ShowMathWindow();
	}
	if (configurationActive)
	{
		ShowConfigurationWindow();
	}
	if (closeApp)
	{
		return UPDATE_STOP;
	}

	ImGui::Render();

	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleImGui::ShowDebugWindow(bool* p_open)
{
	static bool openMenuWindow = false;
	static bool openConsoleWindow = false;
	static bool openMathPlaygroundWindow = false;

	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Debug", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	if (ImGui::CollapsingHeader("Debug Options"))
	{
		if (ImGui::Checkbox("Show Menu", &openMenuWindow))
		{
			menuActive = !menuActive;
		}	
		if (ImGui::Checkbox("Show Console", &openConsoleWindow))
		{
			consoleActive = !consoleActive;
		}
		if (ImGui::Checkbox("Show Math Test Playground", &openMathPlaygroundWindow))
		{
			mathPlaygroundActive = !mathPlaygroundActive;
		}
	}
	ImGui::End();
}

void ModuleImGui::ShowMenuWindow(bool* p_open)
{
	static bool openTestWindow = false;
	static bool viewGrid = false;

	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("ImGui Menu", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("ImGui Menu");

	if (ImGui::CollapsingHeader("Options"))
	{
		ImGui::TextWrapped("Options Menu");

		if (ImGui::Checkbox("Show Demo", &openTestWindow))
		{
			testWindowActive = !testWindowActive;
		}
	
		if (ImGui::TreeNode("Graphic Options"))
		{
			ImGui::Text("Not functional");
			if (ImGui::Checkbox("View ---", &viewGrid))
			{
			}

			ImGui::TreePop();
		}
	}

	if (ImGui::Button("Close App", ImVec2(300,20)))
	{
		closeApp = true;
	}
	ImGui::End(); 
}

void ModuleImGui::ShowConsoleWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Console", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	//ImGui::Text("%s", consoleText);

	if (ImGui::Button("Clear"))
	{
		consoleText.clear();
	}

	for (int i = consoleText.size() - 1; i >= 0; i--)
	{
		ImGui::Text("%s", consoleText[i].c_str());
	}

	ImGui::End();
}

void ModuleImGui::AddLogToWindow(std::string toAdd)
{
	consoleText.push_back(toAdd);
}

void ModuleImGui::ShowMathWindow(bool* p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Math Library Tests", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("Fill the following parameters:");

	if (ImGui::CollapsingHeader("Intersection Sphere-Sphere"))
	{
		ImGui::Text("Sphere 1:");
		ImGui::InputInt("Radius", &sphereRadius);
		ImGui::InputInt("X", &sphereX);
		ImGui::InputInt("Y", &sphereY);
		ImGui::InputInt("Z", &sphereZ);

		ImGui::Text("Sphere 2:");
		ImGui::InputInt("Radius 2", &sphereRadius2);
		ImGui::InputInt("X 2", &sphereX2);
		ImGui::InputInt("Y 2", &sphereY2);
		ImGui::InputInt("Z 2", &sphereZ2);


		if (ImGui::Button("Run Math Test"))
		{
			math::Sphere sphere1(float3(sphereX, sphereY, sphereZ), sphereRadius);
			math::Sphere sphere2(float3(sphereX2, sphereY2, sphereZ2), sphereRadius2);

			intersects = sphere1.Intersects(sphere2);

			if (intersects)
			{
				intersectsTrue = true;
				intersectsFalse = false;
			}
			else
			{
				intersectsTrue = false;
				intersectsFalse = true;
			}
		}

		if (intersectsTrue)
		{
			ImGui::Text("True");
		}
		if (intersectsFalse)
		{
			ImGui::Text("False");
		}
	}

	if (ImGui::CollapsingHeader("Intersection Sphere-Capsule"))
	{
		ImGui::Text("Sphere 1:");
		ImGui::InputInt("Sphere 1 Radius", &sphereRadius);
		ImGui::InputInt("X", &sphereX);
		ImGui::InputInt("Y", &sphereY);
		ImGui::InputInt("Z", &sphereZ);

		ImGui::Text("Capsule 1:");
		ImGui::InputInt("Capsule 1 Radius", &capsuleRadius);
		ImGui::InputInt("Bottom X", &capsuleBotX);
		ImGui::InputInt("Bottom Y", &capsuleBotY);
		ImGui::InputInt("Bottom Z", &capsuleBotZ);
		ImGui::InputInt("Top X", &capsuleTopX);
		ImGui::InputInt("Top Y", &capsuleTopY);
		ImGui::InputInt("Top Z", &capsuleTopZ);

		if (ImGui::Button("Run Math Test"))
		{
			math::Sphere sphere1(float3(sphereX, sphereY, sphereZ), sphereRadius);
			math::Capsule capsule1(float3(capsuleBotX, capsuleBotY, capsuleBotZ), float3(capsuleTopX, capsuleTopY, capsuleTopZ), capsuleRadius);

			intersects = sphere1.Intersects(capsule1);

			if (intersects)
			{
				intersectsTrue = true;
				intersectsFalse = false;
			}
			else
			{
				intersectsTrue = false;
				intersectsFalse = true;
			}
		}
		if (intersectsTrue)
		{
			ImGui::Text("True");
		}
		if (intersectsFalse)
		{
			ImGui::Text("False");
		}
	}

	if (ImGui::Button("Reset"))
	{
		//Properties Sphere 1
		sphereRadius = 0;
		sphereX = 0;
		sphereY = 0;
		sphereZ = 0;

		//Sphere 2
		sphereRadius2 = 0;
		sphereX2 = 0;
		sphereY2 = 0;
		sphereZ2 = 0;

		//Capsule 1
		capsuleRadius = 0;
		capsuleBotX = 0;
		capsuleBotY = 0;
		capsuleBotZ = 0;
		capsuleTopX = 0;
		capsuleTopY = 0;
		capsuleTopZ = 0;

		//Booleans
		intersects = false;
		intersectsTrue = false;
		intersectsFalse = false;
	}


	ImGui::End();
}


void ModuleImGui::ShowConfigurationWindow(bool* p_open)
{
	if (!ImGui::Begin("Configuration", p_open))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("Options");

	if (ImGui::CollapsingHeader("Application"))
	{
		CycleFPSAndMsData(App->GetFPS(), App->GetMs());
		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", FPSData[FPSData.size() - 1]);
		ImGui::PlotHistogram("##framerate", &FPSData[0], FPSData.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", MsData[MsData.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &MsData[0], MsData.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
	}
	if (ImGui::CollapsingHeader("Window"))
	{

	}
	if (ImGui::CollapsingHeader("File System"))
	{

	}
	if (ImGui::CollapsingHeader("Input"))
	{

	}
	if (ImGui::CollapsingHeader("Hardware"))
	{

	}

	ImGui::End();
}

void ModuleImGui::CycleFPSAndMsData(float fps, float ms)
{
	//FPS
	if (FPSData.size() >= MAX_FPS_MS_COUNT)
	{
		for (int i = 0; i < MAX_FPS_MS_COUNT - 2; i++)
		{
			FPSData[i] = FPSData[i + 1];
		}
		FPSData[MAX_FPS_MS_COUNT - 1] = fps;
	}
	else
	{
			FPSData.push_back(fps);
	}

	//MS
	if (MsData.size() >= MAX_FPS_MS_COUNT)
	{
		for (int i = 0; i < MAX_FPS_MS_COUNT - 2; i++)
		{
			MsData[i] = MsData[i + 1];
		}
		MsData[MAX_FPS_MS_COUNT - 1] = ms;
	}
	else
	{
		MsData.push_back(ms);
	}
}