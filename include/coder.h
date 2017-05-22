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
 * @brief The JsonKeyDictionary class
 */
class JsonKeyDictionary {
public:
	using Ptr = std::shared_ptr<JsonKeyDictionary>;
	int AddKey(const std::string& key);
	void AddKey(const std::string& key, int index);
	std::string operator [] (int index);
	void Clear();
	std::map<std::string, int>& Keys() {return m_keys;}
private:
	int m_current {0};
	std::map<std::string, int> m_keys;
};

/**
 * @brief The JsonPackerBase class
 */
class JsonPackerBase {
public:
	using Ptr = std::shared_ptr<JsonPackerBase>;
	virtual ~JsonPackerBase();
	virtual void Run(JsonPackerStream& stream) = 0;
	virtual std::ios_base::openmode InputOpenModeFlags() = 0;
	virtual std::ios_base::openmode OutputOpenModeFlags() = 0;
	JsonKeyDictionary::Ptr GetDictionary();
protected:
	JsonKeyDictionary::Ptr m_dictionary {new JsonKeyDictionary()};
};

/**
 * @brief GetPacker
 * @param name
 * @return
 */
JsonPackerBase::Ptr GetPacker(const std::string& name);

#define PARSER_ERROR_MESSAGE "Parse error (code __error_code__) at line __line_number__:\n__json_line__\n__error_pos__\n__msg__"
#define TLV_INVALID_FORMAT_ERROR_MESSAGE "TLV input contains data in wrong format"

class JsonParseError : public app_err::JsonPackerError {
public:
	JsonParseError(int error_code, int line_number, size_t error_postition, const std::string& json_line, const std::string& message);
};

class TlvInvalidFormatError : public app_err::JsonPackerError {
public:
	TlvInvalidFormatError();
};


template<typename SizeType>
class TlvRecord {
public:
	typedef std::vector<char> DataVector;
	enum class TlvRecordType : char {
		rtUnknown		= -1,
		rtMemberCount	= 0,
		rtInt			= 1,
		rtUInt			= 2,
		rtNull			= 3,
		rtBool			= 4,
		rtInt64			= 5,
		rtUInt64		= 6,
		rtDouble		= 7,
		rtFloat			= 8,
		rtString		= 9,
		rtDictionary	= 127
	};
	TlvRecord() {}
	TlvRecord(TlvRecordType type, const char* data, SizeType data_size)
		: m_chartype(static_cast<typename std::underlying_type<TlvRecordType>::type>(type))
		, m_data(data, data + data_size)
		, m_data_size(data_size)
	{
	}
	TlvRecord<SizeType>& Init(TlvRecordType type, const char* data, SizeType data_size) {
		m_chartype = static_cast<typename std::underlying_type<TlvRecordType>::type>(type);
		m_data_size = data_size;
		m_data.assign(data, data + m_data_size);
		return *this;
	}
	TlvRecord<SizeType>& Init(const std::string& data) {
		return Init(TlvRecordType::rtString, data.c_str(), static_cast<SizeType>(data.length()));
	}
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
	TlvRecord<SizeType>& operator() (TlvRecordType type, const char* data, SizeType data_size) {
		return Init(type, data, data_size);
	}
	TlvRecord<SizeType>& operator() (const std::string& data) {
		return Init(data);
	}
	TlvRecord<SizeType>& operator() (const rapidjson::Document::ValueType& value) {
		return Init(value);
	}

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

	TlvRecordType Type() {return static_cast<TlvRecordType>(m_chartype);}
	char& CharType() {return m_chartype;}
	DataVector& Data() {return m_data;}
	SizeType& DataSize() {return m_data_size;}
	void SetIgnoreDataOnRead(bool value) {m_ignore_data_on_read = value;}
	bool IgnoreDataOnRead() {return  m_ignore_data_on_read;}

	std::string GetString() {
		m_data.push_back(0);
		return std::string(m_data.data());
	}
	int GetInt() {
		return *reinterpret_cast<int*>(m_data.data());
	}

private:
	char m_chartype {-1};
	DataVector m_data;
	SizeType m_data_size {0};
	bool m_ignore_data_on_read {false};
};

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

template<typename SizeType>
std::ostream& operator << (std::ostream &os, TlvRecord<SizeType> &record) {
	os.write(&record.CharType(), 1);
	os.write(reinterpret_cast<const char*>(&record.DataSize()), sizeof(SizeType));
	if (record.DataSize())
		os.write(reinterpret_cast<const char*>(record.Data().data()), record.DataSize());
	return os;
}



/**
 * @brief The JsonToTlv class
 */
class JsonToTlv : public JsonPackerBase {
public:
	void Run(JsonPackerStream& stream) override;
	std::ios_base::openmode InputOpenModeFlags() override;
	std::ios_base::openmode OutputOpenModeFlags() override;
};

/**
 * @brief The TlvToJson class
 */
class TlvToJson : public JsonPackerBase {
public:
	void Run(JsonPackerStream& stream) override;
	std::ios_base::openmode InputOpenModeFlags() override;
	std::ios_base::openmode OutputOpenModeFlags() override;
};

} //end of namespace jsonpacker_coder

#endif // CODER_H
