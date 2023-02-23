
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

std::string ReadFileIntoString(const std::string& file_path) {
    std::ifstream file(file_path);
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

int main() {
    std::string file_contents = ReadFileIntoString("example.txt");
    std::cout << file_contents << std::endl;
    return 0;
}
