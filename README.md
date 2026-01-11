# Option-Pricing-System
This project provides a high-performance C++ library for pricing various types of financial options, using industry-standard models such as Black-Scholes and Monte Carlo simulations. 

# Features
## Models Included
* Black-Scholes-Merton (BSM): Closed-form solutions for European vanilla call and put options.
* Monte Carlo Simulation: A flexible method for pricing path-dependent (e.g., Asian) and multi-asset options.
* Binomial Tree Models: An iterative approach for pricing American options with potential early exercise.
* Finite Difference Methods: Numerical solutions for solving the Black-Scholes PDE.

# Future Improvements
The current state is nice for experimentation but some key features should be added to fully productionize this system.
* Comprehensive folder structure
* Market Data Loader: This component reads financial data (like stock prices, interest rates, and volatilities) from external sources, such as CSV files, to prepare the data for the pricing models.
* Greeks Calculator: This module calculates the sensitivities of the option price to changes in underlying parameters (e.g., Delta, * Gamma, Theta, Vega, Rho), which are essential for risk management and hedging.
* User Interface/Output System: A simple command-line interface or a basic graphical interface to input parameters and display the calculated option price and Greeks.
* Testing Framework: Essential for ensuring the accuracy of your models. You can compare your results against a well-known library like QuantLib or established values from resources like Bloomberg terminals
