#pragma once

template <typename T>
class TypeInfo {
public:	
	static const char* getName() {
		static const std::string name = generateTypeName();
		return name.c_str();
	}

	static size_t getUniqueId() {
		static const size_t uniqueId = hash(getName());
		return uniqueId;
	}

private:
	static const char *generateTypeName() {
#ifdef _WIN32
		static const std::string fullName(__FUNCSIG__);
#else
		static const std::string fullName(__PRETTY_FUNCTION__);
#endif
		static const std::string name = fullName.substr(29, fullName.length() - 25 - 29);
		return name.c_str();
	}

	// djb2 hash function
	static size_t hash(const char* str)
	{
		unsigned long hash = 5381;
		int c;
		while (c = *str++) {
			hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
		}
		return hash;
	}
};
