# boatSimulator

A simple boat simulation.

You set the amount of "ticks" per second and N for grid size of the simulation zone.

Boat X is placed at the center of the grid(N x N) and boat Y randomly spawns somewhere inside of the grid. Boat Y then proceeds to randomly choose a direction and travel one unit in that direction. The boat can randomly disapear with the coordinates (-1, -1). The boat will then re-appear somehwere on the grid randomly. Once boat Y reaches zone "A" which is a predefined area around boat X that is based off of the grid size. 

Overall  there are 4 zones; A-D. A being the smallest and closest to boat X and gets progressivly bigger in size as you get to D.
