#ifndef JSONCODER_TESTS_H
#define JSONCODER_TESTS_H

#include <gtest/gtest.h>
#include "coder.h"
#include "jsoncoder_tests_defines.h"

namespace jsoncoder_tests {

using namespace jsonpacker_coder;

class JsonKeyDictionaryTest : public ::testing::Test {
protected:
	JsonKeyDictionary m_dictionary;
};

class JsonTlvTestBase : public ::testing::Test {
public:
	JsonTlvTestBase();
protected:
	jsonpacker_stream::JsonPackerStringStream m_stream;
	std::stringstream m_input_stream;
	std::stringstream m_output_stream;

	using StringVector = std::vector<std::string>;

	StringVector m_json_records_valid = JSON_RECORDS_VALID;
	StringVector m_json_records_valid_all_datatypes = JSON_RECORDS_VALID_ALL_DATATYPES;
	StringVector m_json_records_invalid_keyname = JSON_RECORDS_INVALID_KEY_NAME;
	StringVector m_json_records_invalid_value = JSON_RECORDS_INVALID_VALUE;
	StringVector m_json_records_invalid_novalue = JSON_RECORDS_INVALID_WITHOUT_VALUE;
	std::map<std::string, int> m_dictionary_valid = DICTIONARY_VALID;

	std::vector<unsigned char> m_tlv_data_valid = TLV_DATA_VALID;
	std::vector<unsigned char> m_valid_tlv_data_without_dictionary = TLV_DATA_WITHOUT_DICTIONARY;
	std::vector<unsigned char> m_tlv_data_dictionary_invalid_data = TLV_DATA_WITH_DICTIONARY_INVALID_DATA;
	std::vector<unsigned char> m_tlv_data_dictionary_invalid_datasize = TLV_DATA_WITH_DICTIONARY_INVALID_DATASIZE;
	std::vector<unsigned char> m_tlv_data_dictionary_invalid_keyname = TLV_DATA_WITH_DICTIONARY_INVALID_KEYNAME;
	std::vector<unsigned char> m_tlv_data_dictionary_invalid_keyindex_type = TLV_DATA_WITH_DICTIONARY_INVALID_KEYINDEX_TYPE;
	std::vector<unsigned char> m_tlv_data_dictionary_invalid_keyname_type = TLV_DATA_WITH_DICTIONARY_INVALID_KEYNAME_TYPE;

	std::vector<unsigned char> m_tlv_data_valid_empty_dictionary = TLV_DATA_WITH_EMPTY_DICTIONARY;
	std::vector<unsigned char> m_tlv_data_valid_dictionary_without_valid_key = TLV_DATA_WITH_DICTIONARY_WITHOUT_VALID_KEY;
	void FillInputStream(const std::vector<std::string>& data);
	void FillInputStream(const std::vector<unsigned char>& data);
};

class JsonToTlvTest : public JsonTlvTestBase {
public:
	JsonToTlvTest();
};

class TlvToJsonTest : public JsonTlvTestBase {
public:
	TlvToJsonTest();
};

}; // end of namespace jsoncoder_tests

#endif // JSONCODER_TESTS_H
