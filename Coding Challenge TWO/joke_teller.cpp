// Challenge 2: Alexa tell me a Joke
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

struct Joke {
    std::string setup;
    std::string punchline;
};

std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

bool loadJokes(const std::string& filename, std::vector<Joke>& jokes) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Cannot open joke file: " << filename << '\n';
        return false;
    }
    std::string line;
    int skipped = 0;
    while (std::getline(file, line)) {
        if (trim(line).empty()) continue;
        // The first question mark separates the setup from the punchline.
        const auto split = line.find('?');
        if (split == std::string::npos || trim(line.substr(0, split)).empty()
            || trim(line.substr(split + 1)).empty()) {
            ++skipped;
            continue;
        }
        jokes.push_back({trim(line.substr(0, split + 1)),
                         trim(line.substr(split + 1))});
    }
    if (file.bad()) {
        std::cerr << "Could not finish reading the joke file.\n";
        return false;
    }
    if (skipped > 0)
        std::cerr << "Warning: skipped " << skipped << " malformed joke(s).\n";
    if (jokes.empty()) {
        std::cerr << "The file contains no usable jokes.\n";
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cerr << "Usage: joke_teller [joke_file]\n";
        return 1;
    }
    std::vector<Joke> jokes;
    if (!loadJokes(argc == 2 ? argv[1] : "randomJokes.txt", jokes)) return 1;
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<std::size_t> choose(0, jokes.size() - 1);
    const std::string request = "Alexa, tell me a joke";
    std::cout << "Type \"" << request << "\" for a joke, or \"quit\" to exit.\n";
    std::string input;
    while (true) {
        std::cout << "\nCommand: ";
        if (!std::getline(std::cin, input)) break;
        input = trim(input);
        if (input == "quit") break;
        if (input != request) {
            std::cout << "Please type \"" << request << "\" or \"quit\".\n";
            continue;
        }
        const Joke& joke = jokes[choose(generator)];
        std::cout << joke.setup << "\nPress Enter to reveal the punchline: ";
        if (!std::getline(std::cin, input)) break;
        std::cout << joke.punchline << '\n';
    }
    std::cout << "\nGoodbye!\n";
    return 0;
}
