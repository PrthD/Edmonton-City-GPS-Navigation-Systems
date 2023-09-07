// ---------------------------------------------------
// Name: Parth Dadhania
// SID: 1722612
// CCID: pdadhani
// AnonID: 1000330704
// CMPUT 275, Winter 2023
// Assessment: Assignment - Navigation System (Part 2)
// ---------------------------------------------------

#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <list>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstring>

#include "wdigraph.h"
#include "dijkstra.h"

#define Msg_Size 1024

struct Point
{
  long long lat, lon;
};

// return the manhattan distance between the two points
long long manhattan(const Point &pt1, const Point &pt2)
{
  long long dLat = pt1.lat - pt2.lat, dLon = pt1.lon - pt2.lon;
  return abs(dLat) + abs(dLon);
}

// find the ID of the point that is closest to the given point "pt"
int findClosest(const Point &pt, const unordered_map<int, Point> &points)
{
  pair<int, Point> best = *points.begin();

  for (const auto &check : points)
  {
    if (manhattan(pt, check.second) < manhattan(pt, best.second))
    {
      best = check;
    }
  }
  return best.first;
}

// read the graph from the file that has the same format as the "Edmonton graph" file
void readGraph(const string &filename, WDigraph &g, unordered_map<int, Point> &points)
{
  ifstream fin(filename);
  string line;

  while (getline(fin, line))
  {
    // split the string around the commas, there will be 4 substrings either way
    string p[4];
    int at = 0;
    for (auto c : line)
    {
      if (c == ',')
      {
        // start new string
        ++at;
      }
      else
      {
        // append character to the string we are building
        p[at] += c;
      }
    }

    if (at != 3)
    {
      // empty line
      break;
    }

    if (p[0] == "V")
    {
      // new Point
      int id = stoi(p[1]);
      assert(id == stoll(p[1])); // sanity check: asserts if some id is not 32-bit
      points[id].lat = static_cast<long long>(stod(p[2]) * 100000);
      points[id].lon = static_cast<long long>(stod(p[3]) * 100000);
      g.addVertex(id);
    }
    else
    {
      // new directed edge
      int u = stoi(p[1]), v = stoi(p[2]);
      g.addEdge(u, v, manhattan(points[u], points[v]));
    }
  }
}

int create_and_open_fifo(const char *pname, int mode)
{
  // create a fifo special file in the current working directory with
  // read-write permissions for communication with the plotter app
  // both proecsses must open the fifo before they perform I/O operations
  // Note: pipe can't be created in a directory shared between
  // the host OS and VM. Move your code outside the shared directory
  if (mkfifo(pname, 0666) == -1)
  {
    cout << "Unable to make a fifo. Make sure the pipe does not exist already!" << endl;
    cout << errno << ": " << strerror(errno) << endl
         << flush;
    exit(-1);
  }

  // opening the fifo for read-only or write-only access
  // a file descriptor that refers to the open file description is
  // returned
  int fd = open(pname, mode);

  if (fd == -1)
  {
    cout << "Error: failed on opening named pipe." << endl;
    cout << errno << ": " << strerror(errno) << endl
         << flush;
    exit(-1);
  }

  return fd;
}

/*
  Description: This function converts the given character array into two Point objects and records
               their coordinates.

  Arguments:
      input (char*): The array to convert
      start (Point*): The starting point to record coordinates
      end (Point*): The ending point to record coordinates

  Returns: This function does not return anything.
*/
void convertArrayToPoints(char *input, Point *start, Point *end)
{
  // create a vector to store the coordinates extracted from the input array
  vector<double> coordinates;

  // create a temporary string to store each coordinate as it is extracted from the input array
  string temp = "";

  // iterate until four coordinates have been extracted and stored in the vector
  int index = 0;
  while (coordinates.size() < 4)
  {
    // if the current character is a space or newline character, convert the temporary string
    // to a double and add it to the coordinates vector
    if (input[index] == ' ' || input[index] == '\n')
    {
      coordinates.push_back(stod(temp) * 100000);
      temp = "";
    }
    // else, add the current character to the temporary string
    else
    {
      temp += input[index];
    }

    // iterator increment
    index++;
  }

  // assign the first two coordinates in the vector to the starting point's latitude and longitude,
  // and the last two coordinates to the ending point's latitude and longitude
  start->lat = static_cast<long long>(coordinates[0]);
  start->lon = static_cast<long long>(coordinates[1]);
  end->lat = static_cast<long long>(coordinates[2]);
  end->lon = static_cast<long long>(coordinates[3]);

  return;
}

// keep in mind that in part 1, the program should only handle 1 request
// in part 2, you need to listen for a new request the moment you are done
// handling one request
int main()
{
  WDigraph graph;
  unordered_map<int, Point> points;

  const char *inpipe = "inpipe";
  const char *outpipe = "outpipe";

  // Open the two pipes
  int in = create_and_open_fifo(inpipe, O_RDONLY);
  cout << "inpipe opened..." << endl;
  int out = create_and_open_fifo(outpipe, O_WRONLY);
  cout << "outpipe opened..." << endl;

  // build the graph
  readGraph("server/edmonton-roads-2.0.1.txt", graph, points);

  // read a request
  while (true)
  {
    Point sPoint, ePoint;
    char line[Msg_Size] = {0};
    int bytes_read = read(in, line, Msg_Size);

    // Exit program
    if (line[0] == 'Q')
    {
      close(in);
      close(out);
      unlink(inpipe);
      unlink(outpipe);

      return 0;
    }
    convertArrayToPoints(line, &sPoint, &ePoint);

    // get the points closest to the two points we read
    int start = findClosest(sPoint, points), end = findClosest(ePoint, points);

    // run dijkstra's algorithm, this is the unoptimized version that
    // does not stop when the end is reached but it is still fast enough
    unordered_map<int, PIL> tree;
    dijkstra(graph, start, tree);

    // NOTE: in Part II you will use a different communication protocol than Part I
    // So edit the code below to implement this protocol

    // path found
    if (tree.find(end) != tree.end())
    {
      // read off the path by stepping back through the search tree
      list<int> path;
      while (end != start)
      {
        path.push_front(end);
        end = tree[end].first;
      }
      path.push_front(start);

      // output the path by sending each point's latitude and longitude as a string through the output pipe
      for (int v : path)
      {
        double out_lat = static_cast<double>(points[v].lat) / 100000;
        double out_lon = static_cast<double>(points[v].lon) / 100000;

        string temp_str = to_string(out_lat) + " " + to_string(out_lon) + "\n";
        char out_line[temp_str.length()];
        for (int i = 0; i < sizeof(out_line); i++)
        {
          out_line[i] = temp_str[i];
        }
        write(out, out_line, sizeof out_line);
      }
    }
    
    // send an End message through the output pipe to signal the end of the path
    char out_line[2] = {'E', '\n'};
    write(out, out_line, sizeof out_line);
  }

  // close and unlink from named pipes
  close(in);
  close(out);
  unlink(inpipe);
  unlink(outpipe);

  return 0;
}