#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <image_gcode.h>
#include <QDebug>
//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>
#include "LaserGcodeGenerate.h"
#include "dragramitem.h"
#include "ui_mainwindow.h"

class  QGraphicsScene;
class DiagramScene;
class  QAbstractButton;
class  QButtonGroup;
class QToolBox;
class DrawScene;
class SSTestToolBox;
class SelectToolBox;

QT_FORWARD_DECLARE_CLASS(GraphWidget)


namespace Ui {
class MainWindow;
}
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
	void  CreategrapView();
	void  CreateCenterWidget();
	void  CreateFifthgrapView();
	void dumpStructture(QObject*obj, int spacecount);
	void CreateToolBoxA();
	void CretteToolBoxB();
	void CreateToolBoxC();

	void DataStreamTest();

	void  Create3dView();
	void  CreateToolbar();
	void  CreateOsgView();
	void  CreateGcode(QImage image,int id= 0);
	void  CreateGcodeNew(QString name);
	QString GetSelectFilePath();
    cv::Mat srcImage,dstImage,tempImage;
    QImage img;

public :
	static MainWindow* GetMainWindow();


private slots:
         void on_openLenaJpg_triggered();

         void on_exitSystem_triggered();

         void on_openCustomeFile_triggered();

         void on_restoreFile_triggered();

         void on_copyright_triggered();

         void on_about_triggered();

         void on_Clear_triggered();

         //void on_GcodeImg_triggered();

         void on_GcodePram_triggered();

         void on_GcodePre_triggered();
         void on_clearButton_clicked();

         void on_sendButton_clicked();

         void on_openButton_clicked();

		 void GcodeClicked(int id);
      

         void Read_Data();
		 void RenderPic();
		 
		 
		void on_pushButton_clicked();
		void msecSleep(int msec);
public slots:

	   void itemInserted(Dragramitem *item);
	   void textInserted(QGraphicsTextItem *item);
	   void itemSelected(QGraphicsItem *item);
	   void verticalAction();
	   void horicalAction();

private slots:
	   void buttonGroupClicked(int id);
	   void newbuttonGroupClicked(int id);

	   void GrayDiffuseClicked(int id);
	   void TextAndFontChanged(QString& text, QString &font);

	   void backgroundButtonGroupClicked(QAbstractButton* button);

private:
	   void createToolBox();
	   void createMenus();
	   void createActions();
	   void bringToFront();
	   void sendToBack(bool del);
	   void deleteItem(bool del);

	   void DsendToBack();
	   void DbringToFront();
	   void DdeleteItem();
	   //void connect();
	   QWidget* createCellWidget(const QString &text, Dragramitem::DiagramType type);
	   QWidget* createBackgroundCellWidget(const QString& text,const QString &image);
	   QWidget* createCellWidgetNew(const QString &text,const QString& pic,int id = 0);

private :
	static MainWindow* m_pMainWindow;

private:
    Ui::MainWindow *ui;
	 LaserGcodeGenerate m_LaserWork;
     //QSerialPort *    serial;
	 QGraphicsScene * m_scene;
	 DrawScene *      m_newscene;
	 DiagramScene *   scene;
	 QGraphicsView * view;
	 QGraphicsView*  newView;
	 GraphWidget*    newViewA;
	 QMenu *         itemMenu;
	 QButtonGroup *  buttonGroup;
	 QButtonGroup *  backgroundButtonGroup;
	 QButtonGroup*   newbuttonGroup;
	 QToolBox *    toolBox;
	 QToolBox*     newtoolBox;
	 QAction *    deleteAction;
	 QAction *    toFrontAction;
	 QAction *    sendBackAction;
	 SSTestToolBox*  m_pToolBoxA;
	 SelectToolBox*  m_pToolBoxC;

};

#endif // MAINWINDOW_H
