#ifndef ROBINLE_MD_TO_HTML_LINEBUFFER
#define ROBINLE_MD_TO_HTML_LINEBUFFER





#include <istream>
#include <vector>
#include <string>



class LineBuffer
{
private: // variables

    mutable bool m_bFullyRead = false;
    mutable std::vector<std::string> m_oBuffer;


private: // methods

    bool fillBuffer(bool bPeek) const;


protected: // methods

    virtual bool readLine(std::string &dest) const = 0;


public: // methods

    LineBuffer(const std::vector<std::string> &buffer);
    LineBuffer(std::vector<std::string> &&buffer);
    LineBuffer() = default;
    virtual ~LineBuffer() = default;

    const std::string &current() const;

    bool eof() const;
    bool hasNext() const;

    std::string popCurrent();
    bool next();

    const std::string &peekNext();

};

class LineBuffer_Prebuffered final : public LineBuffer
{
protected: // methods

    bool readLine(std::string &dest) const override;


public: // methods

    LineBuffer_Prebuffered(const std::vector<std::string> &buffer);
    LineBuffer_Prebuffered(std::vector<std::string> &&buffer);
    ~LineBuffer_Prebuffered() = default;

};

class LineBuffer_InputStream : public LineBuffer
{
private: // variables

    std::istream &m_oInput;


protected: // methods

    bool readLine(std::string &dest) const override;


public: // methods

    LineBuffer_InputStream(std::istream &input);
    ~LineBuffer_InputStream() = default;

};





#endif // ROBINLE_MD_TO_HTML_LINEBUFFER