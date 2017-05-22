#include <vector>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "apperror.h"
#include "utils.h"
#include "jsoncoder_tests.h"

namespace jsoncoder_tests {

JsonTlvTestBase::JsonTlvTestBase()
	: Test()
	, m_stream(m_input_stream, m_output_stream)
{
}

void JsonTlvTestBase::FillInputStream(const std::vector<std::string> &data) {
	m_input_stream.str(std::string());
	for (auto& rec : data) {
		m_input_stream << rec << std::endl;
	}
}

void JsonTlvTestBase::FillInputStream(const std::vector<unsigned char> &data) {
	for (auto& c : data) {
		m_input_stream << c;
	}
}

JsonToTlvTest::JsonToTlvTest()
	: JsonTlvTestBase()
{
}

TlvToJsonTest::TlvToJsonTest()
	: JsonTlvTestBase()
{
}

TEST_F(JsonKeyDictionaryTest, FillingKeys) {
	const std::vector<std::string> key_sequence = {
		"key1", "key2", "key3", "key1", "key4", "key5", "key2", "key1"
	};
	for (auto& key : key_sequence) {
		EXPECT_NO_THROW(m_dictionary.AddKey(key));
	}
}

TEST_F(JsonKeyDictionaryTest, FillingKeysWithIndex) {
	const std::vector<std::string> key_sequence = {
		"key1", "key2", "key3", "key4", "key5"
	};

	//initial fill
	int i = 0;
	for (auto& key : key_sequence) {
		EXPECT_NO_THROW(m_dictionary.AddKey(key, ++i));
	}

	//try to add duplicates
	for (auto& key : key_sequence) {
		EXPECT_THROW(m_dictionary.AddKey(key, ++i), app_err::JsonPackerExists);
	}
}

TEST_F(JsonKeyDictionaryTest, GetInvalidKeyIndex) {
	const std::vector<std::string> key_sequence = {
		"key1", "key2", "key3", "key4", "key5"
	};

	//initial fill
	for (auto& key : key_sequence) {
		m_dictionary.AddKey(key);
	}

	EXPECT_THROW(m_dictionary[123], app_err::JsonPackerMissed);
}

TEST_F(JsonKeyDictionaryTest, CheckKeyIndexes) {
	const std::vector<std::string> key_sequence = {
		"key1", "key2", "key3", "key1", "key4", "key5", "key2", "key1"
	};
	const std::map<std::string, int> expected_indexes = {
		{"key1", 1},
		{"key2", 2},
		{"key3", 3},
		{"key4", 4},
		{"key5", 5}
	};

	//fill keys
	for (auto& key : key_sequence) {
		EXPECT_NO_THROW(m_dictionary.AddKey(key));
	}

	EXPECT_TRUE(std::equal(m_dictionary.Keys().begin(), m_dictionary.Keys().end(), expected_indexes.begin()));

	for (auto& key : expected_indexes) {
		EXPECT_EQ(m_dictionary[key.second], key.first);
	}
}


TEST_F(JsonToTlvTest, ValidJsonRun) {
	JsonToTlv coder;
	FillInputStream(m_json_records_valid);
	EXPECT_NO_THROW(coder.Run(m_stream));
}

TEST_F(JsonToTlvTest, ValidJsonAlldatatypesRun) {
	JsonToTlv coder;
	FillInputStream(m_json_records_valid_all_datatypes);
	EXPECT_NO_THROW(coder.Run(m_stream));
}

TEST_F(JsonToTlvTest, ValidJsonRunCheckDictionary) {
	FillInputStream(m_json_records_valid);
	JsonToTlv coder;
	coder.Run(m_stream);

	auto dictionary = coder.GetDictionary();
	EXPECT_TRUE(std::equal(dictionary->Keys().begin(), dictionary->Keys().end(), m_dictionary_valid.begin()));
}

TEST_F(JsonToTlvTest, ValidJsonRunCheckOutput) {
	FillInputStream(m_json_records_valid);
	JsonToTlv coder;
	coder.Run(m_stream);

	std::vector<unsigned char> result_data;
	while (!m_output_stream.eof()) {
		char c;
		m_output_stream.get(c);
		if (m_output_stream.eof())
			break;
		result_data.push_back(static_cast<unsigned char>(c));
	}

	EXPECT_TRUE(std::equal(result_data.begin(),result_data.end(), m_tlv_data_valid.begin()));
}

TEST_F(JsonToTlvTest, InvalidKeyNameJsonRun) {
	FillInputStream(m_json_records_invalid_keyname);
	JsonToTlv coder;
	EXPECT_THROW(coder.Run(m_stream), jsonpacker_coder::JsonParseError);
}

TEST_F(JsonToTlvTest, InvalidJsonNoValueRun) {
	FillInputStream(m_json_records_invalid_novalue);
	JsonToTlv coder;
	EXPECT_THROW(coder.Run(m_stream), jsonpacker_coder::JsonParseError);
}

TEST_F(JsonToTlvTest, InvalidValueJsonRun) {
	FillInputStream(m_json_records_invalid_value);
	JsonToTlv coder;
	EXPECT_THROW(coder.Run(m_stream), jsonpacker_coder::JsonParseError);
}

TEST_F(TlvToJsonTest, ValidTlvRun) {
	FillInputStream(m_tlv_data_valid);
	TlvToJson coder;
	EXPECT_NO_THROW(coder.Run(m_stream));
}

TEST_F(TlvToJsonTest, ValidTlvRunCheckOutput) {
	FillInputStream(m_tlv_data_valid);
	TlvToJson coder;
	coder.Run(m_stream);

	std::vector<std::string> result_data;
	std::string line;
	while (getline(m_output_stream, line)) {
		result_data.push_back(line);
	}

	auto valid_it = m_json_records_valid.begin();
	for (auto& s : result_data) {
		std::string str = s;
		str = str::Replace(str, " ", "");
		boost::algorithm::to_lower(str);

		std::string valid_str = *valid_it;
		valid_str = str::Replace(valid_str, " ", "");
		boost::algorithm::to_lower(valid_str);

		EXPECT_EQ(str, valid_str);

		++valid_it;
	}
}

TEST_F(TlvToJsonTest, TlvToJsonToTlv) {
	FillInputStream(m_tlv_data_valid);
	TlvToJson coder;
	coder.Run(m_stream); // tlv --> json

	std::stringstream result_stream;
	jsonpacker_stream::JsonPackerStringStream json_stream(m_output_stream, result_stream);
	JsonToTlv decoder;
	decoder.Run(json_stream); //json --> tlv

	std::vector<unsigned char> result_data;
	while (!result_stream.eof()) {
		char c;
		result_stream.get(c);
		if (result_stream.eof())
			break;
		result_data.push_back(static_cast<unsigned char>(c));
	}

	EXPECT_TRUE(std::equal(result_data.begin(),result_data.end(), m_tlv_data_valid.begin()));
}

TEST_F(TlvToJsonTest, TlvWithoutDictionary) {
	FillInputStream(m_valid_tlv_data_without_dictionary);
	TlvToJson coder;
	EXPECT_THROW(coder.Run(m_stream), app_err::JsonPackerMissed);
}

TEST_F(TlvToJsonTest, TlvDictionaryInvalidData) {
	FillInputStream(m_tlv_data_dictionary_invalid_data);
	TlvToJson coder;
	EXPECT_THROW(coder.Run(m_stream), jsonpacker_coder::TlvInvalidFormatError);
}

TEST_F(TlvToJsonTest, TlvDictionaryInvalidDataSize) {
	FillInputStream(m_tlv_data_dictionary_invalid_datasize);
	TlvToJson coder;
	EXPECT_THROW(coder.Run(m_stream), jsonpacker_coder::TlvInvalidFormatError);
}

TEST_F(TlvToJsonTest, TlvDictionaryInvalidKeyName) {
	FillInputStream(m_tlv_data_dictionary_invalid_keyname);
	TlvToJson coder;
	EXPECT_THROW(coder.Run(m_stream), jsonpacker_coder::TlvInvalidFormatError);
}

TEST_F(TlvToJsonTest, TlvDictionaryInvalidKeyIndexType) {
	FillInputStream(m_tlv_data_dictionary_invalid_keyindex_type);
	TlvToJson coder;
	EXPECT_THROW(coder.Run(m_stream), jsonpacker_coder::TlvInvalidFormatError);
}

TEST_F(TlvToJsonTest, TlvDictionaryInvalidKeyNameType) {
	FillInputStream(m_tlv_data_dictionary_invalid_keyname_type);
	TlvToJson coder;
	EXPECT_THROW(coder.Run(m_stream), jsonpacker_coder::TlvInvalidFormatError);
}

TEST_F(TlvToJsonTest, TlvEmptyDictionary) {
	FillInputStream(m_tlv_data_valid_empty_dictionary);
	TlvToJson coder;
	EXPECT_THROW(coder.Run(m_stream), app_err::JsonPackerMissed);
}

TEST_F(TlvToJsonTest, TlvDictionaryWithoutValidKey) {
	FillInputStream(m_tlv_data_valid_dictionary_without_valid_key);
	TlvToJson coder;
	EXPECT_THROW(coder.Run(m_stream), app_err::JsonPackerMissed);
}

}; // end of namespace jsoncoder_tests
