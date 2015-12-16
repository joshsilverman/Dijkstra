Part 2A

I implemented Prim using the adjaceny list for the graph. I made the heap/dictionary structure more general so it can now be used to do operations on both vertices (time/distance from origin to vertex) and edges (edge weight).

OutputPrim.txt is a tour using Prim's algorithm. The tour begining at T takes 292 legs and 15.9 miles.

I'm not aware of any bugs in the Prim methods

2B

I was debugging the triangle methods in Tour.h but was unable to get them working. I have left my code which looks for 3 sided polygons with one incoming adjacency and seeks to create a cycle.

My bug is almost certainly in the conditional for testing if a triangle can exist at the bottom of a MST.