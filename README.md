BING BONG whatup! My eyes were opened to the possibilities of maze generation recently (see photo). A common approach is to employ a class of algorithms called backtracking algorithms.

**To Run** \
Make sure the **picture.h** file from the previous ImageEditor project is in the same directory.

**Description** \
This one in particular is recursive, but you can also use a stack-based solution which may perform better and be easier to reason about. I included the blog with the algorithm I ported into C++. I made several changes, including a static direction array which eliminates much of the separate logic for each direction. And, my program optimizes the creation of the maze by avoiding modulo operations altogether.

**Future Changes**
* paths and walls are the same size, 1 pixel each. I want to make the paths much larger than the walls themselves.
* Program seg-faults at 1300 x 1300 px. Why?? Recursion depth? I want to make my maze arbitrarily large and only limited by the PNG format.

**References**
* [Blog article w/ python code](<https://aryanab.medium.com/maze-generation-recursive-backtracking-5981bc5cc766>)
* [List of maze generation algorithms (with visualizations)](<https://professor-l.github.io/mazes/>)
