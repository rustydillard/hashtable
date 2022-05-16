This is my work as a student at CSUSM.
At this point, I'm just trying to get used to coding larger programs and data structures, but I do hope this helps someone that needs it.

This is a hash table that implements the separate chaining method through the use of an array and linked lists at each index of the array corresponding to that hash value.
The hashing algorith is DJBX33A: hash = hash * 33 + c, where c is the ascii value of the character being added.
A manual entry into the hash table is implemented, but an automatic entry generator is also possible.
