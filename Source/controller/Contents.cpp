#include "Contents.h"

Contents::Contents(QWidget *parent)
	: QObject(parent)
{
	mWidget = parent;
}
Contents::Contents(QWidget *parent, QLayout *parent_layout)
	: QObject(parent)
{
	mWidget = parent;

	QVBoxLayout *contents_left_layout = new QVBoxLayout;
	contents_left_layout->setAlignment(Qt::AlignCenter);
	contents_left_layout->setMargin(1);
	QWidget *contents_left_widget = new QWidget;
	contents_left_tabwidget = new QTabWidget(contents_left_widget);
	contents_left_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	contents_left_widget->setFixedSize(300, 870);
	contents_left_widget->setStyleSheet("background-color:(35,35,35);");
	contents_left_layout->addWidget(contents_left_widget);

	contents_center_layout = new QVBoxLayout;
	contents_center_layout->setAlignment(Qt::AlignCenter);
	contents_center_layout->setMargin(1);
	QWidget *contents_center_widget = new QWidget;
	contents_center_tabwidget = new QTabWidget(contents_center_widget);
	contents_center_widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	contents_center_widget->setFixedSize(1600, 870 * 0.60);

	contents_center_widget->setStyleSheet("background-color:(35,35,35);");
	contents_center_layout->addWidget(contents_center_widget);

	GraphEditorView = new ViewGraphEditor(mWidget);
	GraphEditorView->initialize(contents_center_layout);


	LeftTabInit(contents_left_tabwidget, contents_left_widget->width(), contents_left_widget->height());
	CenterTabInit(contents_center_tabwidget, contents_center_widget->width(), contents_center_widget->height());

	parent_layout->addItem(contents_left_layout);
	parent_layout->addItem(contents_center_layout);
}


Contents::~Contents()
{
}

void Contents::LeftTabInit(QTabWidget *target, int width, int height) {
	//target->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	target->setFixedSize(width, height);
	//target->setStyleSheet("background-color:red;");

	target->addTab(new QWidget(), "Project Information");
	target->addTab(new QWidget(), "Subregion & Feature");
}
void Contents::CenterTabInit(QTabWidget *target, int width, int height) {
	//target->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	//target->setFixedWidth(300);
	target->setFixedSize(width, height);
	
	target->addTab(new QWidget(), "4-View");
	//QOpenGLWidget *glWidgets = new ViewAxisGLWidget;
	target->addTab(new QWidget(), "XY");
	target->addTab(new QWidget(), "YZ");
	target->addTab(new QWidget(), "ZX");
}
//
//void Contents::RightTabInit(QTabWidget *target, int width, int height) {
//	//target->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
//	target->setFixedSize(width, height);
//	qDebug() << " Right size " << width << height;
//	target->addTab(new QWidget(), "Morphology Feature");
//	target->addTab(new QWidget(), "2D Scatter plot");
//}


void Contents::replaceTab(QTabWidget * tabs, int index, QWidget * replacement, QString label)
{
	Q_ASSERT(tabs && tabs->count() > index);
	tabs->removeTab(index);
	if (replacement) tabs->insertTab(index, replacement, label);
}
void Contents::updateAllWidget(int index, bool scaleflag) {
	GL_XYAxis_Main->update();
	GL_YZAxis_Main->update();
	GL_ZXAxis_Main->update();
	GL_XYAxis_Sub->update();
	GL_YZAxis_Sub->update();
	GL_ZXAxis_Sub->update();
}


//
//void Contents::RightContentsOn() {
//	QVBoxLayout *HistListLayout = new QVBoxLayout;
//	QString nameList[5] = { "Volume", "Surfacearea", "Sphericity", "Eccentricity", "Intensity" };
//
//	for (int k = 0; k < 5; ++k) {
//		
//		MorphoHistBox[k] = new ViewHistogramBox(mWidget);
//
//		switch (k) {
//			case 0:
//				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->volumeHist);
//				break;
//			case 1:
//				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->surfaceareaHist);
//				break;
//			case 2:
//				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->sphericityHist);
//				break;
//			case 3:
//				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->eccentricityHist);
//				break;
//			case 4:
//				MorphoHistBox[k]->Init(nameList[k], k, &mGlobals.CurrentProject->mAnalytics->intensityHist);
//				break;
//		}
//		
//		HistListLayout->addWidget(MorphoHistBox[k]->HistBoxFrame);
//		connect(MorphoHistBox[k], SIGNAL(focusedHist(int)), this, SLOT(focusmanage_histogram(int)));
//
//	}
//
///*
//	QWidget *HistListWidget = new QWidget();
//	HistListWidget->setLayout(HistListLayout);*/
//	QScrollArea* scroller = new QScrollArea;
//	scroller->horizontalScrollBar()->setEnabled(false);
//	scroller->setLayout(HistListLayout);
//
//	replaceTab(contents_right_tabwidget, 0, scroller, "Morphology Feature");
//
//}


void Contents::InitGLView() {
	GL_XYAxis_Main = new ViewAxisGLWidget;
	GL_XYAxis_Main->setAxisIndex(1, 0);
	connect(GL_XYAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_YZAxis_Main = new ViewAxisGLWidget;
	GL_YZAxis_Main->setAxisIndex(2, 1);
	connect(GL_YZAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_ZXAxis_Main = new ViewAxisGLWidget;
	GL_ZXAxis_Main->setAxisIndex(3, 2);
	connect(GL_ZXAxis_Main, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	replaceTab(contents_center_tabwidget, 1, GL_XYAxis_Main, "XY");
	replaceTab(contents_center_tabwidget, 2, GL_YZAxis_Main, "YZ");
	replaceTab(contents_center_tabwidget, 3, GL_ZXAxis_Main, "ZX");


	GL_XYAxis_Sub = new ViewAxisGLWidget;
	GL_XYAxis_Sub->setAxisIndex(1, 3);
	connect(GL_XYAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_YZAxis_Sub = new ViewAxisGLWidget;
	GL_YZAxis_Sub->setAxisIndex(2, 4);
	connect(GL_YZAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	GL_ZXAxis_Sub = new ViewAxisGLWidget;
	GL_ZXAxis_Sub->setAxisIndex(3, 5);
	connect(GL_ZXAxis_Sub, SIGNAL(updateAll(int, bool)), this, SLOT(updateAllWidget(int, bool)));

	QVBoxLayout *view4_layout = new QVBoxLayout;
	QHBoxLayout *first_lay = new QHBoxLayout;
	QHBoxLayout *second_lay = new QHBoxLayout;
	first_lay->addWidget(GL_XYAxis_Sub);
	first_lay->addWidget(GL_YZAxis_Sub);
	view4_layout->addLayout(first_lay);
	second_lay->addWidget(GL_ZXAxis_Sub);
	second_lay->addWidget(new QWidget());
	view4_layout->addLayout(second_lay);
	QWidget *view4_Container = new QWidget;
	view4_Container->setLayout(view4_layout);
	replaceTab(contents_center_tabwidget, 0, view4_Container, "4-View");

}

void Contents::InitGraph() {
	GraphEditorView->addOrigin();
}

void Contents::InitProjectInfo() {
	QVBoxLayout *layout = new QVBoxLayout;
	QHBoxLayout *proj_name_layout = new QHBoxLayout;
	QHBoxLayout *proj_path_layout = new QHBoxLayout;
	QHBoxLayout *proj_back_layout = new QHBoxLayout;
	QHBoxLayout *proj_cell_layout = new QHBoxLayout;

	QLabel *proj_name_label = new QLabel(mWidget);
	proj_name_label->setAlignment(Qt::AlignCenter);
	proj_name_label->setFixedSize(100, 30);
	proj_name_label->setText("Project Name");
	ProjectName = new QLineEdit(mWidget);
	ProjectName->setReadOnly(true);
	ProjectName->setText(mGlobals.CurrentProject->ProjectName);
	proj_name_layout->addWidget(proj_name_label);
	proj_name_layout->addWidget(ProjectName);

	QLabel *proj_path_label = new QLabel(mWidget);
	proj_path_label->setAlignment(Qt::AlignCenter);
	proj_path_label->setFixedSize(100, 30);
	proj_path_label->setText("Project Name");
	ProjectPath = new QLineEdit(mWidget);
	ProjectPath->setReadOnly(true);
	ProjectPath->setText(mGlobals.CurrentProject->ProjectPath);
	proj_path_layout->addWidget(proj_path_label);
	proj_path_layout->addWidget(ProjectPath);


	QLabel *proj_back_label = new QLabel(mWidget);
	proj_back_label->setAlignment(Qt::AlignCenter);
	proj_back_label->setFixedSize(100, 30);
	proj_back_label->setText("EM Path");
	BackPath = new QLineEdit(mWidget);
	BackPath->setReadOnly(true);
	BackPath->setText(QString::fromStdString(mGlobals.CurrentProject->mLayerBack->BackgroundPath));
	proj_back_layout->addWidget(proj_back_label);
	proj_back_layout->addWidget(BackPath);


	QLabel *proj_cell_label = new QLabel(mWidget);
	proj_cell_label->setAlignment(Qt::AlignCenter);
	proj_cell_label->setFixedSize(100, 30);
	proj_cell_label->setText("Cell Path");
	CellPath = new QLineEdit(mWidget);
	CellPath->setReadOnly(true);
	CellPath->setText(QString::fromStdString(mGlobals.CurrentProject->mLayerCell->CellPath));
	proj_cell_layout->addWidget(proj_cell_label);
	proj_cell_layout->addWidget(CellPath);


	QHBoxLayout *filler_layout = new QHBoxLayout;
	QWidget *filler = new QWidget;
	filler->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
	filler_layout->addWidget(filler);


	layout->addLayout(proj_name_layout);
	layout->addLayout(proj_path_layout);
	layout->addLayout(proj_back_layout);
	layout->addLayout(proj_cell_layout);
	layout->addLayout(filler_layout);
		
	QWidget *ProjectInfo_Container = new QWidget;
	ProjectInfo_Container->setLayout(layout);
	replaceTab(contents_left_tabwidget, 0, ProjectInfo_Container, "Project Information");
}
void Contents::InitSubregionFeatureList() {
	QVBoxLayout *layout = new QVBoxLayout;
	QWidget *subregion = new QWidget(mWidget);
	QWidget *feature = new QWidget(mWidget);
	QVBoxLayout *subregion_layout = new QVBoxLayout;
	QHBoxLayout *subregion_title_layout = new QHBoxLayout;
	QVBoxLayout *feature_layout = new QVBoxLayout;
	QHBoxLayout *feature_title_layout = new QHBoxLayout;

	subregionList = new QListWidget(mWidget);
	featureList = new QListWidget(mWidget);

	QFont title_font;
	title_font.setPointSize(11);
	title_font.setBold(true);

	QLabel *subregion_title = new QLabel(mWidget);
	subregion_title->setText("Subregion");
	subregion_title->setFont(title_font);
	QWidget *filler1 = new QWidget;
	filler1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QPushButton *subregionAddBtn = new QPushButton(mWidget);
	subregionAddBtn->setText("Add");
	subregion_title_layout->addWidget(subregion_title);
	subregion_title_layout->addWidget(filler1);
	subregion_title_layout->addWidget(subregionAddBtn);
	connect(subregionAddBtn, SIGNAL(released()), this, SLOT(handleSubregionAddBtn()));

	QLabel *feature_title = new QLabel(mWidget);
	feature_title->setText("Feature");
	feature_title->setFont(title_font);
	QPushButton *featureAddBtn = new QPushButton(mWidget);
	featureAddBtn->setText("Add");
	QWidget *filler2 = new QWidget;
	filler2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	feature_title_layout->addWidget(feature_title);
	feature_title_layout->addWidget(filler2);
	feature_title_layout->addWidget(featureAddBtn);
	connect(featureAddBtn, SIGNAL(released()), this, SLOT(handleFeatureAddBtn()));



	subregion_layout->addLayout(subregion_title_layout);
	subregion_layout->addWidget(subregionList);
	feature_layout->addLayout(feature_title_layout);
	feature_layout->addWidget(featureList);

	subregion->setLayout(subregion_layout);
	feature->setLayout(feature_layout);

	layout->addWidget(subregion);
	layout->addWidget(feature);

	QWidget *SubregionFeatureList_Container = new QWidget;
	SubregionFeatureList_Container->setLayout(layout);
	replaceTab(contents_left_tabwidget, 1, SubregionFeatureList_Container, "Subregion & Feature");
}

void Contents::updateColorBox(QPushButton *target, QColor color) {
	target->setStyleSheet("");
	QPalette mPalette = target->palette();
	mPalette.setColor(target->backgroundRole(), color);
	target->setPalette(mPalette);
	target->setStyleSheet(QString("background-color: %1;foreground-color: %1; border-style: none;").arg(color.name()));
	target->setAutoFillBackground(true);
	target->setFlat(true);
	target->repaint();
}


void Contents::handleSubregionAddBtn() {
	mGlobals.mDialogManager->mDialogAddSubregion->exec();
}
void Contents::handleFeatureAddBtn() {
	mGlobals.mDialogManager->mDialogAddFeature->exec();
}
void Contents::feature_updated() {
	qDebug() << "feature_updated";
	featureList->clear();
	featureList->setContentsMargins(1, 1, 1, 1);
	int index = 0;
	std::vector<DataFeature>::iterator iter;
	for (iter = mGlobals.CurrentProject->mFeature.begin(); iter != mGlobals.CurrentProject->mFeature.end(); ++iter) {
		
		
		QWidget *widget = new QWidget(mWidget);
		QHBoxLayout *list_item_layout= new QHBoxLayout;
		QFont font;
		font.setPointSize(10);
		font.setBold(true);

		QLabel *name= new QLabel(mWidget);
		name->setFont(font);
		name->setText(QString::fromStdString(iter->FeatureName));
		
		QPushButton *detailBtn = new QPushButton(mWidget);
		detailBtn->setIcon(QIcon("Resources/icon_detail.png"));
		detailBtn->setObjectName(QString::fromStdString(std::to_string(index)));
		detailBtn->setIconSize(QSize(15, 15));
		detailBtn->setFixedSize(QSize(20, 20));
		connect(detailBtn, SIGNAL(clicked()), this, SLOT(handleDetailBtn_feature()));

		QPushButton *deleteBtn = new QPushButton(mWidget);
		deleteBtn->setIcon(QIcon("Resources/icon_trash.png"));
		deleteBtn->setObjectName(QString::fromStdString(std::to_string(index)));
		deleteBtn->setIconSize(QSize(15, 15));
		deleteBtn->setFixedSize(QSize(20, 20));
		connect(deleteBtn, SIGNAL(clicked()), this, SLOT(handleDeleteBtn_feature()));
		

		QWidget *filler2 = new QWidget;
		filler2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		list_item_layout->addWidget(name);
		list_item_layout->addWidget(filler2);
		list_item_layout->addWidget(detailBtn);
		list_item_layout->addWidget(deleteBtn);
		widget->setLayout(list_item_layout);
		QListWidgetItem *item = new QListWidgetItem();
		
		item->setSizeHint(widget->sizeHint());

		featureList->addItem(item);
		featureList->setItemWidget(item, widget);

		index++;
	}


}
void Contents::subregion_updated() {
	subregionList->clear();
	subregionList->setContentsMargins(1, 1, 1, 1);
	int index = 0;
	std::vector<LayerSubregion>::iterator iter;
	for (iter = mGlobals.CurrentProject->mSubregion.begin(); iter != mGlobals.CurrentProject->mSubregion.end(); ++iter) {
		
		QWidget *widget = new QWidget(mWidget);
		QHBoxLayout *list_item_layout = new QHBoxLayout;
		QFont font;
		font.setPointSize(10);
		font.setBold(true);

		QLabel *name = new QLabel(mWidget);
		name->setFont(font);
		name->setText(QString::fromStdString(iter->SubregionName));
		QCheckBox *CheckBox = new QCheckBox;
		CheckBox->setObjectName(QString::fromStdString(std::to_string(index)));
		CheckBox->setStyleSheet("QCheckBox {background-color: #555555}");
		iter->SubregionIndex = index;
		CheckBox->setChecked(iter->SubregionActivated);

		connect(CheckBox, SIGNAL(stateChanged(int)), this, SLOT(handleCheckBox_subregion(int)));
		
		iter->SubregionColorBtn = new QPushButton;
		iter->SubregionColorBtn->setObjectName(QString::fromStdString(std::to_string(index)));
		iter->SubregionColorBtn->setFixedSize(QSize(15, 15));
		iter->SubregionColorBtn->setFlat(true);
		iter->SubregionColorBtn->setAutoFillBackground(true);
		updateColorBox(iter->SubregionColorBtn, iter->SubregionColor);
		connect(iter->SubregionColorBtn, SIGNAL(released()), this, SLOT(handleColorBtn_subregion()));
		

		QPushButton *deleteBtn = new QPushButton(mWidget);
		deleteBtn->setIcon(QIcon("Resources/icon_trash.png"));
		deleteBtn->setObjectName(QString::fromStdString(std::to_string(index)));
		deleteBtn->setIconSize(QSize(15, 15));
		deleteBtn->setFixedSize(QSize(20, 20));
		connect(deleteBtn, SIGNAL(clicked()), this, SLOT(handleDeleteBtn_subregion()));
		
		QSlider *slider = new QSlider(Qt::Horizontal);



		QWidget *filler2 = new QWidget;
		filler2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		list_item_layout->addWidget(name);
		list_item_layout->addWidget(filler2);
		list_item_layout->addWidget(iter->SubregionColorBtn);
		list_item_layout->addWidget(CheckBox);
		list_item_layout->addWidget(deleteBtn);
		widget->setLayout(list_item_layout);
		QListWidgetItem *item = new QListWidgetItem();

		item->setSizeHint(widget->sizeHint());

		subregionList->addItem(item);
		subregionList->setItemWidget(item, widget);

		index++;
	}
}

void Contents::handleDetailBtn_feature() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	qDebug() << "detail Button: " << senderObjName;
	int index = std::stoi(senderObjName.toStdString());
	mGlobals.mDialogManager->mDialogDetailFeature->setIndexAndDraw(index);
	mGlobals.mDialogManager->mDialogDetailFeature->exec();
}


void Contents::handleDeleteBtn_feature() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	qDebug() << "delete Button feature: " << senderObjName;

	mGlobals.CurrentProject->removeFeature(index);
	featureList->removeItemWidget(featureList->takeItem(index));
}

void Contents::handleDeleteBtn_subregion() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	qDebug() << "delete Button: subregion" << senderObjName;

	mGlobals.CurrentProject->removeSubregion(index);
	subregion_updated();
}

void Contents::handleCheckBox_subregion(int state) {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	qDebug() << state;
	
	if (state == 2) {
		int count = 0;
		std::vector<LayerSubregion>::iterator iter;
		for (iter = mGlobals.CurrentProject->mSubregion.begin(); iter != mGlobals.CurrentProject->mSubregion.end(); ++iter) {
			if (iter->SubregionActivated) {
				++count;
			}
		}
		if (count >= 10) {
			QMessageBox::warning(0, "Error", "Up to 10 can be selected.");
		}
		else {
			mGlobals.CurrentProject->mSubregion[index].SubregionActivated = true;
		}		
	}	
	else {
		mGlobals.CurrentProject->mSubregion[index].SubregionActivated = false;
	}
	subregion_updated();
}


void Contents::handleColorBtn_subregion() {
	QObject *senderObj = sender(); // This will give Sender object
	QString senderObjName = senderObj->objectName();
	int index = std::stoi(senderObjName.toStdString());
	QColorDialogTester color_test;
	QString color_str = color_test.onColor();
	qDebug() << color_str;

	if (color_str.length() > 2) {
		mGlobals.CurrentProject->mSubregion[index].SubregionColor = QColor(color_str);
		updateColorBox(mGlobals.CurrentProject->mSubregion[index].SubregionColorBtn, QColor(color_str));
	}
}