#ifndef WORKWITHXML_H
#define WORKWITHXML_H


#include <QDebug>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include <QXmlDefaultHandler>
#include <QXmlAttributes>
#include <QString>



class AddressBookParser : public QXmlDefaultHandler {
private:
    QString m_strText;
    QTreeWidget *treeWidget;
    QTreeWidgetItem *currentItem;
    int iteration = 0;
    QString parsing_title;
    QString temp_text;
public:
    AddressBookParser(QTreeWidget* tree, const QString&);
    bool startElement(const QString&,
                      const QString&,
                      const QString& tag,
                      const QXmlAttributes& attributes);

    bool characters(const QString& strText);

    bool endElement(const QString&, const QString&, const QString& str);

    bool fatalError(const QXmlParseException& exception);
};

#endif // WORKWITHXML_H
