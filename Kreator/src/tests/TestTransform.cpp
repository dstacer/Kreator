
#include "TestTransform.h"


namespace test
{
	TestTransform::TestTransform()
	{
		m_Proj = std::make_unique<glm::mat4>(glm::ortho(-1.6f, 1.6f, -1.2f, 1.2f, -1.0f, 1.0f));
		m_Pos1 = std::make_unique<glm::vec3>(1.0f, 0.0f, 0.0f);
		m_Pos2 = std::make_unique<glm::vec3>(-1.0f, 0.0f, 0.02f);
		m_CamPos = std::make_unique<glm::vec3>(0.0f, 0.0f, -0.5f);
		m_CamRot = std::make_unique<float>(0.0f);

		float verts[] = {
			-0.6f, -0.25f,
			 0.6f, -0.25f,
			 0.6f,  0.25f,
			-0.6f,  0.25f,
		};

		unsigned int indices[6] = {
			0, 1, 2,
			2, 3, 0
		};

		m_Vao = std::make_unique<VertexArray>();
		m_Vbo = std::make_unique<VertexBuffer>(verts, 4 * 2 * sizeof(float));

		VertexBufferLayout vblayout;
		vblayout.Push<float>(2);

		m_Vao->AddBuffer(*m_Vbo, vblayout);

		m_Ibo = std::make_unique<IndexBuffer>(indices, 6);
		m_Ibo->Bind();

		m_Shader = std::make_unique<Shader>("resources/shaders/VFUniColorMVP.glsl");

	}

	void TestTransform::OnUpdate(float delta)
	{
		
	}

	void TestTransform::OnRender()
	{
		Renderer rend;
		rend.SetClearColor({ 0.4f, 0.4f, 0.4f, 1.0f });
		rend.Clear();
		

		glm::mat4 view = glm::inverse( 
			glm::translate(glm::mat4(1.0), *m_CamPos)
		  * glm::rotate(glm::mat4(1.0), glm::radians(*m_CamRot), glm::vec3(0.0f, 0.0f, 1.0f)) 
		);
		glm::mat4 model1 = glm::translate(glm::mat4(1.0), *m_Pos1);
		glm::mat4 model2 = glm::translate(glm::mat4(1.0), *m_Pos2);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", *m_Proj * view * model1);
		m_Shader->SetUniform4f("u_Color", 0.2f, 0.2f, 0.8f, 0.9f);
		rend.Draw(*m_Vao, *m_Ibo, *m_Shader);

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_MVP", *m_Proj * view * model2);
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.2f, 0.2f, 0.9f);
		rend.Draw(*m_Vao, *m_Ibo, *m_Shader);

	}

	void TestTransform::OnImguiRender()
	{
		ImGui::SliderFloat3("Position Blue", glm::value_ptr(*m_Pos1), -1.0f, 1.0f);
		ImGui::SliderFloat3("Position Red", glm::value_ptr(*m_Pos2), -1.0f, 1.0f);
		ImGui::SliderFloat3("Camera Position", glm::value_ptr(*m_CamPos), -1.0f, 1.0f);
		ImGui::SliderFloat("Camera Rotation", m_CamRot.get(), -360.0f, 360.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	}
}
