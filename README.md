# PieceMeal v1.0.0
This is the first version of the PieceMeal chess programming library. This library was built as part of my [chess bot project](https://github.com/AlexanderB184/Chess-Bot), I am releasing this separately to allow others to use this library for their own projects. This is a robust and performant library that has been tested rigorously during the development of my chess bot.

# Key Features
- Move Generation
A reliable move generation system, handling special moves such as castling and en passent.
-  Board Representation
Uses 0x88 board representation and piece lists for fast performance.
- Zobrist Hashing
Integrated hashing for state tracking and transposition table support
- Chess Notation Parsing
Integrated chess notation parsing tools.
# Whats New in v1.0.0
- Improved test coverage for enhanced reliability.
- Updated and streamlined build scripts for easier integration.
# Getting Started
1. Clone the repository:
``` bash
git clone https://github.com/AlexanderB184/PieceMeal.git
cd PieceMeal
````
2. Build the library:
``` bash
python3 scripts/build_lib.py
````
3. Link the library to your project and start building!

# Future Plans
- Incremental Move Generation
Separate the generation of captures, promotions, and quiet moves for finer control and improved performance in alpha-beta search.

- Chess960 Support
Add support for Chess960 (Fischer Random Chess).

- Cross-Platform Compatibility
Full support for Windows and macOS alongside Linux.

- Comprehensive Documentation
Provide detailed guides and API references for users.
