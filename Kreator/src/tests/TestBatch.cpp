#include <array>
#include "TextureArray.h"
#include "TestBatch.h"

namespace test
{

	TestBatch::TestBatch()
	{						
		unsigned int indices[MAX_INDICES];
		unsigned int offset = 0;
		for (unsigned int i = 0; i < MAX_INDICES; i+=6) 
		{
			/*
				3-----2
				|    /|
				|   / | 
				|  /  |
				| /   |
				0-----1
			*/
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			// 4 verts, 6 indices per quad -> increment offset by 4 after every 6 indices
			offset += 4;
		}

		m_Vao = std::make_unique<VertexArray>();
		m_Vbo = std::make_unique<VertexBuffer>();

		VertexBufferLayout vblayout;
		vblayout.Push<float>(3);
		vblayout.Push<float>(4); 
		vblayout.Push<float>(2);
		vblayout.Push<float>(1);

		m_Vao->AddBuffer(*m_Vbo, vblayout);

		m_Ibo = std::make_unique<IndexBuffer>(indices, MAX_INDICES); 
		m_Ibo->Bind();

		std::vector<std::string> filepaths{ "resources/textures/Kreator.png",
											"resources/textures/KreatorBW.png" };

		m_Texture = std::make_unique<TextureArray>(filepaths);
		
		m_Shader = std::make_unique<Shader>("resources/shaders/VFBatch.glsl");
		m_Shader->Bind();
		
		glm::mat4 proj = glm::ortho(-1.7778f, 1.778f, -0.9f, 0.9f, -1.0f, 1.0f);
		glm::mat4 view = glm::mat4(1.0);
		glm::mat4 mvp = proj * view;

		m_Shader->SetUniformMat4f("u_MVP", mvp);

		/*
			Arrays of samplers can only be indexed by a compile time const
			expr in GLSL 3.30.  This is why we're using TextureArray.
		*/
		m_Shader->SetUniform1i("u_TexUnit", 0);
		m_Texture->Bind(0);
	}

	void TestBatch::OnUpdate(float delta)
	{
	}

	void TestBatch::OnRender()
	{
		Renderer rend;

		std::array<Vertex, 36> vertices;
		Vertex* vertBuffer = vertices.data();
		int currQuad = 0;
		unsigned int indexCount = 0;
		for (int x = -1; x < 2; x++)
		{
			for (int y = -1; y < 2; y++) 
			{
				vertBuffer = m_Vao->CreateQuad(vertBuffer, { -0.3f + 1.2f*x, 0.75f*y, 0.0f}, { 1.2f, 0.5f }, currQuad++ % 3);
				indexCount += 6;
			}
		}
		m_Vbo->Bind();
		m_Vbo->Fill(vertices.data(), vertices.size()*sizeof(Vertex));
		
		rend.SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });
		rend.Clear();

		rend.Draw(*m_Vao, *m_Ibo, *m_Shader, indexCount);
	}

	void TestBatch::OnImguiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}
