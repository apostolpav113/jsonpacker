#include "packerstream.h"

namespace jsonpacker_stream {

JsonPackerStream::~JsonPackerStream()
{
}

JsonPackerFileStream::JsonPackerFileStream(std::ifstream &input_stream, std::ofstream &output_stream)
	: JsonPackerStream()
	, m_input_stream(input_stream)
	, m_output_stream(output_stream)
{
}

std::istream &JsonPackerFileStream::InputStream() {
	return m_input_stream;
}

std::ostream &JsonPackerFileStream::OutputStream() {
	return m_output_stream;
}

JsonPackerStringStream::JsonPackerStringStream(std::istringstream &input_stream, std::ostringstream &output_stream)
	: JsonPackerStream()
	, m_input_stream(input_stream)
	, m_output_stream(output_stream)
{
}

std::istream &JsonPackerStringStream::InputStream() {
	return m_input_stream;
}

std::ostream &JsonPackerStringStream::OutputStream() {
	return m_output_stream;
}


} // end of namespace jsonpacker_stream
