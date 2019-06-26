#include "notebook.h"
#include "ui_notebook.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QDir>
#include <QPushButton>
#include <QTextStream>
#include <QFileInfo>
#include <QFile>
#include <QCloseEvent>
#include <QContextMenuEvent>
#include <QTextEdit>
#include <QScrollBar>
#include <QDesktopServices>
#include <QTextLayout>
#include <QTextDocument>
#include <QTextCursor>
#include <QDateTime>
#include <QTextBlock>
#include <QFlags>
#include <QMessageBox>
NoteBook::NoteBook(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NoteBook){
    ui->setupUi(this);
    ini_connect();
    ui->text_area->setContextMenuPolicy(Qt::NoContextMenu);
    ui->text_area->verticalScrollBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->text_area->horizontalScrollBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->text_area->setLineWrapMode(QTextEdit::WidgetWidth);
    ui->text_area->setAcceptRichText(false);
    ui->statusBar->addPermanentWidget(row_col_msg);
    }
void NoteBook::receive_save_statue(int receive){
    save_status=receive;
}
NoteBook::~NoteBook(){
    delete ui;
}
QMenu * NoteBook::summon_right_click_menu(){
    QMenu * right_click_menu=new QMenu();
    right_click_menu->addAction(ui->action_U);
    right_click_menu->addSeparator();
    right_click_menu->addAction(ui->action_T);
    right_click_menu->addAction(ui->action_C);
    right_click_menu->addAction(ui->action_P);
    right_click_menu->addAction(ui->action_L);
    right_click_menu->addSeparator();
    right_click_menu->addAction(ui->action_All);
    return right_click_menu;
}
void NoteBook::contextMenuEvent(QContextMenuEvent * event){
    check_can_paste();
    QMenu * right_clicked_menu=summon_right_click_menu();
    if(ui->text_area->geometry().contains(event->pos())){
        right_clicked_menu->exec(event->globalPos());
    }
}
void NoteBook::ini_connect(){
    connect(ui->action_N,SIGNAL(triggered()),this,SLOT(new_file()));//新建
    connect(ui->action_O,SIGNAL(triggered()),this,SLOT(open_file()));//打开
    connect(ui->action_S,SIGNAL(triggered()),this,SLOT(save_file()));//保存
    connect(ui->action_As,SIGNAL(triggered()),this,SLOT(save_as()));//另存为

    connect(ui->action_X,SIGNAL(triggered()),this,SLOT(quit()));//退出

    connect(ui->action_U,SIGNAL(triggered()),this,SLOT(undo()));//撤销
    connect(ui->action_T,SIGNAL(triggered()),this,SLOT(cut()));//剪切
    connect(ui->action_C,SIGNAL(triggered()),this,SLOT(copy()));//复制
    connect(ui->action_P,SIGNAL(triggered()),this,SLOT(paste()));//粘贴
    connect(ui->action_L,SIGNAL(triggered()),this,SLOT(del()));//删除

    connect(ui->action_Find,SIGNAL(triggered()),this,SLOT(search_slot()));//查找
    connect(ui->action_Find_N,SIGNAL(triggered()),this,SLOT(search_next()));//查找下一个
    connect(ui->action_R,SIGNAL(triggered()),this,SLOT(replace_slot()));//替换
    connect(ui->action_G,SIGNAL(triggered()),this,SLOT(go_to()));//转到
    connect(ui->action_All,SIGNAL(triggered()),this,SLOT(all_select()));//全选
    connect(ui->action_D,SIGNAL(triggered()),this,SLOT(date_and_time()));//日期/时间

    connect(ui->action_W,SIGNAL(toggled(bool)),this,SLOT(auto_warp(bool)));//自动换行
    connect(ui->action_Font,SIGNAL(triggered()),this,SLOT(font()));//字体

    connect(ui->action_H,SIGNAL(triggered()),this,SLOT(help()));//帮助
    connect(ui->action_About,SIGNAL(triggered()),this,SLOT(about()));//关于

    connect(ui->text_area,SIGNAL(undoAvailable(bool)),this,SLOT(can_undo(bool)));
    connect(ui->text_area,SIGNAL(copyAvailable(bool)),this,SLOT(can_cut(bool)));
    connect(ui->menu_E,SIGNAL(aboutToShow()),this,SLOT(check_can_paste()));


    connect(&searchwindow,SIGNAL(send_search_signal(QString,bool,bool,bool)),this,SLOT(receive_search_signal(QString,bool,bool,bool)));
    connect(&replacewindow,SIGNAL(send_replace_signal(QString,QString,bool,bool,int)),this,SLOT(replace_replace(QString,QString,bool,bool,int)));
}
void NoteBook::on_text_area_textChanged(){
    is_Save=false;
}
void NoteBook::show_save_dialog(){//确定是否保存
    SaveDialog * save_dialog=new SaveDialog(this);
    save_dialog->change_path(path);
    connect(save_dialog,SIGNAL(send_status(int)),this,SLOT(receive_save_statue(int)));
    save_dialog->exec();
    if(save_status==1){//保存
        qDebug()<<"Save";
        save_file();
    }
    else if (save_status==2) {//不保存
        qDebug()<<"NotSave";
        is_Save=true;
        return;
    }
    else if (save_status==3) {//取消
        qDebug()<<"Cancel";
    }
}
void NoteBook::new_file(){//新建
    if(!is_Save){//给一个保存的机会
        show_save_dialog();
    }
    if(is_Save){//如果is_Save还是假这里就直接跳掉
        is_New=true;
        ui->text_area->setText("");
        setWindowTitle("新建文本文档.txt - 记事本");
        is_Save=true;
    }
}
void NoteBook::open_file(){//打开
    if(!is_Save){//给一个保存的机会
        show_save_dialog();
    }
    if(is_Save){//如果is_Save还是假这里就直接跳掉
        path=QFileDialog::getOpenFileName(this, QStringLiteral("打开"), QDir::currentPath(), QStringLiteral("文本文档(*.txt);;所有文件(*.*)"));
        if(!path.isEmpty()){
            QFileInfo fileinfo(path);
            setWindowTitle(fileinfo.fileName() + QStringLiteral(" - 记事本"));
            QFile * file=new QFile(path);
            file->open(QIODevice::ReadOnly|QIODevice::Text);
            QTextStream stream(file);
            ui->text_area->setText(stream.readAll());
            file->close();
            is_New=false;
            is_Save=true;
        }
    }
}
void NoteBook::save_file(){//保存
    if(is_New){
        save_as();
    }
    else {
        if(!path.isEmpty()){
            QFile * file=new QFile(path);
            file->open(QIODevice::WriteOnly|QIODevice::Text);
            QTextStream stream(file);
            stream<<ui->text_area->toPlainText();
            file->close();
            is_Save=true;
        }
    }
}
void NoteBook::save_as(){//另存为
    path = QFileDialog::getSaveFileName(this, QStringLiteral("另存为"),"新建文本文档.txt", QStringLiteral("文本文档(*.txt);;所有文件(*.*)"));
    if(!path.isEmpty()){
        QFile * file=new QFile(path);
        QFileInfo fileinfo(path);
        setWindowTitle(fileinfo.fileName() + QStringLiteral(" - 记事本"));
        file->open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream stream(file);
        stream<<ui->text_area->toPlainText();
        file->close();
        is_Save=true;
        is_New=false;
    }
}
void NoteBook::quit(){//退出
    if(!is_Save){
        show_save_dialog();
    }
    if(is_Save)close();
}
void NoteBook::closeEvent(QCloseEvent *event){//重写closeEvent
    if(!is_Save){
        show_save_dialog();
        event->ignore();
    }
    if(is_Save){
        exit(0);
    }
}
void NoteBook::can_undo(bool b){
    if(b)ui->action_U->setEnabled(true);
    else ui->action_U->setEnabled(false);
}
void NoteBook::can_cut(bool b){
    if(b){
        ui->action_T->setEnabled(true);
        ui->action_C->setEnabled(true);
        ui->action_L->setEnabled(true);
    }
    else {
        ui->action_T->setEnabled(false);
        ui->action_C->setEnabled(false);
        ui->action_L->setEnabled(false);
    }
}
void NoteBook::check_can_paste(){
    if(ui->text_area->canPaste())ui->action_P->setEnabled(true);
    else ui->action_P->setEnabled(false);
}
void NoteBook::undo(){//撤销
    ui->text_area->undo();
}
void NoteBook::cut(){//剪切
    ui->text_area->cut();
}
void NoteBook::copy(){//复制
    ui->text_area->copy();
}
void NoteBook::paste(){//粘贴
    ui->text_area->paste();
}
void NoteBook::del(){//删除
    QTextCursor(ui->text_area->textCursor()).removeSelectedText();
}
void NoteBook::search_slot(){//查找
    show_searchwindow();
}
void NoteBook::search(bool is_replace_mode=false){
    QFlags<QTextDocument::FindFlag> flags;
    if(is_replace_mode){
        if(is_Re_CaseSensitive)flags|=QTextDocument::FindCaseSensitively;
        if(!ui->text_area->find(search_content,flags)){
            if(is_Re_RepeatFind){
                ui->text_area->moveCursor(QTextCursor::Start);
                if(!ui->text_area->find(search_content,flags)){
                    show_cant_find_window();
                }
                return;
            }
            show_cant_find_window();
        }
    }
    else {
        if(is_CaseSensitive)flags|=QTextDocument::FindCaseSensitively;
        if(is_FindBackward)flags|=QTextDocument::FindBackward;
        if(!ui->text_area->find(search_content,flags)){
            if(is_RepeatFind){
                if(!is_FindBackward)ui->text_area->moveCursor(QTextCursor::Start);
                else ui->text_area->moveCursor(QTextCursor::End);
                if(!ui->text_area->find(search_content,flags)){
                    show_cant_find_window();
                }
                return;
            }
            show_cant_find_window();
        }
    }
}
void NoteBook::show_cant_find_window(){
    QMessageBox cant_find_window(this);
    cant_find_window.setWindowTitle("记事本");
    cant_find_window.setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    cant_find_window.setButtonText(1,"确定");
    cant_find_window.setText("找不到\""+search_content+"\"");
    cant_find_window.setIcon(QMessageBox::Information);
    cant_find_window.exec();
}
void NoteBook::receive_search_signal(QString receive,bool b1,bool b2,bool b3){
    search_content=receive;
    is_CaseSensitive=b1;
    is_FindBackward=b2;
    is_RepeatFind=b3;
    //qDebug()<<search_content<<is_CaseSensitive<<is_positive_dire_search;
    search();
}
void NoteBook::show_searchwindow(){
    search_content=ui->text_area->textCursor().selectedText();
    searchwindow.set_search_text(search_content);
    searchwindow.show();
    searchwindow.activateWindow();
}
void NoteBook::search_next(){//查找下一个
    if(!search_content.isEmpty())search();
    else show_searchwindow();
}
void NoteBook::replace_slot(){//替换
    show_replacewindow();
}
void NoteBook::replace_replace(QString replace_thing,QString replaced_thing,bool b1,bool b2,int mode){
    search_content=replace_thing;
    replace_content=replaced_thing;
    is_Re_CaseSensitive=b1;
    is_Re_RepeatFind=b2;
    switch (mode) {
        case 0:search(true);break;
        case 1:replace();break;
        case 2:all_replace();break;
    }
}
void NoteBook::replace(){
    ui->text_area->textCursor().insertText(replace_content);
    search(true);
}
void NoteBook::all_replace(){
    QFlags<QTextDocument::FindFlag> flags;
    if(is_Re_CaseSensitive)flags|=QTextDocument::FindCaseSensitively;
    ui->text_area->moveCursor(QTextCursor::Start);
    while(ui->text_area->find(search_content,flags)){
        ui->text_area->textCursor().insertText(replace_content);
    };
}
void NoteBook::show_replacewindow(){
    search_content=ui->text_area->textCursor().selectedText();
    replacewindow.set_search_text(search_content);
    replacewindow.show();
    replacewindow.activateWindow();
}
void NoteBook::go_to(){//转到
    show_gotowindow();
    QTextCursor tc = ui->text_area->textCursor();
    if(goto_line==0)return;
    else if(goto_line<=ui->text_area->document()->blockCount()){
    int position=ui->text_area->document()->findBlockByNumber(goto_line-1).position();
    tc.setPosition(position,QTextCursor::MoveAnchor);
    ui->text_area->setTextCursor(tc);
    }
    else {
        LineOverFlow * lineoverflow=new LineOverFlow();
        lineoverflow->exec();
    }
}
void NoteBook::receive_goto_line(int receive){
    goto_line=receive;
}
void NoteBook::show_gotowindow(){
    GotoWindow * gotowindow=new GotoWindow();
    connect(gotowindow,SIGNAL(send_line_num(int)),this,SLOT(receive_goto_line(int)));
    gotowindow->exec();
}
void NoteBook::all_select(){//全选
    ui->text_area->selectAll();
}
void NoteBook::date_and_time(){//日期/时间
    QTextCursor cursor=ui->text_area->textCursor();
    QDateTime localtime(QDateTime::currentDateTime());
    cursor.insertText(localtime.toString("hh:mm yyyy-M-d"));
}
void NoteBook::auto_warp(bool is_warp){//自动换行
    if (is_warp){
        ui->text_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->text_area->setLineWrapMode(QTextEdit::WidgetWidth);
        ui->action_G->setEnabled(false);
    }
    else {
        ui->text_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        ui->text_area->setLineWrapMode(QTextEdit::NoWrap);
        ui->action_G->setEnabled(true);
    }
}
void NoteBook::font(){//字体
    bool is_set;
    _font = QFontDialog::getFont(&is_set,_font,this);
    if(is_set)ui->text_area->setFont(_font);
}
void NoteBook::help(){//查看帮助
    QDesktopServices::openUrl(QUrl("https://www.baidu.com/s?wd=%E8%AE%B0%E4%BA%8B%E6%9C%AC%E6%80%8E%E4%B9%88%E7%94%A8"));
}
void NoteBook::about(){//关于
    About * about= new About();
    about->show();
}

void NoteBook::on_text_area_cursorPositionChanged(){
    int row,col;
    QTextCursor cursor;
    cursor=ui->text_area->textCursor();
    row=cursor.blockNumber();
    col=cursor.positionInBlock();
    row_col_msg->setText("第 "+QString::number(row+1)+" 行, 第 "+QString::number(col+1)+" 列");
}
