/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include "tic_toc.hpp"

#include <typeinfo>

#include <glog/logging.h>

#ifdef TIC_TOC_PRIVATE_IMPL

#define TIC_TOC_IMPL_NS
#define TIC_TOC_IMPL_NS_
#define TIC_TOC_IMPL_STORAGE

#else

#define TIC_TOC_IMPL_NS detail
#define TIC_TOC_IMPL_NS_ detail::
#define TIC_TOC_IMPL_STORAGE inline

#endif

namespace TIC_TOC_IMPL_NS
{

template <typename T>
struct UnitName
{
	static constexpr auto value = typeid(T).name();
};

template <>
struct UnitName<std::chrono::seconds>
{
	static constexpr auto value = "s";
};

template <>
struct UnitName<std::chrono::milliseconds>
{
	static constexpr auto value = "ms";
};

template <>
struct UnitName<std::chrono::microseconds>
{
	static constexpr auto value = "us";
};

template <>
struct UnitName<std::chrono::nanoseconds>
{
	static constexpr auto value = "ns";
};

} // namespace TIC_TOC_IMPL_NS

template <typename UnitType>
TicToc<UnitType>::TicToc(const char* file, int line, std::string name, log_level_t log_level)
	: m_file{file}
	, m_line{line}
	, m_name{std::move(name)}
	, m_log_level{log_level}
	, m_start_time{std::chrono::system_clock::now()}
{}

template <typename UnitType>
TicToc<UnitType>::~TicToc()
{
	if (VLOG_IS_ON(m_log_level))
	{
		synchronize();

		auto end_time = std::chrono::system_clock::now();

		google::LogMessage(m_file, m_line, google::INFO).stream()
				<< "TicToc[" << m_name << "]: "
				<< std::chrono::duration_cast<UnitType>(end_time - m_start_time).count()
				<< TIC_TOC_IMPL_NS_ UnitName<UnitType>::value;
	}
}

template <typename UnitType>
TicToc<UnitType>::TicToc(TicToc<UnitType>&& xvalue) noexcept = default;
