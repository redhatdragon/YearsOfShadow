At the moment this doc is mostly meant for folks writing mods, game systems, or doing any general gameplay programming.  But should generally be kept in mind for those of us doing backend work as well.

# RULES
* Prefer HAL_Alloc(T, count) over malloc.
* Prefer HAL_LOG ect over printf/cout ect.
* Prefer using HAL to do IO, threading, networking, whenever possible over using any libraries standard or not.  Give it a quick read over there's likely some useful utility functions.
* Avoid as much as possible to store state in any System.
* DO NOT RUN SYSTEMS LOGIC WITH ASYNC STYLE THREADING!  This will screw up the delicate synced threading technique being used, read below for details.
* For any System which seems as though it may become a runtime bottleneck, aside generally optimizing, use differment and the thread pool already allocated for you in System.h if possible.  Utility functions are inside HAL.  For an example on how to thread check out SystemEnemy.h.
* All game systems must totally finish before a bit less than 16ms.  Any wrench thrown into this guarentee may cause crashes or worse.  See above ^