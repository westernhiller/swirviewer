#ifndef ANALYZER_H
#define ANALYZER_H

#include <QDialog>
#include <QImage>
#include <QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

QT_CHARTS_USE_NAMESPACE

#include "global.h"
#include "glcanvas.h"
#include "linepicker.h"

class Analyzer : public QDialog
{
    Q_OBJECT
public:
    explicit Analyzer(QWidget* parent = nullptr);
    ~Analyzer();

signals:
    void imageSaved(QString);

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    QLineEdit*      m_pImageName;
    QList<QColor>   m_colorlist;
    QList<QLineSeries*> m_seriesList;
    QList<LinePicker*> m_linepickerList;
    QChart  *       m_pChart;
    QChartView *    m_pChartView;
    QVector<QPointF > m_testData;
    QValueAxis *    m_axisX;
    QValueAxis *    m_axisY;
    SWIRSETTINGS*   m_pSettings;
    PACKAGEBUFFER   m_packageBuffer;

    void createChart(QString title);
    QImage imageFromChart();

public slots:
    void updateImage(QByteArray);
    void dumpImage();
};

#endif // ANALYZEDIALOG_H
