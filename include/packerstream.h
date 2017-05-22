#ifndef PACKERSTREAM_H
#define PACKERSTREAM_H

#include <fstream>
#include <sstream>

namespace jsonpacker_stream {

/**
 * @brief The JsonPackerStream class is a base class for streams used in packer classes
 */
class JsonPackerStream {
public:
	virtual ~JsonPackerStream();
	/**
	 * @brief InputStream virtual function that allows packer classes to work with input stream; must be overridden in derived classes
	 * @return reference to std::istream
	 */
	virtual std::istream& InputStream() = 0;
	/**
	 * @brief OutputStream virtual function that allows packer classes to work with output stream; must be overridden in derived classes
	 * @return
	 */
	virtual std::ostream& OutputStream() = 0;
};


/**
 * @brief The JsonPackerFileStream class allows packer classes to work with files
 */
class JsonPackerFileStream : public JsonPackerStream {
public:
	/**
	 * @brief JsonPackerFileStream constructor
	 * @param input_stream[in] reference to opened input file stream
	 * @param output_stream[in] reference to opened output file stream
	 */
	JsonPackerFileStream(std::ifstream& input_stream, std::ofstream& output_stream);

	/**
	 * @brief InputStream provides an access to input file stream
	 * @return reference to std::istream for input file stream
	 */
	std::istream &InputStream() override;
	/**
	 * @brief OutputStream provides an access to output file stream
	 * @return reference to std::ostream for output file stream
	 */
	std::ostream &OutputStream() override;
private:
	std::ifstream& m_input_stream; ///input file stream
	std::ofstream& m_output_stream; ///output file stream
};

/**
 * @brief The JsonPackerStringStream class allows packer classes to work with string streams
 */
class JsonPackerStringStream : public JsonPackerStream {
public:
	/**
	 * @brief JsonPackerStringStream constructor
	 * @param input_stream reference to input string stream
	 * @param output_stream reference to output string stream
	 */
	JsonPackerStringStream(std::stringstream &input_stream, std::stringstream &output_stream);

	/**
	 * @brief InputStream provides an access to intput string stream
	 * @return reference to std::istream for intput string stream
	 */
	std::istream &InputStream() override;
	/**
	 * @brief OutputStream provides an access to outtput string stream
	 * @return reference to std::istream for outtput string stream
	 */
	std::ostream &OutputStream() override;
private:
	std::stringstream& m_input_stream;/// input string stream
	std::stringstream& m_output_stream;/// output string stream
};

} // end of namespace jsonpacker_stream
#endif // PACKERSTREAM_H
