#include "Note.h"

namespace uComposer
{
	const char *KeyNames[12 * 9] =
	{
		"C0", "C0_Sharp", "D0", "D0_Sharp", "E0", "F0", "F0_Sharp", "G0", "G0_Sharp", "A0", "A0_Sharp", "B0",
		"C1", "C1_Sharp", "D1", "D1_Sharp", "E1", "F1", "F1_Sharp", "G1", "G1_Sharp", "A1", "A1_Sharp", "B1",
		"C2", "C2_Sharp", "D2", "D2_Sharp", "E2", "F2", "F2_Sharp", "G2", "G2_Sharp", "A2", "A2_Sharp", "B2",
		"C3", "C3_Sharp", "D3", "D3_Sharp", "E3", "F3", "F3_Sharp", "G3", "G3_Sharp", "A3", "A3_Sharp", "B3",
		"C4", "C4_Sharp", "D4", "D4_Sharp", "E4", "F4", "F4_Sharp", "G4", "G4_Sharp", "A4", "A4_Sharp", "B4",
		"C5", "C5_Sharp", "D5", "D5_Sharp", "E5", "F5", "F5_Sharp", "G5", "G5_Sharp", "A5", "A5_Sharp", "B5",
		"C6", "C6_Sharp", "D6", "D6_Sharp", "E6", "F6", "F6_Sharp", "G6", "G6_Sharp", "A6", "A6_Sharp", "B6",
		"C7", "C7_Sharp", "D7", "D7_Sharp", "E7", "F7", "F7_Sharp", "G7", "G7_Sharp", "A7", "A7_Sharp", "B7",
		"C8", "C8_Sharp", "D8", "D8_Sharp", "E8", "F8", "F8_Sharp", "G8", "G8_Sharp", "A8", "A8_Sharp", "B8",
	};

	Note::Note()
	{

	}

	Note::Note(Key_t key, float velocity, float tuning) :
		m_key(key),
		m_velocity(velocity),
		m_tuning(tuning)
	{

	}

	Note::Note(Keys key, float velocity, float tuning) :
		m_key((Key_t)key),
		m_velocity(velocity),
		m_tuning(tuning)
	{

	}

	Key_t Note::GetKey() const
	{
		return m_key;
	}

	float Note::GetVelocity() const
	{
		return m_velocity;
	}

	float Note::GetTuning() const
	{
		return m_tuning;
	}

	void Note::SetKey(Key_t key)
	{
		m_key = key;
	}

	void Note::SetVelocity(float velocity)
	{
		m_velocity = velocity;
	}

	void Note::SetTuning(float tuning)
	{
		m_tuning = tuning;
	}
}