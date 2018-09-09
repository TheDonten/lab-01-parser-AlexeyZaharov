#include <iostream>
#include <map>
#include <any>
#include <sstream> 
#include <vector> 

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

	Json take_object(int begin) {
		int i = 0;
		int end;

		for (auto ch = begin; i != 0 && ch < json_str.length(); ch++) {
			if (ch == '{')
				i++;
			else if (ch == '}')
				i--;

			if (i == 0)
				end = ch;
		}
		std::string str = json_str.substr(begin, end - begin + 1);

		return Json::parse(str);
	}

	void take_bool_or_object(std::any& any, int begin, int end) {
		end = json_str.find(',');
		json_str[end] = ' ';
		std::string str = json_str.substr(begin, end - begin);
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

	std::string take_string(int begin, int end) {
		json_str[begin] = ' ';
		end = json_str.find('"');
		json_str[end] = ' ';

		return json_str.substr(begin + 1, end - begin);
	}

	std::vector<std::any> take_array(int begin) {
		std::vector<std::any> vector;
		std::any any_vector;
		int end;
		int i = 0;

		for (auto ch = begin; i != 0 && ch < json_str.length(); ch++) {
			if (ch == '[')
				i++;
			else if (ch == ']')
				i--;
			if (i == 0)
				end = ch;
		}
		json_str[begin++] = ' ';
		json_str[end] = ' ';

		while (json_str.find(',') && json_str.find(',') > begin && json_str.find(',') < end) {

			switch (json_str[begin]) {
				case '"': {
					any_vector = take_string(begin, end);
					break;
				}
				case '{': {
					any_vector = take_object(begin);
					break;
				}
				case '[': {
					any_vector = take_array(begin);
					break;
				}
				default: {
					take_bool_or_object(any_vector, begin, end);
				}
			}
			json_str[json_str.find(',')] = ' ';
		}

		vector.push_back(any_vector);
	}

	std::pair<std::string, std::any> take_pair_in_object() {
		std::string key;
		std::any any;
		int begin = json_str.find('"');
		json_str[begin] = ' ';
		int end = json_str.find('"');
		json_str[end] = ' ';
		key = json_str.substr(begin + 1, end - begin);

		begin = json_str.find(':');
		json_str[begin++] = ' ';

		switch (json_str[begin]) {
			case '"': {
				any = take_string(begin, end);
				break;
				}
			case '{': {
				any = take_object(begin);
				break;
			}
			case '[': {
				any = take_array(begin);
				break;
			}
			default: {
				take_bool_or_object(any, begin, end);
			}
		}

		return std::pair<std::string, std::any>(key, any);
	}

public:
	Json(const std::string& s) {
		json_str = read(s);
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

		if (object.is_object()) {
			object.json_str[object.json_str.find('{')] = ' ';
			object.json_str[object.json_str.find('}')] = ' ';
				
			while (object.json_str.find(':')) {
				std::pair<std::string, std::any> pair = object.take_pair_in_object();
				object.map.insert(pair);
			}
		}
		else
			throw std::logic_error("not implemented");

		return object;
	}

	static Json parseFile(const std::string& path_to_file) {
		throw std::logic_error("not implemented");
	}
};
