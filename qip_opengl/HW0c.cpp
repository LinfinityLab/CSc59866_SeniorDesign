// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// HW0c.cpp - HW0c class
//
// Written by: George Wolberg, 2015
// ===============================================================

#include "HW0c.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::HW0c:
//
// HW0c constructor.
//
HW0c::HW0c(QWidget *parent)
	: HW(parent)
{
	// init vars
	m_reverseColor = false;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
HW0c::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions();

	// init vertex and fragment shaders
	initShaders();

	// initialize vertex buffer and write positions to vertex shader
	initVertexBuffer();

	// init state variables
	glClearColor(0.0, 0.0, 0.0, 0.0);	// set background color
	glColor3f   (1.0, 1.0, 0.0);		// set foreground color
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
HW0c::resizeGL(int w, int h)
{
	// save window dimensions
	m_winW = w;
	m_winH = h;

	// compute aspect ratio
	float ar = (float) w / h;

	// set xmax, ymax;
	float xmax, ymax;
	if(ar > 1.0) {		// wide screen
		xmax = ar;
		ymax = 1.;
	} else {		// tall screen
		xmax = 1.;
		ymax = 1 / ar;
	}

	// set viewport to occupy full canvas
	glViewport(0, 0, w, h);

	// init viewing coordinates for orthographic projection
	glLoadIdentity();
	glOrtho(-xmax, xmax, -ymax, ymax, -1.0, 1.0);

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::paintGL:
//
// Update GL scene.
//
void
HW0c::paintGL()
{
	// clear canvas with background color
	glClear(GL_COLOR_BUFFER_BIT);

	// draw triangles
	glDrawArrays(GL_TRIANGLES, 0, 3);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::controlPanel:
//
// Create control panel groupbox.
//
QGroupBox*
HW0c::controlPanel()
{
	// init group box
	QGroupBox *groupBox = new QGroupBox("Homework 0c");

	// create labels
	QLabel *label[2];
	label[0] = new QLabel("Theta");
	label[1] = new QLabel("Reverse color?");

	// create sliders
	m_sliderTheta  = new QSlider(Qt::Horizontal);

	// init sliders
	m_sliderTheta->setRange(0, 360);
	m_sliderTheta->setValue(0);

	// create spinBoxes
	m_spinBoxTheta = new QSpinBox;
	m_spinBoxTheta->setRange(0, 360);
	m_spinBoxTheta->setValue(0);

	// init radio buttons
	m_radio[0] = new QRadioButton("Yes");
	m_radio[1] = new QRadioButton("No");

	// set "No" radio button to be default
	m_radio[1]->setChecked(true);

	// assemble radio buttons into horizontal widget
	QWidget *widget   = new QWidget;
	QHBoxLayout *hbox = new QHBoxLayout;
	hbox->addWidget(m_radio[0]);
	hbox->addWidget(m_radio[1]);
	hbox->addStretch();
	widget->setLayout(hbox);

	// layout for assembling widgets
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(label[0],	  0, 0);
	layout->addWidget(m_sliderTheta,  0, 1);
	layout->addWidget(m_spinBoxTheta, 0, 2);
	layout->addWidget(label[1],	  1, 0);
	layout->addWidget(widget,	  1, 1);

	// assign layout to group box
	groupBox->setLayout(layout);

	// init signal/slot connections
	connect(m_sliderTheta,  SIGNAL(valueChanged(int)), this, SLOT(changeTheta(int)));
	connect(m_spinBoxTheta, SIGNAL(valueChanged(int)), this, SLOT(changeTheta(int)));
	connect(m_radio[0],	SIGNAL(clicked()),	   this, SLOT(reverseOn()));
	connect(m_radio[1],	SIGNAL(clicked()),	   this, SLOT(reverseOff()));

	return(groupBox);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::reset:
//
// Reset parameters.
//
void
HW0c::reset()
{
	// reset parameters
	m_reverseColor = false;

	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(0);
	m_sliderTheta->blockSignals(false);

	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(0);
	m_spinBoxTheta->blockSignals(false);

	// set "No" radio button to be default
	m_radio[1]->setChecked(true);

	// if geometry will change, call initVertexBuffer() by uncommenting next line:
	// initVertexBuffer();

	// reset 4x4 transformation matrix for model
	m_ModelMatrix.setToIdentity();

	// copy data to vertex shader
	glUniformMatrix4fv(m_u_ModelMatrix, 1, GL_FALSE, m_ModelMatrix.constData());
	glUniform1i(m_u_reverseColor, m_reverseColor);

	// draw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW2a::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
HW0c::initShaders()
{
	// compile vertex shader
	if(!m_program.addShaderFromSourceFile(QGLShader::Vertex, ":/vshader0c.glsl")) {
		QMessageBox::critical(0, "Error", "Vertex shader error", QMessageBox::Ok); 
		QApplication::quit();
	}

	// compile fragment shader
	if(!m_program.addShaderFromSourceFile(QGLShader::Fragment, ":/fshader0c.glsl")) {
		QMessageBox::critical(0, "Error", "Fragment shader error",QMessageBox::Ok); 
		QApplication::quit();
	}

	// bind the attribute variable in the glsl program with a generic vertex attribute index;
	// values provided via ATTRIB_VERTEX will modify the value of "a_Position")
	glBindAttribLocation(m_program.programId(), ATTRIB_VERTEX, "a_Position");

	// bind the attribute variable in the glsl program with a generic vertex attribute index;
	// values provided via ATTRIB_COLOR will modify the value of "a_Color")
	glBindAttribLocation(m_program.programId(), ATTRIB_COLOR,  "a_Color");

	// link shader pipeline; attribute bindings go into effect at this point
	if(!m_program.link()) {
		QMessageBox::critical(0, "Error", "Could not link shader", QMessageBox::Ok); 
		QApplication::quit();
	}

	// bind the glsl progam
	glUseProgram(m_program.programId());

	// get storage location of u_ModelMatrix in vertex shader
	m_u_ModelMatrix = glGetUniformLocation(m_program.programId(), "u_ModelMatrix");
	if((int) m_u_ModelMatrix < 0) {
		qDebug() << "Failed to get the storage location of u_ModelMatrix";
		exit(-1);
	}

	// get storage location of u_ReverseColor in vertex shader
	m_u_reverseColor = glGetUniformLocation(m_program.programId(), "u_ReverseColor");
	if((int) m_u_reverseColor < 0) {
		qDebug() << "Failed to get the storage location of u_ReverseColor";
		exit(-1);
	}

	// init model matrix; pass it to vertex shader along with reverse color flag
	m_ModelMatrix.setToIdentity();
	glUniformMatrix4fv(m_u_ModelMatrix, 1, GL_FALSE, m_ModelMatrix.constData());
	glUniform1i(m_u_reverseColor, m_reverseColor);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::initVertexBuffer:
//
// Initialize vertex buffer.
//
void
HW0c::initVertexBuffer()
{
	// set flag for creating buffers (1st time only)
	static bool flag = 1;

	// verify that we have valid vertex/color buffers
	static GLuint vertexBuffer = -1;
	static GLuint colorBuffer  = -1;
	if(flag) {	// create vertex and color buffers
		glGenBuffers(1, &vertexBuffer);
		glGenBuffers(1, &colorBuffer );
		flag = 0;	// reset flag
	}

	// init geometry data
	const vec2 vertices[] = {
		vec2( 0.0f,   0.75f ),
		vec2( 0.65f, -0.375f),
		vec2(-0.65f, -0.375f)
	};

	// init color data
	const vec3 colors[] = {
		vec3(1.0f, 0.0f, 0.0f),
		vec3(0.0f, 1.0f, 0.0f),
		vec3(0.0f, 0.0f, 1.0f)
	};

	// store vertex positions and colors in m_points and m_colors, respectively
	for(int i=0; i<3; i++) {
		m_points.push_back(vertices[i]);
		m_colors.push_back(colors  [i]);
	}
	m_numPoints = m_points.size();		// save number of vertices

	// bind the buffer to the GPU; all future drawing calls gets data from this buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// copy the vertices from CPU to GPU
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_points[0], GL_STATIC_DRAW);

	// enable the assignment of attribute vertex variable
	glEnableVertexAttribArray(ATTRIB_VERTEX);

	// assign the buffer object to the attribute vertex variable
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, false, 0, NULL);

	// bind color buffer to the GPU; all future buffer ops operate on this buffer
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);

	// copy the color from CPU to GPU
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec3), colors, GL_STATIC_DRAW);

	// enable the assignment of attribute color variable
	glEnableVertexAttribArray(ATTRIB_COLOR);

	// assign the buffer object to the attribute color variable
	glVertexAttribPointer(ATTRIB_COLOR, 3, GL_FLOAT, false, 0, NULL);

	// clear vertex and color vectors because they have already been copied into GPU
	m_points.clear();
	m_colors.clear();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::changeTheta:
//
// Slot function to change rotation angle.
//
void
HW0c::changeTheta(int angle)
{
	// update slider and spinbox
	m_sliderTheta->blockSignals(true);
	m_sliderTheta->setValue(angle);
	m_sliderTheta->blockSignals(false);

	m_spinBoxTheta->blockSignals(true);
	m_spinBoxTheta->setValue(angle);
	m_spinBoxTheta->blockSignals(false);

	// init vars
	m_reverseColor = false;			// do not update color during rotation

	// update model's rotation matrix
	m_ModelMatrix.setToIdentity();
	m_ModelMatrix.rotate(angle, QVector3D(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(m_u_ModelMatrix, 1, GL_FALSE, m_ModelMatrix.constData());
	glUniform1f(m_u_reverseColor, m_reverseColor);

	// draw
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::reverseOn:
//
// Slot function to turn on m_reverseColor;
//
void
HW0c::reverseOn()
{
	// init vars
	m_reverseColor = true;

	// redraw with flag set to 1
	glUniform1i(m_u_reverseColor, m_reverseColor);
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::reverseOff:
//
// Slot function to turn off m_reverseColor;
//
void
HW0c::reverseOff()
{
	// init vars
	m_reverseColor = false;

	// redraw with flag set to 0
	glUniform1i(m_u_reverseColor, m_reverseColor);
	updateGL();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::mousePressEvent:
//
// Event handler for mouse press events.
//
void
HW0c::mousePressEvent(QMouseEvent *event)
{
	m_mousePosition = event->pos();
	QGLWidget::mousePressEvent(event);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::mouseReleaseEvent:
//
// Event handler for mouse release events.
//
void
HW0c::mouseReleaseEvent(QMouseEvent *event)
{
	QGLWidget::mouseReleaseEvent(event);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW0c::mouseMoveEvent:
//
// Event handler for mouse move events.
//
void
HW0c::mouseMoveEvent(QMouseEvent *event)
{
	// compute a1: angle of previous mouse position
	int x  = m_mousePosition.x() - m_winW/2;
	int y  = m_winH/2 - m_mousePosition.y();
	int a1 = atan2(y,x) * 180/M_PI;

	// get current mouse position and save it
	m_mousePosition = event->pos();

	if(event->buttons() & Qt::LeftButton) {
		// compute a2: angle of current mouse position
		x = m_mousePosition.x() - m_winW/2;
		y = m_winH/2 - m_mousePosition.y();
		int a2 = atan2(y,x) * 180/M_PI;

		// update angle based on difference between a1 and a2;
		int angle = m_sliderTheta->value() + (a2-a1);

		// convert angle into [0,360] range
		if(angle < 0)	angle += 360;
		if(angle > 360)	angle -= 360;

		// call slot function to update slider/spinbox and render
		changeTheta(angle);
	}

	QGLWidget::mouseMoveEvent(event);
}
