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
	std::vector<std::any> array;

	Json take_object(int begin) {
		int i = 1;
		int end;

		for (auto ch = begin + 1; i != 0 && ch < json_str.length(); ch++) {
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

	Json take_array(int begin) {
		int i = 1;
		int end;

		for (auto ch = begin + 1; i != 0 && ch < json_str.length(); ch++) {
			if (ch == '[')
				i++;
			else if (ch == ']')
				i--;

			if (i == 0)
				end = ch;
		}
		std::string str = json_str.substr(begin, end - begin + 1);

		return Json::parse(str);
	}

	void take_array_value() {
		std::any any_vector;
		char first = json_str[json_str.front() + 1];

		while (json_str.find(',')) {

			switch (first) {
				case '"': {
					int begin = json_str.find('"');
					json_str[begin] = ' ';
					int end;

					any_vector = take_string(begin, end);
					json_str[json_str.find(',')] = ' ';
					break;
				}
				case '{': {
					any_vector = take_object(json_str.front() + 1);
					json_str[json_str.find(',')] = ' ';
					break;
				}
				case '[': {
					any_vector = take_array(json_str.front() + 1);
					json_str[json_str.find(',')] = ' ';
					break;
				}
				default: {
					int begin = json_str.find(' ');
					int end;
					take_bool_or_object(any_vector, begin + 1, end);
					json_str[begin] = '.';
				}
			}
			array.push_back(any_vector);
		}

		switch (first) {
			case '"': {
				int begin = json_str.find('"');
				json_str[begin] = ' ';
				int end;

				any_vector = take_string(begin, end);
				json_str[json_str.find(',')] = ' ';
				break;
			}
			case '{': {
				any_vector = take_object(json_str.front() + 1);
				json_str[json_str.find(',')] = ' ';
				break;
			}
			case '[': {
				any_vector = take_array(json_str.front() + 1);
				json_str[json_str.find(',')] = ' ';
				break;
			}
			default: {
				int begin = json_str.find(' ');
				int end;
				take_bool_or_object(any_vector, begin + 1, end);
				json_str[begin] = '.';
			}
		}
		array.push_back(any_vector);
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

	bool is_array() const {
		if (json_str.front() == '[' && json_str.back() == ']')
			return true;
		else
			return false;
	}

	std::any& operator[](const std::string& key) {
		if (is_object()) {
			if (map.find(key) != map.end()) {
				return map[key];
			}
		}
		else
			throw std::logic_error("not implemented");
	}

	std::any& operator[](int index) {
		if (is_array()) {
			return array[index];
		}
		else
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

			return object;
		}
		else if (object.is_array()) {
			object.json_str[object.json_str.find('[')] = ' ';
			object.json_str[object.json_str.find(']')] = ' ';
			object.take_array_value();

			return object;
		}
			throw std::logic_error("not implemented");

		return object;
	}

	static Json parseFile(const std::string& path_to_file) {
		throw std::logic_error("not implemented");
	}
};
