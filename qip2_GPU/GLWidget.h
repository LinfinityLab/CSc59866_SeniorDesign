// ======================================================================
// 
// Copyright (C) 2015 by George Wolberg
//
// GLWidget.h - Header file for openGL display.
//
// Written by: George Wolberg, 2015
// ======================================================================

#ifndef GLWIDGET_H
#define GLWIDGET_H

#define MXPROGRAMS	32
#define MXUNIFORMS	32

#include <QtWidgets>
#include <QGLWidget>
#include <QGLFunctions>
#include <QGLShaderProgram>
#include <QtOpenGL>

typedef QVector2D vec2;
typedef QVector3D vec3;

enum {
	ATTRIB_VERTEX,
	ATTRIB_TEXCOORD
};

typedef std::map<QString, GLuint> UniformMap;

// ----------------------------------------------------------------------
// standard include files
//
class GLWidget : public QGLWidget, protected QGLFunctions {

public:
	GLWidget(QWidget *parent = 0);
	void	setViewport(int w, int h, int ww, int hh);
	void	setInTexture(QImage &);
	void	setOutTexture(QImage &);
	void	allocateTextureFBO(int w, int h);
	void	initShader(QGLShaderProgram &, QString, QString, UniformMap &, int *);
	void	applyFilterGPU(int);
	void	setDstImage(int);

protected:

	void		initializeGL();		// init GL state
	void		resizeGL(int, int);	// resize GL widget
	void		paintGL();		// render GL scene
	void		initVertices();
	void		initBuffers();
	void		initShaders();

private:
	int			m_winW;					// window width
	int			m_winH;					// window height
	int			m_imageW;				// input image width
	int			m_imageH;				// input image height
	std::vector<vec2>	m_points;				// vector of 2D points (XY)
	int			m_numPoints;
	std::vector<vec2>	m_texCoord;				// vector of 2D texture coordinates (s,t)
	GLuint			m_vertexBuffer;				// handle to vertex buffer
	GLuint			m_texCoordBuffer;			// handle to texture coordinate buffer
	GLuint			m_inTexture;				// texture unit for dispalying input
	GLuint			m_outTexture;				// texture unit for dispalying output
	GLuint			m_fbo[2];				// handle to frame buffer object
	GLuint			m_texture_fbo[2];			// texture unit for render to texture
	QGLShaderProgram	m_program;				// GLSL programs
	GLint			m_uniform[MXUNIFORMS];			// uniform vars for each program

	bool			m_imageFlag;				// true if an image is uploaded to GPU
	QMatrix4x4	        m_projection;				// 4x4 projection matrix

};



#endif // GLWIDGET_H
