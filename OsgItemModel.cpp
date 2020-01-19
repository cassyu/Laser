//#include "OsgItemModel.h"
//#include <QBrush>
//#include <osg/Node>
//#include <osg/MatrixTransform>
//#include <osgDB/ReadFile>
//#include <osgDB/WriteFile>
//
//static bool debugModel = false;
//#define modelDebug if (debugModel)  qDebug
//
//static QString stringFromRole(const int role);
//
//OsgItemModel::OsgItemModel(QObject * parent)
//	:QAbstractItemModel(parent)
//	,m_root(new osg::Group)
//	,m_loadedModel(new osg::MatrixTransform)
//	,m_clipBoard(new osg::Group)
//{
//	m_root->setName("__root");
//	m_loadedModel->setName("__loadedModel");
//
//	m_root->addChild(m_loadedModel);
//	m_root->setUserValue("fred", 10);
//
//	m_clipBoard->setName("__clipBoard");
//
//}
//
//int OsgItemModel::columnCount(const QModelIndex & parent)
//{
//	int numberOfColumns = 1;
//	if (!parent.isValid())
//	{
//		if (m_loadedModel->getNumChildren() > 0)
//			numberOfColumns = 3;
//	}
//	else
//	{
//		numberOfColumns = 3;
//	}
//}
//
//QVariant OsgItemModel::data(const QModelIndex & index, int role) const
//{
//	return QVariant();
//}
//
//Qt::ItemFlags OsgItemModel::flags(const QModelIndex & index) const
//{
//	return Qt::ItemFlags();
//}
//
//bool OsgItemModel::hasChildren(const QModelIndex & parent) const
//{
//	return false;
//}
//
//QVariant OsgItemModel::headerData(int section, Qt::Orientation orientation, int role) const
//{
//	return QVariant();
//}
//
//QModelIndex OsgItemModel::index(int row, int column, const QModelIndex & parent) const
//{
//	return QModelIndex();
//}
//
//QModelIndex OsgItemModel::parent(const QModelIndex & index) const
//{
//	return QModelIndex();
//}
//
//int OsgItemModel::rowCount(const QModelIndex & parent) const
//{
//	return 0;
//}
//
//bool OsgItemModel::setData(const QModelIndex & index, const QVariant & value, int role)
//{
//	return false;
//}
//
//void OsgItemModel::importFileByName(const QString fileName)
//{
//}
//
//void OsgItemModel::saveToFileByName(const QString fileName)
//{
//}
//
//void OsgItemModel::printTree() const
//{
//}
//
//void OsgItemModel::printNode(osg::ref_ptr<osg::Node> n, const int level) const
//{
//}
//
//void OsgItemModel::insertNode(osg::ref_ptr<osg::Group> parent, osg::ref_ptr<osg::Node> newChild, int childPositionInParent, int row)
//{
//}
//
//osg::ref_ptr<osg::Object> OsgItemModel::getObjectFromModelIndex(const QModelIndex & index) const
//{
//	return osg::ref_ptr<osg::Object>();
//}
//
//QString OsgItemModel::maskToString(const osg::Node::NodeMask mask) const
//{
//	return QString();
//}
//
//QModelIndex OsgItemModel::modelIndexFromNode(osg::ref_ptr<osg::Node> ptr, int column) const
//{
//	return QModelIndex();
//}
//
//QModelIndex OsgItemModel::parentOfNode(osg::Node * childNode) const
//{
//	return QModelIndex();
//}
//
//QModelIndex OsgItemModel::parentOfDrawable(osg::Drawable * childDrawable) const
//{
//	return QModelIndex();
//}
//
//bool OsgItemModel::setObjectMask(const QModelIndex & index, const QVariant & value)
//{
//	return false;
//}
//
//bool OsgItemModel::setObjectName(const QModelIndex & index, const QVariant & value)
//{
//	return false;
//}
