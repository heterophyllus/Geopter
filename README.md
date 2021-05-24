# Geopter

![demo](images/Geopter_demo_windows.png)
## Overview
Geopter is optical design software based on geometrical ray tracing.

As the first noteworthy feature, Geopter provides intuitive graphical user interface, putting a priority on usability.  Geopter aims to be a comprehensive environment where the user can focus on optical design. 

From the viewpoint of license, Geopter is a free and open source software, distributed with source codes so that the user has an access to its most internal aspects. It will provide an introductory experience of optical design and computing.

## Getting Started
### Prebuilt Binaries
Prebuilt binaries for Windows can be downloaded in [Release page](https://github.com/heterophyllus/Geopter/releases/latest).

### Build from Source
Qt is supposed to be installed in your computer.
All of the dependencies except Qt itself have been included in this repository, so QtCreator can build project by reading CMakeLists.txt.

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
This project is licensed under GPL license - see the [LICENSE](LICENSE.md) for details.


## Contributing
Contributions and feedbacks are greatly appreciated.
Please read [CONTRIBUTING](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.

## Acknowledgement
The author would like to express respect and gratitude to the following excellent projects for their reference implementation of optical calculation.

- [mjhoptics/ray-optics](https://github.com/mjhoptics/ray-optics) : Highly matured python module for geometrical optical simulation.  Most of Geopter's optics calculation is based on translation of this implementation.
- [Goptical](https://www.gnu.org/software/goptical/) : C++ implemenation of optical simulation with great deal of functionalities. The original project, however, has not been updated since 2012.
- [dibyendumajumdar/goptical](https://github.com/dibyendumajumdar/goptical) : A progressive fork of Goptical.  It still continues to be maintained with greatest effort.

