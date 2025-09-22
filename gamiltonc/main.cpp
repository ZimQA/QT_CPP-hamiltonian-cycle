#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QSpinBox>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
class Window: public QWidget
{
  QSpinBox* lineEdit;
  QTableView* view;
  QStandardItemModel* srcMatrix;
  QPlainTextEdit* resMatrix;
  int n;
  int** arr; // глобальный указатель на массив
  int* c;    // номер хода, на котором посещается вершина
  int* path; // номера посещаемых вершин // массив, где хранится гамильтонов цикл
  int v0 = 2;    // начальная вершина

  void vuvod()
  {
    QString s;
    for (int p = 0; p < n; p++)
      s.append(QString::number(path[p] + 1) + "\t");
    s.append(QString::number(path[0] + 1));
    resMatrix->appendPlainText(s);
  }

  // ввод кол-ва вершин графа
  void vvod()
  {
    n = lineEdit->value();
  }

  // функция глобального динамического массива
  int** functarr(int n)
  {
    int** arr = new int* [n];
    for (int i = 0; i < n; i++)
      arr[i] = new int[n];
    return arr;
  }

  // нахождение гамильтонова цикла
  int gamilton(int k)
  {
    int q1 = 0;
    for (int v = 0; v < n && !q1; v++)
    {
      if (arr[v][path[k - 1]] || arr[path[k - 1]][v])
      {
        if (k == n && v == v0) q1 = 1;
        else
          if (c[v] == -1)
          {
            c[v] = k; path[k] = v;
            q1 = gamilton(k + 1);
            if (!q1) c[v] = -1;
          }
          else
            continue;
      }
    }
    return q1;
  }
public:
  Window(): QWidget(nullptr)
  {
    QLabel*         label1    = new QLabel("Кол-во вершин");
                    lineEdit  = new QSpinBox();
    QLabel*         label2    = new QLabel("Матрица смежности");
    QLabel*         label3    = new QLabel("Гамильтонов цикл");
                    view      = new QTableView();
                    srcMatrix = new QStandardItemModel(1, 1);
                    resMatrix = new QPlainTextEdit();
    QPushButton*    button1   = new QPushButton("Вычислить");
    QPushButton*    button2   = new QPushButton("Выйти из программы");
    QHBoxLayout*    l1        = new QHBoxLayout();
    QGridLayout*    l2        = new QGridLayout();
    QVBoxLayout*    l3        = new QVBoxLayout();
    l1->addWidget(label1);
    l1->addWidget(lineEdit);
    l2->addWidget(label2, 0, 0);
    l2->addWidget(label3, 0, 1);
    l2->addWidget(view, 1, 0);
    l2->addWidget(resMatrix, 1, 1);
    l2->addWidget(button1);
    l2->addWidget(button2);
    l3->addLayout(l1);
    l3->addLayout(l2);
    setLayout(l3);
    lineEdit->setMinimum(1);
    view->setModel(srcMatrix);
    view->horizontalHeader()->hide();
    view->verticalHeader()->hide();
    connect(button2, &QPushButton::clicked, this, &Window::close);
    connect(button1, &QPushButton::clicked, this, &Window::click);
    connect(lineEdit, QOverload<int>::of(&QSpinBox::valueChanged), this, &Window::resizeMatrix);
    //тестовое заполнение
    lineEdit->setValue(3);
    srcMatrix->setData(srcMatrix->index(0,0), "0");
    srcMatrix->setData(srcMatrix->index(0,1), "1");
    srcMatrix->setData(srcMatrix->index(0,2), "1");
    srcMatrix->setData(srcMatrix->index(1,0), "1");
    srcMatrix->setData(srcMatrix->index(1,1), "0");
    srcMatrix->setData(srcMatrix->index(1,2), "1");
    srcMatrix->setData(srcMatrix->index(2,0), "1");
    srcMatrix->setData(srcMatrix->index(2,1), "1");
    srcMatrix->setData(srcMatrix->index(2,2), "0");
  }

  void resizeMatrix(int value)
   {
     n = value;
     srcMatrix->setRowCount(value);
     srcMatrix->setColumnCount(value);
     view->resizeColumnsToContents();
   }

   void click()
   {
     vvod();
     c = new int[n];   // номер хода, на котором посещается вершина
     path = new int[n]; // номера посещаемых вершин // массив, где хранится гамильтонов цикл
     arr = functarr(n);
     for (int i = 0; i < n; i++)
       for (int j = 0; j < n; j++)
         arr[i][j] = srcMatrix->index(i, j).data().toInt();
     for (int j = 0; j < n; j++)
       c[j] = -1;
     path[0] = v0;
     c[v0] = v0;
     if (gamilton(1))
       vuvod();
     else
       resMatrix->appendPlainText("Нет решений");
     // удаление двумерного динамического массива
     for (int i = 0; i < n; i++)
       delete[] arr[i];  // удаляем массив
     delete[] arr;
     delete[] path;
     delete[] c;
   }
 };

 int main(int argc, char** argv)
 {
   QApplication app(argc, argv);
   Window window;
   window.show();
   return app.exec();
 }
