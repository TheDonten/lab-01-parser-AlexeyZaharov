#include <iostream>
#include <sstream>
#include <https://github.com/nlohmann/json/include/nlohmann/json.hpp>


using nlohmann::json;

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

int main() {
	 json data = json::parse(read());

	 json array = json::array();

	 for (auto i : data) {
		 array.push_back(i);
	 }

	 data.clear();

	 unsigned int i = 0;
	 for (auto j : array) {
		 json object = {
							{"ticker", array[0][i]},
							{"id", array[1][i]},
							{"description", array[2][i++]}
					   };
		 data.push_back(object);
	 }
   
   return 0;
}
