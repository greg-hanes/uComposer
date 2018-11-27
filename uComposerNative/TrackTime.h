#pragma once

#include <cstdint>
#include <cmath>

namespace uComposer
{
	const double kEpsilon = 1e-9;

	struct TrackTime
	{
	public:
		static const TrackTime Zero;
		static const int64_t kTicksPerBeat;
		static const int64_t kTicksPerQuarterBeat;
		static const int64_t kTicksPerSixteenthBeat;


		static TrackTime FromSeconds(double seconds, double bpm);
		static TrackTime GetBeat(TrackTime b0, double v0, TrackTime b1, double v1, double t);
		static double GetTime(TrackTime b0, double v0, TrackTime b1, double v1, TrackTime b);

		TrackTime();
		explicit TrackTime(int64_t ticks);
		TrackTime(int64_t beats, int64_t quarterBeats);
		TrackTime(int64_t beats, int64_t quarterBeats, int64_t sixteenthBeats);

		int64_t Beats() const;
		int64_t Ticks() const;
		double TotalBeats() const;
		int64_t TotalTicks() const;
		double ToSeconds(double bpm) const;



		TrackTime &operator+=(const TrackTime &rhs)
		{
			this->m_ticks += rhs.m_ticks;
			return *this;
		}

		TrackTime &operator-=(const TrackTime &rhs)
		{
			this->m_ticks -= rhs.m_ticks;
			return *this;
		}

		TrackTime &operator*=(const TrackTime &rhs)
		{
			this->m_ticks *= rhs.m_ticks;
			return *this;
		}

		TrackTime &operator/=(const TrackTime &rhs)
		{
			this->m_ticks /= rhs.m_ticks;
			return *this;
		}

		TrackTime &operator%=(const TrackTime &rhs)
		{
			this->m_ticks %= rhs.m_ticks;
			return *this;
		}

		friend TrackTime operator+(TrackTime lhs, const TrackTime &rhs)
		{
			lhs += rhs;
			return lhs;
		}

		friend TrackTime operator-(TrackTime lhs, const TrackTime &rhs)
		{
			lhs -= rhs;
			return lhs;
		}

		friend TrackTime operator*(TrackTime lhs, const TrackTime &rhs)
		{
			lhs *= rhs;
			return lhs;
		}

		friend TrackTime operator/(TrackTime lhs, const TrackTime &rhs)
		{
			lhs /= rhs;
			return lhs;
		}

		friend TrackTime operator%(TrackTime lhs, const TrackTime &rhs)
		{
			lhs %= rhs;
			return lhs;
		}

		friend bool operator<(const TrackTime &lhs, const TrackTime &rhs)
		{
			return lhs.m_ticks < rhs.m_ticks;
		}

		friend bool operator>(const TrackTime &lhs, const TrackTime &rhs)
		{
			return lhs.m_ticks > rhs.m_ticks;
		}

		friend bool operator<=(const TrackTime &lhs, const TrackTime &rhs)
		{
			return !(lhs > rhs);
		}

		friend bool operator>=(const TrackTime &lhs, const TrackTime &rhs)
		{
			return !(lhs < rhs);
		}

		friend bool operator==(const TrackTime &lhs, const TrackTime &rhs)
		{
			return lhs.m_ticks == rhs.m_ticks;
		}

		friend bool operator!=(const TrackTime &lhs, const TrackTime &rhs)
		{
			return !(lhs == rhs);
		}

	private:
		int64_t m_ticks;
	};
}