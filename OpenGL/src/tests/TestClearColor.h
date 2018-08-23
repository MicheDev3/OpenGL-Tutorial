#pragma once

#include "Test.h"

namespace test {

	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor() override;

		void OnRender() override;
		void OnImGuiRender() override;

		void OnUpdate(float deltaTime) override;

	private:
		float m_ClearColor[4];
	};
}

