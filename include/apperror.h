/**
  @file
  @brief Header file with description of base JSON packer exception classes

  Contains several classes used to generate exceptions in the program. All other exception classes must be derived from one of these classes.
  **/

#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>

namespace app_err {

#define EXISTS_MESSAGE	"The __object__ __value__already exists"
#define MISSED_MESSAGE	"The __object__ __value__is missed"

/**
  @defgroup JSONPACKER_ERRORS JSON packer exception classes
  @{
  **/

/**
 * @brief The JsonPackerError class is the base class for all exception classes in program
 */
class JsonPackerError : public std::exception {
public:
	/**
	 * @brief JsonPackerError class constructor
	 * @param[in] message The message associated with exception (would be redirected to stdout)
	 */
	JsonPackerError(const std::string& message);
	/**
	 * @brief what Overriden function of std::exception class
	 * @return The message passed into the constructor
	 */
	const char *what() const _GLIBCXX_USE_NOEXCEPT override;
private:
	std::string m_message;
};

/**
 * @brief The JsonPackerExists class is general class representing errors of some existing objects
 */
class JsonPackerExists : public JsonPackerError {
public:
	/**
	 * @brief JsonPackerExists class constructor
	 * @param object[in] The type of existing object (i.e. "file")
	 * @param value[in] The name of existing object (i.e. "data.out")
	 */
	JsonPackerExists(const std::string& object, const std::string& value);
};


/**
 * @brief The JsonPackerFileExists class represents error of already existing file
 */
class JsonPackerFileExists : public JsonPackerExists {
public:
	/**
	 * @brief JsonPackerFileExists class constructor
	 * @param filename[in] The name of existing file
	 */
	JsonPackerFileExists(const std::string& filename);
};


/**
 * @brief The JsonPackerMissed class is general class representing errors of some missied objects
 */
class JsonPackerMissed :public JsonPackerError {
public:
	/**
	 * @brief JsonPackerMissed class constructor
	 * @param object[in] The type of missed object (i.e. "file")
	 * @param value[in] The name of missed object (i.e. "data.in")
	 */
	JsonPackerMissed(const std::string& object, const std::string& value);
};

/**
 * @brief The JsonPackerFileMissed class represents error of missed file
 */
class JsonPackerFileMissed : public JsonPackerMissed {
public:
	/**
	 * @brief JsonPackerFileMissed class constructor
	 * @param filename[in] The name of missed file
	 */
	JsonPackerFileMissed(const std::string& filename);
};

/**
  @}
  **/

}// end of namespace error

#endif // ERROR_H
