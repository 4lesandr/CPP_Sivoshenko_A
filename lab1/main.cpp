#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iterator>

class TextFormatter {
private:
    std::string filename;
    int line_length;

public:
    TextFormatter(std::string filename, int line_length) : filename(filename), line_length(line_length) {}

    void format() {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Could not open file " << filename << "\n";
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream is(line);
            std::vector<std::string> words((std::istream_iterator<std::string>(is)), std::istream_iterator<std::string>());

            std::string output;
            for (const auto& word : words) {
                if (output.size() + word.size() > line_length) {
                    int sp = (line_length - output.size()) / 2;
                    for (int i = 0; i < sp; i++) {
                        std::cout << " ";
                    }
                    std::cout << output << "\n";
                    output.clear();
                }
                if (!output.empty()) {
                    output += " ";
                }
                output += word;
            }
            if (output.size() > line_length) {
                std::cerr << "Word too long" << "\n";
            }
            else if (output.size() > 0) {
                int sp = (line_length - output.size()) / 2;
                    for (int i = 0; i < sp; i++) {
                        std::cout << " ";
                    }
                std::cout << output << "\n";
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " file length\n";
        return 1;
    }

    TextFormatter formatter(argv[1], std::stoi(argv[2]));
    formatter.format();

    return 0;
}

//cd /Users/aleksandr/Documents/GitHub/CPP_Sivoshenko_A/lab1 && g++ -std=c++11 main.cpp -o formatter && ./formatter lines2.txt 50