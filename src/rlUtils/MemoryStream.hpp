#ifndef RLUTILS_MEMORYSTREAM
#define RLUTILS_MEMORYSTREAM





#include <cstdint>
#include <istream>
#include <streambuf>



namespace rlUtils
{

	/// <summary>
	/// A stream for reading fixed-length data from a buffer in memory.<para/>
	/// The pointed-to data is not copied, so make sure to keep the data valid for as long as you
	/// use the <c>MemoryStream</c> object.
	/// </summary>
	class MemoryStream : public std::istream
	{
	public: // methods

		MemoryStream(const char *pData, size_t iLen);
		~MemoryStream() = default;


	private: // types

		class Buffer : public std::streambuf
		{
		public: // methods

			Buffer(const char* pData, size_t iLen);
			

		protected: // methods

			// these methods must be implemented in order for seekg() and tellg() to work
			// properly.

			std::streampos seekoff(
				std::streamoff off,
				std::ios_base::seekdir dir,
				std::ios_base::openmode which
			) override;

			std::streampos seekpos(std::streampos sp, std::ios_base::openmode which) override;

		};


	private: // variables

		Buffer m_oBuf;

	};

}





#endif // RLUTILS_MEMORYSTREAM