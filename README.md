# CacheFalseSharing

Demonstration of CPU cache False Sharing problem:
https://mechanical-sympathy.blogspot.ru/2011/07/false-sharing.html

Tests will be performed simultaneously in THREADS_CNT threads.

1) Modification of the same variable in array (distance between values = 0).

2) Modification of the different variables that are very near to each other in the same array.
   They will be placed in the same CPU cache line (distance 1).
   
3) Modification of the different variables that are very far from each other in the same array.
   They will be placed in the different CPU cache lines (distance 2000).

[Prefmonitor](http://www.cpuid.com/softwares/perfmonitor-2.html) screenshot demonstrates, how many latency has been added to the each instruction in the tests:

![CPU cycles](/falsesharing.png?raw=true "screenshot")
