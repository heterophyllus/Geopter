# Geopter

![demo](data/screenshots/Geopter_demo_windows.png)

## Introduction
Geopter is <u>**opt**</u>ical design software based on <u>**geo**</u>metrical ray tracing.

There have been developed several proprietary optical design software with advanced functionalities. They must be a definitive choice for professional work, but never be used for personal work due to expensive annual licensing fees.

This is where free software comes in as the first option for those who are beginning to grow an interest in optics. Fortunately, more and more optical design libraries are being developed recently. One of their drawbacks, however, is to require matured programming skills to master.

Geopter aims to be somewhere in between. It features an intuitive graphical interface that enables to concentrate on the optical design process. Also, as is common with open source projects, Geopter is freely available and its whole internal aspects are open to the public. This feature allows anyone to learn, validate and even modify it on their own.

The primary user of Geopter could be a fresh optical engineer or a student.  Geopter hopes to be a starting platform to gain an introductory experience in optical design.


## Getting Started
### Prebuilt Binaries
Prebuilt binaries can be downloaded from [Release page](https://github.com/heterophyllus/Geopter/releases/latest).

### Examples
Some example lenses are stored in "example" folder in json format.  In Geopter GUI, go File->Open and select a json file.

### Python scripting
Unlike the other optical design software such as Zemax and CodeV, Geopter doesn't have its own "macro" language.  Instead, the embedded Python console enables command scripting in order to automate a complex procedure. The "app" and "osys" object provide accesses to the application and the optical system respectively.
For the moment, only a few functions have been implemented.

### Note
In Geopter, the array indexes for fields, wavelengths and other arrays start at 0 because of the necessity for consistency with Python.


## Development
### Dependencies

|Library|Description|
|---|---|
|[Qt](https://www.qt.io)|widget toolkit for GUI|
|[Qt Advansed Docking System](https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System)|Flexible docking layout system|
|[QCustomPlot](https://www.qcustomplot.com)|Graph plotting library |
|[nlohman/json](https://github.com/nlohmann/json)|JSON utility|
|[Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page)|template library for linear algebra|
|[ttk592/spline](https://github.com/ttk592/spline)|spline interpolation|
|[PythonQt](https://mevislab.github.io/pythonqt/index.html)|dynamic Python binding for the Qt framework|


### Building from Source
All of the dependencies except Qt itself have been included in this repository.
This application is built with Qt5 and Qt6 is currently not supported.


## License
This project is licensed under GPL license - see the [LICENSE](LICENSE.md) for details.


## Contributing
Contributions and feedbacks are greatly appreciated.
Please read [CONTRIBUTING](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.


## Contact
- Hiiragi [ e-mail ](<heterophyllus.work@gmail.com>)


## Acknowledgement
The author would like to express respect and gratitude to the following excellent projects for their reference implementation of optical calculation.

- [mjhoptics/ray-optics](https://github.com/mjhoptics/ray-optics) : Highly matured python module for geometrical optical simulation.  Most of Geopter's optics calculation is based on this implementation.
- [quartiq/rayopt](https://github.com/quartiq/rayopt) : Another python optical design library with more advanced features.
- [Goptical](https://www.gnu.org/software/goptical/) : C++ implemenation of optical simulation with great deal of functionalities. Though this original project has not been updated since 2012.
- [dibyendumajumdar/goptical](https://github.com/dibyendumajumdar/goptical) : A modified fork of Goptical which has been maintained with greatest effort. This project features to analyze many lens data from patent documents.


