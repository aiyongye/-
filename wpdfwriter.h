#ifndef WPDFWRITER_H
#define WPDFWRITER_H
#include <QFileDialog>
#include <QPdfWriter>
#include <QTextDocument>
#include <QAbstractItemModel>
#include <QPrinter>
#include <QPainter>

class WPdfWriter
{
public:
    WPdfWriter();
    void writePdf();
    void addTable(const QString &title, const QString &subTitle, int rows, int cols, QList<int> colWidth, const QList<QList<QString> > &values);
void exportPdf();
private:
 QString m_html;  // 用于存储生成的 HTML 内容
 QPdfWriter *m_pdfWriter;
};

#endif // WPDFWRITER_H
