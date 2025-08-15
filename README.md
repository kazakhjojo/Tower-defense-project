# Tower-defense-project
Repo for a tower defense game I am working on
This is a project focused on creating a ASCII-art tower defense game, where you can place towers that attack enemies that try to reach a destination via a certain path. 
Next steps as of 7th august 2025:
print the path textures on the map, as well as towers and enemies. Hardcoded for now
Reference from ESP SS 2025 A4: 
# Assignment 4: Crossy Frog

## Learning Goals
File I/O, Nontrivial Data Structures (e.g. 2D-Array, Linked List, Tree), Bit Operations, Dynamic Memory, Command Line Arguments

## Specification
Some general specifications must be followed for every assignment in this course. A list of these specifications can be
found here: [Assessment of Assignments - Deductions](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3?pvs=4#e3203e50b1fc4ba787bdbf5fb46262ae).
Please follow these specifications to avoid point deductions. 

Furthermore, there is no maximum input length. *Theoretically*, the program should be capable of handling an **input of infinite length**.

## Submission
* File: a4.c
* **Deadline:** 11.06.2025, 23:59 CET

## Grading

| Category	    | Points | 
|---------------|--------|
| Functionality | 27.3   |
| Code Quality  | 11.7   |
|               | 39     |

**Attention:** In order to pass the course, you must pass at least half of the public test cases! (Among other conditions)

More detailed information can be found in the [grading scheme](https://coding-tugraz.notion.site/Assessment-of-Assignments-How-do-you-get-your-points-d7cca6cc89a344e38ad74dd3ccb73cb3). 

---

## Description
This assignment will be about an arcade classic Crossy Frog. The premise of the game is that we, as the player, control a frog that needs to cross several hazardous zones:

    A street filled with moving cars
    A water section where the frog must ride logs
    A safe resting area between the street and the water

Everytime the frog moves, the elements on the map will move as well.
The frog has a limited number of moves before the game ends. Avoid being hit by vehicles or falling into the water without a ride!

<details>
<summary><h2>Command Line Arguments</h2></summary>

The program receives five optional command line arguments: `--size`, `--moves`, `--street-distance`, `--water-distance`, and `--seed`.
Each of these arguments comes with some parameters, as described in the table below. The arguments can appear in any order, 
however, the parameters must directly follow the argument; otherwise, an error is thrown (see [Error Messages and Exit Codes](#error-messages-and-exit-codes)). 
If any of the arguments is not given in the command line, its default value should be used, as described in the table below.  

| Argument            | Num. Params (Type) | Parameters      | Default  | Bounds (lower, upper)   | Description                                                               |
|---------------------|--------------------|-----------------|----------|-------------------------|---------------------------------------------------------------------------|
| `--size`            | 2 (int, int)       | `height width ` | **9 40** | 6, INT_MAX / 6, INT_MAX | number of lanes and number of tiles in width                              |
| `--moves`           | 1 (int)            | `moves`         | **50**   | 1, INT_MAX              | the number of moves the player has to reach the goal                      |
| `--street-distance` | 2 (int, int)       | `min max`       | **4 20** | 2, INT_MAX              | the minimum and maximum distance between two vehicles on a street         |
| `--water-distance`  | 2 (int, int)       | `min max`       | **4 8**  | 2, INT_MAX              | the minimum and maximum distance between two objects on a water lane      |
| `--seed`            | 1 (int)            | `seed`          | **0**    | INT_MIN, INT_MAX        | the seed for the [pseudorandom generator](#pseudorandom-number-generator) |

An argument may be given multiple times; in that case, anything but its last occurrence should be ignored.
If the arguments are given in a wrong format or not within their respective bounds, an error is thrown and the 
program should exit with the corresponding exit code (see [Error Messages and Exit Codes](#error-messages-and-exit-codes)).

</details>

<details>
<summary><h2>Map Layout</h2></summary>

The width and height of the map are specified by the `--size` argument.

The map is represented by a set of stacked lanes. Each lane has a height of 2 ASCII characters.
The number of lanes is given by the `height` of the map. Since every lane take sup 2 characters, the height of the map
in terms of ASCII characters is `height * 2`.
The width of the map in terms of ASCII characters corresponds to the `width` parameter from the `--size` argument.

There are three types of lanes in the game, each with its own characteristics and ASCII representations.

- *Road lanes* - represented by `' '` (Spaces): These lanes are filled with cars 
- *Water lanes* - represented by `'~'`: These lanes are filled with logs
- *Safe zone* - represented by `░`: These lanes are resting areas, where the frog can rest and plan its next move.

The layout of these lanes from top to bottom is always as follows:

| Lane Type | Number of Lanes         |
|-----------|-------------------------|
| Safe zone | 1                       |
| Water     | ceil(( lanes - 3 ) /2)  |
| Safe zone | 1                       |
| Road      | floor(( lanes - 3 ) /2) |
| Safe zone | 1                       |

_The ceiling function `ceil()` rounds up to the nearest integer, and the floor function `floor()` rounds down to the nearest integer._

<details>
<summary><h3>Map Output</h3></summary>

### Printing Safe Zones

A Safe zone lane starts with three spaces (`' '`), followed by a Hashtag (`'#'`) followed by `width` times the character `░`.
At the end another Hashtag (`'#'`) is placed.

This is one safe zone lane, with a width of 10 characters:

````
   #░░░░░░░░░░#
   #░░░░░░░░░░#
````

### Printing Water and Road Lanes

Instead of three leading spaces, the water and road lanes start with a number indicating the speed of the lane,
followed by the direction of the lane (either `'>'` or `'<'`), followed by a space (`' '`).
The direction and speed are randomly generated (see [Map initialisation](#map-initialisation)).
The speed is an integer from `1` to `4`.

A Single 10 character wide water lane with a speed of 2 and a direction to the right would look like this:

````
2> #~~~~~~~~~~#
2> #~~~~~~~~~~#
````

A single 10 character wide road lane with a speed of 1 and a direction to the left would look like this:

````
1< #          #
1< #          #
````

Here is an example of an 8x40 map (8 lanes, which are 2 characters high and 40 characters wide).

````
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░# <- This is a safe zone and the goal of the frog
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# <- Water lane
4< #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
2> #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# <- Another water lane
2> #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
2> #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~# <- Another water lane
2> #~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░# <- Safe zone
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
1> #                                        # <- Road lane
1> #                                        #
2< #                                        # <- Another road lane
2< #                                        #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░# <- Safe zone
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
````

In this map are 3 water lanes because ceil((8 - 3) / 2) = 3. 
The water lanes are followed by 2 road lanes, because floor((8 - 3) / 2) = 2.

### Map Objects

#### Frog

The frog is represented by the following ASCII art:

````
oo
/\
````

The movement of the frog can then be controlled with a set of commands (see [Gameplay](#gameplay)).
The goal of the player is to navigate the frog from the bottom of the map to the top of the map,
without being hit by a vehicle or falling into the water.

#### Cars

**Road lanes** contain **cars**, which have the following ascii representation when moving from left to right:

````
||O\_
O---O
````

Or the following ascii representation when moving from right to left:

````
_/O||
O---O
````

#### Logs

**Water lanes** contain **logs**. Logs are represented by `'B'`s. The length of logs
is variable and can be between 3 and 10 characters (see [Map Generation Algorithm](#map-initialisation)).
This is a log of length 4:

````
2> #  BBBB                   #
2> #  BBBB                   #
````

</details>


<details>
<summary><h3>Map Initialisation</h3></summary>
<a id="map-initialisation"></a>

Everytime the [start](#commands) command is entered, the map should be initialized. 
The frog is placed in the bottom safe zone of the map, in the middle of the lane.
If the lane width is odd, the frog is placed one field to the left of the middle. 

````
   #░░░oo░░░░#
   #░░░/\░░░░#
````

Otherwise exactly in the middle of the lane.

````
   #░░░░oo░░░░#
   #░░░░/\░░░░#
````

Then an initial map layout is generated.
First, the speed and direction of the lane are randomly generated. 
For this the function `random(min, max)` is used.
See [Pseudorandom Number Generator](#pseudorandom-number-generator) for more detail.
This process is described in the following pseudocode:

````
for each lane from top to bottom:
    random_number <- random(0, 1)
    if random_number equals 0:
        direction <- right
    else:
        direction <- left
    speed <- random(1, 4)
    
    setLaneSpeedAndDirection(speed, direction)
````

Things to note:
- The direction is generated before the speed! Additionally, the lanes speed and direction are generated from top 
to bottom.
- The random function is also called for safe zones! (even though the values are never used)

Next the obstacles are placed on the map. This also happens randomly. The process
uses the `--street-distance` and `--water-distance` arguments to determine the minimum and maximum distance between objects.
The process uses three key functions:

- `initializeMap()`: This iterates over the lane positions from the end of the lane to the start of the lane
  and calls the `randomlySpawnObject` function for each position.
- `getFirstObstaclePosition()`: This function reports the position of the first obstacle in the lane.
- `randomlySpawnObject(position)`: This function randomly spawns an object at the given position.


#### Initialize Map

The following pseudocode describes the `initializeMap` function:

````
function initializeMap:
    for lane from top to bottom:
        if lane is a safe zone:
            continue
        for position from end of lane to start of lane:
            randomlySpawnObject(position)
`````

Note, first position from the end is width - 1 and the last position is 0.

#### First Position Of An Obstacle

The first position of the obstacle should be the index of the
first character of the obstacle in the lane.

<details>
<summary><h5>Example</h5></summary>

In this example the first position of the obstacle is 2.

````
2> #  BBBB                   #
2> #  BBBB                   #
````

In the following example the first position of the obstacle is 3.

````
2< #                  BBBB   #
2< #                  BBBB   #
````
</details>

The function `getFirstObstaclePosition` should return a very large number if there is no obstacle in the lane.

#### Randomly Spawn An Obstacle

The following pseudocode describes the `randomlySpawnObject` function:

````
function randomlySpawnObject(position):
     first_obstacle_position <- getFirstObstaclePosition()
     
     if first_obstacle_position - position <= minimum_distance:
         return
     
     upper_limit_for_random <- 0
     if no obstacle on lane:
         upper_limit_for_random <- position - lane_width + maximum_distance + 1
     else:
         upper_limit_for_random <- position - first_obstacle_position + maximum_distance + 1   
 
     random_number = random(0, upper_limit_for_random)
     if random_number equals 0:
         if lane is street:
             spawn_new_car(position)
         else if lane is water:
             length <- random(minimum_length, maximum_length)
             spawn_new_log(position, length)
````

Note, that the values of `minimum_distance`, `maximum_distance`
are given by the `--street-distance` and `--water-distance` arguments. They depend on the type of the lane.
The `minimum_length`, and `maximum_length` are 3 and 10.
Also, if the algorithm decides to spawn a new object at a `position`, the tip of the object should be at that `position`.

<details>
<summary><h5>Example</h5></summary>

````
2< #                   #
2< #                   #
````

In this example, the algorithm decided to spawn a new car at the position 2 on the lane above.

````
2< #                _/O#
2< #                O--#
````

</details>
</details>

<details>
<summary><h3>Updating The Lanes</h3></summary>
<a id="updating-the-map"></a>

Everytime the frog moves, or the `stay` command is used, the map should be updated, 
and the objects on the map should move, in the direction of the lane at the speed of the lane.
Logically that happens **after** the frog has moved.
The speed of the lane determines the number of characters an object moves in one update.
If the frog is on a log, it should move with the log.

<details>
<summary><h5>Example</h5></summary>
If the speed of a lane is 2 and the direction is right, a car would move like that:

Before the update:

````
2> #   ||O\_         #
2> #   O---O         #
````

After the update

````
2> #     ||O\_       #
2> #     O---O       #
````

</details>

If the object reaches the end of the lane, it will disappear, character by character, until it is gone.
Once no character is visible on the lane, the object should cease to exist. This is important
for the `save` and `load` commands.

<details>
<summary><h5>Example</h5></summary>

E.g. if the speed of a street is 2 and the direction is left, a Car would move like that:
 
Before the update

````
2< #_/O||                 #
2< #O---O                 #
````
After the update

````
2< #O||                   #
2< #--O                   #
````

</details>

When the objects are updated, it must also be decided whether a new object should be spawned at the start of a lane (position 0). 
This is done by the `randomlySpawnObject` function after the objects have been moved.

If the speed of the lane is greater than 1, the obstacles should be moved in steps of one character at
a time until the correct number of characters has been moved. After every step, the `randomlySpawnObject` function
should be called to check if a new object should be spawned at the start of the lane.

</details>

<details>
<summary><h3>Putting It All Together</h3></summary>
<a id="putting-it-all-together"></a>

If the game **is running** (or one time when the player won or lost) the map should be printed after every **successful** command execution
The map should be printed from the top lane to the bottom lane.
Note, that the frog is prioritized over all other objects. If the frog is on the same position as another object,
the frog should be drawn.
The map is followed up by an empty line.
Under this empty line should be an indicator of the number of moves left.

```
Moves left: <number of moves left>\n
```

<details>

<summary><h4>Example</h4></summary>

````
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #~BBB~~~~~~~BBBB~~~~~~~BBB~~~~~~~BBBBB~~~#
4< #~BBB~~~~~~~BBBB~~~~~~~BBB~~~~~~~BBBBB~~~#
4< #~~~~~~~BBBBB~~~~~~~BBB~~~~~~~~BBBBBBB~~~#
4< #~~~~~~~BBBBB~~~~~~~BBB~~~~~~~~BBBBBBB~~~#
1> #BB~~~~~BBB~~~~BBBBBBBBB~~~~~~~BBBBBBBBB~#
1> #BB~~~~~BBB~~~~BBBBBBBBB~~~~~~~BBBBBBBBB~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #O\_       ||O\_               ||O\_     #
4> #--O       O---O               O---O     #
4> #||O\_                    ||O\_          #
4> #O---O                    O---O          #
3< #           _/O||                     _/O#
3< #           O---O                     O--#
   #░░░░░░░░░░░░░░░░░░░oo░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░/\░░░░░░░░░░░░░░░░░░░#

Moves left: 50
````

</details>
</details>
</details>

<details>
<summary><h2>Gameplay</h2></summary>

When the game first starts, the command line arguments are parsed.
If that didn't result in any errors, which would lead to an exit code,
the [pseudorandom number generator](#pseudorandom-number-generator) 
should be seeded with the seed given by the `--seed` argument.
After that, the game should print the following welcome message:
    
```
Welcome to Crossy Frog! Get ready for an adventure!\n\n
```

From now on, the player can enter commands to control the frog or save/load the game.

### Commands
When running, the program should always output the following command prompt (mind the spaces):

```
 > 
```

If the prompt is not printed, the testcases will time out!
The user can then enter commands to control the frog or save/load the game.
**The length of the input is not limited.** The following commands are available:

| Command | Parameters  | Description                                       |
|---------|-------------|---------------------------------------------------|
| `start` | *none*      | Generates the map and starts the game             |
| `up`    | *none*      | Makes the frog go 2 characters upwards            |
| `down`  | *none*      | Makes the frog go 2 characters downwards          |
| `left`  | *none*      | Makes the frog go 2 characters to the left        |
| `right` | *none*      | Makes the frog go 2 characters to the right       |
| `stay`  | *none*      | The frog stays in place for one turn              |
| `crawl` | `direction` | The frog moves only by one character instead of 2 |
| `save`  | `filename`  | Saves the current game to a file                  |
| `load`  | `filename`  | Loads a game from a file                          |
| `quit`  | *none*      | Quits the game                                    |
| `EOF`   | *none*      | Quits the game without a message                  |

If a command is entered that is not in the list above or if the command is missing arguments,
the program should print an [error message](#error-messages-and-exit-codes) and continue with another command prompt.
More than one space between commands and parameters will not be tested.

Initially, the player has the option to either start the game, load a game from a file, or quit the game.
If the game is running, the map should be printed after every successful command execution (except for quit and EOF).

Also, everytime the frog moves (or the `stay` command is used), the map should be updated.

If the frog reaches the top of the Safe Zone, the game should end and the following message should be printed.
Note, that the program execution should not end, but give the player the option to start a new game.

```
Congratulations! You made it to the top!\n\n
```

The game can also be lost. The following things can happen and should be handled in this order:

| Event                                            | Message                                        |
|--------------------------------------------------|------------------------------------------------|
| The player runs out of moves                     | `Game Over! You ran out of moves!\n\n`         |
| Frog collides with a vehicle                     | `Game Over! You got run over!\n\n`             |
| Frog falls into the water                        | `Game Over! You drowned!\n\n`                  | 
| Frog gets flushed off of the map on a water lane | `Game Over! You got flushed from the map!\n\n` |

If the game is lost or won, the map should be printed one last time, after printing
the message.

<details>
<summary><h4>EOF</h4></summary>

Ends the game without a message with exit code 0. Note, that this is not the string `"EOF"`, but the value that indicates
that the input stream has been closed (e.g. when the terminal is closed).
EOF in a terminal can be triggered by pressing `Ctrl+D` key combination on Unix systems and `Ctrl+Z` on Windows systems.

</details>

<details>
<summary><h4>quit</h4></summary>

Ends the game with exit code 0 and the following message:

```
Bye!\n
```

</details>

<details>
<summary><h4>start</h4></summary>

When the `start` command is entered, the game starts. The frog is placed
at its starting position and the map is initialized with cars and logs
according to the map generation algorithm.
Throughout the game, the player should always be able to enter this command to restart the game. After every
call, a new map should be generated. 
The number of remaining moves should be reset to the value given by the `--moves` argument.
After that, the following message should be printed:

```
The Game started! Hop fast, dodge smart and reach the top!\n\n
```

Followed by the map and the number of moves left.

<details>
<summary><h4>Example</h4></summary>

````
 > start
The Game started! Hop fast, dodge smart and reach the top!

   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #~~~~~BBBBBBBBBB~~~~~~~BBBBBB~~~~~~~BBBBB#
4< #~~~~~BBBBBBBBBB~~~~~~~BBBBBB~~~~~~~BBBBB#
4< #~~~~~~~~BBBBBBBB~~~~~BBBB~~~~~BBBBBB~~~~#
4< #~~~~~~~~BBBBBBBB~~~~~BBBB~~~~~BBBBBB~~~~#
1> #~~~~~~BBBBBBBB~~~~BBBB~~~~~~~BBBBBBB~~~~#
1> #~~~~~~BBBBBBBB~~~~BBBB~~~~~~~BBBBBBB~~~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #O\_     ||O\_              ||O\_        #
4> #--O     O---O              O---O        #
4> #_              ||O\_               ||O\_#
4> #O              O---O               O---O#
   #░░░░░░░░░░░░░░░░░░░oo░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░/\░░░░░░░░░░░░░░░░░░░#

Moves left: 50
 > 
````

</details>
</details>

<details>
<summary><h4>up, down, left, right, stay</h4></summary>

When the `up`, `down`, `left`, `right` or `stay` command is entered, the frog should move in the corresponding direction
by two ASCII characters or stay in place in case of the `stay` command.

If the frog is at the leftmost or rightmost position, it stays in place and
this will still count as a move. That means the number of moves left should be decremented by one.

If the frog is only able to move one character and would hit one of the ends of the map otherwise, it should
move by only one character. The same holds true for the bottom of the map. If the frog is at the bottom of the map
and moves down, the frog shouldn't move, but the move should still count.

Despite that, the frog can indirectly still be partially moved outside the map if it is on a
log that moves out of the map.

</details>

<details>
<summary><h4>crawl</h4></summary>

The command `crawl` is used to move the frog only by one character instead of two. 
It expects one parameter, which is the direction the frog should move. The direction can be one of the following values:

- `left`
- `right`

If the direction is not valid or the command is missing arguments, the program should print an [error message](#error-messages-and-exit-codes).

Otherwise, the movement behaviour should be the same as with the `left`, `right` commands, but the frog should only move by one character.

</details>
<details>
<summary><h4>load, save</h4></summary>

When the `save` and `load` commands are entered, the current game state should be saved to a file or loaded from a file, respectively.
If the file cannot be opened, the corresponding error message should be printed (see [Error Messages and Exit Codes](#error-messages-and-exit-codes)).

<details>
<summary><h4>save</h4></summary>

The game state should be saved to a file using the format described in the section [File Format](#file-format).
This command should only work if the game has been started. Otherwise, the corresponding error message should be printed.
Once the game state is saved, the following message should be printed.

```
Game saved successfully!\n\n
```

If a file already exists with the same name, it should be overwritten.

</details>

<details>
<summary><h4>load</h4></summary>

The game state should be loaded from a file using the format described in the section [File Format](#file-format).
The current state of the game should be replaced by the state loaded from the file.
Once the game state is loaded, the following message should be printed.

```
Game loaded successfully!\n\n
```

Even if the game hasn't been started yet, after loading the game, the game should be considered started.
The start message should not be printed again.

If the file cannot be opened, or the magic info bytes are missing, the corresponding error message should be printed. (see [Error Messages and Exit Codes](#error-messages-and-exit-codes))

</details>

</details>

</details>

<details>
<summary><h2>File Format</h2></summary>
<a id="file-format"></a>

The file format is a binary file, which contains the following general game state information in that order:

    +----------------------------------------+
    | Magic Info "ESP\n" (4B)                |
    +----------------------------------------+
    | Map Height (4B)                        |
    +----------------------------------------+
    | Map Width (4B)                         |
    +----------------------------------------+
    | Min Street Dist (4B)                   |
    +----------------------------------------+
    | Max Street Dist (4B)                   |
    +----------------------------------------+
    | Min Water Dist (4B)                    |
    +----------------------------------------+
    | Max Water Dist (4B)                    |
    +----------------------------------------+
    | Moves Left (4B)                        |
    +----------------------------------------+
    | Frog Lane Index (4B)                   |
    +----------------------------------------+
    | Frog Position In Lane (4B)             |
    +----------------------------------------+
    | --- Lane 1 Data ---                    |
    | Lane Info Byte (1B)                    |
    | [If obstacles present:]                |
    |    Obstacle Info Byte (1B)             |
    |    Obstacle Position (4B)              |
    |    Obstacle Width (4B)                 |
    |    ... (Additional obstacles) ...      |
    +----------------------------------------+
    | --- Lane 2 Data ---                    |
    | Lane Info Byte (1B)                    |
    | [If obstacles present:]                |
    |    ...                                 |
    +----------------------------------------+
    | ...                                    |
    +----------------------------------------+

If the first four magic bytes are present, the file can be assumed to be valid.
The frog and obstacle positions refer to the index of the leftmost character of the frog or obstacle in the lane.

The lanes are stored from top to bottom. For each lane there is a lane info byte, which contains some information about the lane.
The LSB of the info byte indicates whether there is at least one obstacle on the lane. In that case, an obstacle info byte follows.
The Lane info byte contains the following information:

| **Bits** (from MSB to LSB) | **Field**        | **Possible Values**                                                            |
|----------------------------|------------------|--------------------------------------------------------------------------------|
| **7-6**                    | **Lane Type**    | `00`: Street lane<br>`01`: Water lane<br>`10`: Safe zone                       |
| **5-4**                    | **Lane Speed**   | `00`: Speed = 1<br>`01`: Speed = 2<br>`10`: Speed = 3<br>`11`: Speed = 4       |
| **3**                      | **Direction**    | `0`: Left<br>`1`: Right                                                        |
| **2-1**                    | **Ignored**      | (These bits are not used)                                                      |
| **0**                      | **Obstacle Flag**| `0`: No obstacle on lane<br>`1`: At least one obstacle is present              |

The obstacles are stored in the order from left to right (not in lane direction!) <br>
The info byte of the contains the following information:

| **Bits** (from MSB to LSB) | **Field**         | **Possible Values**                                          |
|----------------------------|-------------------|--------------------------------------------------------------|
| **7-2**                    | **Ignored**       | (These 6 bits are not used)                                  |
| **1**                      | **Obstacle Type** | `0`: Car<br>`1`: Log                                         |
| **0**                      | **Obstacle Flag** | `0`: No additional obstacle<br>`1`: Another obstacle follows |

After the info byte, the position of the obstacle in the lane is stored (4 bytes),
followed by the width of the obstacle in characters (4 bytes).
Cars have a fixed width of 5 characters and the width of logs is variable.

Note: Since the whole obstacle does not have to be on the lane at any given time, the position can be **negative**!

</details>

<details>
<summary><h2>Pseudorandom Number Generator</h2></summary>
<a id="pseudorandom-number-generator"></a>

To generate random numbers, we use the `rand()` function from the C standard library. This function returns a sufficiently
random number on each call.

The numbers are not truly random, but [pseudorandom](https://en.wikipedia.org/wiki/Pseudorandomness). This has the side
effect that the generated numbers are reproducible by using a seed. This seed is passed to the pseudorandom number
generator once, before all numbers are generated. In the C implementation, the function `srand()` is used to pass the
seed to the generator. **Call `srand()` exactly *once* at the very beginning of your program with the seed passed as a
command line argument.**

If the description states that random(a,b) should be used to generate a random number between a and b, this means
the following algorithm should be used.

```
random_number = a + rand() % (b - a + 1)
```

</details>

<details>
<summary><h2>Error Messages and Exit Codes</h2></summary>
<a id="error-messages-and-exit-codes"></a>

While the program is running, the following errors must be detected and handled:

| Exit Code | Error Message                                    | Error to detect                                                                                       | Game phase    |
|-----------|--------------------------------------------------|-------------------------------------------------------------------------------------------------------|---------------|
| 1         | `Out of memory!\n`                               | Out of memory, allocation failed                                                                      | All           |
| 2         | `Invalid number of arguments given!\n`           | The argument count has an inconsistent value                                                          | CLI Arguments |
| 3         | `Unexpected argument provided!\n`                | An argument which does not exist was passed to the program                                            | CLI Arguments |
| 4         | `Invalid argument type!\n`                       | A parameter for an argument was not of the expected type (if a string cannot be parsed as an integer) | CLI Arguments |
| 5         | `Invalid argument value!\n`                      | The value of a parameter was invalid (invalid bounds of an integer)                                   | CLI Arguments |
|           | `Error: Unknown command!\n`                      | An unknown command was entered                                                                        | Gameplay      |
|           | `Error: Command is missing arguments!\n`         | The command requires more arguments than entered                                                      | Gameplay      | 
|           | `Error: Too many arguments given for command!\n` | There were more arguments given than the command requires                                             | Gameplay      |
|           | `Error: Invalid arguments given!\n`              | A given argument was not valid, e.g., because it has the wrong type                                   | Gameplay      |         
|           | `Error: Failed to open file!\n`                  | The file could not be opened/does not exist                                                           | Gameplay      |
|           | `Error: Invalid file content!\n`                 | The magic number in the file was incorrect/not there                                                  | Gameplay      |
|           | `Error: Game is not running!\n`                  | A command was entered before the game was started                                                     | Gameplay      |

Other errors, which are not mentioned here, do not have to be detected or handled; you may assume they will never occur.
If an exit code is specified, the program should terminate with that return value. Otherwise, the program should not
exit, but continue to run. After a successful run without any errors, the program should return 0.

If an error is encountered within the game, while handling a command, the command prompt should be printed again after
the corresponding error message has been output and the game should continue as if the failing command has never been 
entered.

*Note: For the sake of simplicity, we will not test cases where multiple errors occur at once.*

</details>

<details>
<summary><h2>Gameplay examples</h2></summary>

<details>
<summary><h3>Gameplay example 1</h3></summary>

````
Welcome to Crossy Frog! Get ready for an adventure!

 > start
The Game started! Hop fast, dodge smart and reach the top!

   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #~BBBBBB~~~~~~BBBBBBBBB~~~~BBBBBBBBB~~~~~#
4< #~BBBBBB~~~~~~BBBBBBBBB~~~~BBBBBBBBB~~~~~#
4< #~~~BBBBBBBBB~~~~~~BBBB~~~~~~~~BBBBBBBB~~#
4< #~~~BBBBBBBBB~~~~~~BBBB~~~~~~~~BBBBBBBB~~#
1> #~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~BBBB~#
1> #~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~BBBB~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> # ||O\_         ||O\_                    #
4> # O---O         O---O                    #
4> # ||O\_              ||O\_               #
4> # O---O              O---O               #
3< #      _/O||                    _/O||    #
3< #      O---O                    O---O    #
   #░░░░░░░░░░░░░░░░░░░oo░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░/\░░░░░░░░░░░░░░░░░░░#

Moves left: 50
 > up
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #BBB~~~~~~BBBBBBBBB~~~~BBBBBBBBB~~~~~~~BB#
4< #BBB~~~~~~BBBBBBBBB~~~~BBBBBBBBB~~~~~~~BB#
4< #BBBBBBBB~~~~~~BBBB~~~~~~~~BBBBBBBB~~~~~B#
4< #BBBBBBBB~~~~~~BBBB~~~~~~~~BBBBBBBB~~~~~B#
1> #~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~BBBB#
1> #~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~BBBB#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #     ||O\_         ||O\_                #
4> #     O---O         O---O                #
4> #     ||O\_              ||O\_           #
4> #     O---O              O---O           #
3< #   _/O||           oo       _/O||       #
3< #   O---O           /\       O---O       #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 49
 > up
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #~~~~~BBBBBBBBB~~~~BBBBBBBBB~~~~~~~BBBBBB#
4< #~~~~~BBBBBBBBB~~~~BBBBBBBBB~~~~~~~BBBBBB#
4< #BBBB~~~~~~BBBB~~~~~~~~BBBBBBBB~~~~~BBB~~#
4< #BBBB~~~~~~BBBB~~~~~~~~BBBBBBBB~~~~~BBB~~#
1> #~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~BBB#
1> #~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~BBB#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #         ||O\_         ||O\_            #
4> #         O---O         O---O            #
4> #_        ||O\_     oo       ||O\_       #
4> #O        O---O     /\       O---O       #
3< #_/O||                    _/O||          #
3< #O---O                    O---O          #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 48
 > up
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #~BBBBBBBBB~~~~BBBBBBBBB~~~~~~~BBBBBB~~~~#
4< #~BBBBBBBBB~~~~BBBBBBBBB~~~~~~~BBBBBB~~~~#
4< #~~~~~~BBBB~~~~~~~~BBBBBBBB~~~~~BBB~~~~~~#
4< #~~~~~~BBBB~~~~~~~~BBBBBBBB~~~~~BBB~~~~~~#
1> #~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~BB#
1> #~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~BB#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #             ||O\_ oo      ||O\_        #
4> #             O---O /\      O---O        #
4> #||O\_        ||O\_              ||O\_   #
4> #O---O        O---O              O---O   #
3< #||                    _/O||             #
3< #-O                    O---O             #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 47
 > up
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #BBBBBB~~~~BBBBBBBBB~~~~~~~BBBBBB~~~~~BBB#
4< #BBBBBB~~~~BBBBBBBBB~~~~~~~BBBBBB~~~~~BBB#
4< #~~BBBB~~~~~~~~BBBBBBBB~~~~~BBB~~~~~~~BBB#
4< #~~BBBB~~~~~~~~BBBBBBBB~~~~~BBB~~~~~~~BBB#
1> #~~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~B#
1> #~~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~~~B#
   #░░░░░░░░░░░░░░░░░░░oo░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░/\░░░░░░░░░░░░░░░░░░░#
4> #|O\_             ||O\_         ||O\_    #
4> #---O             O---O         O---O    #
4> #    ||O\_        ||O\_              ||O\#
4> #    O---O        O---O              O---#
3< #                   _/O||              _/#
3< #                   O---O              O-#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 46
 > up
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #BB~~~~BBBBBBBBB~~~~~~~BBBBBB~~~~~BBBBB~~#
4< #BB~~~~BBBBBBBBB~~~~~~~BBBBBB~~~~~BBBBB~~#
4< #BB~~~~~~~~BBBBBBBB~~~~~BBB~~~~~~~BBBBB~~#
4< #BB~~~~~~~~BBBBBBBB~~~~~BBB~~~~~~~BBBBB~~#
1> #~~~~~~BBBB~~~~~~~BBBooBBB~~~~BBBBBB~~~~~#
1> #~~~~~~BBBB~~~~~~~BBB/\BBB~~~~BBBBBB~~~~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #   ||O\_             ||O\_         ||O\_#
4> #   O---O             O---O         O---O#
4> #        ||O\_        ||O\_              #
4> #        O---O        O---O              #
3< #                _/O||              _/O||#
3< #                O---O              O---O#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 45
 > crawl left
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #~~BBBBBBBBB~~~~~~~BBBBBB~~~~~BBBBB~~~~~~#
4< #~~BBBBBBBBB~~~~~~~BBBBBB~~~~~BBBBB~~~~~~#
4< #~~~~~~BBBBBBBB~~~~~BBB~~~~~~~BBBBB~~~~~~#
4< #~~~~~~BBBBBBBB~~~~~BBB~~~~~~~BBBBB~~~~~~#
1> #~~~~~~~BBBB~~~~~~~BBooBBBB~~~~BBBBBB~~~~#
1> #~~~~~~~BBBB~~~~~~~BB/\BBBB~~~~BBBBBB~~~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #       ||O\_             ||O\_         |#
4> #       O---O             O---O         O#
4> #            ||O\_        ||O\_          #
4> #            O---O        O---O          #
3< #             _/O||              _/O||   #
3< #             O---O              O---O   #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 44
 > up
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #BBBBBBB~~~~~~~BBBBBB~~~~~BBBBB~~~~~~~BBB#
4< #BBBBBBB~~~~~~~BBBBBB~~~~~BBBBB~~~~~~~BBB#
4< #~~BBBBBBBB~~~~~Boo~~~~~~~BBBBB~~~~~~BBBB#
4< #~~BBBBBBBB~~~~~B/\~~~~~~~BBBBB~~~~~~BBBB#
1> #B~~~~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~#
1> #B~~~~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #           ||O\_             ||O\_      #
4> #           O---O             O---O      #
4> #\_              ||O\_        ||O\_      #
4> #-O              O---O        O---O      #
3< #          _/O||              _/O||      #
3< #          O---O              O---O      #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 43
 > up
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #BBB~~~~~~~BBooBB~~~~~BBBBB~~~~~~~BBBBBBB#
4< #BBB~~~~~~~BB/\BB~~~~~BBBBB~~~~~~~BBBBBBB#
4< #BBBBBB~~~~~BBB~~~~~~~BBBBB~~~~~~BBBBBB~~#
4< #BBBBBB~~~~~BBB~~~~~~~BBBBB~~~~~~BBBBBB~~#
1> #BB~~~~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~#
1> #BB~~~~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> #\_             ||O\_             ||O\_  #
4> #-O             O---O             O---O  #
4> # ||O\_              ||O\_        ||O\_  #
4> # O---O              O---O        O---O  #
3< #       _/O||              _/O||         #
3< #       O---O              O---O         #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 42
 > up
Congratulations! You made it to the top!

   #░░░░░░░░░░░░oo░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░/\░░░░░░░░░░░░░░░░░░░░░░░░░░#
4< #~~~~~~BBBBBB~~~~~BBBBB~~~~~~~BBBBBBB~~~~#
4< #~~~~~~BBBBBB~~~~~BBBBB~~~~~~~BBBBBBB~~~~#
4< #BB~~~~~BBB~~~~~~~BBBBB~~~~~~BBBBBB~~~~~B#
4< #BB~~~~~BBB~~~~~~~BBBBB~~~~~~BBBBBB~~~~~B#
1> #BBB~~~~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~#
1> #BBB~~~~~~~BBBB~~~~~~~BBBBBBBB~~~~BBBBBB~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
4> # ||O\_             ||O\_             ||O#
4> # O---O             O---O             O--#
4> #     ||O\_              ||O\_        ||O#
4> #     O---O              O---O        O--#
3< #    _/O||              _/O||            #
3< #    O---O              O---O            #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 41
 > quit
Bye!
````

</details>

<details>
<summary><h3>Gameplay example 2</h3></summary>

````
Welcome to Crossy Frog! Get ready for an adventure!

 > start
The Game started! Hop fast, dodge smart and reach the top!

   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
2> #BBBB~~~~BBBBBBBBB~~~~~~BBBBBBBBBB~~~~~~~#
2> #BBBB~~~~BBBBBBBBB~~~~~~BBBBBBBBBB~~~~~~~#
1> #BBBBBB~~~~~~~BBBBBBB~~~~~~BBBBBBBBB~~~~~#
1> #BBBBBB~~~~~~~BBBBBBB~~~~~~BBBBBBBBB~~~~~#
1> #~~~~~BBBB~~~~~~~~BBBB~~~~~~BBBBBBBBB~~~~#
1> #~~~~~BBBB~~~~~~~~BBBB~~~~~~BBBBBBBBB~~~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
3< #       _/O||      _/O||    _/O||        #
3< #       O---O      O---O    O---O        #
4> #       ||O\_                    ||O\_   #
4> #       O---O                    O---O   #
4< #     _/O||                  _/O||     _/#
4< #     O---O                  O---O     O-#
   #░░░░░░░░░░░░░░░░░░░oo░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░/\░░░░░░░░░░░░░░░░░░░#

Moves left: 50
 > up
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
2> #BBBBBB~~~~BBBBBBBBB~~~~~~BBBBBBBBBB~~~~~#
2> #BBBBBB~~~~BBBBBBBBB~~~~~~BBBBBBBBBB~~~~~#
1> #BBBBBBB~~~~~~~BBBBBBB~~~~~~BBBBBBBBB~~~~#
1> #BBBBBBB~~~~~~~BBBBBBB~~~~~~BBBBBBBBB~~~~#
1> #~~~~~~BBBB~~~~~~~~BBBB~~~~~~BBBBBBBBB~~~#
1> #~~~~~~BBBB~~~~~~~~BBBB~~~~~~BBBBBBBBB~~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
3< #    _/O||      _/O||    _/O||           #
3< #    O---O      O---O    O---O           #
4> #O\_        ||O\_                    ||O\#
4> #--O        O---O                    O---#
4< # _/O||             oo   _/O||     _/O|| #
4< # O---O             /\   O---O     O---O #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 49
 > up
Game Over! You got run over!

   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
2> #BBBBBBBB~~~~BBBBBBBBB~~~~~~BBBBBBBBBB~~~#
2> #BBBBBBBB~~~~BBBBBBBBB~~~~~~BBBBBBBBBB~~~#
1> #~BBBBBBB~~~~~~~BBBBBBB~~~~~~BBBBBBBBB~~~#
1> #~BBBBBBB~~~~~~~BBBBBBB~~~~~~BBBBBBBBB~~~#
1> #B~~~~~~BBBB~~~~~~~~BBBB~~~~~~BBBBBBBBB~~#
1> #B~~~~~~BBBB~~~~~~~~BBBB~~~~~~BBBBBBBBB~~#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
3< # _/O||      _/O||    _/O||              #
3< # O---O      O---O    O---O              #
4> #  ||O\_        ||O\oo                   #
4> #  O---O        O---/\                   #
4< #||                  _/O||     _/O||     #
4< #-O                  O---O     O---O     #
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#
   #░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░#

Moves left: 48
 > quit
Bye!

````

</details>
</details>