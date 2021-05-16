# Geopter

![demo](images/Geopter_demo_windows.png)
## Overview
Geopter is optical design software based on geometrical ray tracing.

As the first noteworthy feature, Geopter provides intuitive graphical user interface. While there are many projects on optical simulation library, what seems to be unsatisfactory is their usability. Geopter aims to be comprehensive environment where the user can focus on optical design. 

From the viewpoint of license, Geopter is a free and open source software. Although some powerful production such as Zemax, CODEV etc have been published, they are not open and not affordable for non-professional users.  Geopter is available freely and is distributed with source code so that the user has an access to its most internal aspects, getting an introductory experience of optical design.

## Getting Started
### Prebuilt Binaries
Prebuilt binaries can be downloaded in Release page.

### Build
Qt is supposed to be installed in your computer.
All dependencies except Qt itself have been included in this repository, so QtCreator can build project by reading CMakeLists.txt.


Another way is to create Visual Studio Solution using CMake.


### Examples
Some example lenses are stored in "example" folder in json format.  In Geopter GUI, go File->Open and select a json file.

## Dependencies

|Library|Description|
|---|---|
|[Qt](https://www.qt.io)|widget toolkit for GUI|
|[Qt Advansed Docking System](https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System)|Flexible docking layout system|
|[QCustomPlot](https://www.qcustomplot.com)|Graph plotting library |
|[nlohman/json](https://github.com/nlohmann/json)|JSON utility|
|[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)|template library for linear algebra|
|[ttk592/spline](https://github.com/ttk592/spline)|spline interpolation|

## License
This software is licensed under GPL license - see the [LICENSE](LICENSE.md) for details.


## Contributing
Contributions and feedbacks are greatly appreciated.
Please read [CONTRIBUTING](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Acknowledgement
The author would like to express respect and gratitude to the following excellent projects for their reference implementation of optical calculation.

- [mjhoptics/ray-optics](https://github.com/mjhoptics/ray-optics) : Python implementation of geometrical optical simulation.
- [Goptical](https://www.gnu.org/software/goptical/) : C++ implemenation of optical simulation. It has a great deal of functionalities, but the original project has not been updated since 2012.
- [dibyendumajumdar/goptical](https://github.com/dibyendumajumdar/goptical) : A progressive fork of Goptical.  It still continues to be developed with greatest effort.
