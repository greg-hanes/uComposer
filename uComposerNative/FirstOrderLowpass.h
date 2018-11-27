#pragma once

namespace uComposer
{
	namespace Filters
	{
		class FirstOrderLowpass
		{
		public:
			FirstOrderLowpass() :
				m_x1(0.0f),
				m_x2(0.0f),
				m_a0(0.5f),
				m_a1(0.5f),
				m_a2(0.0f),
				m_a3(0.0f)
			{

			}

			void Reset()
			{
				m_x1 = 0.0f;
				m_x2 = 0.0f;
				m_x3 = 0.0f;
			}

			float Process(float in)
			{
				// Second order feed forward:
				float val = in * m_a0 + m_x1 * m_a1 + m_a2 * m_x2 + m_a3 * m_x3;

				m_x3 = m_x2;
				m_x2 = m_x1;
				m_x1 = in;


				return val;

				// Feedback:
				//float x = in * m_a0;
				//float val = m_lastInput * -m_a1;
				//m_lastInput = x;
				//return val;

				// Feed forward:
				//float val = in * m_a0 + m_a1 * m_lastInput;
				//m_lastInput = in;
				//return val;
			}

		
			float m_a0;
			float m_a1;
			float m_a2;
			float m_a3;
		private:
			float m_x1 = 0.0f;
			float m_x2 = 0.0f;
			float m_x3 = 0.0f;
		};
	}
}