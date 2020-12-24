#pragma once

#include "Test.h"

namespace test 
{
	class TestStaticBatch : public Test
	{
	private:
		
	public:
		TestStaticBatch();
		~TestStaticBatch();

		virtual void OnUpdate(float delta) override;
		virtual void OnRender() override;
		virtual void OnImguiRender() override;
	};

}