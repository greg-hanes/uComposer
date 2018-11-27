#pragma once

#include <cstdint>

namespace uComposer
{
	typedef uint8_t Key_t;

	enum class Keys : Key_t
	{
		C0, C0_Sharp, D0, D0_Sharp, E0, F0, F0_Sharp, G0, G0_Sharp, A0, A0_Sharp, B0,
		C1, C1_Sharp, D1, D1_Sharp, E1, F1, F1_Sharp, G1, G1_Sharp, A1, A1_Sharp, B1,
		C2, C2_Sharp, D2, D2_Sharp, E2, F2, F2_Sharp, G2, G2_Sharp, A2, A2_Sharp, B2,
		C3, C3_Sharp, D3, D3_Sharp, E3, F3, F3_Sharp, G3, G3_Sharp, A3, A3_Sharp, B3,
		C4, C4_Sharp, D4, D4_Sharp, E4, F4, F4_Sharp, G4, G4_Sharp, A4, A4_Sharp, B4,
		C5, C5_Sharp, D5, D5_Sharp, E5, F5, F5_Sharp, G5, G5_Sharp, A5, A5_Sharp, B5,
		C6, C6_Sharp, D6, D6_Sharp, E6, F6, F6_Sharp, G6, G6_Sharp, A6, A6_Sharp, B6,
		C7, C7_Sharp, D7, D7_Sharp, E7, F7, F7_Sharp, G7, G7_Sharp, A7, A7_Sharp, B7,
		C8, C8_Sharp, D8, D8_Sharp, E8, F8, F8_Sharp, G8, G8_Sharp, A8, A8_Sharp, B8,
	};

	const char *KeyNames[];

	// Represents a Note. This includes the key, velocity, and tuning, but does not
	// include any timing information.
	class Note
	{
	public:
		Note();
		Note(Key_t key, float velocity, float tuning = 0.0f);
		Note(Keys key, float velocity, float tuning = 0.0f);

		Key_t GetKey() const;
		float GetVelocity() const;
		float GetTuning() const;

		void SetKey(Key_t key);
		void SetVelocity(float velocity);
		void SetTuning(float tuning);

		friend bool operator==(const Note& lhs, const Note &rhs)
		{
			return  lhs.m_key == rhs.m_key
				&& lhs.m_velocity == rhs.m_velocity
				&& lhs.m_tuning == rhs.m_tuning;
		}

		friend bool operator!=(const Note& lhs, const Note &rhs)
		{
			return !(lhs == rhs);
		}

	private:
		Key_t m_key;
		float m_velocity;
		float m_tuning;
	};
}