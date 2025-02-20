# PieceMeal v1.2.0

PieceMeal Chess Bot and Chess Library. This project contains 2 components, the chess programming library, and the chess bot. The core library is fully implemented in C. This is a robust and performant library that has been tested rigorously during the development of my chess bot.

## Key Features

- **Board Representation** Uses 0x88 board representation and piece lists for fast performance.
- **Move Generation** A reliable move generation system, handling special moves such as castling and en passent.
- **Chess Notation** Parsing Integrated chess notation parsing tools.
- **Chess Evaluation Functions** for both static and dynamic analysis.
- **Universal Chess Interface** Chess Bot uses the [Universal Chess Interface (UCI)](https://gist.github.com/DOBRO/2592c6dad754ba67e6dcaec8c90165bf).

## Whats New in v1.2.0

- Added bindings for Golang.
- Merged my chess bot from [Chess Bot](https://github.com/AlexanderB184/Chess-Bot) into the project.
- Replaced build process with a Unity build system

## Getting Started

1. Clone the repository:

    ``` bash
    git clone https://github.com/AlexanderB184/PieceMeal.git
    cd PieceMeal
    ```

2. Compile PieceMealLib.c:

    ``` bash
    cc -c -O3 PieceMealLib.c -o PieceMealLib.o
    ````

    or

    ``` bash
    cc -O3 -pthread PieceMealBot.c -o PieceMealBot.exe
    ````

3. Link with your project and start building!

## Future Plans

- **Cross-Platform Compatibility**
Full support for Windows and macOS alongside Linux.
Biggest Limitation currently is the usage of `time.h` and `pthread`.

- **Comprehensive Documentation** Provide detailed guides and API references for users.

- **Pawn Structure Evaluation and Pawn Hashing** [see](https://www.chessprogramming.org/Pawn_Hash_Table)
