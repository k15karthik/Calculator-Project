#include <iostream>
#include <stack>
#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>

// Helper functions
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

double applyOperation(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) throw std::runtime_error("Division by zero");
            return a / b;
        case '^': return std::pow(a, b);
        default: throw std::invalid_argument("Unknown operator");
    }
}

double evaluate(const std::string& expression) {
    std::stack<double> values;
    std::stack<char> operators;

    for (size_t i = 0; i < expression.length(); i++) {
        if (std::isspace(expression[i])) {
            continue;
        } else if (std::isdigit(expression[i]) || expression[i] == '.') {
            std::string value;
            while (i < expression.length() && (std::isdigit(expression[i]) || expression[i] == '.')) {
                value += expression[i++];
            }
            values.push(std::stod(value));
            i--; // Adjust for the next iteration
        } else if (expression[i] == '(') {
            operators.push(expression[i]);
        } else if (expression[i] == ')') {
            while (!operators.empty() && operators.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.pop(); // Remove '('
        } else {
            while (!operators.empty() && precedence(operators.top()) >= precedence(expression[i])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = operators.top(); operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.push(expression[i]);
        }
    }

    while (!operators.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = operators.top(); operators.pop();
        values.push(applyOperation(a, b, op));
    }

    return values.top();
}

void displayMenu() {
    std::cout << "\n--- Calculator Menu ---\n";
    std::cout << "Enter a mathematical expression to calculate.\n";
    std::cout << "Supported operations: +, -, *, /, ^, and parentheses.\n";
    std::cout << "Type 'd' to exit the calculator.\n";
    std::cout << "------------------------\n";
}

void runCalculator() {
    char choice;
    std::string input;

    do {
        displayMenu();
        std::cout << "Your input: ";
        std::getline(std::cin, input);

        if (input == "d" || input == "D") {
            std::cout << "Exiting the calculator. Goodbye!\n";
            break;
        }

        try {
            double result = evaluate(input);
            std::cout << "Result: " << result << "\n";
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    } while (true);
}

int main() {
    runCalculator();
    return 0;
}
