```
=======================================================================
  01. PROJECT DESCRIPTION
=======================================================================

  This project implements a simulation of autonomous units competing
  for territorial control in a two-dimensional environment.

  Each unit independently evaluates possible movements based on
  territorial advantage and the local balance of forces.

  The simulation includes two types of units: Personnel and Vehicle.

  The primary objective is to demonstrate object-oriented software
  design, encapsulation, inheritance, runtime polymorphism, and
  interaction between independent components.

  +-----------------------------------------------------------------+
  |                        MAIN FEATURES                            |
  +-----------------------------------------------------------------+
  |                                                                 |
  |  [01] Autonomous unit movement                                  |
  |  [02] Territorial influence calculation                         |
  |  [03] Local position evaluation                                 |
  |  [04] Combat and counterattacks                                 |
  |  [05] Armor and armor penetration                                |
  |  [06] Damage-dependent territorial control                      |
  |  [07] Removal of destroyed units                                |
  |  [08] Console-based visualization                               |
  |  [09] Unit testing with Catch2                                  |
  |                                                                 |
  +-----------------------------------------------------------------+


=======================================================================
  02. SYSTEM ARCHITECTURE
=======================================================================

  The application is divided into independent components.
  Each component has a specific responsibility.

                         +-------------------+
                         |                   |
                         |    SIMULATION     |
                         |                   |
                         +---------+---------+
                                   |
                   +---------------+---------------+
                   |               |               |
                   v               v               v
             +-----------+  +-------------+  +-------------+
             |           |  |             |  |             |
             | COMMANDER |  | FIELD SYSTEM|  |COMBAT SYSTEM|
             |           |  |             |  |             |
             +-----+-----+  +------+------+  +------+------+
                   |               |                |
                   v               v                v
             +-----------+  +-------------+  +-------------+
             | GEOMETRY  |  |  GRIDFIELD  |  |    UNIT     |
             |  SYSTEM   |  |             |  |             |
             +-----------+  +-------------+  +------+------+
                                                      ^
                                                      |
                                          +-----------+-----------+
                                          |                       |
                                    +-----+------+         +------+-----+
                                    |            |         |            |
                                    | PERSONNEL  |         |  VEHICLE   |
                                    |            |         |            |
                                    +------------+         +------------+

  World owns the units.

  Simulation coordinates World, Commander, FieldSystem,
  GeometrySystem and CombatSystem.

  Components communicate through well-defined interfaces.


-----------------------------------------------------------------------
  02.1 CORE CLASSES
-----------------------------------------------------------------------

  +------------------+-----------------------------------------------+
  | CLASS            | RESPONSIBILITY                                |
  +------------------+-----------------------------------------------+
  | Unit             | Base class for all units                      |
  | Personnel        | Personnel-specific territorial control       |
  | Vehicle          | Armor-based damage protection                 |
  | World            | Unit ownership and lifetime management       |
  | Simulation       | Simulation step coordination                  |
  | Commander        | Movement evaluation and position selection   |
  | FieldSystem      | Territorial influence calculations           |
  | GeometrySystem   | Territorial geometry analysis                |
  | CombatSystem     | Combat and counterattack processing          |
  | GridField<T>     | Two-dimensional data storage                  |
  | Math::Vector     | Two-dimensional vector operations            |
  +------------------+-----------------------------------------------+


-----------------------------------------------------------------------
  02.2 SUPPORTING STRUCTURES
-----------------------------------------------------------------------

  GeometryStats
      Stores area, perimeter, connected components and holes.

  LocalForces
      Stores the local strength of allied and enemy units.

  ScoreBreakdown
      Stores individual components of a position evaluation.


=======================================================================
  03. OBJECT-ORIENTED DESIGN
=======================================================================

-----------------------------------------------------------------------
  03.1 ENCAPSULATION
-----------------------------------------------------------------------

  Class attributes are stored in private sections.

  Public methods provide controlled access to object state.

  Examples:

      Unit::getStrength()
      Unit::getPos()
      Unit::setPos()
      Unit::setCoefficients()

  Each class is responsible for managing its own data.


-----------------------------------------------------------------------
  03.2 INHERITANCE
-----------------------------------------------------------------------

                            +----------------+
                            |      UNIT      |
                            +----------------+
                            |  position      |
                            |  team          |
                            |  strength      |
                            |  damage        |
                            |  area          |
                            +--------+-------+
                                     ^
                                     |
                        +------------+------------+
                        |                         |
                +-------+-------+         +-------+-------+
                |   PERSONNEL    |         |    VEHICLE    |
                +---------------+         +---------------+
                | Control logic |         | Armor logic   |
                +---------------+         +---------------+

  Personnel and Vehicle publicly inherit from Unit.

  They share common properties while implementing different
  behavior where necessary.


-----------------------------------------------------------------------
  03.3 RUNTIME POLYMORPHISM
-----------------------------------------------------------------------

  The project uses virtual methods and method overriding.

  The two main polymorphic mechanisms are:

  [A] DAMAGE PROCESSING

      Unit::receiveAttack()

          |
          +-- Personnel
          |      Uses the base damage behavior.
          |
          +-- Vehicle
                 Checks armor penetration before applying damage.

  [B] TERRITORIAL CONTROL

      Unit::getControlStrength()

          |
          +-- Personnel
          |      Territorial effectiveness decreases faster
          |      as the unit loses strength.
          |
          +-- Vehicle
                 Uses the base implementation based on
                 the unit's current strength.

  FieldSystem and CombatSystem operate through the Unit interface.

  They do not need to identify the concrete type of each unit
  when invoking these virtual methods.


-----------------------------------------------------------------------
  03.4 COMPOSITION AND OBJECT OWNERSHIP
-----------------------------------------------------------------------

  World owns its units using:

      std::vector<std::unique_ptr<Unit>>

  This provides automatic memory management.

  Simulation stores references to the existing system objects
  rather than creating or owning them.

  FieldSystem contains its own GridField<float> object.


-----------------------------------------------------------------------
  03.5 OPERATOR OVERLOADING
-----------------------------------------------------------------------

  The Math::Vector class overloads two operators:

      operator+
          Adds two vectors.
          Used to calculate candidate positions.

      operator-
          Subtracts two vectors.
          Used in distance calculations.

  Both operators are used in the application.


-----------------------------------------------------------------------
  03.6 TEMPLATES
-----------------------------------------------------------------------

  GridField<T> is a reusable two-dimensional container.

  It supports access to individual cells and provides information
  about the grid dimensions.

  The implementation includes explicit instantiations for
  float and int.


-----------------------------------------------------------------------
  03.7 EXCEPTIONS
-----------------------------------------------------------------------

  Standard C++ exceptions are used for invalid operations.

      std::invalid_argument
          Invalid unit data or null unit insertion.

      std::out_of_range
          Grid access outside valid boundaries.

  Catch2 tests verify selected invalid-input behavior.


=======================================================================
  04. SIMULATION WORKFLOW
=======================================================================

  Each simulation step follows the sequence below.

                     +-----------------------+
                     |                       |
                     |   BEGIN SIMULATION    |
                     |                       |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |   CALCULATE INITIAL   |
                     |    INFLUENCE FIELD    |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |   SELECT NEXT UNIT    |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |   EVALUATE POSSIBLE   |
                     |      POSITIONS        |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |   UPDATE POSITION     |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |    PROCESS COMBAT     |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |   UPDATE INFLUENCE    |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |   PROCESS REMAINING   |
                     |        UNITS          |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |   REMOVE DESTROYED    |
                     |        UNITS          |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |   RENDER MAP AND      |
                     |     STATISTICS        |
                     +-----------+-----------+
                                 |
                                 v
                     +-----------------------+
                     |      NEXT STEP        |
                     +-----------------------+


=======================================================================
  05. UNIT BEHAVIOR
=======================================================================

-----------------------------------------------------------------------
  05.1 MOVEMENT
-----------------------------------------------------------------------

  Commander evaluates five possible actions:

      [0] Remain in the current position
      [1] Move one cell north
      [2] Move one cell south
      [3] Move one cell east
      [4] Move one cell west

  Positions outside the map are rejected.

  The selected action maximizes the position evaluation score.

  This is a local greedy algorithm without long-term planning.


-----------------------------------------------------------------------
  05.2 TERRITORIAL CONTROL
-----------------------------------------------------------------------

  Each unit contributes to the territorial influence field.

  The contribution depends on:

      - Unit position
      - Unit team
      - Current territorial control strength
      - Distance from the evaluated map cell

  Personnel loses territorial effectiveness more rapidly
  when damaged.

  Vehicles use their remaining strength without an additional
  personnel-specific control penalty.

  The resulting influence field is analyzed by GeometrySystem.


-----------------------------------------------------------------------
  05.3 COMBAT
-----------------------------------------------------------------------

  CombatSystem processes interactions between opposing units.

  An attack can occur when the target is within the attacker's
  configured range.

  Damage is affected by the unit's combat profile.

  A surviving target can counterattack if the attacker is
  within its own attack range.

  Vehicle armor prevents incoming damage unless the attacker's
  armor penetration is sufficient.

  Destroyed units are removed from World.


=======================================================================
  06. BUILD INSTRUCTIONS
=======================================================================

  REQUIREMENTS:

      - Windows
      - Visual Studio with C++ development tools
      - MSVC toolset compatible with the project
      - C++20 support
      - MSBuild
      - vcpkg and Catch2 for tests

  The repository contains:

      Project2.slnx
      Project2/Project2.vcxproj
      vcpkg.json

  Use Visual Studio Developer PowerShell or Developer Command Prompt.


-----------------------------------------------------------------------
  06.1 BUILD AND RUN TESTS
-----------------------------------------------------------------------

  From the repository root:

      msbuild Project2.slnx /p:Configuration=Debug /p:Platform=x64

  Run:

      .\x64\Debug\Project2.exe

  Configure VCPKG_ROOT or vcpkg integration if required.


-----------------------------------------------------------------------
  06.2 BUILD AND RUN SIMULATION
-----------------------------------------------------------------------

  From the repository root:

      msbuild Project2.slnx /p:Configuration=Debug /p:Platform=x64 /p:BuildSimulation=true

  Run:

      .\x64\Debug\Simulation.exe

  The BuildSimulation property selects main.cpp and excludes
  TestCore.cpp from the simulation build.


=======================================================================
  07. CONSOLE VISUALIZATION
=======================================================================

  DEFAULT SCENARIO:

      Map size     : 60 x 60
      Total steps  : 60
      Team 0       : 2 Personnel, 1 Vehicle
      Team 1       : 5 Personnel

  MAP SYMBOLS:

      +--------+----------------------------------+
      | SYMBOL | DESCRIPTION                      |
      +--------+----------------------------------+
      |   A    | Team 0 Personnel                 |
      |   V    | Team 0 Vehicle                   |
      |   B    | Team 1 Personnel                 |
      |   +    | Positive territorial influence   |
      |   -    | Negative territorial influence   |
      |   .    | Neutral / low-influence cell     |
      +--------+----------------------------------+

  The console displays a status line followed by the map.

  OUTPUT FORMAT:

      Step 1/60: team 0 area=..., team 1 area=...,
                 coefA=12, coefR=0.05

      A=team 0 personnel, V=team 0 vehicle,
      B=team 1 personnel, '+/-'=influence

  The exact map and territorial statistics depend on the
  current simulation state.


=======================================================================
  08. UNIT TESTING
=======================================================================

  TEST FRAMEWORK: Catch2

  The test suite contains the following scenarios:

      [01] Vector arithmetic and distance
      [02] GridField cell storage
      [03] Influence grid initialization
      [04] Signed movement prediction
      [05] World unit ownership and invalid arguments
      [06] Polymorphic territorial control
      [07] GeometryStats default values
      [08] Geometry analysis of connected regions

  The polymorphism test constructs Personnel and Vehicle through
  pointers to the Unit base class and verifies their different
  territorial control behavior.


=======================================================================
  09. UML DOCUMENTATION
=======================================================================

  A separate UML class diagram describes:

      - Classes and attributes
      - Public and private methods
      - Inheritance
      - Composition
      - Dependencies between systems

  UML FILE:

      docs/class-diagram.png

  The diagram is intended to be stored separately from this
  ASCII documentation.


=======================================================================
  10. PROJECT LIMITATIONS
=======================================================================

  The simulation is an abstract software model.

  The following features are outside the current implementation:

      - Long-term path planning
      - Advanced coordination between units
      - Detailed environmental simulation
      - Realistic military behavior
      - Complex tactical decision-making

  These limitations are intentional.

  The main goal is to demonstrate modular object-oriented design,
  runtime polymorphism and interactions between simulation systems.

```
