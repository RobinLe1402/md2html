#include "MemoryStream.hpp"

#include <streambuf>

namespace rlUtils
{
	MemoryStream::Buffer::Buffer(const char* pData, size_t iLen)
	{
		char *p = const_cast<char*>(pData);
		this->setg(p, p, p + iLen);
	}

	std::streampos MemoryStream::Buffer::seekoff(
		std::streamoff off, std::ios_base::seekdir dir, std::ios_base::openmode which
	)
	{
		switch (dir)
		{
		case std::ios_base::cur:
			gbump(static_cast<int>(off));
			break;

		case std::ios_base::end:
			setg(eback(), egptr() + off, egptr());
			break;

		case std::ios_base::beg:
			setg(eback(), eback() + off, egptr());
			break;
		}

		return gptr() - eback();
	}

	std::streampos MemoryStream::Buffer::seekpos(
		std::streampos sp, std::ios_base::openmode which
	)
	{
		return seekoff(sp, std::ios_base::beg, which);
	}





	MemoryStream::MemoryStream(const char *pData, size_t iLen) :
		m_oBuf(pData, iLen),
		std::istream(&m_oBuf)
	{

	}

}
