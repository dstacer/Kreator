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
		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++) 
			{
				BatchRenderer::AddQuad({ -0.3f + 1.2f*x, 0.75f*y, 0.0f}, { 1.2f, 0.5f }, 
					{ 1.0f, 1.0f, 1.0f, 1.0f }, currQuad++ % 3);
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
