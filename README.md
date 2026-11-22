# Project2

Build `Project2.slnx` with Visual Studio's C++ workload. Set `VCPKG_ROOT` to
your vcpkg installation or enable user-wide integration with `vcpkg integrate install`.
The manifest restores Catch2 for the existing tests. A Gaussian influence kernel is
provided in `Project2/Gaussian.cpp`.

Build and run the tests:

```powershell
msbuild Project2.slnx /p:Configuration=Debug /p:Platform=x64
.\x64\Debug\Project2.exe
```

Build and run a Catch2-free simulation on a 30x30 map with two units on each team:

```powershell
msbuild Project2.slnx /p:Configuration=Debug /p:Platform=x64 /p:BuildSimulation=true
.\x64\Debug\Simulation.exe
```
