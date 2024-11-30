#include "wpdfwriter.h"

WPdfWriter::WPdfWriter()
{

}
void WPdfWriter::exportPdf()
{
    // 选择保存路径
    QString path = QFileDialog::getSaveFileName(nullptr, QString("Open file"),
                                                QString("."), QString("PDF Files(*.pdf)"));
    if (path.isEmpty())
        return;

    // 如果文件没有后缀，则添加 .pdf 后缀
    if (QFileInfo(path).suffix().isEmpty())
        path.append(".pdf");

    // 创建 QPdfWriter 对象并设置 PDF 输出路径
    QPdfWriter pdfWriter(path);
    pdfWriter.setPageSize(QPagedPaintDevice::A4);
    pdfWriter.setResolution(QPrinter::ScreenResolution);

    // 创建 QTextDocument 来保存 HTML 内容
    QTextDocument textDocument;

    // 添加 HTML 内容
    m_html.clear(); // 确保清空之前的内容
    m_html.append("<h1 style='text-align:center;'>转向架悬挂件节点压装力曲线</h1><br />");

    // 写入更多 PDF 内容（如果有）
    writePdf();  // 确保这个函数已正确实现

    // 添加图片
    m_html.append("<img src='qtLogo.png' width=\"100\" height=\"100\">");

    // 将 HTML 内容设置到 QTextDocument
    textDocument.setHtml(m_html);

    // 使用 QPdfWriter（即 QPrinter） 打印 PDF
    textDocument.print(&pdfWriter);

    // 关闭文件
    pdfWriter.newPage();  // 如果需要新页面，使用 newPage
}

void WPdfWriter::addTable(const QString &title, const QString &subTitle, int rows, int cols, QList<int> colWidth, const QList<QList<QString> > &values)
{
    m_html.append("<table border='0.5' cellspacing='0' cellpadding='3' width:100%>");

    //主标题
    if(!title.isEmpty())
    {
        m_html.append(QString("<tr><td align='center' style='vertical-align:middle;font-weight:bold;' colspan='%1'>").arg(cols));
        m_html.append(title);
        m_html.append("</td></tr>");
    }

    //副标题
    if(!subTitle.isEmpty())
    {
        m_html.append(QString("<tr><td align='left' style='vertical-align:middle;font-weight:bold;' colspan='%1'>").arg(cols));
        m_html.append(subTitle);
        m_html.append("</td></tr>");
    }

    //添加字段/字段值
    for(int i = 0;i < rows;i++)
    {
        m_html.append("<tr>");
        QList<QString> rowValues = values.at(i);
        for(int j = 0;j < cols;j++)
        {
            m_html.append(QString("<td width=%1% valign='center' style='vertical-align:middle;font-size:100px;'>").arg(colWidth.at(j)));
            m_html.append(rowValues.at(j));
            m_html.append("</td>");
        }
        m_html.append("</tr>");
    }

    m_html.append("</table><br /><br />");
}

void WPdfWriter::writePdf()
{
    ///添加基本信息
    QList<QString> rowsValues;
    QList<QList<QString>> values;

    //导出t1
    rowsValues.append("姓名");
    rowsValues.append("班级");
    values.append(rowsValues);

    rowsValues.clear();
    rowsValues.append("Tom");
    rowsValues.append("二年级");
    values.append(rowsValues);

    rowsValues.clear();
    rowsValues.append("mary");
    rowsValues.append("一年级");
    values.append(rowsValues);

    rowsValues.clear();
    rowsValues.append("Bob");
    rowsValues.append("二年级");
    values.append(rowsValues);

    rowsValues.clear();
    rowsValues.append("lili");
    rowsValues.append("三年级");
    values.append(rowsValues);

    QList<int> width;
    width.append(30);
    width.append(70);
    addTable("T1主标题","T1子标题",5,2,width,values);

    //导出t2
    rowsValues.clear();
    values.clear();
    rowsValues.append("姓名");
    rowsValues.append("地址");
    rowsValues.append("年龄");
    rowsValues.append("性别");
    values.append(rowsValues);

    rowsValues.clear();
    rowsValues.append("Tom");
    rowsValues.append("四川成都");
    rowsValues.append("16");
    rowsValues.append("男");
    values.append(rowsValues);

    rowsValues.clear();
    rowsValues.append("mary");
    rowsValues.append("四川成都");
    rowsValues.append("15");
    rowsValues.append("女");
    values.append(rowsValues);

    width.clear();
    width.append(25);
    width.append(25);
    width.append(25);
    width.append(25);

    addTable("T2主标题","T2子标题",3,4,width,values);
}
