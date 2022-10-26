#include <iostream>
#include <vector>
#include <cmath>
#include <functional>
#include <chrono>

std::vector<double>
operateVector(std::vector<double> x, std::vector<double> y, const std::function<double(double, double)> &operation) {
    std::vector<double> newVector;
    for (size_t i = 0U; i < x.size(); ++i) {
        newVector.push_back(operation(x[i], y[i]));
    }
    return newVector;
}

double sumVector(const std::vector<double> &v) {
    double sum = 0.0;
    for (double i: v) {
        sum += i;
    }
    return sum;
}

std::string vectorToString(const std::vector<double> &v) {
    std::string s;
    for (double d: v) {
        (void) s.append(std::to_string(d) + " ");
    }
    return s;
}

double expectation(const std::vector<double> &v) {
    size_t size = v.size();
    std::vector<double> probabilityVector;
    for (size_t i = 0U; i < size; ++i) {
        probabilityVector.push_back(1.0 / static_cast<double>(size));
    }

    return sumVector(operateVector(v, probabilityVector, std::multiplies<>()));
}

double average(const std::vector<double> &v) {
    double sum = 0.0;
    for (double i: v) {
        sum += i;
    }
    return sum / static_cast<double>(v.size());
}

std::vector<double> squareVector(const std::vector<double> &v) {
    std::vector<double> newVector;
    for (double i: v) {
        newVector.push_back(i * i);
    }
    return newVector;
}

double variance(const std::vector<double> &v) {
    size_t size = v.size();
    std::vector<double> meanVector;
    for (size_t i = 0U; i < size; ++i) {
        meanVector.push_back(average(v));
    }

    return sumVector(squareVector(operateVector(v, meanVector, std::minus<>()))) / static_cast<double>(size);
}

double covariance(const std::vector<double> &x, const std::vector<double> &y) {
    size_t size = x.size();
    std::vector<double> expectationX;
    std::vector<double> expectationY;
    for (size_t i = 0U; i < size; ++i) {
        expectationX.push_back(expectation(x));
        expectationY.push_back(expectation(y));
    }

    return sumVector(operateVector(operateVector(x, expectationX, std::minus<>()),
                                   operateVector(y, expectationY, std::minus<>()), std::multiplies<>())) /
           static_cast<double>(x.size());

}

double pearson(const std::vector<double> &x, const std::vector<double> &y) {
    return covariance(x, y) / sqrt(variance(x) * variance(y));
}

std::vector<double> functionalVector(size_t size, const std::function<double(double)> &monoFunction) {
    std::vector<double> v;
    for (size_t i = 1U; i <= size; ++i) {
        v.push_back(monoFunction(static_cast<double>(i)));
    }
    return v;
}

double square(double n) {
    return n * n;
}

double constant([[maybe_unused]] double n) {
    return 1.0;
}

int main() {
    std::vector<double> x = {1.0, 2.0, 3.0, 4.0, 5.0};
    std::vector<double> y = {1.0, 4.0, 6.0, 6.0, 7.0};

    std::vector<double> times;
    long passedTimes = 0;

    // Calculando el Coeficiente de correlación de Pearson para 'x' y 'y' 1000 veces
    for (int i = 1; i <= 1000; ++i) {
        // Inicio contar
        const auto start = std::chrono::steady_clock::now();
        (void) pearson(x, y);
        // Fin contar
        const auto end = std::chrono::steady_clock::now();
        const auto timeElapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        passedTimes += timeElapsed;
        times.push_back(static_cast<double>(passedTimes));
    }

    // Comparando la complejidad
    std::vector<double> vConst = functionalVector(1000U,constant);
    std::vector<double> vLog = functionalVector(1000U, log);
    std::vector<double> vCuadratic = functionalVector(1000U, square);
    std::vector<double> vLineal = functionalVector(1000U, std::identity());

    std::cout << "Constante: " << pearson(times, vConst) << std::endl;
    std::cout << "Logaritmica: " << pearson(times, vLog) << std::endl;
    std::cout << "Lineal: " << pearson(times, vLineal) << std::endl;
    std::cout << "Cuadrática: " << pearson(times, vCuadratic) << std::endl;

    return 0;
}
