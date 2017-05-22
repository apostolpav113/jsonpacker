/**
  @file
  @brief The header file with description of classes used in coding data in JSON and TLV format
  **/

#ifndef CODER_H
#define CODER_H

#include <memory>
#include <map>
#include <vector>
#include "rapidjson/document.h"
#include "rapidjson/error/error.h"
#include "rapidjson/error/en.h"
#include "packerstream.h"
#include "apperror.h"

namespace jsonpacker_coder {

using namespace jsonpacker_stream;

/**
  @defgroup JSONPACKER_CODER_CLASSES JSON packer coder classes
  @{
  **/

/**
 * @brief The JsonKeyDictionary class represents JSON key dictionary
 */
class JsonKeyDictionary {
public:
	using Ptr = std::shared_ptr<JsonKeyDictionary>;
	/**
	 * @brief AddKey adds key to dictionary and gives it a unique index
	 * @param key[in] the key name
	 * @return the index of the key
	 */
	int AddKey(const std::string& key);
	/**
	 * @brief AddKey adds key with the specified index to dictionary
	 * @param key[in] the key name
	 * @param index[in] the index value of the key
	 */
	void AddKey(const std::string& key, int index);
	/**
	 * @brief operator [] retrieves key by its index
	 * @param index[in] the index of the key
	 * @return
	 */
	std::string operator [] (int index);
	/**
	 * @brief Clear removes all keys from dictionary
	 */
	void Clear();
	/**
	 * @brief Keys returns map with key-index pairs
	 * @return map with key-index pairs
	 */
	std::map<std::string, int>& Keys() {return m_keys;}
private:
	int m_current {0};
	std::map<std::string, int> m_keys;
};

/**
 * @brief The JsonPackerBase class is the abstract base class for coders
 */
class JsonPackerBase {
public:
	using Ptr = std::shared_ptr<JsonPackerBase>;
	virtual ~JsonPackerBase();
	/**
	 * @brief Run start coding process
	 * @param stream the stream to process
	 */
	virtual void Run(JsonPackerStream& stream) = 0;
	/**
	 * @brief InputOpenModeFlags returns flags for opening input file (in the case of string streams may be ignored)
	 * @return combination of flags to open file
	 */
	virtual std::ios_base::openmode InputOpenModeFlags() = 0;
	/**
	 * @brief OutputOpenModeFlags returns flags for opening output file (in the case of string streams may be ignored)
	 * @return combination of flags to open file
	 */
	virtual std::ios_base::openmode OutputOpenModeFlags() = 0;
	/**
	 * @brief GetDictionary returns stored JSON keys dictionary
	 * @return shared pointer to dictionary
	 */
	JsonKeyDictionary::Ptr GetDictionary();
protected:
	JsonKeyDictionary::Ptr m_dictionary {new JsonKeyDictionary()};/// the JSON keys dictionary
};

/**
 * @brief GetPacker creates an instance of coder class by its name ('json2tlv' and 'tlv2json' are available at the moment)
 * @param name[in] the name of class to instantiate
 * @return
 */
JsonPackerBase::Ptr GetPacker(const std::string& name);


/**
  @addtogroup JSONPACKER_ERRORS
  @{
  **/

#define PARSER_ERROR_MESSAGE "Parse error (code __error_code__) at line __line_number__:\n__json_line__\n__error_pos__\n__msg__"
#define TLV_INVALID_FORMAT_ERROR_MESSAGE "TLV input contains data in wrong format"

/**
 * @brief The JsonParseError class is the class used to throw exceptions in the case of parsing JSON data errors
 */
class JsonParseError : public app_err::JsonPackerError {
public:
	/**
	 * @brief JsonParseError constructor
	 * @param error_code[in] the code of the error
	 * @param line_number[in] the line number of input data in which a parse error occurs
	 * @param error_postition[in] the position of the first error symbol in the line
	 * @param json_line[in] the line containing parse error
	 * @param message[in] the error message
	 */
	JsonParseError(int error_code, int line_number, size_t error_postition, const std::string& json_line, const std::string& message);
};

/**
 * @brief The TlvInvalidFormatError class is the class used to throw exceptions in the case of parsing TLV data errors
 */
class TlvInvalidFormatError : public app_err::JsonPackerError {
public:
	TlvInvalidFormatError();
};

/**
  @}
  **/


/**
 * @brief The TlvRecord class is used to process data in TLV-format
 */
template<typename SizeType>
class TlvRecord {
public:
	typedef std::vector<char> DataVector;
	/**
	 * @brief The TlvRecordType enum describes TLV-record data types
	 */
	enum class TlvRecordType : char {
		rtUnknown		= -1,	///datatype is unknown
		rtMemberCount	= 0,	///JSON record member count
		rtInt			= 1,	///TLV record contain an integer value
		rtUInt			= 2,	///TLV record contain an unsigned integer value
		rtNull			= 3,	///TLV record contain a null value
		rtBool			= 4,	///TLV record contain a boolean value
		rtInt64			= 5,	///TLV record contain 64-bit integer value
		rtUInt64		= 6,	///TLV record contain 64-bit unsigned integer value
		rtDouble		= 7,	///TLV record contain double value
		rtFloat			= 8,	///TLV record contain float value
		rtString		= 9,	///TLV record contain string value
		rtDictionary	= 127	///TLV record represent the begining of dictionary
	};
	/**
	 * @brief TlvRecord default constructor
	 */
	TlvRecord() {}
	/**
	 * @brief TlvRecord constructor
	 * @param type[in] type of the record (@see TlvRecordType)
	 * @param data[in] pointer to data
	 * @param data_size[in] data size in bytes
	 */
	TlvRecord(TlvRecordType type, const char* data, SizeType data_size)
		: m_chartype(static_cast<typename std::underlying_type<TlvRecordType>::type>(type))
		, m_data(data, data + data_size)
		, m_data_size(data_size)
	{
	}
	/**
	 * @brief Init initiaizes record by new data
	 * @param type[in] type of the record (@see TlvRecordType)
	 * @param data[in] pointer to data
	 * @param data_size[in] data size in bytes
	 * @return reference to itself
	 */
	TlvRecord<SizeType>& Init(TlvRecordType type, const char* data, SizeType data_size) {
		m_chartype = static_cast<typename std::underlying_type<TlvRecordType>::type>(type);
		m_data_size = data_size;
		m_data.assign(data, data + m_data_size);
		return *this;
	}
	/**
	 * @brief Init initiaizes record by string
	 * @param data[in] string data
	 * @return reference to itself
	 */
	TlvRecord<SizeType>& Init(const std::string& data) {
		return Init(TlvRecordType::rtString, data.c_str(), static_cast<SizeType>(data.length()));
	}
	/**
	 * @brief Init initiaizes record by RapidJson value
	 * @param value[in] reference to RapidJson value
	 * @return reference to itself
	 */
	TlvRecord<SizeType>& Init(const rapidjson::Document::ValueType& value) {
		if (value.IsBool()) {
			bool v = value.GetBool();
			return Init(TlvRecordType::rtBool, reinterpret_cast<const char*>(&v), sizeof(bool));
		}
		else if (value.IsDouble()) {
			double v = value.GetDouble();
			return Init(TlvRecordType::rtDouble, reinterpret_cast<const char*>(&v), sizeof(double));
		}
		else if (value.IsFloat()) {
			float v = value.GetFloat();
			return Init(TlvRecordType::rtFloat, reinterpret_cast<const char*>(&v), sizeof(float));
		}
		else if (value.IsInt()) {
			int v = value.GetInt();
			return Init(TlvRecordType::rtInt, reinterpret_cast<const char*>(&v), sizeof(int));
		}
		else if (value.IsUint()) {
			unsigned int v = value.GetUint();
			return Init(TlvRecordType::rtUInt, reinterpret_cast<const char*>(&v), sizeof(int));
		}
		else if (value.IsInt64()) {
			int64_t v = value.GetInt64();
			return Init(TlvRecordType::rtInt64, reinterpret_cast<const char*>(&v), sizeof(int64_t));
		}
		else if (value.IsUint64()) {
			uint64_t v = value.GetUint64();
			return Init(TlvRecordType::rtUInt64, reinterpret_cast<const char*>(&v), sizeof(int64_t));
		}
		else if (value.IsNull()) {
			return Init(TlvRecordType::rtNull, nullptr, 0);
		}
		else if (value.IsString()) {
			return Init(value.GetString());
		}
		return *this;
	}
	/**
	 * @brief operator () function call operator for initializing record by new data
	 * @param type[in] type of the record (@see TlvRecordType)
	 * @param data[in] pointer to data
	 * @param data_size[in] data size in bytes
	 * @return reference to itself
	 */
	TlvRecord<SizeType>& operator() (TlvRecordType type, const char* data, SizeType data_size) {
		return Init(type, data, data_size);
	}
	/**
	 * @brief operator () function call operator for initializing record by string
	 * @param data[in] string data
	 * @return reference to itself
	 */
	TlvRecord<SizeType>& operator() (const std::string& data) {
		return Init(data);
	}
	/**
	 * @brief operator () function call operator for initializing record by RapidJson value
	 * @param value[in] reference to RapidJson value
	 * @return reference to itself
	 */
	TlvRecord<SizeType>& operator() (const rapidjson::Document::ValueType& value) {
		return Init(value);
	}

	/**
	 * @brief GetJsonValue convert TLV-record to RapidJson value
	 * @param allocator memory allocator (is used with string values)
	 * @return RapidJson value
	 */
	rapidjson::Value GetJsonValue(rapidjson::Document::AllocatorType& allocator) {
		rapidjson::Value v;
		switch (Type()) {
		case TlvRecordType::rtInt:
			v.SetInt(*(reinterpret_cast<int*>(m_data.data())));
			break;
		case TlvRecordType::rtUInt:
			v.SetUint(*(reinterpret_cast<unsigned int*>(m_data.data())));
			break;
		case TlvRecordType::rtNull:
			v.SetNull();
			break;
		case TlvRecordType::rtBool:
			v.SetBool(*(reinterpret_cast<bool*>(m_data.data())));
			break;
		case TlvRecordType::rtInt64:
			v.SetInt64(*(reinterpret_cast<int64_t*>(m_data.data())));
			break;
		case TlvRecordType::rtUInt64:
			v.SetUint64(*(reinterpret_cast<uint64_t*>(m_data.data())));
			break;
		case TlvRecordType::rtDouble:
			v.SetDouble(*(reinterpret_cast<double*>(m_data.data())));
			break;
		case TlvRecordType::rtFloat:
			v.SetFloat(*(reinterpret_cast<float*>(m_data.data())));
			break;
		case TlvRecordType::rtString:
			m_data.push_back(0);
			v.SetString(m_data.data(), m_data.size() - 1, allocator);
			break;
		default:
			throw app_err::JsonPackerError("Unknown data type!");
		}
		return v;
	}

	/**
	 * @brief Type returns the type (@see TlvRecordType) of the record
	 * @return the record type
	 */
	TlvRecordType Type() {return static_cast<TlvRecordType>(m_chartype);}
	/**
	 * @brief CharType returns underlying type of the record
	 * @return the underlying record type
	 */
	char& CharType() {return m_chartype;}
	/**
	 * @brief Data returns TLV-data field
	 * @return reference to TLV-data field
	 */
	DataVector& Data() {return m_data;}
	/**
	 * @brief DataSize returns size of TLV data
	 * @return reference to DataSize member
	 */
	SizeType& DataSize() {return m_data_size;}
	/**
	 * @brief SetIgnoreDataOnRead sets option indicating the need of ignoring data on read; this option is used in search procedure of dictionary to speed up reading data from stream
	 * @param value if true - ignore data on read
	 */
	void SetIgnoreDataOnRead(bool value) {m_ignore_data_on_read = value;}
	/**
	 * @brief IgnoreDataOnRead returns the value of ignoring data option
	 * @return true if data must be ignored, false otherwise
	 */
	bool IgnoreDataOnRead() {return  m_ignore_data_on_read;}

	/**
	 * @brief GetString returns value of TLV-record as string
	 * @return string representation of TLV-data
	 */
	std::string GetString() {
		m_data.push_back(0);
		return std::string(m_data.data());
	}
	/**
	 * @brief GetInt returns value of TLV-record as integer
	 * @return integer representation of TLV-data
	 */
	int GetInt() {
		return *reinterpret_cast<int*>(m_data.data());
	}

private:
	char m_chartype {-1};
	DataVector m_data;
	SizeType m_data_size {0};
	bool m_ignore_data_on_read {false};
};

/**
 * @brief operator >> is an extraction operator; allows to read TlvRecord from input stream
 * @param is[in] reference to input stream
 * @param record[in] TLV-record
 * @return reference to input stream
 */
template<typename SizeType>
std::istream& operator >> (std::istream &is, TlvRecord<SizeType> &record) {
	is.read(&record.CharType(), 1);
	if (!is.eof()) {
		bool failed_read = false;
		try {
			is.read(reinterpret_cast<char*>(&record.DataSize()), sizeof(SizeType));
			if (!record.IgnoreDataOnRead()) {
				record.Data().resize(static_cast<unsigned long>(record.DataSize()));
				is.read(record.Data().data(), record.DataSize());
			} else
				is.seekg(record.DataSize(), std::ios::cur);
		} catch (...) {
			failed_read = true;
		}
		if (is.fail() || is.eof() || failed_read)
			throw TlvInvalidFormatError();
	}
	return is;
}

/**
 * @brief operator << is an insertion operator; allows to write TlvRecord to output stream
 * @param os[in] reference to output stream
 * @param record[in] TLV record
  * @return reference to outut stream
 */
template<typename SizeType>
std::ostream& operator << (std::ostream &os, TlvRecord<SizeType> &record) {
	os.write(&record.CharType(), 1);
	os.write(reinterpret_cast<const char*>(&record.DataSize()), sizeof(SizeType));
	if (record.DataSize())
		os.write(reinterpret_cast<const char*>(record.Data().data()), record.DataSize());
	return os;
}



/**
 * @brief The JsonToTlv class is a class to convert input data containing JSON records separated by line into TLV format
 */
class JsonToTlv : public JsonPackerBase {
public:
	/**
	 * @brief Run start coding process
	 * @param stream the stream (@see JsonPackerStream) to process
	 */
	void Run(JsonPackerStream& stream) override;
	/**
	 * @brief InputOpenModeFlags returns flags for opening input file (in the case of string streams may be ignored)
	 * @return combination of flags to open file
	 */
	std::ios_base::openmode InputOpenModeFlags() override;
	/**
	 * @brief OutputOpenModeFlags returns flags for opening output file (in the case of string streams may be ignored)
	 * @return combination of flags to open file
	 */
	std::ios_base::openmode OutputOpenModeFlags() override;
};

/**
 * @brief The TlvToJson class is a class to convert input data in TLV format into JSON records separated by line
 */
class TlvToJson : public JsonPackerBase {
public:
	/**
	 * @brief Run start coding process
	 * @param stream the stream (@see JsonPackerStream) to process
	 */
	void Run(JsonPackerStream& stream) override;
	/**
	 * @brief InputOpenModeFlags returns flags for opening input file (in the case of string streams may be ignored)
	 * @return combination of flags to open file
	 */
	std::ios_base::openmode InputOpenModeFlags() override;
	/**
	 * @brief OutputOpenModeFlags returns flags for opening output file (in the case of string streams may be ignored)
	 * @return combination of flags to open file
	 */
	std::ios_base::openmode OutputOpenModeFlags() override;
};

/**
  @}
  **/

} //end of namespace jsonpacker_coder

#endif // CODER_H
