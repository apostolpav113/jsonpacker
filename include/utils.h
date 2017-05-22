/**
  @file
  @brief Header file with description of some util functions and classes used in program
  **/

#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <map>
#include "apperror.h"

namespace util {

/**
  @defgroup JSONPACKER_UTIL JSON packer util function and classes
  @{
  **/

/**
  @brief The RegisterInFactory macro is used for auto registeration of classes in abstract factory
  @param[in] name the name of the registerred class
  @param[in] type the type of the registerred class
  @param[in] base the type of base class of the registerred class
  **/
#define RegisterInFactory(name, type, base) class reg_name_##type { \
	public: \
		reg_name_##type() { \
			util::Factory<base>::Instance()->Register<type>(name); \
		} \
	}; \
	reg_name_##type reg_name_##type##_object


/**
 * @brief The Factory class template is used to register and create instances of classes derived from one base class which type is defined by Base template parameter
 */
template<class Base>
class Factory {
public:
	using Ptr = std::shared_ptr<Factory>;

	/**
	 * @brief Register register new class of type T in factory (T must derived from Base)
	 * @param name[in] the name of the registerred class
	 */
	template<class T>
	void Register(const std::string& name) {
		auto it = m_creators.find(name);
		if (it != m_creators.end())
			throw app_err::JsonPackerExists("creator", name);
		m_creators[name] = typename Creator::Ptr(new ConcreteCreator<T>());
	}
	/**
	 * @brief Get allows to create an instance of the registerred class
	 * @param name[in] the name of class to instantiate
	 * @return shared pointer to base class of the registerred class
	 */
	typename Base::Ptr Get(const std::string& name) {
		auto it = m_creators.find(name);
		if (it == m_creators.end())
			throw app_err::JsonPackerMissed("creator", name);
		return m_creators[name]->Create();
	}
	/**
	 * @brief Clear removes all information about registerred classes
	 */
	void Clear() {
		m_creators.clear();
	}
	/**
	 * @brief Instance returns the instance of factory object (simple realisation of SINGLETONE pattern)
	 * @return shared pointer with factory instance
	 */
	static Ptr Instance() {
		static Ptr instance;
		if (!instance)
			instance.reset(new Factory);
		return instance;
	}
private:
	/**
	 * @brief Factory prohibited constructor; to obtain an instance of the factory use Instance() function
	 */
	Factory() {}

	/**
	 * @brief The Creator class is the base class for creator of instances of registered classes
	 */
	class Creator {
	public:
		typedef std::shared_ptr<Creator> Ptr;
		virtual ~Creator() {}
		/**
		 * @brief Create return an instance of registered class
		 * @return shared pointer with the instance of registered class
		 */
		virtual typename Base::Ptr Create() = 0;
	};

	/**
	 * @brief The ConcreteCreator class construct concrete instances of registered classes
	 */
	template<class T>
	class ConcreteCreator : public Creator {
	public:
		/**
		 * @brief Create creates an instance of registered class
		 * @return shared pointer with the instance of registered class
		 */
		typename Base::Ptr Create() override {
			return typename Base::Ptr(new T);
		}
	};

	std::map<std::string, typename Creator::Ptr> m_creators;///collection with creator objects of registered classes
};

/**
  @}
  **/

} // end of namespace util

namespace str {

/**
  @addtogroup JSONPACKER_UTIL
  @{
  **/

	/**
	 * @brief Replace replaces all occurrences of substring with another substring
	 * @param source[in] the source string
	 * @param from[in] the substring to be replaced
	 * @param to[in] the substring onto which all occurrences of substring 'from' would be replaced
	 * @return the string after replacement
	 */
	std::string Replace(const std::string& source, const std::string& from, const std::string& to);
	/**
	 * @brief Replace replaces all occurrences of substrings with another substrings, all the replace pairs 'from' and 'to' are given with the replace map
	 * @param source[in] the source string
	 * @param replace_map[in] the map containing pairs 'from' and 'to' of substrings to be replaced
	 * @return the string after replacement
	 */
	std::string Replace(const std::string& source, const std::map<std::string, std::string>& replace_map);

/**
  @}
  **/
}

#endif // UTILS_H
