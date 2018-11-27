#pragma once

namespace uComposer
{
	namespace Filters
	{
		class SecondOrderButterworthBPF
		{
		public:
			void Reset();
			void SetBandwidth(double bw);
			void SetFrequency(double freq);
			double Process(double in);
		private:
			void UpdateCoefficients();

			// Parameters
			double m_bw = 0;
			double m_freq = 0;

			// Old samples.
			double xn1 = 0;
			double xn2 = 0;
			double yn1 = 0;
			double yn2 = 0;

			// Constants
			double a0;
			double a1;
			double a2;
			double b1;
			double b2;
		};
	}
}