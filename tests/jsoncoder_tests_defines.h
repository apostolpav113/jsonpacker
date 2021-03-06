#ifndef JSONCODER_TESTS_DEFINES_H
#define JSONCODER_TESTS_DEFINES_H

#define JSON_RECORDS_VALID {\
	"{\"key1\":\"value\", \"key2\":42, \"key3\":true}",\
	"{\"sadsf\":\"dsewtew\", \"dsre\":3221, \"sdfds\":\"dsfewew\"}"\
}

//valid dictionary for JSON_RECORDS_VALID
#define DICTIONARY_VALID {{"key1", 1},{"key2", 2},{"key3", 3},{"sadsf", 4},{"dsre", 5},{"sdfds", 6}}

#define JSON_RECORDS_VALID_ALL_DATATYPES {\
	"{\"key1\":\"value\", \"key2\":42, \"key3\":true}",\
	"{\"sadsf\":\"dsewtew\", \"dsre\":3221, \"sdfds\":\"dsfewew\"}",\
	"{\"key1\": null, \"key2\":\"somestr\", \"key3\": 1.234, \"key4\": 123445, \"key5\": true, \"key6\": false}"\
}

//key2:42 must be "key2":42
#define JSON_RECORDS_INVALID_KEY_NAME {\
	"{\"key1\":\"value\", key2:42, \"key3\":true}",\
	"{\"sadsf\":\"dsewtew\", \"dsre\":3221, \"sdfds\":\"dsfewew\"}"\
}

//"key1":value must be "key1":"value"
#define JSON_RECORDS_INVALID_VALUE {\
	"{\"key1\":value, \"key2\":42, \"key3\":true}",\
	"{\"sadsf\":\"dsewtew\", \"dsre\":3221, \"sdfds\":\"dsfewew\"}"\
}

//"sadsf" must have value
#define JSON_RECORDS_INVALID_WITHOUT_VALUE {\
	"{\"key1\":\"value\", \"key2\":42, \"key3\":true}",\
	"{\"sadsf\":, \"dsre\":3221, \"sdfds\":\"dsfewew\"}"\
}


//valid TLV data for JSON_RECORDS_VALID
#define TLV_DATA_VALID {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x03, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65, /*'value' in ascii codes*/\
	\
	/*"key2": 42 --> 2: 42*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x02, 0x00, 0x00, 0x00, /*index of 'key2'*/\
	0x01, /*int, type of value (42)*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x2a, 0x00, 0x00, 0x00, /*42 hex*/\
	\
	/*"key3": true --> 3: true*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x03, 0x00, 0x00, 0x00, /*index of 'key3'*/\
	0x04, /*bool, type of value*/\
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, /*true*/\
	\
	/*2nd record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x03, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"sadsf":"dsewtew" --> 4: "dsewtew"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x04, 0x00, 0x00, 0x00, /*index of 'sadsf'*/\
	0x09, /*string, type of value*/\
	0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsewtew')*/\
	0x64, 0x73, 0x65, 0x77, 0x74, 0x65, 0x77, /*'dsewtew' in ascii codes*/\
	\
	/*"dsre":3221 --> 5: 3221*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x05, 0x00, 0x00, 0x00, /*index of 'sadsf'*/\
	0x01, /*int, type of value (3221)*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x95, 0x0c, 0x00, 0x00, /*3221 hex*/\
	\
	/*"sdfds":"dsfewew" --> 6: "dsfewew"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x06, 0x00, 0x00, 0x00, /*index of 'sadsf'*/\
	0x09, /*string, type of value*/\
	0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsfewew')*/\
	0x64, 0x73, 0x66, 0x65, 0x77, 0x65, 0x77, /*'dsfewew' in ascii codes*/\
	\
	/*dictionary*/\
	0x7f, /*dictionary*/\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size = 0 --> no data!*/\
	\
	/*due to using the ordered map to store keys*/\
	/*keys order would be*/\
	/*"dsre" : 5 - "key1" : 1 - "key2" : 2 - "key3" : 3 - "sadsf" : 4 - "sdfds" : 6*/\
	\
	/*"dsre" : 5*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsre')*/\
	0x64, 0x73, 0x72, 0x65, /*'dsre' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x05, 0x00, 0x00, 0x00, /*5 hex*/\
	\
	/*"key1": 1*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'key1')*/\
	0x6b, 0x65, 0x79, 0x31, /*'key1' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*1 hex*/\
	\
	/*"key2": 2*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'key2')*/\
	0x6b, 0x65, 0x79, 0x32, /*'key2' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x02, 0x00, 0x00, 0x00, /*2 hex*/\
	\
	/*"key3": 3*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'key3')*/\
	0x6b, 0x65, 0x79, 0x33, /*'key3' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x03, 0x00, 0x00, 0x00, /*3 hex*/\
	\
	/*"sadsf": 4*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'sadsf')*/\
	0x73, 0x61, 0x64, 0x73, 0x66, /*'sadsf' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x04, 0x00, 0x00, 0x00, /*4 hex*/\
	\
	/*"sdfds": 6*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'sdfds')*/\
	0x73, 0x64, 0x66, 0x64, 0x73, /*'sdfds' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x06, 0x00, 0x00, 0x00 /*6 hex*/\
}

#define TLV_DATA_WITHOUT_DICTIONARY {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65 /*'value' in ascii codes*/\
}

#define TLV_DATA_WITH_DICTIONARY_INVALID_DATA {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65, /*'value' in ascii codes*/\
	\
	/*dictionary*/\
	0x7f, /*dictionary*/\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size = 0 --> no data!*/\
	\
	/*"dsre" : 5*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsre')*/\
	0x64, 0x73, 0x72, 0x65, /*'dsre' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x05, 0x00, 0x00 /*5 hex									-- invalid data (must be 4 bytes)*/\
}

#define TLV_DATA_WITH_DICTIONARY_INVALID_DATASIZE {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65, /*'value' in ascii codes*/\
	\
	/*dictionary*/\
	0x7f, /*dictionary*/\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size = 0 --> no data!*/\
	\
	/*"dsre" : 5*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsre')*/\
	0x64, 0x73, 0x72, 0x65, /*'dsre' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size			-- invalid data size (must be 4 bytes)*/\
	0x05, 0x00, 0x00, 0x00 /*5 hex*/\
}

#define TLV_DATA_WITH_DICTIONARY_INVALID_KEYNAME {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65, /*'value' in ascii codes*/\
	\
	/*dictionary*/\
	0x7f, /*dictionary*/\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size = 0 --> no data!*/\
	\
	/*"dsre" : 5*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsre')*/\
	0x64, 0x73, 0x65, /*'dsre' in ascii codes						-- must be 0x64, 0x73, 0x72, 0x65*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x05, 0x00, 0x00, 0x00 /*5 hex*/\
}

#define TLV_DATA_WITH_DICTIONARY_INVALID_KEYINDEX_TYPE {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65, /*'value' in ascii codes*/\
	\
	/*dictionary*/\
	0x7f, /*dictionary*/\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size = 0 --> no data!*/\
	\
	/*"dsre" : 5*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsre')*/\
	0x64, 0x73, 0x72, 0x65, /*'dsre' in ascii codes*/\
	0x09, /*string, type of value									-- must be int*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsre')*/\
	0x64, 0x73, 0x72, 0x65 /*'dsre' in ascii codes*/\
}

#define TLV_DATA_WITH_DICTIONARY_INVALID_KEYNAME_TYPE {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65, /*'value' in ascii codes*/\
	\
	/*dictionary*/\
	0x7f, /*dictionary*/\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size = 0 --> no data!*/\
	\
	/*"dsre" : 5*/\
	0x01, /*int, type key index						-- first must be string*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x05, 0x00, 0x00, 0x00, /*5 hex*/\
	0x09, /*string, type of value*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsre')*/\
	0x64, 0x73, 0x72, 0x65 /*'dsre' in ascii codes*/\
}

#define TLV_DATA_WITH_EMPTY_DICTIONARY {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65, /*'value' in ascii codes*/\
	\
	/*dictionary*/\
	0x7f, /*dictionary*/\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 /*size = 0 --> no data!*/\
}

#define TLV_DATA_WITH_DICTIONARY_WITHOUT_VALID_KEY {\
	/*1st record*/\
	0x00, /*membercount*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*count of member in 1st rec*/\
	\
	/*"key1": "value" --> 1: "value"*/\
	0x01, /*int, type of key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x01, 0x00, 0x00, 0x00, /*index of 'key1'*/\
	0x09, /*string, type of value*/\
	0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'value')*/\
	0x76, 0x61, 0x6c, 0x75, 0x65, /*'value' in ascii codes*/\
	\
	/*dictionary*/\
	0x7f, /*dictionary*/\
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size = 0 --> no data!*/\
	\
	/*"dsre" : 5*/\
	0x09, /*string, type of value												-- do not have key 'key1'*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size (length of 'dsre')*/\
	0x64, 0x73, 0x72, 0x65, /*'dsre' in ascii codes*/\
	0x01, /*int, type key index*/\
	0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*size*/\
	0x05, 0x00, 0x00, 0x00 /*5 hex*/\
}
#endif // JSONCODER_TESTS_DEFINES_H
