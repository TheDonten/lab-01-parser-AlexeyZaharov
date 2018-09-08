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

public:
	Json(const std::string& s) {
		throw std::logic_error("not implemented");
	}


	bool is_array() const {
		throw std::logic_error("not implemented");
	}

	bool is_object() const {
		throw std::logic_error("not implemented");
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
		std::string string = read(s);

		return Json(string);

		throw std::logic_error("not implemented");
	}

	static Json parseFile(const std::string& path_to_file) {
		throw std::logic_error("not implemented");
	}
};
