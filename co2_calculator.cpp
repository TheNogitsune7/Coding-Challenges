// Challenge 1: CO2 Calculator
#include <algorithm>
#include <cctype>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

constexpr double LITRES_PER_100_MILES = 9.66;
constexpr double DIESEL_KG_PER_LITRE = 2.68;
constexpr double PETROL_KG_PER_LITRE = 2.31;

// Read a whole line so text such as "100abc" cannot pass as a number.
bool getDistance(double& miles) {
    std::string input;
    while (true) {
        std::cout << "Distance travelled in miles: ";
        if (!std::getline(std::cin, input)) return false;
        std::istringstream reader(input);
        if (reader >> miles) {
            reader >> std::ws;
            if (reader.eof() && std::isfinite(miles) && miles >= 0) return true;
        }
        std::cout << "Enter a finite number of miles, zero or greater.\n";
    }
}

// Accept either fuel name regardless of capitalisation.
bool getFuelType(std::string& fuel) {
    std::string input;
    while (true) {
        std::cout << "Fuel type (petrol or diesel): ";
        if (!std::getline(std::cin, input)) return false;
        std::istringstream reader(input);
        if (reader >> fuel) {
            reader >> std::ws;
            std::transform(fuel.begin(), fuel.end(), fuel.begin(),
                [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
            if (reader.eof() && (fuel == "petrol" || fuel == "diesel")) return true;
        }
        std::cout << "Please enter petrol or diesel.\n";
    }
}

// Fuel has already been checked by getFuelType before this function is called.
double calculateEmissions(double miles, const std::string& fuel) {
    double litres = (miles / 100.0) * LITRES_PER_100_MILES;
    double factor = (fuel == "diesel") ? DIESEL_KG_PER_LITRE : PETROL_KG_PER_LITRE;
    return litres * factor;
}

int main() {
    std::cout << "CO2 Calculator\n";
    std::cout << "Assumed fuel consumption: 9.66 litres per 100 miles.\n";
    double miles = 0;
    std::string fuel;
    if (!getDistance(miles) || !getFuelType(fuel)) {
        std::cout << "\nInput ended. No calculation made.\n";
        return 0;
    }
    double litres = (miles / 100.0) * LITRES_PER_100_MILES;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Distance: " << miles << " miles\n";
    std::cout << "Fuel type: " << fuel << '\n';
    std::cout << "Estimated fuel used: " << litres << " litres\n";
    std::cout << "Estimated CO2 emissions: " << calculateEmissions(miles, fuel);
    std::cout << " kg CO2\n";
    return 0;
}
