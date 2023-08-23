# Jewel Swap Game

Welcome to the Jewel Swap Game! This game is a fun and addictive puzzle game where you get to swap adjacent jewels to create matches and score points. Dive into the world of sparkling jewels and strategic swapping to beat your own high scores and challenge your friends!

## How to Play

1. **Game Area:** The game is played on an 8x8 grid filled with jewels.

2. **Swap Jewels:** Interact with the grid by swapping two horizontally or vertically adjacent jewels. You can swap jewels in two ways:
   - Dragging the first jewel towards the second.
   - Clicking on the first jewel and then clicking on the second.

3. **Create Matches:** When you swap jewels, if you create a sequence of 3 or more jewels of the same color in a row or column, those jewels will disappear, and you'll score points.

4. **No Valid Sequence:** If a swap doesn't result in a valid sequence, the jewels will switch back to their original positions.

5. **Falling Jewels:** When jewels disappear, the jewels above the empty spaces will fall down to fill the gaps.

6. **New Jewels:** New jewels will spawn from the top of the grid to fill the gaps left by disappearing jewels and falling jewels.

7. **Time and Score:** You have a limited time of 60 seconds to achieve your best score. The timer starts when you begin the game. For each successful match you make, one second is added to the timer. Be quick and strategic to score as many points as you can before the time runs out!

8. **Game Over:** The game ends when the timer runs out.

## Controls

- **Swap Jewels:** Click on a jewel and then click on an adjacent jewel to swap them. Or, click and drag a jewel towards an adjacent jewel to swap them.
- **Restart:** If you want to start over, you can click the "New Game" button at the left side of the screen.
- **Exit:** Click the "Exit" button to close the game.

## Dependencies

This game is built using the SDL2 library for graphics and input handling. SDL2_image is used for loading image assets, and SDL2_ttf is used for rendering text. You can find these libraries and their documentation on the [SDL website](https://www.libsdl.org/).

## Getting Started

1. Clone or download this repository (https://github.com/runei/JewelsGame) to your local machine.

2. **Option 1: Build and Run:** You can build the game using your preferred build system (CMake, Makefile, etc.), and then run the built executable to start playing the game.

3. **Option 2: Use Pre-built Executables:** If you prefer not to build and compile the game yourself, there are pre-built executables available for Linux, Windows, and macOS in the "build" folder of this repository. You can directly run the appropriate executable for your platform to start playing the game without the need for compilation.

4. Make sure you have the required dependencies (SDL2, SDL2_image, SDL2_ttf) installed on your system.

5. Enjoy the Jewel Swap Game and have a blast swapping and matching those dazzling jewels!

## Credits

This game was inspired by classic match-3 puzzle games like Bejeweled. The implementation and code structure were developed by Leonardo Invernizzi.
