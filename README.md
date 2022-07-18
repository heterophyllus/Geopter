# Geopter

![demo](data/screenshots/Geopter_demo_windows.png)

## Introduction
Geopter is <u>**opt**</u>ical design software based on <u>**geo**</u>metrical ray tracing.

Nowadays there have been developed several commercial software for optical design.  Their advanced functionality deserves a definitive choice for professional work, while their expensive licensing fees discourages trying for private purpose.

This is why free software becomes the first choice for starters in optics. Fortunately, an increasing number of optical design libraries are being developed as open source projects these days. One of their drawbacks, however, is that they require matured programming skills to master or even to get touch with.

Geopter aims to be somewhere in between. As is common with open source projects, Geopter is freely available and its whole internal aspects are open to the public. This fundamental feature allows anyone to use, learn, validate and even modify it on their own.  It also features an intuitive graphical interface putting higher priority to let the user concentrate on the design workflow.

The primary user of Geopter could be a fresh optical engineer or a student who majors in optics.  Geopter wishes to be a starting platform to gain an introductory experience in optical design.


## Getting Started
### Prebuilt Binaries
Prebuilt binaries can be downloaded from [Release page](https://github.com/heterophyllus/Geopter/releases/latest).

### Examples
Some example lenses are stored in "example" folder in json format.  In Geopter GUI, go File->Open and select a json file.

### Python scripting
Geopter doesn't have its own "macro" language unlike the exemplary software such as Zemax and CodeV.  Instead, the embedded Python console enables command scripting in order to automate a complex procedure. The "app" and "osys" object provide the accesses to the application and the optical system respectively.
For the moment, only a few functions have been implemented.

### Note
In Geopter, the array indexes(fields, wavelengths, etc) start at 0 because of the necessity for consistency with Python.


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
All of the dependencies except Qt itself have been included in this repository.  Use cmake to generate Visual Studio solution.

Note that this application is built with Qt5 and Qt6 is currently not supported.


## License
This project is licensed under GPL license - see the [LICENSE](LICENSE.md) for details.


## Contributing
Contributions and feedbacks are greatly appreciated.
Please read [CONTRIBUTING](CONTRIBUTING.md) for details on our code of conduct, and the process for submitting pull requests to us.


## Contact
- Hiiragi [ e-mail ](mailto:heterophyllus.work@gmail.com)


## Acknowledgement
The author would like to express the heartfelt respect and gratitude to the following excellent projects for their reference implementation of optical calculation.

- [mjhoptics/ray-optics](https://github.com/mjhoptics/ray-optics) : Highly matured python module for geometrical optical simulation developed by the engineer who worked at Synopsys and ORA.
- [quartiq/rayopt](https://github.com/quartiq/rayopt) : Another python optical design library with more advanced features.
- [Goptical](https://www.gnu.org/software/goptical/) : C++ implemenation of optical simulation with great deal of functionalities. Though this original project has not been updated since 2012.
- [dibyendumajumdar/goptical](https://github.com/dibyendumajumdar/goptical) : A modified fork of Goptical which has been maintained with greatest effort. This project features to analyze many lens data obtained from patent documents.


