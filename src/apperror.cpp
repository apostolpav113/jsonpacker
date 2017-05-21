#include "apperror.h"
#include "utils.h"

namespace app_err {

JsonPackerError::JsonPackerError(const std::string &message)
	: std::exception()
	, m_message(message)
{
}

const char *JsonPackerError::what() const _GLIBCXX_USE_NOEXCEPT {
	return m_message.c_str();
}

JsonPackerExists::JsonPackerExists(const std::string &object, const std::string &value)
	: JsonPackerError(str::Replace(std::string(EXISTS_MESSAGE), {{"__object__", object}, {"__value__", "\"" + value + "\" "}}))
{
}

JsonPackerMissed::JsonPackerMissed(const std::string &object, const std::string &value)
	: JsonPackerError(str::Replace(std::string(MISSED_MESSAGE), {{"__object__", object}, {"__value__", "\"" + value + "\" "}}))
{
}

JsonPackerFileExists::JsonPackerFileExists(const std::string &filename)
	: JsonPackerExists("file", filename)
{
}

JsonPackerFileMissed::JsonPackerFileMissed(const std::string &filename)
	: JsonPackerMissed("file", filename)
{
}


} // end of namespace error
