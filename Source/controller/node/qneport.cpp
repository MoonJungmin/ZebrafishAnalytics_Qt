/* Copyright (c) 2012, STANISLAW ADASZEWSKI
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of STANISLAW ADASZEWSKI nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL STANISLAW ADASZEWSKI BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */

#include "qneport.h"

#include <QGraphicsScene>
#include <QFontMetrics>

#include <QPen>

#include "qneconnection.h"

QNEPort::QNEPort(QGraphicsItem *parent):
    QGraphicsPathItem(parent)
{
	label = new QGraphicsTextItem(this);

	radius_ = 5;
	margin = 2;

	QPainterPath p;
	p.addEllipse(-radius_, -radius_, 2*radius_, 2*radius_);
	setPath(p);

	setPen(QPen(Qt::darkRed));
	setBrush(Qt::red);

	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);

	m_portFlags = 0;
}

QNEPort::~QNEPort()
{
	foreach(QNEConnection *conn, m_connections)
		delete conn;
}

void QNEPort::setNEBlock(QNEBlock *b)
{
	m_block = b;
}

void QNEPort::setName(const QString &n)
{
	name = n;
	label->setPlainText(n);
}

void QNEPort::setAlign(int a)
{
	m_portAlign = a;
}


void QNEPort::setIsOutput(bool o)
{
	isOutput_ = o;

	QFontMetrics fm(scene()->font());
	QRect r = fm.boundingRect(name);

	if (isOutput_)
		label->setPos(-radius_ - margin - label->boundingRect().width(), -label->boundingRect().height()/2);
	else
		label->setPos(radius_ + margin, -label->boundingRect().height()/2);
}

void QNEPort::setIsInput(bool o)
{
	isInput_ = o;
}

int QNEPort::radius()
{
	return radius_;
}

bool QNEPort::isOutput()
{
	return isOutput_;
}

bool QNEPort::isInput()
{
	return isInput_;
}

QVector<QNEConnection*>& QNEPort::connections()
{
	return m_connections;
}
void QNEPort::clearConnection() {
	foreach(QNEConnection *conn, m_connections)
		if (conn->connected) {
			delete conn;
		}
}


void QNEPort::setPortFlags(int f)
{
	m_portFlags = f;
	QString backgroundcolor_style = "background-color:" + m_block->node_color.name();

	if (m_portFlags == NamePort) {
		QFont font(scene()->font());
		font.setPointSize(12);
		font.setBold(true);
		//font.setBold(false);
		label->setFont(font);
		setPath(QPainterPath());
		QFontMetrics fm(font);
		m_width = fm.width(name);
		m_height = fm.height();

	} 
	else if (m_portFlags == TypePort) {
		QFont font(scene()->font());
		font.setPointSize(8);
		font.setBold(false);
		label->setFont(font);
		qDebug() << "TypePort";
		setPath(QPainterPath());

		QFontMetrics fm(font);
		m_width = fm.width(name);
		m_height = fm.height();
	}
	else if (m_portFlags == DataWidgetPort) {
		QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
		pMyProxy->setWidget(m_block->mBlock->DataHeatmap);
		setPath(QPainterPath());
		m_width = m_block->mBlock->DataHeatmap->width();
		m_height = m_block->mBlock->DataHeatmap->height();

	}
	else if (m_portFlags == SubregionWidgetPort) {
		QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
		QLabel *icon_label = new QLabel;
		icon_label->setStyleSheet(backgroundcolor_style);
		QIcon icon("Resources/icon_subregion.png");
		QPixmap pixmap = icon.pixmap(QSize(70, 70));
		icon_label->setPixmap(pixmap);
		pMyProxy->setWidget(icon_label);
		qDebug() << "DataWidgetPort";
		setPath(QPainterPath());

		m_width = 70;
		m_height = 70;
	}
	else if (m_portFlags == DataSizePort) {
		QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
		pMyProxy->setWidget(m_block->mBlock->DataInputOutput);
		setPath(QPainterPath());
		m_width = m_block->mBlock->DataInputOutput->width();
		m_height = m_block->mBlock->DataInputOutput->height();
	}
	else if (m_portFlags == SubregionDropdownPort) {
		QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
		pMyProxy->setWidget(m_block->mBlock->SubregionDropdown);
		setPath(QPainterPath());
		m_width = m_block->mBlock->SubregionDropdown->width();
		m_height = m_block->mBlock->SubregionDropdown->height();
	}
	else if (m_portFlags == ToolBoxPort) {
		QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
		
		pMyProxy->setWidget(m_block->mBlock->ToolBox);

		setPath(QPainterPath());

		m_width = m_block->mBlock->ToolBox->width();
		m_height = m_block->mBlock->ToolBox->height();
	}
	else if (m_portFlags == FeatureWidgetPort) {
		QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
		pMyProxy->setWidget(m_block->mBlock->FeatureHistogramMaster);
		setPath(QPainterPath());
		m_width = m_block->mBlock->FeatureHistogramMaster->width();
		m_height = m_block->mBlock->FeatureHistogramMaster->height();
	}
	else if (m_portFlags == FeatureDropdownPort) {
		QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
		pMyProxy->setWidget(m_block->mBlock->FeatureDropdown);
		setPath(QPainterPath());
		m_width = m_block->mBlock->FeatureDropdown->width();
		m_height = m_block->mBlock->FeatureDropdown->height();
	}
	else if (m_portFlags == SetDropdownPort) {
		QGraphicsProxyWidget* pMyProxy = new QGraphicsProxyWidget(this);
		pMyProxy->setWidget(m_block->mBlock->SetDropdown);
		setPath(QPainterPath());
		m_width = m_block->mBlock->SetDropdown->width();
		m_height = m_block->mBlock->SetDropdown->height();
	}

}

QNEBlock* QNEPort::block() const
{
	return m_block;
}

quint64 QNEPort::ptr()
{
	return m_ptr;
}

void QNEPort::setPtr(quint64 p)
{
	m_ptr = p;
}

bool QNEPort::isConnected(QNEPort *other)
{
	foreach(QNEConnection *conn, m_connections)
		if (conn->port1() == other || conn->port2() == other)
			return true;

	return false;
}

QVariant QNEPort::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if (change == ItemScenePositionHasChanged)
	{
		foreach(QNEConnection *conn, m_connections)
		{
			conn->updatePosFromPorts();
			conn->updatePath();
		}
	}
	return value;
}

void QNEPort::handleDropdownSubregion(int index) {
	qDebug() << "Subregion " << index;
}
void QNEPort::handleDropdownFeature(int index) {
	qDebug() << "Feature " << index;
}