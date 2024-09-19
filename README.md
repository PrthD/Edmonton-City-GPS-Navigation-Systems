# Edmonton City GPS Navigation Systems

## Project Overview
The Edmonton City GPS Navigation Systems project provides a graphical interface to navigate and determine the shortest path between two points in Edmonton using a map. The system includes a server component for processing and a client component for interacting with the map.

## Project Structure

- **/server/**: Server-side code and resources
   - `server.cpp`: Main server implementation
   - `dijkstra.cpp`: Implementation of Dijkstra's algorithm
   - `digraph.cpp`: Implementation of directed graph data structure
   - `digraph.h`: Header file for directed graph
   - `wdigraph.h`: Header file for weighted directed graph
   - `dijkstra.h`: Header file for Dijkstra's algorithm
   - `edmonton-roads-2.0.1.txt`: Road network data file
   - `Makefile`: Build instructions for server components
- **/client/**
   - `client.py`: Client-side implementation
- **/map/**: Directory for map-related resources
- `Makefile`: Top-level build instructions

### Server Makefile Targets
- `make server`: Builds the executable `server`
- `make clean`: Removes all object files and the executable
- `make server.o`: Builds the object file for `server.cpp`
- `make digraph.o`: Builds the object file for `digraph.cpp`
- `make dijkstra.o`: Builds the object file for `dijkstra.cpp`

### Top-level Makefile Targets
- `make run`: Builds the executable `server` and runs both the server and the client
- `make clean`: Removes all object files, executable, and named pipes
- `make all`: Executes both compile and run targets
- `make compile`: Builds the executable `server` in the server directory

## Running Instructions
1. Navigate to the `server` directory in the terminal:
   ```bash
   cd server
2. Compile and link the server-related files:
   ```bash
   make server
3. Return to the soln directory and run both the server and the client:
   ```bash
   cd ..
   make run
4. Interact with the map:
   - Click on locations with the left mouse button to select a starting point and an endpoint.
   - The shortest path between the two points will be highlighted.
   - Use the W, S, D, A keys to navigate through the map.
   - Use Q and E keys to zoom in and out, respectively.
5. Clean up after running:
   ```bash
   make clean

## Precautions
- Follow the running instructions accurately to ensure proper functionality.
- The program is designed to run on Linux-based operating systems only.
- Ensure the server executable is built before running the Makefile in the soln directory.
- The client program must not be run before the server program; otherwise, the program will fail.
- When selecting the second point on the map, the server will send an 'E', and when the map window is closed, the server will send a 'Q'.
