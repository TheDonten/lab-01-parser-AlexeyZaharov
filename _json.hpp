#include <iostream>
#include <map>
#include <any>
#include <sstream> 
#include <vector>
#include <fstream>

std::string read_file(std::ifstream & file_stream) {
	std::string buf, string;

	do {
		std::getline(file_stream, buf);
		string += buf;
	} while (buf != "");
	std::istringstream stream(string);
	string.clear();
	while (stream >> buf) {
		string += buf;
	}

	return string;
}

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
	std::string format;
	std::string json_str;
	std::vector<std::any> array;

	Json take_object(int begin) {
		int i = 1;
		int end;

		for (auto ch = begin + 1; i != 0 && ch < json_str.length(); ch++) {
			if (json_str[ch] == '{')
				i++;
			else if (json_str[ch] == '}') {
				i--;
				if (i == 0)
					end = ch;
				break;
			}
		}
		std::string str = json_str.substr(begin, end - begin + 1);

		return Json::parse(str);
	}

	void take_bool_or_object(std::any& any, int begin, int end) {
		end = json_str.find(',');
		if (end == -1)
			end = json_str.length() - 1;
		json_str[end] = ' ';

		while (end < begin) {
			json_str[end] = ' ';
			end = json_str.find(',');
		}
		
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

		return json_str.substr(begin + 1, end - begin - 1);
	}

	Json take_array(int begin) {
		int i = 1;
		int end;
		for (auto ch = begin + 1; i != 0 && ch < json_str.length(); ch++) {
			if (json_str[ch] == '[')
				i++;
			else if (json_str[ch] == ']') {
				i--;
				if (i == 0)
					end = ch;
				break;
			}
		}
		std::string str = json_str.substr(begin, end - begin + 1);

		return Json::parse(str);
	}

	void take_array_value() {
		std::any any_vector;
		char first = json_str[1];
		json_str.push_back(',');

		while (json_str.find(',') != std::string::npos) {

			switch (first) {
			case '"': {
				int begin = json_str.find('"');
				json_str[begin] = ' ';
				int end = 0;

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
				int end = 0;
				take_bool_or_object(any_vector, begin + 1, end);
				json_str[begin] = '!';
			}
			}
			array.push_back(any_vector);
		}
	}

	std::pair<std::string, std::any> take_pair_in_object() {
		std::string key;
		std::any any;
		int begin = json_str.find('"');
		json_str[begin] = ' ';
		int end = json_str.find('"');
		json_str[end] = ' ';
		key = json_str.substr(begin + 1, end - begin - 1);

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
		format = json_str;
	}

	bool is_object() const {
		if (format.front() == '{' && format.back() == '}')
			return true;
		else
			return false;
	}

	bool is_array() const {
		if (format.front() == '[' && format.back() == ']')
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

			while (object.json_str.find(':') != std::string::npos) {
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
		std::ifstream file(path_to_file);

		if (file.is_open()) {
			std::string string = read_file(file);
			file.close();
			return Json::parse(string);
		}
		else
			throw std::logic_error("not implemented");
	}
};
