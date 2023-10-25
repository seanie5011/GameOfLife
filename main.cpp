#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

// function used to account for wrapped world
int wrapValue(int v, int vMax) {
    int remainder = v % vMax;
    // have to account for if remainder is negative
    if (remainder < 0) {
        return vMax - remainder;
    } else {
        return remainder;
    }
}

int main()
{
    // initialise random seed with current time
    std::srand(std::time(NULL));

    // constants for building grid and cells
    const int CELL_SIZE = 30;  // size of each cell in pixels
    const sf::Vector2f CELL_SIZE_VECTOR(CELL_SIZE, CELL_SIZE);  // used to define the width and height of a cell when creating a shape
    const int GRID_WIDTH = 30;  // width of grid in cells
    const int GRID_HEIGHT = 20;  // height of grid in cells
    const int N_CELLS = GRID_HEIGHT * GRID_WIDTH;

    // use delay between timesteps
    const int DELAY = 300;

    // build grid using integer arrays
    // have two grids, one is the grid in the next timestep, the other is the current grid
    int grid[N_CELLS] = {};
    int gridNext[N_CELLS];
    // initialise with random cells
    for (int i = 0; i < N_CELLS; i++) {
        // know that rand() can be transformed to double safely, so static_cast
        // get random number between 0 and 1, if less than 0.1 then it is alive (1)
        grid[i] = (static_cast<double>(std::rand()) / RAND_MAX) < 0.1 ? 1 : 0;
    }

    // create window and start gameloop
    sf::RenderWindow window(sf::VideoMode(CELL_SIZE * GRID_WIDTH, CELL_SIZE * GRID_HEIGHT), "Game of Life");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear window with white background
        window.clear(sf::Color::White);

        // loop over whole grid
        for (int x = 0; x < GRID_WIDTH; x++) {
            for (int y = 0; y < GRID_HEIGHT; y++) {
                // draw cell
                sf:: RectangleShape cell;
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                cell.setSize(CELL_SIZE_VECTOR);
                cell.setOutlineThickness(1);
                cell.setOutlineColor(sf::Color(153, 153, 153));
                // check if cell is alive or dead
                if (grid[x + y * GRID_WIDTH] == 1) {
                    // alive
                    cell.setFillColor(sf::Color::Black);
                } else {
                    cell.setFillColor(sf::Color::White);
                }
                // draw cell
                window.draw(cell);

                // now  will update all cells with rules of game
                int numberNeighbours = 0;  // how many neighbours current cell has
                // pass through surrounding neighbours
                for (int i =-1; i < 2; i++) {
                    for (int j = -1; j < 2; j++) {
                        // account for world wrapping
                        int xi = wrapValue(x + i, GRID_WIDTH);
                        int yj = wrapValue(y + j, GRID_HEIGHT);
                        numberNeighbours += grid[xi + yj * GRID_WIDTH];
                    }
                }

                // now go about updating the next grid
                numberNeighbours -= grid[x + y * GRID_WIDTH];  // dont count current cell as a neighbour
                // apply rules of game and update next grid
                if (grid[x + y * GRID_WIDTH] == 1 && (numberNeighbours < 2 || numberNeighbours > 3)) {
                    // now dead cell
                    gridNext[x + y * GRID_WIDTH] = 0;
                } else if (numberNeighbours == 3) {
                    // cell now alive
                    gridNext[x + y * GRID_WIDTH] = 1;
                } else {
                    gridNext[x + y * GRID_WIDTH] = grid[x + y * GRID_WIDTH];  // keep current cell in next grid
                }
            }
        }

        // move gridNext to grid
        for (int i = 0; i < N_CELLS; i++) {
            grid[i] = gridNext[i];
        }

        // display to screen
        window.display();

        // delay before moving to next timestep
        sf::sleep(sf::milliseconds(DELAY));
    }

    return 0;
}