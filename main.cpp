#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget *window = new QWidget();
    window->setWindowTitle("Форма заключения терапевта");

    QLabel *nameLabel = new QLabel("Имя:");
    QLineEdit *nameLineEdit = new QLineEdit();

    QLabel *ageLabel = new QLabel("Возраст:");
    QSpinBox *ageSpinBox = new QSpinBox();
    ageSpinBox->setMinimum(0);
    ageSpinBox->setMaximum(150);

    QLabel *genderLabel = new QLabel("Пол:");
    QComboBox *genderComboBox = new QComboBox();
    genderComboBox->addItem("Мужской");
    genderComboBox->addItem("Женский");

    QLabel *addressLabel = new QLabel("Адрес:");
    QTextEdit *addressTextEdit = new QTextEdit();
    addressTextEdit->setMaximumHeight(50);

    QLabel *diagnosisLabel = new QLabel("Диагноз:");
    QTextEdit *diagnosisTextEdit = new QTextEdit();
    diagnosisTextEdit->setMaximumHeight(50);

    QLabel *textlabel = new QLabel("Заключение");
    textlabel->setAlignment(Qt::AlignCenter);
    QFont font("Arial", 20, QFont::Bold);
    textlabel->setFont(font);
    textlabel->setGeometry(0, 0, 400, 50);

    QPushButton *saveButton = new QPushButton("Сохранить");
    QPushButton *openButton = new QPushButton("Открыть");
    QPushButton *clearButton = new QPushButton("Очистить");

    QVBoxLayout *layout = new QVBoxLayout();
    QHBoxLayout *textLayout = new QHBoxLayout();
    textLayout->addWidget(textlabel);
    layout->addLayout(textLayout);


    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameLabel);
    nameLayout->addWidget(nameLineEdit);
    layout->addLayout(nameLayout);

    QHBoxLayout *ageLayout = new QHBoxLayout();
    ageLayout->addWidget(ageLabel);
    ageLayout->addWidget(ageSpinBox);
    layout->addLayout(ageLayout);

    QHBoxLayout *genderLayout = new QHBoxLayout();
    genderLayout->addWidget(genderLabel);
    genderLayout->addWidget(genderComboBox);
    layout->addLayout(genderLayout);

    QHBoxLayout *addressLayout = new QHBoxLayout();
    addressLayout->addWidget(addressLabel);
    addressLayout->addWidget(addressTextEdit);
    layout->addLayout(addressLayout);

    QHBoxLayout *diagnosisLayout = new QHBoxLayout();
    diagnosisLayout->addWidget(diagnosisLabel);
    diagnosisLayout->addWidget(diagnosisTextEdit);
    layout->addLayout(diagnosisLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(openButton);
    buttonLayout->addWidget(clearButton);
    layout->addLayout(buttonLayout);

    window->setLayout(layout);

   QObject::connect(saveButton, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getSaveFileName(window, "Сохранить файл", "", "Текстовые файлы (*.txt)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << "Имя: " << nameLineEdit->text() << "\n";
                out << "Возраст: " << ageSpinBox->value() << "\n";
                out << "Пол: " << genderComboBox->currentText() << "\n";
                out << "Адрес: " << addressTextEdit->toPlainText() << "\n";
                out << "Диагноз: " << diagnosisTextEdit->toPlainText() << "\n";
                file.close();
                QMessageBox::information(window, "Сохранение", "Данные сохранрены в файл");
            } else {
            QMessageBox::warning(window, "Ошибка", "Не удалось сохранить файл");
            }
            }
            });
    QObject::connect(openButton, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getOpenFileName(window, "Открыть файл", "", "Текстовые файлы (*.txt)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QString name = "";
                QString age = "";
                QString gender = "";
                QString address = "";
                QString diagnosis = "";
                while (!in.atEnd()) {
                    QString line = in.readLine();
                    QStringList parts = line.split(": ");
                    if (parts.length() == 2) {
                        QString key = parts[0];
                        QString value = parts[1];
                        if (key == "Имя") {
                            name = value;
                        } else if (key == "Возраст") {
                            age = value;
                        } else if (key == "Пол") {
                            gender = value;
                        } else if (key == "Адрес") {
                            address = value;
                        } else if (key == "Диагноз") {
                            diagnosis = value;
                        }
                    }
                }
                nameLineEdit->setText(name);
                ageSpinBox->setValue(age.toInt());
                int index = genderComboBox->findText(gender);
                genderComboBox->setCurrentIndex(index);
                addressTextEdit->setText(address);
                diagnosisTextEdit->setText(diagnosis);
                file.close();
                QMessageBox::information(window, "Открытие", "Данные загружены из файла");
            } else {
                QMessageBox::warning(window, "Ошибка", "Не удалось открыть файл");
            }
        }
    });

    QObject::connect(clearButton, &QPushButton::clicked, [=]() {
        nameLineEdit->clear();
        ageSpinBox->setValue(0);
        genderComboBox->setCurrentIndex(0);
        addressTextEdit->clear();
        diagnosisTextEdit->clear();
    });

    window->show();

    return app.exec();
}
