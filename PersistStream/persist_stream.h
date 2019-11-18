/*

Copyright (c) 2019 Macrobull

*/

#pragma once

#include <iostream>

class ForwardOutStreamBuf;

class PersistOutStream : public std::ostream
{
public:
	PersistOutStream();
	explicit PersistOutStream(const std::ostream& stream);
	~PersistOutStream() override;

	PersistOutStream(PersistOutStream&& xvalue) noexcept;

	std::ostream& operator=(const std::ostream& stream);

	PersistOutStream& operator=(PersistOutStream&& xvalue) noexcept;

	PersistOutStream& reset(const std::ostream& stream);

private:
	ForwardOutStreamBuf* m_streambuf{nullptr};
};
