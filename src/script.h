#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef VOID
#undef VOID
#endif

namespace Retro {

class Variant {
public:
	enum class Type {
		BOOL,
		INT,
		FLOAT,
		VOID
	};

	Variant() {}
	Variant(int64_t);
	Variant(double);
	Variant(bool);

	template<typename T>
	T cast() const {
		switch (m_type) {
		case Type::BOOL:
			return m_vb;
		case Type::INT:
			return m_vi;
		case Type::FLOAT:
			return m_vf;
		case Type::VOID:
		default:
			return T();
		}
	}

	operator int() const;
	operator int64_t() const;
	operator float() const;
	operator double() const;
	operator bool() const;

private:
	Type m_type = Type::VOID;
	union {
		bool m_vb;
		int64_t m_vi;
		double m_vf;
	};
};

class GameData;
class ScriptContext {
public:
	static std::shared_ptr<ScriptContext> get(const std::string& type);
	static std::vector<std::string> listContexts();
	static void reset();

	virtual void setData(const GameData*);

	virtual bool init() = 0;
	virtual bool load(const std::string&) = 0;
	virtual Variant callFunction(const std::string&) = 0;
	virtual std::vector<std::string> listFunctions() = 0;

protected:
	const GameData* data();

private:
	const GameData* m_data;
};
}
