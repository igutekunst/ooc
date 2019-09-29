Implementation
=============


Parser
------
The 'Parser' consumes one byte at a time, and emits unsigned ints when a complete 12 bit number is completed. It uses three states to make the logic
very clear. See the source code to understand the reasoning.

For numbers other than 12 bits, the algorithm could be generalized. A few observations:
    - The number of states would be the least common multiple of 8, and the number of bits, divided by 8.
    - You might be able to reduce the number of states. I haven't thought about this much


Number Statistics
-----------------

The number statistics module consists of two data structures, the RecentValuesList, and the LargestValuesList.

RecentValuesList
++++++++++++++++

The recent values list is implemented using a simple ring buffer. There is not much to it.

LargestValueList
++++++++++++++++

The largest value list is implemented using a Min-heap. A min heap is a data structure of O( log(N)) insertion and deletion.
It is considered a maximally efficient implementation of a priority queue.

You can keep adding items to the Queue until there are LIST_SIZE items. After that, every new item with a value larger than the smallest value in the queue
will displace that small value.

Items can be removed from the queue smallest to largest.
