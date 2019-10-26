#include "Application.h"
#include "WindowAbout.h"


WindowAbout::WindowAbout() : WindowEngine() {}

WindowAbout::~WindowAbout() {}

bool WindowAbout::Draw()
{
	ImGuiWindowFlags aboutFlags = 0;
	aboutFlags |= ImGuiWindowFlags_NoFocusOnAppearing;

	if (ImGui::Begin("About", &enabled, aboutFlags))
	{
		// Engine description
		ImGui::Text("Engine");
		ImGui::Spacing();
		ImGui::TextWrapped("An engine done by two students from CITM, UPC, Terrassa (Barcelona).");
		if (ImGui::Button("Engine")) { ShellExecute(0, 0, "https://github.com/Beautiful-Engines/Engine", 0, 0, SW_SHOW); }
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Authors
		ImGui::Text("Authors");
		ImGui::Spacing();
		if (ImGui::Button("Cere Venteo")) { ShellExecute(0, 0, "https://github.com/CereVenteo", 0, 0, SW_SHOW);	}
		if (ImGui::Button("Alejandro París")) { ShellExecute(0, 0, "https://github.com/AlejandroParis", 0, 0, SW_SHOW);	}
		ImGui::Spacing();
		if (ImGui::Button("Beautiful Engines")) { ShellExecute(0, 0, "https://github.com/Beautiful-Engines", 0, 0, SW_SHOW); }
		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// Libraries
		ImGui::Text("Libraries");
		ImGui::Spacing();

		SDL_version version;
		SDL_GetVersion(&version);
		if (ImGui::Button("SDL")) { ShellExecute(0, 0, "https://www.libsdl.org/", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("%i.%i.%i", version.major, version.minor, version.patch);

		if (ImGui::Button("Glew")) { ShellExecute(0, 0, "http://glew.sourceforge.net/", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("%s", glewGetString(GLEW_VERSION));

		if (ImGui::Button("ImGui")) { ShellExecute(0, 0, "https://github.com/ocornut/imgui", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("%s", ImGui::GetVersion());

		if (ImGui::Button("MathGeoLib")) { ShellExecute(0, 0, "https://github.com/juj/MathGeoLib", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("1.5");

		if (ImGui::Button("OpenGL")) { ShellExecute(0, 0, "https://www.opengl.org/", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("%s", glGetString(GL_VERSION));

		if (ImGui::Button("PCG Random Number Generator")) { ShellExecute(0, 0, "http://www.pcg-random.org/", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("0.98");

		if (ImGui::Button("JSON for modern C++")) { ShellExecute(0, 0, "https://github.com/nlohmann/json", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("3.7.0");

		if (ImGui::Button("Open-Asset-Importer-Lib")) { ShellExecute(0, 0, "http://www.assimp.org/", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("5.0.0");

		if (ImGui::Button("DevIL")) { ShellExecute(0, 0, "http://openil.sourceforge.net", 0, 0, SW_SHOW); }
		ImGui::SameLine(); ImGui::TextWrapped("1.8");

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		// License
		ImGui::TextWrapped("MIT License");
		ImGui::TextWrapped("Copyright (c) 2019 Beautiful-Engines");
		ImGui::TextWrapped("Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the %cSoftware%c), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:", 34, 34);
		ImGui::TextWrapped("The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.", 34, 34);
		ImGui::TextWrapped("THE SOFTWARE IS PROVIDED %cAS IS%c, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.", 34, 34);
	}
	ImGui::End();

	return true;
}