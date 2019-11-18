/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <chrono>
#include <cstdint>
#include <string>

template <typename UnitType = std::chrono::milliseconds>
class TicToc
{
public:
	using log_level_t = int32_t;

	TicToc(const char* file, int line, std::string name, log_level_t log_level = 0);
	~TicToc();

	TicToc(const TicToc&) = delete;
	TicToc(TicToc&& xvalue) noexcept;

	TicToc& operator=(const TicToc&) = delete;

	inline void synchronize() const
	{
#ifdef CUDARTAPI
		cudaStreamSynchronize(0);
#endif
	}

private:
	const char* const m_file{nullptr};
	const int         m_line{};
	const std::string m_name;
	const log_level_t m_log_level{};

	const std::chrono::time_point<std::chrono::system_clock> m_start_time;
};

#define TIC_TOC_VARNAME_CONCAT(base, line) base##line
#define TIC_TOC_VARNAME(line) TIC_TOC_VARNAME_CONCAT(tic_, line)
#define TIC(...) TicToc<> TIC_TOC_VARNAME(__LINE__) { __FILE__, __LINE__, __VA_ARGS__ }
#define TIC_MS(...) TicToc<std::chrono::milliseconds> \
	TIC_TOC_VARNAME(__LINE__) { __FILE__, __LINE__, __VA_ARGS__ }
#define TIC_US(...) TicToc<std::chrono::microseconds> \
	TIC_TOC_VARNAME(__LINE__) { __FILE__, __LINE__, __VA_ARGS__ }
#define TIC_NS(...) TicToc<std::chrono::nanoseconds> \
	TIC_TOC_VARNAME(__LINE__) { __FILE__, __LINE__, __VA_ARGS__ }
