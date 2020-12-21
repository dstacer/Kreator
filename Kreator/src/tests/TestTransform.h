#pragma once

#include "Test.h"

namespace test 
{
	class TestTransform : public Test
	{
	private:
		std::unique_ptr<Texture> m_Texture;
		std::unique_ptr<glm::vec3> m_Pos1;
		std::unique_ptr<glm::vec3> m_Pos2;
		std::unique_ptr<glm::vec3> m_CamPos;
		std::unique_ptr<float> m_CamRot;
		std::unique_ptr<glm::mat4> m_Proj;

		
	public:
		TestTransform();

		virtual void OnUpdate(float delta) override;
		virtual void OnRender() override;
		virtual void OnImguiRender() override;
	};

}