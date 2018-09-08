#include <iostream>
#include <map>
#include <any>
#include <sstream> 

std::string read(std::string string = "") {
	std::string buf;

	if (string == "") {
		do {
			std::getline(std::cin, buf);
			string += buf;
		} while (buf != "");
	}
	std::istringstream stream(string);
	string.clear();
	while (stream >> buf) {
		string += buf;
	}

	return string;
}

class Json {
private:
	std::map <std::string, std::any> map;
	std::string json_str;
public:
	Json(const std::string& s) {
		json_str = read(s);
	}

	bool is_array() const {
		if (json_str.front() == '[' && json_str.back() == ']')
			return true;
		else if ( json_str.front() == '{' && json_str.back() == '}' )
			return false;
		else
			throw std::logic_error("not implemented");
	}

	bool is_object() const {
		return !(is_array);
	}

	// Метод возвращает значение по ключу key, если экземпляр является JSON-объектом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
	// Если экземпляр является JSON-массивом, генерируется исключение.
	std::any& operator[](const std::string& key) {
		throw std::logic_error("not implemented");
	}

	// Метод возвращает значение по индексу index, если экземпляр является JSON-массивом.
	// Значение может иметь один из следующих типов: Json, std::string, double, bool или быть пустым.
	// Если экземпляр является JSON-объектом, генерируется исключение.
	std::any& operator[](int index) {
		throw std::logic_error("not implemented");
	}

	static Json parse(const std::string& s) {
		Json object(s);

		if (object.is_array()) {
			
		}
		else if (object.is_object()) {

		}
		else
			throw std::logic_error("not implemented");

		return object;
	}

	static Json parseFile(const std::string& path_to_file) {
		throw std::logic_error("not implemented");
	}
};
