#ifndef PACKERSTREAM_H
#define PACKERSTREAM_H

#include <fstream>
#include <sstream>

namespace jsonpacker_stream {

class JsonPackerStream {
public:
	virtual ~JsonPackerStream();
	virtual std::istream& InputStream() = 0;
	virtual std::ostream& OutputStream() = 0;
};

class JsonPackerFileStream : public JsonPackerStream {
public:
	JsonPackerFileStream(std::ifstream& input_stream, std::ofstream& output_stream);

	std::istream &InputStream() override;
	std::ostream &OutputStream() override;
private:
	std::ifstream& m_input_stream;
	std::ofstream& m_output_stream;
};

class JsonPackerStringStream : public JsonPackerStream {
public:
	JsonPackerStringStream(std::istringstream& input_stream, std::ostringstream& output_stream);

	std::istream &InputStream() override;
	std::ostream &OutputStream() override;
private:
	std::istringstream& m_input_stream;
	std::ostringstream& m_output_stream;
};

} // end of namespace jsonpacker_stream
#endif // PACKERSTREAM_H
