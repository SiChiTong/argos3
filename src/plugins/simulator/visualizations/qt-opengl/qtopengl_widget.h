/**
 * @file <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_WIDGET_H
#define QTOPENGL_WIDGET_H

namespace argos {
   class CQTOpenGLWidget;
   class CQTOpenGLMainWindow;
   class CSpace;
   class CSimulator;
   class CQTOpenGLBox;
   class CQTOpenGLUserFunctions;
   class CPositionalEntity;
   class CControllableEntity;
}

#ifdef QTOPENGL_WITH_SDL
#   include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_joystick.h>
#endif

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_camera.h>
#include <argos3/core/simulator/entity/entity.h>
#include <argos3/core/utility/datatypes/datatypes.h>
#include <QGLWidget>

#ifdef __APPLE__
#include <glu.h>
#else
#include <GL/glu.h>
#endif

namespace argos {

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawNormal : public CEntityOperation<CQTOpenGLOperationDrawNormal, CQTOpenGLWidget, void> {
   public:
      virtual ~CQTOpenGLOperationDrawNormal() {}
   };

   class CQTOpenGLOperationDrawSelected : public CEntityOperation<CQTOpenGLOperationDrawSelected, CQTOpenGLWidget, void> {
   public:
      virtual ~CQTOpenGLOperationDrawSelected() {}
   };

   /****************************************/
   /****************************************/

   class CQTOpenGLWidget : public QGLWidget {

      Q_OBJECT

    public:

      struct SFrameGrabData {
         bool Grabbing;
         QString Directory;
         QString BaseName;
         QString Format;
         SInt32 Quality;

         SFrameGrabData() :
            Grabbing(false),
            Directory("."),
            BaseName("frame_"),
            Format("png"),
            Quality(-1) {}

         void Init(TConfigurationNode& t_tree);
      };

      struct SSelectionInfo {
         bool IsSelected;
         size_t Index;

         SSelectionInfo() :
            IsSelected(false),
            Index(0) {}
      };

   public:

      CQTOpenGLWidget(const QGLFormat& c_format,
                      QWidget* pc_parent,
                      CQTOpenGLMainWindow* pc_main_window,
                      CQTOpenGLUserFunctions& c_user_functions);
      virtual ~CQTOpenGLWidget();

      virtual void initializeGL();
      virtual void resizeGL(int n_width,
                            int n_height);
      virtual void paintEvent(QPaintEvent*);

      void DrawScene();

      void SelectInScene(UInt32 un_x,
                         UInt32 un_y);

      void DrawPositionalEntity(CPositionalEntity& c_entity);

      void DrawRays(CControllableEntity& c_entity);

      inline virtual int heightForWidth(int w) const {
         return (w * 3) / 4;
      }

      inline virtual QSize sizeHint() const {
         return QSize(1024,768);
      }

      inline virtual QSize minimumSize() const {
         return QSize(320,240);
      }

      inline CQTOpenGLCamera& GetCamera() {
         return m_cCamera;
      }

      inline SFrameGrabData& GetFrameGrabData() {
         return m_sFrameGrabData;
      }

      inline void SetInvertMouse(bool b_InvertMouse) {
    	  m_bInvertMouse = b_InvertMouse;
      }

   signals:

      void StepDone(int n_step);
      void SimulationDone();
      void EntitySelected(size_t un_index);
      void EntityDeselected(size_t un_index);

   public slots:

      void PlayPauseSimulation(bool b_play);
      void FastForwardPauseSimulation(bool b_play);
      void StepSimulation();
      void ResetSimulation();
      void SetAntiAliasing(bool b_antialias_on);
      void SetDrawFrameEvery(int n_every);
      void SetGrabFrame(bool b_grab_on);
      void SetCamera(int n_camera);
      void SetCameraFocalLength(double f_length);

   protected:

      void InitializeArena();

      virtual void timerEvent(QTimerEvent* pc_event);
      virtual void mouseMoveEvent(QMouseEvent* pc_event);
      virtual void mousePressEvent(QMouseEvent* pc_event);
      virtual void mouseReleaseEvent(QMouseEvent* pc_event);
      virtual void keyPressEvent(QKeyEvent* pc_event);
      virtual void keyReleaseEvent(QKeyEvent* pc_event);
      virtual void resizeEvent(QResizeEvent* pc_event);
      void reactToKeyEvent();

   private:

      CQTOpenGLMainWindow* pcMainWindow;
      CQTOpenGLUserFunctions& m_cUserFunctions;

      SInt32 nTimerId;
      bool m_bAntiAliasing;
      bool m_bFastForwarding;
      SInt32 m_nDrawFrameEvery;
      SInt32 m_nFrameCounter;

      bool m_bMouseGrabbed;
      bool m_bShiftPressed;
      QPoint m_cMouseGrabPos;
      bool m_bInvertMouse;
      SSelectionInfo m_sSelectionInfo;

      CSimulator& m_cSimulator;
      CSpace& m_cSpace;

      bool   m_bUsingFloorTexture;
      GLuint m_unFloorTexture;
      GLuint m_unGroundTexture;
      GLuint m_unCeilingTexture;
      GLuint m_unBoxTexture;

      GLfloat* m_pfLightAmbient;
      GLfloat* m_pfLightDiffuse;
      GLfloat* m_pfLight0Position;
      GLfloat* m_pfLight1Position;

      GLuint m_unArenaList;
      GLuint m_unFloorList;
      GLuint* m_punSelectionBuffer;

#ifdef QTOPENGL_WITH_SDL
      CQTOpenGLJoystick m_cJoystick;
#endif
      CQTOpenGLCamera m_cCamera;
      SFrameGrabData m_sFrameGrabData;

      enum EDirection {
         DIRECTION_UP = 1,
         DIRECTION_DOWN,
         DIRECTION_LEFT,
         DIRECTION_RIGHT,
         DIRECTION_FORWARDS,
         DIRECTION_BACKWARDS,
      };

      QMap<EDirection, int> m_mapPressedKeys;
   };

}

#endif
