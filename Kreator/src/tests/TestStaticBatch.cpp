#include "TestStaticBatch.h"

namespace test
{

	TestStaticBatch::TestStaticBatch()
	{							
		BatchRenderer::Init();
	}

	TestStaticBatch::~TestStaticBatch()
	{
		BatchRenderer::Shutdown();
	}

	void TestStaticBatch::OnUpdate(float delta)
	{
	}

	void TestStaticBatch::OnRender()
	{
		BatchRenderer::BeginFrame();

		BatchRenderer::BeginBatch();
		
		int currQuad = 0;
		for (float y = -330.f; y < 330.f; y+=10.f)
		{
			for (float x = -600.f; x < 600.f; x+=10.f) 
			{
				glm::vec4 color = { (x + 600) / 1200.f, 0.2f, (y + 300) / 600.f, 1.0f };
				BatchRenderer::AddQuad({ x, y, 0.0f}, { 9.f, 9.f }, color, 0);
			}
		}

		BatchRenderer::EndBatch();
		BatchRenderer::Flush();
	}

	void TestStaticBatch::OnImguiRender()
	{
		ImGui::Begin("Stats");
		ImGui::Text("Quads: %d", BatchRenderer::GetStats().quadCount);
		ImGui::Text("Draw Calls: %d", BatchRenderer::GetStats().drawCount);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}
}
