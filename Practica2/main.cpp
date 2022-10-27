#include <iostream>
#include <iomanip>

#define MES 4
#define CAPITAL_INICIAL 16
#define INTERES_MENSUAL 17
#define CAPITAL_FINAL 14

// Potencia recursiva
double rPow(double n, unsigned int k) {
    if (k == 0U) {
        return 1.0;
    }
    --k;
    return n * rPow(n, k);

}

std::string getRow(const std::string &mes, const std::string &cI, const std::string &rM, const std::string &cF) {
    std::stringstream ss;
    ss << std::left << std::setw(MES) << mes
       << std::left << std::setw(CAPITAL_INICIAL) << cI
       << std::left << std::setw(INTERES_MENSUAL) << rM
       << std::left << std::setw(CAPITAL_FINAL) << cF
       << std::endl;
    return ss.str();
}

std::string getTable(double cI, double rY, unsigned int n) {
    std::basic_stringstream<char> ss;
    ss << getRow("Mes", "Capital Inicial", "Interés mensual", "Capital final");
    for (unsigned int i = 0U; i < n * 12U; ++i) {
        ss << getRow(std::to_string(i),
                     std::to_string(cI * rPow(1.0 + rY / 12.0, i)),
                     std::to_string(rY / 12.0),
                     std::to_string(cI * rPow(1.0 + rY / 12.0, i + 1U))
        );
    }

    return ss.str();
}

int main([[maybe_unused]] int argc, char **argv) {
    std::cout << getTable(std::stod(argv[1]), std::stod(argv[2]), static_cast<unsigned int>(std::stoi(argv[3])));
    return 0;
}
