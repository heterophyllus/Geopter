# Geopter

![demo](data/screenshots/Geopter_demo_windows.png)

## Introduction
Geopter is <u>**opt**</u>ical design software based on <u>**geo**</u>metrical ray tracing.

In the field of optical design, there have been several well-known proprietary software with advanced features. They must be a definitive choice for professional work, but they are not available for personal use due to expensive annual licensing fees.

For personal users, open source projects may well be another option to get in touch with optical design. In fact, more and more optical simulation library have been developed recently. One of the drawbacks of these libraries is that highly matured programming skills are required to master how to use them.

Geopter aims to be somewhere in between. This software features to emphasize reasonable graphical interface, allowing the users to focus on the optical design process. Also, as is common with open source projects, the software is freely available and its whole internal aspects are open to the public, which let the users learn, validate and even modify it on their own.

The primary user of this software may be a fresh optical engineer or a student having interests in optics.  Geopter hopes to be a starting platform to gain an introductory experience in optical design.


## Getting Started
### Prebuilt Binaries
Prebuilt binaries can be downloaded in [Release page](https://github.com/heterophyllus/Geopter/releases/latest).

### Examples
Some example lenses are stored in "example" folder in json format.  In Geopter GUI, go File->Open and select a json file.

### Python scripting
Unlike other optical design software such as Zemax and CodeV, Geopter doesn't have its own "macro" language.  Instead, Python is embedded to utilize for scripting. The "app" and "osys" object allow access to the application and optical system.
For the momemt, only a few functions are implemented.

### Note
In Geopter, the array index for fields, wavelengths and other arrays begin with 0. This is because of the necessity to keep consistency with Python as scripting language.


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
- Hiiragi <heterophyllus.work@gmail.com>


## Acknowledgement
The author would like to express respect and gratitude to the following excellent projects for their reference implementation of optical calculation.

- [mjhoptics/ray-optics](https://github.com/mjhoptics/ray-optics) : Highly matured python module for geometrical optical simulation.  Most of Geopter's optics calculation is based on this implementation.
- [Goptical](https://www.gnu.org/software/goptical/) : C++ implemenation of optical simulation with great deal of functionalities. Though this original project has not been updated since 2012.
- [dibyendumajumdar/goptical](https://github.com/dibyendumajumdar/goptical) : A modified fork of Goptical, which has been maintained with greatest effort. This project features to analyze many lens data from patent documents.


