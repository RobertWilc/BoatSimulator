#include "simu.h"

/************************************************************************/
/*                                                                      */
/* Setters                                                              */
/* Getters are in the class declaration above.                          */
/*                                                                      */
/************************************************************************/
void simu::setBoatXCoords(const int& x, const int& y)
{
    boatX.x = x;
    boatX.y = y;
}

void simu::setBoatYCoords(const int& x, const int& y)
{
    boatY.x = x;
    boatY.y = y;
}

void simu::setSimulationZone(const int& n) {
    gridSize = n;
    std::cout << "Grid size: " << gridSize << std::endl;
    coords center = { gridSize / 2, gridSize / 2 };
    /* ZONE A COORDINATES */
    int aWidthHeight = boatX.x + ((gridSize / 8) / 2);
    int aXY = boatX.x - ((gridSize / 8) / 2);
    /* ZONE B COORDINATES */
    int bWidthHeight = boatX.x + (gridSize / 8);
    int bXY = boatX.x - (gridSize / 8);
    /* ZONE C COORDINATES */
    int cWidthHeight = boatX.x + ((gridSize / 8) * 2);
    int cXY = boatX.x - ((gridSize / 8) * 2);

    a = { aXY, aXY, aWidthHeight, aWidthHeight },
        b = { bXY, bXY, bWidthHeight, bWidthHeight },
        c = { cXY, cXY, cWidthHeight, cWidthHeight },
        d = { 0, 0, gridSize, gridSize };
    printf("A Width: %d A Height: %d A x: %d A y: %d\n", a.width, a.height, a.x, a.y);
    printf("B Width: %d B Height: %d B x: %d B y: %d\n", b.width, b.height, b.x, b.y);
    printf("C Width: %d C Height: %d C x: %d C y: %d\n", c.width, c.height, c.x, c.y);
    printf("D Width: %d D Height: %d D x: %d D y: %d\n", d.width, d.height, d.x, d.y);
}
/************************************************************************/
/*                                                                      */
/* end of Setters                                                       */
/*                                                                      */
/*                                                                      */
/************************************************************************/



/************************************************************************/
/*                                                                      */
/* Randomizes boat Y coordinates                                        */
/* Chooses a direction from an array of coordinates                     */
/* Directions are (N, NE, E, SE, S, SW, W, NW, STAY, GO INVIS)          */
/*                                                                      */
/************************************************************************/
void simu::randomizeBoatCoords()
{
    const coords options[] = { {0, 1},   // North
                               {1, 1},   // NorthEast
                               {1, 0},   // East
                               {1, -1},  // SouthEast
                               {0, -1},  // South
                               {-1, -1}, // SouthWest
                               {-1, 0},  // West
                               {-1, 1},  // NorthWest
                               {0, 0},   // Stay
                               {-(boatY.x + 1), -(boatY.y + 1)} // Invisible. This will subtract the boats current position by the current position +1 to obtain -1, -1
    };

    if (boatY.x != -1 && boatY.y != -1)
    {
        coords r = options[rand() % 10];
        setBoatYCoords(boundsCheck(boatY.x + r.x),
            boundsCheck(boatY.y + r.y)
        );
    }
    else
    {
        setBoatYCoords(rand() % gridSize,
            rand() % gridSize
        );
    }
}

/************************************************************************/
/*                                                                      */
/* Checks to make sure the new coordinates are in bounds.               */
/* If they are not, then it sets the coords to the boundry              */
/* instead of going out of bounds                                       */
/*                                                                      */
/************************************************************************/
int simu::boundsCheck(const int& var) {
    if (var < gridSize + 1 && gridSize > -1) return var;
    else if (var >= gridSize) return gridSize;
    else if (var <= -2) return -99;
}

/************************************************************************/
/*                                                                      */
/* end of Getters and setters                                           */
/*                                                                      */
/*                                                                      */
/************************************************************************/



/************************************************************************/
/*                                                                      */
/* Adds to the queue and randomizes the coords                          */
/* until it reaches max queue size                                      */
/*                                                                      */
/*                                                                      */
/************************************************************************/
void simu::addToQueue()
{
    do
    {
        queue.push_back(boatY);
        randomizeBoatCoords();
    } while (queue.size() < MAX_QUEUE_SIZE);

}



/************************************************************************/
/*                                                                      */
/* Where the brains of the simulation lay.                              */
/*                                                                      */
/*                                                                      */
/************************************************************************/
int simu::brains()
{
    std::ofstream file;                                            // DEBUG ************************************************* TAKE OUT BEFORE SUBMISSION
    file.open("file.txt");                                         // DEBUG ************************************************* TAKE OUT BEFORE SUBMISSION

    // Have a predefined max iterations size.
    // Program stops if it reaches the max iteration without
    // calling the "termination" conditions.
    for (int i = 0; i < MAX_ITERATIONS; ++i)
    {
        // outputs the boat coordinates as long as there is still
        // coordinates in the queue
        if (queue.size() > MIN_QUEUE_SIZE)
        {
            coords temp = queue.back(); //puts the next set of coords into a temp variable
            file << temp.x << "," << temp.y << std::endl; //send temp coords to file
            printf("%d, %d, %c", temp.x, temp.y, zoneCheck(temp));
                                                          //std::cout << temp.x << "," << temp.y << "," << zoneCheck(temp);     // DEBUG ************************************************* TAKE OUT BEFORE SUBMISSION
            

            if (termCondition == 1)
            {
                if (zoneCheck(temp) == 'A')
                {
                    
                    std::cout << "\nTermination conditions met, exiting simulation. . ." << std::endl;
                    return 0;
                }
                else
                {
                    std::cout << "\t\t\r" << std::flush;
                    queue.pop_back(); //takes the coordinates out of the queue
                }
            }
        }
        // If the queue runs low, it refills with a new set of
        // directions for the boat.
        else
        {
            addToQueue();
        }

        // Depending on tick amount, it waits that length of time.
        // Smaller tick number means less "FPS", highger tick means more "FPS".
        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / ticks));
    }

    file.close();                                                  // DEBUG ************************************************* TAKE OUT BEFORE SUBMISSION
}

char simu::zoneCheck(const coords& boat)
{
    if (boat.x == -1 && boat.y == -1) return 'N';
    else if (boat.x > a.x &&
        boat.y > a.y &&
        boat.x < a.width &&
        boat.y < a.height) return 'A';
    else if (boat.x > b.x &&
        boat.y > b.y &&
        boat.x < b.width &&
        boat.y < b.height) return 'B';
    else if (boat.x > c.x &&
        boat.y > c.y &&
        boat.x < c.width &&
        boat.y < c.height) return 'C';
    else if (boat.x > d.x &&
        boat.y > d.y &&
        boat.x < d.width &&
        boat.y < d.height) return 'D';

}

void simu::simulation()
{
    // seeds rand() with the time to create pseudo randomness
    srand(time(nullptr));

    // Prompt for information.
    // How many times do you want to iterate?
    // How big do you want the field?
    std::cout << "How many ticks do you want a second? 1-1000 " << std::endl;
    std::cin >> ticks;
    std::cout << "What grid size do you want " << std::endl;
    std::cin >> gridSize;

    // Sets the class to proper information from user.
    setBoatXCoords(gridSize / 2, gridSize / 2);
    setBoatYCoords(rand() % gridSize, rand() % gridSize);
    setSimulationZone(gridSize);

    // Lets user know that everything has initialized and is good to go.
    std::cout << "initialized boat X and boat Y" << std::endl;;
    std::cout << "Boat X starting coords: " << getBoatX().x << " " << getBoatX().y << std::endl;
    std::cout << "Boat Y starting coords: " << getBoatY().x << " " << getBoatY().y << std::endl;
    std::cout << "Grid Size: " << getGridSize() << std::endl;

    brains();
}
/************************************************************************/
/*                                                                      */
/* simu class END                                                       */
/*                                                                      */
/************************************************************************/