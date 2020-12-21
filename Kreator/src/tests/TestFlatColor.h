#pragma once

#include "Test.h"

namespace test 
{
	class TestFlatColor : public Test
	{
	private:

	public:
		TestFlatColor();
		virtual ~TestFlatColor();

		virtual void OnUpdate(float delta) override;
		virtual void OnRender() override;
		virtual void OnImguiRender() override;
	};

}