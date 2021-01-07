#include "workwithxml.h"
#include <QDebug>

AddressBookParser::AddressBookParser(QTreeWidget* tree, const QString& title) {
    QString m_strText;
    treeWidget = tree;
    currentItem = nullptr;
    parsing_title = title;

}

bool AddressBookParser::startElement(const QString&,
                                     const QString&,
                                     const QString& tag,
                                     const QXmlAttributes& attributes) {
    if (tag == "catalog") {
        currentItem = new QTreeWidgetItem(treeWidget);
        currentItem->setText(0, parsing_title);
    }
    else if (tag == "array") {
        ++iteration;
        currentItem = new QTreeWidgetItem(currentItem);
    }
    else {
        currentItem = new QTreeWidgetItem(currentItem);
    }
    return true;
}

bool AddressBookParser::characters(const QString& strText) {
    m_strText = strText;
    return true;
}


bool AddressBookParser::endElement(const QString&, const QString&, const QString& str) {
    if (str != "array" && str != "catalog") {
        currentItem->setText(0, m_strText);
        currentItem = currentItem->parent();
    } else if (str == "array") {
        currentItem = currentItem->parent();
    }
    return true;
}

bool AddressBookParser::fatalError(const QXmlParseException& exception) {
    qDebug() << "Line:" << exception.lineNumber()
             << ", Column:" << exception.columnNumber()
             << ", Message:" << exception.message();
    return false;
}
