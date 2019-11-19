/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include "persist_stream.h"

#include <cassert>

#ifdef PERSIST_OUT_STREAM_PRIVATE_IMPL

#define PERSIST_OUT_STREAM_IMPL_STORAGE

#else

#define PERSIST_OUT_STREAM_IMPL_STORAGE inline

#endif

class ForwardOutStreamBuf : public std::streambuf
{
	std::streambuf* m_target{nullptr};

public:
	inline void reset(std::streambuf* streambuf) noexcept
	{
		m_target = streambuf;
	}

protected:
	inline int overflow(int c) override
	{
		return get_target()->sputc(static_cast<char>(c));
	}

	inline std::streamsize xsputn(const char* s, std::streamsize n) override
	{
		return get_target()->sputn(s, n);
	}

	inline std::streambuf* get_target() const
	{
		assert(m_target != nullptr && "write stream before buffer set");

		return m_target;
	}
};

PERSIST_OUT_STREAM_IMPL_STORAGE PersistOutStream::PersistOutStream() = default;

PERSIST_OUT_STREAM_IMPL_STORAGE PersistOutStream::PersistOutStream(const std::ostream& stream)
	: m_streambuf{new ForwardOutStreamBuf{}}
{
	rdbuf(m_streambuf);
	m_streambuf->reset(stream.rdbuf());
}

PERSIST_OUT_STREAM_IMPL_STORAGE PersistOutStream::~PersistOutStream()
{
	delete m_streambuf;
	m_streambuf = nullptr;
}

PERSIST_OUT_STREAM_IMPL_STORAGE
PersistOutStream::PersistOutStream(PersistOutStream&& xvalue) noexcept
	: PersistOutStream{}
{
	std::swap(m_streambuf, xvalue.m_streambuf);
}

PERSIST_OUT_STREAM_IMPL_STORAGE std::ostream& PersistOutStream::
											  operator=(const std::ostream& stream)
{
	reset(stream);
	return static_cast<std::ostream&>(*this);
}

PERSIST_OUT_STREAM_IMPL_STORAGE PersistOutStream& PersistOutStream::
												  operator=(PersistOutStream&& xvalue) noexcept
{
	m_streambuf        = xvalue.m_streambuf;
	xvalue.m_streambuf = nullptr;
	return *this;
}

PERSIST_OUT_STREAM_IMPL_STORAGE PersistOutStream&
								PersistOutStream::reset(const std::ostream& stream)
{
	if (m_streambuf == nullptr)
	{
		m_streambuf = new ForwardOutStreamBuf{};
		rdbuf(m_streambuf);
	}

	m_streambuf->reset(stream.rdbuf());
	return *this;
}
