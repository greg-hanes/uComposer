#pragma once
namespace uComposer
{
	namespace Filters
	{
		enum BiquadMode : int
		{
			Lowpass = 0,
			Bandpass = 1,
			Highpass = 2
		};

		class Biquad
		{
		public:
			Biquad();

			void Reset();

			BiquadMode GetMode() const;
			void SetMode(BiquadMode mode);

			float GetCutoff() const;
			void SetCutoff(float value);

			float GetResonance() const;
			void SetResonance(float value);

			float Process(float in);

		private:
			BiquadMode m_mode;
			float m_cutoff;
			float m_resonance;

			float m_b0;
			float m_b1;
			float m_b2;
			float m_b3;
			float m_b4;

			float m_q;
			float m_p;
			float m_f;
		};
	}
}