/*	Author:		Jacob Howard
    Supervisor: Dr. Ryan Leduc

    Project created in conformity with the requirements for the Degree of Master of Engineering in Software Engineering,
    Computing and Software Department,
    McMaster University
    2017
*/

#include "RegressionTestSerializer.h"
#include "RegressionTestEditor.h"
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>

RegressionTestSerializer::RegressionTestSerializer(QTreeWidget* regTestWidg, const QMap<std::wstring, std::wstring> &projMap) :
    m_regTestWidg(regTestWidg),
    m_projectMap(projMap),
    m_origProjectMap(projMap)
{
}

QMap<std::wstring, std::wstring> RegressionTestSerializer::getMapToTest(const QString &regTestFile)
{
    bool result = readRegTestFile(regTestFile);
    if(result) return m_projectMap;
    else return m_origProjectMap;
}

bool RegressionTestSerializer::readRegTestFile(const QString &regTestFile)
{
    QFile file(regTestFile);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        return fileOpenError(regTestFile, file);
    }
    regTestReader.setDevice(&file);
    regTestReader.readNext();
    while(!regTestReader.atEnd())
    {
        if(regTestReader.isStartElement())
        {
            if(regTestReader.name() == "regtest")
            {
                readRegTestElement(regTestFile);
            }
            else
            {
                regTestReader.raiseError(QObject::tr("Not a Regression Test file"));
            }
        }
        else regTestReader.readNext();
    }
    file.close();
    if(regTestReader.hasError())
    {
        return parseError(regTestFile);
    }
    else if(file.error() != QFile::NoError)
    {
        return readError(regTestFile, file);
    }
    return true;
}

void RegressionTestSerializer::readRegTestElement(const QString &regTestFile)
{
    regTestReader.readNext();
    while(!regTestReader.atEnd())
    {
        if(regTestReader.isEndElement())
        {
            regTestReader.readNext();
            break;
        }
        if(regTestReader.isStartElement())
        {
            if(regTestReader.name() == "project")
            {
                readProjectElement(m_regTestWidg->invisibleRootItem(), regTestFile);
            }
            else if(regTestReader.name() == "test")
            {
                readTestElement(m_regTestWidg->invisibleRootItem(), regTestFile);
            }
            else skipUnknownElement();
        }
        else regTestReader.readNext();
    }
}

void RegressionTestSerializer::readProjectElement(QTreeWidgetItem *parent, const QString &regTestFile)
{
    QTreeWidgetItem *projItem = new QTreeWidgetItem(parent);
    projItem->setText(0, regTestReader.attributes().value("name").toString());

    regTestReader.readNext();
    while(!regTestReader.atEnd())
    {
        if(regTestReader.isEndElement())
        {
            regTestReader.readNext();
            break;
        }
        if(regTestReader.isStartElement())
        {
            if(regTestReader.name() == "result")
            {
                readResultElement(projItem);
            }
            else if(regTestReader.name() == "type")
            {
                readTypeElement(projItem, regTestFile);
            }
            else if(regTestReader.name() == "test")
            {
                readTestElement(m_regTestWidg->invisibleRootItem(), regTestFile);
            }
            else if(regTestReader.name() == "location")
            {
                readLocationElement(projItem, regTestFile);
            }
            else if(regTestReader.name() == "project")
            {
                readProjectElement(m_regTestWidg->invisibleRootItem(), regTestFile);
            }
            else skipUnknownElement();
        }
        else regTestReader.readNext();
    }
}

void RegressionTestSerializer::readTypeElement(QTreeWidgetItem *parent, const QString &regTestFile)
{
    QTreeWidgetItem *projItem = new QTreeWidgetItem(parent);
    projItem->setText(0, regTestReader.attributes().value("name").toString());

    regTestReader.readNext();
    while(!regTestReader.atEnd())
    {
        if(regTestReader.isEndElement())
        {
            regTestReader.readNext();
            break;
        }
        if(regTestReader.isStartElement())
        {
            if(regTestReader.name() == "result")
            {
                readResultElement(projItem);
            }
            else if(regTestReader.name() == "type")
            {
                readTypeElement(projItem, regTestFile);
            }
            else if(regTestReader.name() == "test")
            {
                readTestElement(m_regTestWidg->invisibleRootItem(), regTestFile);
            }
            else if(regTestReader.name() == "location")
            {
                readLocationElement(projItem, regTestFile);
            }
            else if(regTestReader.name() == "project")
            {
                readProjectElement(m_regTestWidg->invisibleRootItem(), regTestFile);
            }
            else skipUnknownElement();
        }
        else regTestReader.readNext();
    }
}

void RegressionTestSerializer::readTestElement(QTreeWidgetItem *parent, const QString &regTestFile)
{
    QTreeWidgetItem *testItem = new QTreeWidgetItem(parent);
    testItem->setText(0, regTestReader.attributes().value("name").toString());
    testItem->setText(1, "Test");
    regTestReader.readNext();
    while(!regTestReader.atEnd())
    {
        if(regTestReader.isEndElement())
        {
            regTestReader.readNext();
            break;
        }
        if(regTestReader.isStartElement())
        {
            if(regTestReader.name() == "location")
            {
                readLocationElement(testItem, regTestFile);
            }
            else if(regTestReader.name() == "result")
            {
                readResultElement(testItem);
            }
            else if(regTestReader.name() == "type")
            {
                readTypeElement(testItem, regTestFile);
            }
            else if(regTestReader.name() == "test")
            {
                readTestElement(m_regTestWidg->invisibleRootItem(), regTestFile);
            }
            else skipUnknownElement();
        }
        else regTestReader.readNext();
    }
}

void RegressionTestSerializer::readResultElement(QTreeWidgetItem *parent)
{
    QString result = regTestReader.readElementText();
    if(regTestReader.isEndElement())
    {
        regTestReader.readNext();
    }
    parent->setText(1, result);
}

//use test file path and relative file path of item to create absolute file path to item
void RegressionTestSerializer::readLocationElement(QTreeWidgetItem *parent, const QString &regTestFile)
{
    QString location = regTestReader.readElementText();
    if(regTestReader.isEndElement())
    {
        regTestReader.readNext();
    }
    QFileInfo fileInfo(regTestFile);
    QDir dir(fileInfo.dir());
    QString absPath = dir.cleanPath(dir.absoluteFilePath(location));
    if(!m_projectMap.contains(parent->text(0).toStdWString()))
    {
       m_projectMap.insert(parent->text(0).toStdWString(), absPath.toStdWString());
    }
}

void RegressionTestSerializer::skipUnknownElement()
{
    regTestReader.readNext();
    while(!regTestReader.atEnd())
    {
        if(regTestReader.isEndElement())
        {
            regTestReader.readNext();
            break;
        }
        if(regTestReader.isStartElement())
        {
            skipUnknownElement();
        }
        else regTestReader.readNext();
    }
}

bool RegressionTestSerializer::writeRegTestXml(const QString &fileName)
{
    QFile file(fileName);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        fileWriteError(fileName, file);
        return false;
    }

    QXmlStreamWriter regTestWriter(&file);
    regTestWriter.setAutoFormatting(true);
    regTestWriter.writeStartDocument();
    regTestWriter.writeStartElement("regtest");
    for(int i=0; i<m_regTestWidg->topLevelItemCount(); ++i)
    {
        if(m_regTestWidg->topLevelItem(i)->text(1) == "Test")
        {
            writeTestEntry(&regTestWriter, m_regTestWidg->topLevelItem(i), fileName);
        }
        else writeProjectEntry(&regTestWriter, m_regTestWidg->topLevelItem(i), fileName);
    }
    regTestWriter.writeEndDocument();
    file.close();
    if(file.error())
    {
        fileWriteError(fileName, file);
        return false;
    }
    return true;
}

void RegressionTestSerializer::writeProjectEntry(QXmlStreamWriter *regTestWriter, QTreeWidgetItem *item, const QString &fileName)
{
    regTestWriter->writeStartElement("project");
    regTestWriter->writeAttribute("name", item->text(0));
    QFileInfo fileInfo(fileName);
    QDir dir(fileInfo.dir());
    QString location = dir.relativeFilePath(QString::fromStdWString(m_projectMap.value(item->text(0).toStdWString())));
    regTestWriter->writeTextElement("location", location);
    QString result = item->text(1);
    regTestWriter->writeTextElement("result", result);
    for(int i=0; i<item->childCount(); ++i)
    {
        writeTypeEntry(regTestWriter, item->child(i));
    }
    regTestWriter->writeEndElement();
}

void RegressionTestSerializer::writeTestEntry(QXmlStreamWriter *regTestWriter, QTreeWidgetItem *item, const QString &fileName)
{
    regTestWriter->writeStartElement("test");
    regTestWriter->writeAttribute("name", item->text(0));
    QFileInfo fileInfo(fileName);
    QDir dir(fileInfo.dir());
    QString location = dir.relativeFilePath(QString::fromStdWString(m_projectMap.value(item->text(0).toStdWString())));
    regTestWriter->writeTextElement("location", location);
    regTestWriter->writeEndElement();
}

void RegressionTestSerializer::writeTypeEntry(QXmlStreamWriter *regTestWriter, QTreeWidgetItem *item)
{
    regTestWriter->writeStartElement("type");
    regTestWriter->writeAttribute("name", item->text(0));
    QString result = item->text(1);
    if(!result.isEmpty()) regTestWriter->writeTextElement("result", result);
    for(int i=0; i<item->childCount(); ++i)
    {
        writeTypeEntry(regTestWriter, item->child(i));
    }
    regTestWriter->writeEndElement();
}

bool RegressionTestSerializer::fileOpenError(const QString &regTestFile, const QFile &file)
{
    QString openErr = QString("Error: Cannot read file " + regTestFile + ": \n"
                                + file.errorString());
    QMessageBox *errOpenMsg = new QMessageBox();
    errOpenMsg->setText(openErr);
    errOpenMsg->setIcon(QMessageBox::Warning);
    errOpenMsg->exec();
    return false;
}

bool RegressionTestSerializer::parseError(const QString &regTestFile)
{
    QString parseErr = QString("Error: Failed to parse file " + regTestFile + ": \n"
                                + regTestReader.errorString());
    QMessageBox *errParseMsg = new QMessageBox();
    errParseMsg->setText(parseErr);
    errParseMsg->setIcon(QMessageBox::Warning);
    errParseMsg->exec();
    return false;
}

bool RegressionTestSerializer::readError(const QString &regTestFile, const QFile &file)
{
    QString readErr = QString("Error: Cannot read file " + regTestFile + ": \n"
                                + file.errorString());
    QMessageBox *errReadMsg = new QMessageBox();
    errReadMsg->setText(readErr);
    errReadMsg->setIcon(QMessageBox::Warning);
    errReadMsg->exec();
    return false;
}

bool RegressionTestSerializer::fileWriteError(const QString &fileName, const QFile &file)
{
    QString writeErr = QString("Error: Cannot write file " + fileName + ": \n"
                                + file.errorString());
    QMessageBox *errOpenMsg = new QMessageBox();
    errOpenMsg->setText(writeErr);
    errOpenMsg->setIcon(QMessageBox::Warning);
    errOpenMsg->exec();
    return false;
}
