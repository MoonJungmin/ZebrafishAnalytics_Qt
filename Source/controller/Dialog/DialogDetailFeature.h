#pragma once
#include <QObject>
#include <QDialog>
#include <QtWidgets>
#include <fstream>
#include <string>
#include <iostream>
#include <string>
#include "Source/global.h"

class DialogDetailFeature : public QDialog
{
	Q_OBJECT

signals:
	void thread_kill();

public slots:
	int exec();
	void done(int val);
	void accept();
	void reject();


public:
	DialogDetailFeature(QWidget *parent);
	~DialogDetailFeature();
	void setIndexAndDraw(int index);

private:
	QHBoxLayout * main_layout;
	QWidget * pWidget;
	int mIndex = 0;
	QTableWidget *TableWidget;

};