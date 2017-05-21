#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>

namespace app_err {

#define EXISTS_MESSAGE	"The __object__ __value__already exists"
#define MISSED_MESSAGE	"The __object__ __value__is missed"

class JsonPackerError : public std::exception {
public:
	JsonPackerError(const std::string& message);
	const char *what() const _GLIBCXX_USE_NOEXCEPT override;
private:
	std::string m_message;
};

class JsonPackerExists : public JsonPackerError {
public:
	JsonPackerExists(const std::string& object, const std::string& value);
};

class JsonPackerFileExists : public JsonPackerExists {
public:
	JsonPackerFileExists(const std::string& filename);
};

class JsonPackerMissed :public JsonPackerError {
public:
	JsonPackerMissed(const std::string& object, const std::string& value);
};

class JsonPackerFileMissed : public JsonPackerMissed {
public:
	JsonPackerFileMissed(const std::string& filename);
};

}// end of namespace error

#endif // ERROR_H
