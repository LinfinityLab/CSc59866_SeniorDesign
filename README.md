# Image Processing Application
*Basic image processing application written in C++ and OpenGL with Qt*

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
```
cd src/
qmake qip.pro
make
```
generates an executable file called 'qip', then run qip.

## Examples
- Blur

![blur input](/screenshots/blur_in.png)
![blur output](/screenshots/blur_out.png)
