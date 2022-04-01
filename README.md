# bb2d
<p>
 The BB2D Simple 2D graphics "engine" (Copyright (c) Sebastian Kamil Gluch 2022) written in C++, using SFML by Laurent Gomila and json by Niels Nlohmann. Available under the MIT license found in LICENSE. It is based on another of my projects - Bouncing Balls (https://github.com/SebastianKG02/Bouncing-Balls) with many improvements.
 </p>
 <p>
 Simple facilities to create an extensible or ready-made GameContext wrapper around SFML's RenderWindow class, a multi-threaded Asset Manager (which is by default configurable in JSON format), a Scene Manager (to enable easy switching between gameplay levels, with all resources being managed per-scene), a simple Logger (with flat file output as well as console) and very extensible Scene & UI systems.
 </p>

## bb2dDemo
<p>
 A sample project is provided within this repository, and creates one of the simplest (and valid) use cases for the engine - everything is initalised within the GameContext, assets are loaded from the disk and strips of tiles are shown within the window. It is possible to use a leaner version of this, but the demo has been chosen to embody the minimum definition of the engine functionality.
 </p>

### Licenses
<p>All graphics capabilities are provided by SFML (currently using version 2.5.1) by Laurent Gomila under the zlib/png license found in [sfml-license.md](https://github.com/SebastianKG02/bb2d/files/8400970/sfml-license.md), which can be found at https://www.sfml-dev.org/index.php. </p>
<p>Configurability through the JSON file format (as used to load assets in a "cleaner" format as opposed to the ".properties"-style format used in my Bouncing Balls project is provided by json (by Niels Nlohmann under the MIT license at [nlohmann-json-LICENSE.txt](https://github.com/SebastianKG02/bb2d/files/8400990/nlohmann-json-LICENSE.txt) from https://github.com/nlohmann/json).</p>
