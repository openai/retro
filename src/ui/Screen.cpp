#include "Screen.h"

#include <QEvent>
#include <QPainter>

static const GLint _glVertices[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1
};

static const GLint _glTexCoords[] = {
	0, 0,
	1, 0,
	1, 1,
	0, 1
};

Screen::Screen(QWidget* parent)
	: QOpenGLWidget(parent) {
	setFocusPolicy(Qt::ClickFocus);
}

Screen::~Screen() {
	makeCurrent();
	m_texture.reset();
	doneCurrent();
}

void Screen::setImage(const QImage& image) {
	if (image.isNull()) {
		return;
	}

	makeCurrent();
	m_texture->destroy();
	m_texture->setData(image, QOpenGLTexture::DontGenerateMipMaps);
	doneCurrent();
	setMinimumSize(image.size());
	m_started = true;
	update();
}

void Screen::setPaused(bool paused) {
	if (m_paused == paused) {
		return;
	}
	m_paused = paused;
	emit pauseChanged(m_paused);
	if (paused) {
		update();
	} else {
		setFocus();
	}
}

void Screen::paintGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	glVertexPointer(2, GL_INT, 0, _glVertices);
	glTexCoordPointer(2, GL_INT, 0, _glTexCoords);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width(), height(), 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(width(), height(), 1);
	if (m_started) {
		if (m_nostalgia) {
			glColor4f(0.68, 0.91, 0.53, 1.0);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			glColor4f(0.68, 0.91, 0.53, 0.6);
		} else {
			glColor4f(1, 1, 1, 1);
		}
		m_texture->bind();
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		m_texture->release();
	}
	if (m_paused) {
		glColor4f(0.3, 0.3, 0.3, 0.5);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}
}

void Screen::initializeGL() {
	initializeOpenGLFunctions();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glClearColor(0.5, 0.5, 0.5, 1);
	m_texture = std::make_unique<QOpenGLTexture>(QOpenGLTexture::Target2D);
}

bool Screen::event(QEvent* event) {
	switch (event->type()) {
	case QEvent::FocusOut:
	case QEvent::FocusIn:
	case QEvent::MouseButtonPress:
		setPaused(event->type() == QEvent::FocusOut);
		return true;
	default:
		return QOpenGLWidget::event(event);
	}
}
