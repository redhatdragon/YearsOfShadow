## ECHO ENGINE

Welcome to a data oriented and painstakingly crafted engine.  There's a lot of reasons to consider using this engine as it evolves over time.  Though in spirit taking elements from raw low level game frameworks, it'll be gradually crafted into an easy plug and play tool hopefully almost anyone could benefit from.

Be of good cheer! o7



## WARNING: under construction...
Under chaotic development at the moment, this is still in it's baby steps, please have a little patience, it'll get ironed out hopefully soon.



## INSTALL
* Clone/download EchoEngineExample repo: https://github.com/redhatdragon/EchoEngineExampleProjects

* (Optional) copy ./blank to wherever you'd like

* Follow readme.md and build the ./blank project directory

* Profit



# Design
* Plug and play, and portable!  Ideally this should work under any hardware under the sun so long as it at least supports C++ compilation, without hassle from you the end user.  All engine modules will be painstakingly designed to be portable as possible, using fallbacks if necessary, throwing warnings over anything of note if necessary.

* Structured as data oriented AND data driven as possible, intending to support immense flexability to game logic with runtimes we can be proud of.  Why must we be so restrictive on the end user?

* Light weight, we want to avoid object bloat as much as possible.  It helps networking and overall throughput.

* Game programming pattern agnostic, a basic simple enough API that using whatever custom game programming pattern you wish shouldn't be totally painful.  We won't step on your toes!

* It's API should be so simple a mere mortal can use it.

* It's free (no seriously free, as in liberty), The engine is for the most part under the unlicense with exception and respect to whatever software it happens to optionally use (you can swap/remove most of it easily).

* MODULARITY!!!  All engine systems should be agnostic of one another, even put into their own github repos such that downloading/using them in your own projects (without this engine as a whole) won't be a hassle.  We all deserve freedom of knowledge and technology.  This engine is an aspiration to be a testament of our hearts as engineers, researchers, and scientists.

* Near cutting edge runtimes - hopefully in every aspect but moreso especially for things that effect CPU perf of game systems and physics and other related things.  This should be good enough to support RTS and sandbox genres out of the box without much hassle to the developers, unlike the two 'currently' most popular engines (you know who you are) >.>

* Default support for determinism, many custom engine modules are built to avoid floats aside rendering and things that won't effect the simulation.  And everything is very intentionally tick based.  There are optional third party backends you may use that defeat this, but that'll be a conscious decision on your part.  We are deterministic 'first'.