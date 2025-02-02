/* +------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)            |
   |                          https://www.mrpt.org/                         |
   |                                                                        |
   | Copyright (c) 2005-2023, Individual contributors, see AUTHORS file     |
   | See: https://www.mrpt.org/Authors - All rights reserved.               |
   | Released under BSD License. See: https://www.mrpt.org/License          |
   +------------------------------------------------------------------------+ */

#pragma once
#include <mrpt/config.h>
#include <mrpt/gui/CGlCanvasBase.h>
#if MRPT_HAS_Qt5

#include <QtGlobal>
#if QT_VERSION < QT_VERSION_CHECK(5, 4, 0)
#include <QGLWidget>
#define QOpenGLWidget QGLWidget
#else
#include <QOpenGLWidget>
#endif

namespace mrpt::gui
{
class CQtGlCanvasBase : public QOpenGLWidget, public mrpt::gui::CGlCanvasBase
{
   public:
	CQtGlCanvasBase(QWidget* parent = nullptr);
	~CQtGlCanvasBase() override = default;

	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;

	mrpt::opengl::Viewport::Ptr mainViewport() const;

	/** Returns the zoom distance of the camera
	 * See also setZoomDistance(float), getZoomDistance()*/
	float getCameraZoomDistance() const;

   protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

	void swapBuffers() override {}
	void preRender() override {}
	void postRender() override {}
	void renderError(const std::string& err_msg) override;

	virtual void updateCamerasParams();
	virtual void insertToMap(const opengl::CRenderizable::Ptr& newObject);
	virtual void removeFromMap(const opengl::CRenderizable::Ptr& newObject);

	bool isPressLMouseButton() const;
	bool isPressMMouseButton() const;
	bool isPressRMouseButton() const;
	/** m_isPressLMouseButton and m_isPressRMouseButton are saved in
	 * mousePressEvent for mouseMoveEvent as true
	 * This function sets it as false */
	void unpressMouseButtons();

   private:
	bool m_isPressLMouseButton = false;
	bool m_isPressRMouseButton = false;
	bool m_isPressMMouseButton = false;

	mrpt::opengl::Viewport::Ptr m_mainViewport;

};	// end of class

}  // namespace mrpt::gui
#endif	// MRPT_HAS_Qt5
