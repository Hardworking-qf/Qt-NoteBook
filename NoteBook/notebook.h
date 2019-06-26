#ifndef NOTEBOOK_H
#define NOTEBOOK_H

#include <QMainWindow>

#include <savedialog.h>
#include <about.h>
#include <gotowindow.h>
#include <replacewindow.h>
#include <searchwindow.h>
#include <lineoverflow.h>

#include <QtDebug>
#include <QString>
#include <QLabel>
#include <QFontDialog>
namespace Ui {
class NoteBook;
}

class NoteBook : public QMainWindow
{
    Q_OBJECT

public:
    explicit NoteBook(QWidget *parent = nullptr);
    QMenu * summon_right_click_menu();
    void contextMenuEvent(QContextMenuEvent *);
    void ini_connect();//连接槽和信号
    void closeEvent(QCloseEvent *event);//重写closeEvent
    void show_save_dialog();//确定是否保存
    void show_gotowindow();
    void show_searchwindow();
    void show_cant_find_window();
    void show_replacewindow();

    void search(bool);
    void replace();
    void all_replace();
    //菜单功能：
public slots:
    void receive_save_statue(int);

    void new_file();//新建
    void open_file();//打开
    void save_file();//保存
    void save_as();//另存为

    void quit();//退出

    void can_undo(bool);
    void can_cut(bool);
    void check_can_paste();

    void undo();//撤销

    void cut();//剪切
    void copy();//复制
    void paste();//粘贴
    void del();//删除

    void search_slot();//查找
    void receive_search_signal(QString,bool,bool,bool);
    void search_next();//查找下一个
    void replace_slot();//替换

    void replace_replace(QString,QString,bool,bool,int);
    void go_to();//转到
    void receive_goto_line(int);

    void all_select();//全选
    void date_and_time();//日期/时间

    void auto_warp(bool);//自动换行
    void font();//字体

    void help();//查看帮助
    void about();//关于

public:
    ~NoteBook();

private slots:
    void on_text_area_textChanged();

    void on_text_area_cursorPositionChanged();


private:
    bool is_Save=true;
    bool is_New=true;
    Ui::NoteBook *ui;
    int save_status=0;
    int goto_line=0;
    QFont _font=QFont("Arial",24);
    QString path=QStringLiteral("新建文本文档.txt");
    QString search_content="";
    bool is_CaseSensitive=true;
    bool is_FindBackward=true;
    bool is_RepeatFind=false;
    bool is_Re_CaseSensitive=true;
    bool is_Re_RepeatFind=false;
    QString replace_content="";
    QLabel * row_col_msg=new QLabel("第 1 行, 第 1 列");

    SearchWindow searchwindow;
    ReplaceWindow replacewindow;
};
#endif // NOTEBOOK_H
