#include <type_traits>
#include "coder.h"
#include "utils.h"

#include <iostream>
#include <vector>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>


namespace jsonpacker_coder {

JsonPackerBase::~JsonPackerBase()
{
}

JsonKeyDictionary::Ptr JsonPackerBase::GetDictionary() {
	return m_dictionary;
}


JsonPackerBase::Ptr GetPacker(const std::string &name) {
	return util::Factory<JsonPackerBase>::Instance()->Get(name);
}

RegisterInFactory("json2tlv", JsonToTlv, JsonPackerBase);
RegisterInFactory("tlv2json", TlvToJson, JsonPackerBase);


int JsonKeyDictionary::AddKey(const std::string &key) {
	auto it = m_keys.insert(std::make_pair(key, 0));
	if (it.second) {
		it.first->second = ++m_current;
	}
	return it.first->second;
}

void JsonKeyDictionary::AddKey(const std::string &key, int index) {
	auto it = m_keys.find(key);
	if (it == m_keys.end()) {
		m_keys[key] = index;
	} else
		throw app_err::JsonPackerExists("dictionary key", key);
}

std::string JsonKeyDictionary::operator [](int index) {
	for (auto& key_pair : m_keys) {
		if (key_pair.second == index)
			return key_pair.first;
	}
	return "";
}

void JsonKeyDictionary::Clear() {
	m_keys.clear();
	m_current = 0;
}


JsonParseError::JsonParseError(int error_code, int line_number, size_t error_postition, const std::string &json_line, const std::string &message)
	: app_err::JsonPackerError(str::Replace(std::string(PARSER_ERROR_MESSAGE), {{"__error_code__", std::to_string(error_code)},
																			  {"__line_number__", std::to_string(line_number)},
																			  {"__json_line__", json_line},
																			  {"__error_pos__", std::string(error_postition, ' ') + "^"},
																			  {"__msg__", message}}))
{

}

TlvInvalidFormatError::TlvInvalidFormatError()
	: app_err::JsonPackerError(TLV_INVALID_FORMAT_ERROR_MESSAGE)
{
}

void JsonToTlv::Run(JsonPackerStream &stream) {
	using RecType = TlvRecord<std::streamsize>;
	using TlvType = RecType::TlvRecordType;
	RecType record;

	m_dictionary->Clear();
	std::string line;
	int line_number = 0;
	while (getline(stream.InputStream(), line)) {
		++line_number;
		rapidjson::Document json_doc;
		rapidjson::ParseResult ok = json_doc.Parse(line.c_str());
		if (json_doc.HasParseError())
			throw JsonParseError(json_doc.GetParseError(), line_number, json_doc.GetErrorOffset(), line, rapidjson::GetParseError_En(ok.Code()));


		auto count = json_doc.MemberCount();
		stream.OutputStream() << record(TlvType::rtMemberCount, reinterpret_cast<const char*>(&count), sizeof(count));
		for (auto it = json_doc.MemberBegin(); it != json_doc.MemberEnd(); ++it) {
			const int key_index = m_dictionary->AddKey(it->name.GetString());
			stream.OutputStream() << record(TlvType::rtInt, reinterpret_cast<const char*>(&key_index), sizeof(key_index));
			stream.OutputStream() << record(it->value);
		}
	}

	stream.OutputStream() << record(TlvType::rtDictionary, nullptr, 0);
	for(auto& key : m_dictionary->Keys()) {
		const std::string key_name = key.first;
		const int key_index = key.second;
		stream.OutputStream() << record(key_name);
		stream.OutputStream() << record(TlvType::rtInt, reinterpret_cast<const char*>(&key_index), sizeof(key_index));
	}
}

std::ios_base::openmode JsonToTlv::InputOpenModeFlags() {
	return std::ios_base::in;
}

std::ios_base::openmode JsonToTlv::OutputOpenModeFlags() {
	return std::ios_base::out | std::ios_base::binary | std::ios_base::trunc;
}

void TlvToJson::Run(JsonPackerStream &stream) {
	using RecType = TlvRecord<std::streamsize>;
	using TlvType = RecType::TlvRecordType;
	RecType record;

	m_dictionary->Clear();
	stream.InputStream().seekg(0, std::ios::beg);
	bool dictionary_found = false;

	//search for dictionary
	record.SetIgnoreDataOnRead(true);
	std::string dictionary_key;
	int dictionary_index;
	while (!stream.InputStream().eof()) {
		stream.InputStream() >> record;
		if (stream.InputStream().eof())
			break;
		if (record.Type() == TlvType::rtDictionary) {
			dictionary_found = true;
			record.SetIgnoreDataOnRead(false);
			continue;
		}
		if (dictionary_found && record.Type() == TlvType::rtString) {
			dictionary_key = record.GetString();
		} else
		if (dictionary_found && record.Type() == TlvType::rtInt) {
			dictionary_index = record.GetInt();
			m_dictionary->AddKey(dictionary_key, dictionary_index);
		}
	}
	if (!dictionary_found)
		throw app_err::JsonPackerMissed("dictionary", "");

	//read and convert data
	stream.InputStream().clear();
	stream.InputStream().seekg(0, std::ios::beg);
	record.SetIgnoreDataOnRead(false);
	while (!stream.InputStream().eof()) {
		stream.InputStream() >> record;
		if (stream.InputStream().eof())
			break;
		if (record.Type() == TlvType::rtDictionary)
			break;

		if (record.Type() == TlvType::rtMemberCount) {
			rapidjson::Document document;
			document.SetObject();
			int member_count = record.GetInt();
			for (int i = 0; i < member_count; ++i) {
				//key
				stream.InputStream() >> record;
				if (stream.InputStream().eof())
					throw TlvInvalidFormatError();
				//TODO: check type - must be int (or change it to rtKeyIndex)
				std::string key = (*m_dictionary)[record.GetInt()];
				rapidjson::Value key_value;
				key_value.SetString(key.c_str(), static_cast<rapidjson::SizeType>(key.length()), document.GetAllocator());

				//value
				stream.InputStream() >> record;
				if (stream.InputStream().eof())
					throw TlvInvalidFormatError();
				rapidjson::Value v = record.GetJsonValue(document.GetAllocator());

				document.AddMember(key_value, v, document.GetAllocator());
			}
			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
			document.Accept(writer);

			stream.OutputStream() << buffer.GetString() << std::endl;
		} else
			throw TlvInvalidFormatError();
	}
}

std::ios_base::openmode TlvToJson::InputOpenModeFlags() {
	return std::ios_base::in | std::ios_base::binary;
}

std::ios_base::openmode TlvToJson::OutputOpenModeFlags() {
	return std::ios_base::out | std::ios_base::trunc;
}


} // end of namespace jsonpacker_coder
