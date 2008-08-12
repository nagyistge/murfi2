#include "FrMainWindow.h"

FrMainWindow::FrMainWindow(QWidget *parent): QMainWindow(parent){
	setupUi(this);
	
	QTabWidget*  myTabWidget = new QTabWidget(this);
	QWidget* slice2DWidget = new QWidget;
	QWidget* graphWidget = new QWidget;
	
	myQVTKWidget = new QVTKWidget(this);
	myQVTKWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

//----------------------------- test area --------------------
	vtkPNGReader* reader = vtkPNGReader::New();
	reader->SetDataScalarTypeToUnsignedChar();
	reader->SetFileName("test.png");

	vtkImageViewer2* image_view = vtkImageViewer2::New();
	image_view->SetSize(640, 480);
	myQVTKWidget->SetRenderWindow(image_view->GetRenderWindow());
	image_view->SetupInteractor(myQVTKWidget->GetRenderWindow()->GetInteractor());

	vtkRenderer *ren1= vtkRenderer::New();
	ren1->SetBackground( 0.1, 0.2, 0.4 );
	vtkImageActor *image = vtkImageActor::New();
	image->SetInput(reader->GetOutput());
	image_view->GetRenderWindow()->AddRenderer(ren1);
	ren1->AddActor(image);

	image_view->SetColorLevel(138.5);
	image_view->SetColorWindow(233);
//----------------------------- test area --------------------


	myToolsPanel = new FrToolsPanel(this);
	myToolsPanel->setFixedWidth(myToolsPanel->width());

//	scrollAreaOfToolsPanel = new QScrollArea;
//	scrollAreaOfToolsPanel->setWidget(myToolsPanel);
//	scrollAreaOfToolsPanel->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
//	scrollAreaOfToolsPanel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	myToolsPanel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	myBookmarkWidget = new FrBookmarkWidget(this);
	myBookmarkWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

	myLowPanel = new FrLowPanel(this);
	myLowPanel->setFixedHeight(myLowPanel->height());

	QVBoxLayout *verticalLayout = new QVBoxLayout;
//	verticalLayout->addWidget(myBookmarkWidget);
	verticalLayout->addWidget(myQVTKWidget);
	verticalLayout->addWidget(myLowPanel);

	// horizontal layout
	QHBoxLayout *layout = new QHBoxLayout;
//	layout->addWidget(scrollAreaOfToolsPanel);
	layout->addWidget(myToolsPanel);
	//layout->addWidget(myQVTKWidget);
	layout->addLayout(verticalLayout);
	layout->addWidget(myBookmarkWidget);

//	verticalLayout->addLayout(layout);
	
//	QHBoxLayout *bottomLayout = new QHBoxLayout;
//    QSpacerItem* spacerItem = new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum);
//	bottomLayout->setContentsMargins(0, 0, 0, 0);
//	bottomLayout->addWidget(myLowPanel);
////	bottomLayout->addItem(spacerItem);
//
//	verticalLayout->addLayout(bottomLayout);

	// central widget
//	QWidget *centralWidget = new QWidget;
//	centralWidget->setLayout(layout);

//	setCentralWidget(centralWidget);
	
	slice2DWidget->setLayout(layout);
	myTabWidget->addTab(slice2DWidget, "2D Slice View");
	myTabWidget->addTab(graphWidget, "Graphs and calculations");
	setCentralWidget(myTabWidget);

	// actions of File-menu
	connect(actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

	// actions of Help-menu
	connect(actionAbout, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

	// toolbar actions
	connect(actionTool1, SIGNAL(triggered()), this, SLOT(tool1Triggered()));
	connect(actionTool2, SIGNAL(triggered()), this, SLOT(tool2Triggered()));
	connect(actionTool3, SIGNAL(triggered()), this, SLOT(tool3Triggered()));

	// actions of tools panel
	connect(myToolsPanel->mode1Button, SIGNAL(clicked()), this, SLOT(mode1Clicked()));
	connect(myToolsPanel->mode2Button, SIGNAL(clicked()), this, SLOT(mode2Clicked()));
	connect(myToolsPanel->mode3Button, SIGNAL(clicked()), this, SLOT(mode3Clicked()));

	// actions of Low panel
	connect(myLowPanel->brightnessSlider, SIGNAL(sliderReleased()), myLowPanel, SLOT(brightnessSliderPositionChanged()));
	connect(myLowPanel->contrastSlider, SIGNAL(sliderReleased()), myLowPanel, SLOT(contrastSliderPositionChanged()));
	connect(myLowPanel->thresholdSlider, SIGNAL(sliderReleased()), myLowPanel, SLOT(thresholdSliderPositionChanged()));
	
	connect(myLowPanel, SIGNAL(brightnessValueChanged(int)), this, SLOT(brightnessValueChanged(int)));
	connect(myLowPanel, SIGNAL(contrastValueChanged(int)), this, SLOT(contrastValueChanged(int)));
	connect(myLowPanel, SIGNAL(thresholdValueChanged(int)), this, SLOT(thresholdValueChanged(int)));

}

// change brightness of the scene
void FrMainWindow::brightnessValueChanged(int value){
}

// change contrast of the scene
void FrMainWindow::contrastValueChanged(int value){
}

// change threshold of the scene
void FrMainWindow::thresholdValueChanged(int value){
}

void FrMainWindow::tool1Triggered(){
	QMessageBox::information(this, "Info", "tool 1 triggered");

}

void FrMainWindow::tool2Triggered(){
	QMessageBox::information(this, "Info", "tool 2 triggered");

}

void FrMainWindow::tool3Triggered(){
	QMessageBox::information(this, "Info", "tool 3 clicked");

}

void FrMainWindow::mode1Clicked(){
	QMessageBox::information(this, "Info", "mode 1 clicked");

}

void FrMainWindow::mode2Clicked(){
	QMessageBox::information(this, "Info", "mode 2 clicked");

}

void FrMainWindow::mode3Clicked(){
	QMessageBox::information(this, "Info", "mode 3 clicked");

}