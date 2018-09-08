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
		else
			return false;
	}

	bool is_object() const {
		if (json_str.front() == '{' && json_str.back() == '}')
			return true;
		else
			return false;
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
		else 
			if (object.is_object()) {
				std::string key;
				std::any any;

				while (object.json_str.find(':')) {
					int begin = object.json_str.find('{');
					object.json_str[begin] = ' ';
					int end = object.json_str.rfind('}');
					object.json_str[end] = ' ';

					begin = object.json_str.find('"');
					object.json_str[begin] = ' ';
					end = object.json_str.find('"');
					object.json_str[end] = ' ';
					key = object.json_str.substr(begin + 1, end - begin);

					begin = object.json_str.find(':');
					object.json_str[begin++] = ' ';

					if (object.json_str[begin] == '"') {
						object.json_str[begin] = ' ';
						end = object.json_str.find('"');
						object.json_str[end] = ' ';
						any = object.json_str.substr(begin + 1, end - begin);
					}
					else if (object.json_str[begin] == '{') {
						int i = 1;

						for (auto ch = begin + 1; i != 0 && ch < object.json_str.length(); ch++) {
							if (ch == '{')
								i++;
							else if (ch == '}')
								i--;
							if (i == 0)
								end = ch;
						}
						std::string str = object.json_str.substr(begin + 1, end - begin);
						any = Json::parse(str);
					}
					else if (object.json_str[begin] == '[') {
						int i = 1;

						for (auto ch = begin + 1; i != 0 && ch < object.json_str.length(); ch++) {
							if (ch == '[')
								i++;
							else if (ch == ']')
								i--;
							if (i == 0)
								end = ch;
						}
						std::string str = object.json_str.substr(begin + 1, end - begin);
						any = Json::parse(str);
					}
					else {
						end = object.json_str.find(',');
						object.json_str[end] = ' ';
						std::string str = object.json_str.substr(begin, end - begin);
						if (str == "false" || str == "true")
							if (str == "false")
								any = false;
							else
								any = true;
						else {
							int number;
							std::istringstream stream(str);
							stream >> number;
							any = number;
						}
					}
				}

				object.map.insert(std::pair<std::string, std::any>(key, any));
			}

			else
				throw std::logic_error("not implemented");

		return object;
	}

	static Json parseFile(const std::string& path_to_file) {
		throw std::logic_error("not implemented");
	}
};
