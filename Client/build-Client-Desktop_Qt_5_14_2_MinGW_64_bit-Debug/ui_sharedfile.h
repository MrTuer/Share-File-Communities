/********************************************************************************
** Form generated from reading UI file 'sharedfile.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHAREDFILE_H
#define UI_SHAREDFILE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SharedFile
{
public:
    QHBoxLayout *horizontalLayout;
    QListWidget *listWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *selectall_PB;
    QPushButton *concel_PB;
    QSpacerItem *verticalSpacer;
    QPushButton *sure_PB;

    void setupUi(QWidget *SharedFile)
    {
        if (SharedFile->objectName().isEmpty())
            SharedFile->setObjectName(QString::fromUtf8("SharedFile"));
        SharedFile->resize(578, 456);
        horizontalLayout = new QHBoxLayout(SharedFile);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        listWidget = new QListWidget(SharedFile);
        listWidget->setObjectName(QString::fromUtf8("listWidget"));
        listWidget->setSelectionMode(QAbstractItemView::MultiSelection);

        horizontalLayout->addWidget(listWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        selectall_PB = new QPushButton(SharedFile);
        selectall_PB->setObjectName(QString::fromUtf8("selectall_PB"));
        QFont font;
        font.setFamily(QString::fromUtf8("Arial"));
        font.setPointSize(18);
        selectall_PB->setFont(font);

        verticalLayout->addWidget(selectall_PB);

        concel_PB = new QPushButton(SharedFile);
        concel_PB->setObjectName(QString::fromUtf8("concel_PB"));
        concel_PB->setFont(font);

        verticalLayout->addWidget(concel_PB);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        sure_PB = new QPushButton(SharedFile);
        sure_PB->setObjectName(QString::fromUtf8("sure_PB"));
        sure_PB->setFont(font);

        verticalLayout->addWidget(sure_PB);


        horizontalLayout->addLayout(verticalLayout);


        retranslateUi(SharedFile);

        QMetaObject::connectSlotsByName(SharedFile);
    } // setupUi

    void retranslateUi(QWidget *SharedFile)
    {
        SharedFile->setWindowTitle(QCoreApplication::translate("SharedFile", "Form", nullptr));
        selectall_PB->setText(QCoreApplication::translate("SharedFile", "\345\205\250\351\200\211", nullptr));
        concel_PB->setText(QCoreApplication::translate("SharedFile", "\345\217\226\346\266\210\345\267\262\351\200\211", nullptr));
        sure_PB->setText(QCoreApplication::translate("SharedFile", "\347\241\256\345\256\232", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SharedFile: public Ui_SharedFile {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHAREDFILE_H
