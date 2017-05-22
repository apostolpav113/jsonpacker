#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <map>
#include "apperror.h"

namespace util {

#define RegisterInFactory(name, type, base) class reg_name_##type { \
	public: \
		reg_name_##type() { \
			util::Factory<base>::Instance()->Register<type>(name); \
		} \
	}; \
	reg_name_##type reg_name_##type##_object

template<class Base>
class Factory {
public:
	using Ptr = std::shared_ptr<Factory>;
	template<class T>
	void Register(const std::string& name) {
		auto it = m_creators.find(name);
		if (it != m_creators.end())
			throw app_err::JsonPackerExists("creator", name);
		m_creators[name] = typename Creator::Ptr(new ConcreteCreator<T>());
	}
	typename Base::Ptr Get(const std::string& name) {
		auto it = m_creators.find(name);
		if (it == m_creators.end())
			throw app_err::JsonPackerMissed("creator", name);
		return m_creators[name]->Create();
	}
	void Clear() {
		m_creators.clear();
	}
	static Ptr Instance() {
		static Ptr instance;
		if (!instance)
			instance.reset(new Factory);
		return instance;
	}
private:
	Factory() {}

	class Creator {
	public:
		typedef std::shared_ptr<Creator> Ptr;
		virtual ~Creator() {}
		virtual typename Base::Ptr Create() = 0;
	};

	template<class T>
	class ConcreteCreator : public Creator {
	public:
		typename Base::Ptr Create() override {
			return typename Base::Ptr(new T);
		}
	};

	std::map<std::string, typename Creator::Ptr> m_creators;
};

} // end of namespace util

namespace str {

	std::string Replace(const std::string& source, const std::string& from, const std::string& to);
	std::string Replace(const std::string& source, const std::map<std::string, std::string>& replace_map);

}

#endif // UTILS_H
