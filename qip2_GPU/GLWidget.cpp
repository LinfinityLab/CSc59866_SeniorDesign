// ===============================================================
// Computer Graphics Homework Solutions
// Copyright (C) 2015 by George Wolberg
//
// GLWidget.cpp - GLWidget class. Base class of homework solutions.
//
// Written by: George Wolberg, 2015
// ===============================================================

#include "MainWindow.h"
#include "GLWidget.h"

extern MainWindow *g_mainWindowP;

// shader ID
enum { PASSTHROUGH_SHADER};

// uniform ID
enum { SAMPLER };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::GLWidget:
//
// GLWidget constructor.
// 
//
GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent), 
				      m_imageFlag(false)


{}





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::initializeGL:
//
// Initialization routine before display loop.
// Gets called once before the first time resizeGL() or paintGL() is called.
//
void
GLWidget::initializeGL()
{
	// initialize GL function resolution for current context
	initializeGLFunctions(); 

	// init vertex and fragment shaders
	initShaders();

	// init XY vertices in mesh and texture coords
	initVertices();

	// initialize vertex buffer and write positions to vertex shader
	initBuffers();

	// generate input texture name 
	glGenTextures(1, &m_inTexture);

	// generate output texture name 
	glGenTextures(1, &m_outTexture);

	// generate frame buffer
	glGenFramebuffers(1, &m_fbo[PASS1]);
	glGenFramebuffers(1, &m_fbo[PASS2]);
	glGenTextures(1, &m_texture_fbo[PASS1]);
	glGenTextures(1, &m_texture_fbo[PASS2]);

	glClearColor(1.0, 1.0, 1.0, 1.0);	// set background color

//	GLint value;
//	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &value);
//	qDebug() << value;
//	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_VECTORS, &value);
//	qDebug() << value;

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::initVertices:
//
// Initialize XY vertices in quad and texture coords
//
void
GLWidget::initVertices() {

	// init geometry data 
	// two triangles that form a quad
	m_points.push_back(vec2(-1.0f, -1.0f));
	m_points.push_back(vec2(-1.0f,  1.0f));
	m_points.push_back(vec2( 1.0f, -1.0f));
	m_points.push_back(vec2( 1.0f,  1.0f));


	// init texture coordinate
	for(int i=0; i<(int)m_points.size(); ++i) {
		m_texCoord.push_back((m_points[i]+vec2(1.0f, 1.0f))/2.0f);
	}
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::initVertexBuffer:
//
// Initialize vertex buffer.
//
void
GLWidget::initBuffers() {


	m_numPoints = (int) m_points.size();		// save number of vertices

	glGenBuffers(1, &m_vertexBuffer);
	// bind vertex buffer to the GPU and copy the vertices from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_points[0], GL_STATIC_DRAW);

	glGenBuffers(1, &m_texCoordBuffer);
	// bind color buffer to the GPU and copy the colors from CPU to GPU
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, m_numPoints*sizeof(vec2), &m_texCoord[0], GL_STATIC_DRAW);

}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::setInTexture:
//
// Initialize texture for input image.
//
void
GLWidget::setInTexture(QImage &image)
{
	// convert jpg to GL formatted image
	QImage qImage = QGLWidget::convertToGLFormat(image);

	// init vars
	m_imageW = qImage.width ();
	m_imageH = qImage.height();

	// bind texture
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, m_inTexture);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// upload to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageW, m_imageH, 0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits());
	glBindTexture(GL_TEXTURE_2D, 0);
	m_imageFlag = true;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::setOutTexture:
//
// Initialize texture for output image.
//
void
GLWidget::setOutTexture(QImage &image) {
	// convert jpg to GL formatted image
	QImage qImage = QGLWidget::convertToGLFormat(image);

	// init vars
	m_imageW = qImage.width();
	m_imageH = qImage.height();

	// bind texture
	glActiveTexture(1);
	glBindTexture(GL_TEXTURE_2D, m_outTexture);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// upload to GPU
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageW, m_imageH, 0, GL_RGBA, GL_UNSIGNED_BYTE, qImage.bits());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void
GLWidget::allocateTextureFBO(int w, int h) 
{

	// bind texture
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[PASS1]);
	glBindTexture(GL_TEXTURE_2D, m_texture_fbo[PASS1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			       GL_TEXTURE_2D, m_texture_fbo[PASS1], 0);


	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[PASS2]);
	glBindTexture(GL_TEXTURE_2D, m_texture_fbo[PASS2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			       GL_TEXTURE_2D, m_texture_fbo[PASS2], 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::initShader:
//
// Initialize vertex and fragment shaders.
//
void
GLWidget::initShader(QGLShaderProgram &program, QString vshaderName, QString fshaderName, UniformMap &uniformsMap, int *uniforms)
{
	// compile vertex shader
	bool flag = program.addShaderFromSourceFile(QGLShader::Vertex, vshaderName);
	if(!flag) {
		QMessageBox::critical(0, "Error", "Vertex shader error: " + vshaderName + "\n" +
					program.log(), QMessageBox::Ok);
		exit(-1);
	}

	// compile fragment shader
	if(!program.addShaderFromSourceFile(QGLShader::Fragment, fshaderName)) {
		QMessageBox::critical(0, "Error", "Fragment shader error: " + fshaderName + "\n" +
					program.log(), QMessageBox::Ok);
		exit(-1);
	}

	// bind the attribute variable in the glsl program with a generic vertex attribute index;
	// values provided via ATTRIB_VERTEX will modify the value of "a_position")
	glBindAttribLocation(program.programId(), ATTRIB_VERTEX,	"a_Position");
	glBindAttribLocation(program.programId(), ATTRIB_TEXCOORD,	"a_TexCoord");

	// link shader pipeline; attribute bindings go into effect at this point
	if(!program.link()) {
		QMessageBox::critical(0, "Error", "Could not link shader: " + vshaderName + "\n" +
					program.log(), QMessageBox::Ok);
		exit(-1);
	}


	// iterate over all uniform variables; map each uniform name to shader location ID
	std::map<QString, GLuint>::iterator iter;
	for( iter = uniformsMap.begin(); iter != uniformsMap.end(); ++iter) {
		QString uniformName = iter->first;
		GLuint  uniformID   = iter->second;

		// get storage location
		uniforms[uniformID]=glGetUniformLocation(program.programId(),
			  uniformName.toStdString().c_str());
		if(uniforms[uniformID] < 0) {
			qDebug() << "Failed to get the storage location of " + uniformName;
		}
	}
}




// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::initShaders:
//
// Initialize vertex and fragment shaders.
//
void
GLWidget::initShaders()
{

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
	uniforms["u_Sampler"] = SAMPLER;
	
        QString v_name = ":/vshader_passthrough";
        QString f_name = ":/fshader_passthrough";
        
#ifdef __APPLE__
        v_name += "_Mac";
        f_name += "_Mac"; 
#endif    
	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	initShader(m_program , v_name + ".glsl",  f_name + ".glsl",  uniforms, m_uniform);

	g_mainWindowP->imageFilter(THRESHOLD)->initShader();
	g_mainWindowP->imageFilter(CLIP)->initShader();
	g_mainWindowP->imageFilter(QUANTIZE)->initShader();
	g_mainWindowP->imageFilter(GAMMA)->initShader();
	g_mainWindowP->imageFilter(CONTRAST)->initShader();
	g_mainWindowP->imageFilter(HISTOSTRETCH)->initShader();
	g_mainWindowP->imageFilter(HISTOMATCH)->initShader();
	g_mainWindowP->imageFilter(ERRDIFFUSION)->initShader();
	g_mainWindowP->imageFilter(BLUR)->initShader();
	g_mainWindowP->imageFilter(SHARPEN)->initShader();
	g_mainWindowP->imageFilter(MEDIAN)->initShader();
	g_mainWindowP->imageFilter(CONVOLVE)->initShader();


}


void
GLWidget::setViewport(int w, int h, int ww, int hh) 
{

	// compute orthographic projection from viewing coordinates
	m_projection.setToIdentity();
	m_projection.ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	// center glwidget if needed when the image is smaller than screen
	int new_ww = w;
	int new_hh = h;
	if(w > ww || h > hh) {
		// compute aspect ratio of the image and screen
		float screenRatio = (float) ww / hh;
		float imageRatio  = (float) w / h;
		if(imageRatio > screenRatio) {
			new_hh = (int) (((float) ww/w)*h);
			new_ww = ww;
		} else {
			new_ww = (int) (((float) hh/h)*w);
			new_hh = hh;
		}
	}
	resize(new_ww, new_hh);
	// move glwidget to the center
	int dx = (ww - new_ww) / 2;
	int dy = (hh - new_hh) / 2;
	move(dx, dy);

}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::resizeGL:
//
// Resize event handler.
// The input parameters are the window width (w) and height (h).
//
void
GLWidget::resizeGL(int w, int h) {
	// save window dimensions
	m_winW = w;
	m_winH = h;
	glViewport(0, 0, w, h);
	int imgW = g_mainWindowP->imageIn()->width();
	int imgH = g_mainWindowP->imageIn()->height();
	setViewport(imgW, imgH, g_mainWindowP->glFrameW(), g_mainWindowP->glFrameH());


}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::paintGL:
//
// Update GL scene.
//
void
GLWidget::paintGL() 
{

	// clear canvas with background color
	glClear(GL_COLOR_BUFFER_BIT);

	// error checking (nothing to display)
	if(!m_imageFlag) return;

	// enable buffer to be copied to the attribute vertex variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	glEnableVertexAttribArray(ATTRIB_VERTEX);
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, false, 0, NULL);

	// enable buffer to be copied to the attribute texture coord variable and specify data format
	glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
	glEnableVertexAttribArray(ATTRIB_TEXCOORD);
	glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, false, 0, NULL);

	glUseProgram(m_program.programId());	// passthrough glsl progam

	int selection = g_mainWindowP->gpuFlag()*2+g_mainWindowP->isInput();
	switch(selection) {
		case 0: // display out image generated by CPU
			glBindTexture(GL_TEXTURE_2D, m_outTexture);
			glUniform1i(m_uniform[SAMPLER], 0); 
			break;
		case 1:
		case 3: // display input image 
			glBindTexture(GL_TEXTURE_2D, m_inTexture);
			glUniform1i(m_uniform[SAMPLER], 0);
			break;
		case 2: // display rendered texture by GPU filter
			int n = g_mainWindowP->gpuPasses();
			glBindTexture(GL_TEXTURE_2D, m_texture_fbo[n-1]);
			glUniform1i(m_uniform[SAMPLER], 0); 
			break;
	}
	 
	// draw triangles
	glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei) m_numPoints);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glDisableVertexAttribArray(ATTRIB_TEXCOORD);
	glDisableVertexAttribArray(ATTRIB_VERTEX);


}





// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// GLWidget::applyFilterGPU:
//
//  Apply selected filter to input image by render to the texture in GPU.
//
void
GLWidget::applyFilterGPU(int nPasses) 
{

	for(int pass=0; pass<nPasses; ++pass) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[pass]);
		glViewport(0, 0, m_imageW, m_imageH);
		glClearColor(0.0, 0.0, 0.0, 1.0);	// set background color
		glClear(GL_COLOR_BUFFER_BIT);

		// enable buffer to be copied to the attribute vertex variable and specify data format
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
		glEnableVertexAttribArray(ATTRIB_VERTEX);
		glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, false, 0, NULL);

		// enable buffer to be copied to the attribute texture coord variable and specify data format
		glBindBuffer(GL_ARRAY_BUFFER, m_texCoordBuffer);
		glEnableVertexAttribArray(ATTRIB_TEXCOORD);
		glVertexAttribPointer(ATTRIB_TEXCOORD, 2, GL_FLOAT, false, 0, NULL);
		if(pass == 0)
			glBindTexture(GL_TEXTURE_2D, m_inTexture);
		else
			glBindTexture(GL_TEXTURE_2D, m_texture_fbo[pass-1]);


		g_mainWindowP->gpuProgram(pass);

		// draw triangles
		glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei) m_numPoints);

	}

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	if(!g_mainWindowP->timeFlag()) 
		setDstImage(nPasses-1);
	
	glDisableVertexAttribArray(ATTRIB_TEXCOORD);
	glDisableVertexAttribArray(ATTRIB_VERTEX);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}



void
GLWidget::setDstImage(int pass) 
{

	glViewport(0, 0, m_imageW, m_imageH);
	ImagePtr I = IP_allocImage(3*m_imageW, m_imageH, BW_TYPE);
	ChannelPtr<uchar> p = I[0];
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo[pass]);
	glReadPixels(0, 0, m_imageW, m_imageH, GL_RGB, GL_UNSIGNED_BYTE, &p[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// uninterleave image
	ImagePtr ipImage = IP_allocImage(m_imageW, m_imageH, RGB_TYPE);
	IP_uninterleave(I, ipImage);
	g_mainWindowP->setImageDst(ipImage);
	glViewport(0, 0, m_winW, m_winH);


}
