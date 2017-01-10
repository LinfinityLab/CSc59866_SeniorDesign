# Image Processing Application
Basic image processing application written in C++ and OpenGL with Qt on a starter program given by Professor [George Wolberg](http://www-cs.engr.ccny.cuny.edu/~wolberg/)

## Interface
The application interface is implemented with Qt.
Qt is a cross-platform application framework for developing application software.
Qt libraries are used in this application to create windows, layouts, menus, buttons, widgets and etc.

![interface](/screenshots/interface.png)

## Image Filters
- Point Operations
  * [x] Threshold
  * [x] Clip
  * [x] Contrast Enhancement
  * [x] Gamma Corretion
  * [x] Quantization
  * [x] Histogram Stretching
  * [x] Histogram Matching
- Neighborhood Operations
  * [x] Error Diffusion
  * [x] Blur
  * [x] Sharpen
  * [x] Median Filter
  * [x] Convolution
  * [x] Correlation

## Compile & Run Application
[Qt](https://www.qt.io/) must be installed.
```
cd src/
qmake qip.pro
make
```
generates an executable file called 'qip', then run qip.

## Examples
- **Contrast Enhancement**

![contrast input](/screenshots/contrast_in.png)
![contrast output](/screenshots/contrast_out.png)

- **Blur**

![blur input](/screenshots/blur_in.png)
![blur output](/screenshots/blur_out.png)

- **Median Filter**

![median input](/screenshots/median_in.png)
![median output](/screenshots/median_out.png)

- **Correlation**

![correlation input](/screenshots/correlation_in.png)
![correlation output](/screenshots/correlation_out.png)
