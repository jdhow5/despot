/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#ifndef REGRESSIONTESTSERIALIZER_H
#define REGRESSIONTESTSERIALIZER_H
#include <QtXml/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>
#include <QFile>
#include <QTreeWidget>

class RegressionTestSerializer
{
public:
    RegressionTestSerializer(QTreeWidget *regTestWidg, const QMap<std::wstring, std::wstring> &projMap);
    QMap<std::wstring, std::wstring> getMapToTest(const QString &regTestFile);
    bool readRegTestFile(const QString &regTestFile);
    bool writeRegTestXml(const QString &fileName);

private:
    QTreeWidget *m_regTestWidg;
    QMap<std::wstring, std::wstring> m_projectMap;
    QMap<std::wstring, std::wstring> m_origProjectMap;
    QXmlStreamReader regTestReader;

    void readRegTestElement(const QString &regTestFile);
    void readTypeElement(QTreeWidgetItem *parent, const QString &regTestFile);
    void readTestElement(QTreeWidgetItem *parent, const QString &regTestFile);
    void readProjectElement(QTreeWidgetItem *parent, const QString &regTestFile);
    void readResultElement(QTreeWidgetItem *parent);
    void readLocationElement(QTreeWidgetItem *parent, const QString &regTestFile);
    void skipUnknownElement();
    void writeTestEntry(QXmlStreamWriter *regTestWriter, QTreeWidgetItem *item, const QString &fileName);
    void writeProjectEntry(QXmlStreamWriter *regTestWriter, QTreeWidgetItem *item, const QString &fileName);
    void writeTypeEntry(QXmlStreamWriter *regTestWriter, QTreeWidgetItem *item);

    bool fileOpenError(const QString &regTestFile, const QFile &file);
    bool parseError(const QString &regTestFile);
    bool readError(const QString &regTestFile, const QFile &file);
    bool fileWriteError(const QString &fileName, const QFile &file);
};

#endif // REGRESSIONTESTSERIALIZER_H
