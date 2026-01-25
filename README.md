# Option-Pricing-System
This project provides a high-performance C++ library for pricing various types of financial options, using industry-standard models such as Black-Scholes and Monte Carlo simulations. 

## Installation
To compile and run the project, ensure you have a C++ compiler and make installed on your system. The project will eventually be structured to use a Makefile for easy compilation.

```bash
# Clone the repository
git clone https://github.com/bkovalick/Option-Pricing-System.git

# Navigate to the project directory
cd Option-Pricing-System
```

## Overview
* Black-Scholes-Merton (BSM): Closed-form solutions for European vanilla call and put options.
* Monte Carlo Simulation: A flexible method for pricing path-dependent (e.g., Asian) and multi-asset options.
* Binomial Tree Models: An iterative approach for pricing American options with potential early exercise.
* Option Pricers: Houses a variety of pricers (Asian, Barrier, European, etc.)
* Finite Difference Methods: Numerical solutions for solving the Black-Scholes PDE.

## Project Structure
- Makefile: Contains rules for compiling the project.
- include/: Header files for the project.
  - `BusinessLogic/Factory/`: Contains the `MediatorFactory` for creating mediator instances.
  - `BusinessLogic/Builders/`: Contains implementations following the builder pattern, i.e. create the system parts of a monte carlo simulation.
  - `BusinessLogic/Mediators/`: System coordinators, i.e. coordinates the starting and stopping of an engine. 
  - `BusinessLogic/Models/`: Contains models or calculators used by the pricing engines.
  - `BusinessLogic/Pricers/`: Contains models of different option pricers.
  - `Core/Domain/`: Domain-specific classes, like `Option`.
  - `Core/Repositories/`: Mediator between the business logic (domain layer) and the data mapping layers.
  - `Data/`: Lowest level, direct interface to an external data source. Map to DTO's.
- `src/`: Source files mirroring the structure in `include/`.

## Future Improvements
The current state is nice for experimentation but some key features should be added to fully productionize this system.
* Comprehensive folder structure
* Market Data Loader: This component reads financial data (like stock prices, interest rates, and volatilities) from external sources, such as CSV files, to prepare the data for the pricing models.
* Greeks Calculator: This module calculates the sensitivities of the option price to changes in underlying parameters (e.g., Delta, * Gamma, Theta, Vega, Rho), which are essential for risk management and hedging.
* User Interface/Output System: A simple command-line interface or a basic graphical interface to input parameters and display the calculated option price and Greeks.
* Testing Framework: Essential for ensuring the accuracy of your models. You can compare your results against a well-known library like QuantLib or established values from resources like Bloomberg terminals

## Roadmap

### Planned Features
- [ ] PDE solver mediator
- [ ] Binomial tree mediator
- [ ] Greeks calculator (Delta, Gamma, Vega, Theta, Rho)
- [ ] Market data loader (CSV, database)
- [ ] Real-time market data integration
- [ ] Web-based dashboard
- [ ] Python bindings
- [ ] GPU acceleration (CUDA)
- [ ] More exotic options (Lookback, Asian, Rainbow)
- [ ] Calibration module for implied volatility
- [ ] Risk management module (VaR, CVaR)

### Recent Updates
- ✅ Simulation Orchestrator pattern
- ✅ Factory-based component creation
- ✅ JSON configuration support
- ✅ Results container with CSV export
- ✅ Barrier option pricing
- ✅ Multiple execution modes

## Contributing

Contributions are welcome! Please:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## References

### Books
- *Options, Futures, and Other Derivatives* by John C. Hull
- *C++ Design Patterns and Derivatives Pricing* by Mark Joshi
- *Monte Carlo Methods in Financial Engineering* by Paul Glasserman

### Libraries
- [QuantLib](https://www.quantlib.org/) - Comprehensive quantitative finance library
- [Boost](https://www.boost.org/) - C++ libraries
- [nlohmann/json](https://github.com/nlohmann/json) - JSON for Modern C++

## Author

**Ben Kovalick**
- GitHub: [@bkovalick](https://github.com/bkovalick)

## Acknowledgments

- Boost.Signals2 for the observer pattern implementation
- nlohmann for the excellent JSON library
- The quantitative finance community for mathematical models and validation

---

**Note**: This is an educational/research project. Use at your own risk for production trading systems.
