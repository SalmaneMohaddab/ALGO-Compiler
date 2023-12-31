# ALGO-Compiler
This C program is designed to conduct lexical, syntactic, and semantic analyses of a custom programming language (ALGORITHM). It checks the source code against specific grammatical rules and reports any discrepancies it finds.

## Table of Contents

- [Installation](#installation)
- [Code Structure](#code-structure)
- [File Descriptions](#file-descriptions)
- [Contributing](#contributing)
- [License](#license)
- [Contact](#contact)

## Installation

To set up this analyzer on your system, follow these steps:

1. Clone the repository:
 ```bash
git clone https://github.com/salmane34/ALGO-Compiler.git 
```
2. Navigate to the repository directory:

```bash
cd ALGO-compiler
```
3. Compile the source code using a C compiler such as `gcc`:

```bash
gcc -o analyzer main.c
```


The program will output lexical, syntactic, or semantic errors encountered. If no errors are found, it will proceed with the execution of the code.

## Code Structure

The program includes several functions, each dedicated to a specific analysis aspect:

- `fonction`: Converts a file to a list of words.
- `analyse_lix`: Performs lexical analysis.
- `analyse_syntaxe`: Performs syntactic analysis.
- `analyse_sementique`: Performs semantic analysis.
- `execution`: Executes the program if no errors are found.
- `main`: The entry point of the program.

## File Descriptions

This project uses several files, each with a distinct role in the analysis process:

- `main.c`: The main source file containing all the functions for analysis.
- `programme.txt`: An example source file written in the custom language that the analyzer can process.
- `dictionnaire.txt`: Contains the dictionary of valid tokens for the custom language used during lexical analysis.
- `erreur.txt`: Generated log file where all analysis errors are recorded.

## Contributing

Contributions are welcome! If you have a suggestion that would make this better, please fork the repo and create a pull request. 

Don't forget to give the project a star! Thanks again!

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Contact

Salmane Mohaddab - salmanemohaddab@gmail.com

Project Link: [ https://github.com/salmane34/ALGO-Compiler.git ]
