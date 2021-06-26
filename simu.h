#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

#define MIN_QUEUE_SIZE 10
#define MAX_QUEUE_SIZE 3000
#define MAX_ITERATIONS 40000

struct coords
{
    int x{}, y{};
};

struct zone
{
    int x{}, y{}, width{}, height{};
};

class simu
{
    int ticks;
    int gridSize; // Maximum size of the simulation grid
    coords boatX, boatY; // Coordinates for boat x and boat y
    zone a, b, c, d; // The zones that the boats can be inside.
    int termCondition = 1; // What the termination condition will be to stop the simulation.
                           // By default it is set to 1 (Terminate if boat enters zone A).
    std::vector<coords> queue; // 3000 is the max size of the queue.
public:
    //boatSimulator(int n) : gridSize(n) {} // initializes the boat simulator. Requires N(grid size)

    /* Getters */
    coords getBoatY() const { return boatY; };
    int getGridSize() const { return gridSize; };
    coords getBoatX() const { return boatX; };
    /* Setters */
    void setBoatXCoords(const int& x, const int& y);
    void setBoatYCoords(const int& x, const int& y);
    void setSimulationZone(const int& N);
    void simulation();

    void randomizeBoatCoords(); // Randomizes boat Y coordinates
    int boundsCheck(const int& var); // Makes sure that boat Y does not go out of bounds.
    char zoneCheck(const coords& boat);
    void addToQueue(); // Adds directions(new boat coordinates) to the queue.

    int brains(); // Conducts all of the work for the simulator.
};

