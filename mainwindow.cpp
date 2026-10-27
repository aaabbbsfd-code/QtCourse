#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFontDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QCloseEvent>
#include <QTextCursor>
#include <QTextDocument>
#include <QPainter>
#include <QApplication>
#include <QTextCharFormat>
#include <QFileInfo>

// ================= CodeEditor 实现 (行号支持) =================
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent) {
    lineNumberArea = new LineNumberArea(this);

    connect(this, &QPlainTextEdit::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
    connect(this, &QPlainTextEdit::updateRequest, this, &CodeEditor::updateLineNumberArea);
    connect(this, &QPlainTextEdit::cursorPositionChanged, this, [this]() {
        lineNumberArea->update();
    });

    updateLineNumberAreaWidth(0);
}

int CodeEditor::lineNumberAreaWidth() {
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
#if QT_VERSION >= QT_VERSION_CHECK(5, 11, 0)
    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
#else
    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;
#endif
    return space;
}

void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */) {
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void CodeEditor::updateLineNumberArea(const QRect &rect, int dy) {
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}

void CodeEditor::resizeEvent(QResizeEvent *e) {
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event) {
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qRound(blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }
        block = block.next();
        top = bottom;
        bottom = top + qRound(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

// ================= MainWindow 实现 =================
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), isUntitled(true) {
    setupEditor();
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    setWindowTitle("无标题 - 简单文本编辑器");
    resize(800, 600);

    connect(editor->document(), &QTextDocument::modificationChanged, this, &MainWindow::updateWindowTitle);
    connect(editor, &QPlainTextEdit::cursorPositionChanged, this, &MainWindow::updateStatusBar);
    connect(editor, &QPlainTextEdit::textChanged, this, &MainWindow::updateActionStates);
    connect(editor, &QPlainTextEdit::copyAvailable, this, [this](bool b){
        actionCut->setEnabled(b);
        actionCopy->setEnabled(b);
    });
}

MainWindow::~MainWindow() {}

void MainWindow::setupEditor() {
    editor = new CodeEditor(this);
    setCentralWidget(editor);
    editor->setLineWrapMode(QPlainTextEdit::WidgetWidth);

    QFont font("Courier", 12);
    editor->setFont(font);
}

void MainWindow::createActions() {
    // 文件
    actionNew = new QAction("新建(&N)", this); actionNew->setShortcut(QKeySequence::New);
    actionOpen = new QAction("打开(&O)", this); actionOpen->setShortcut(QKeySequence::Open);
    actionSave = new QAction("保存(&S)", this); actionSave->setShortcut(QKeySequence::Save);
    actionSaveAs = new QAction("另存为(&A)", this); actionSaveAs->setShortcut(QKeySequence::SaveAs);
    actionExit = new QAction("退出(&X)", this); actionExit->setShortcut(QKeySequence::Quit);

    connect(actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(actionSaveAs, &QAction::triggered, this, &MainWindow::saveAsFile);
    connect(actionExit, &QAction::triggered, this, &QWidget::close);

    // 编辑
    actionUndo = new QAction("撤销(&U)", this); actionUndo->setShortcut(QKeySequence::Undo);
    actionRedo = new QAction("恢复(&R)", this); actionRedo->setShortcut(QKeySequence::Redo);
    actionCut = new QAction("剪切(&T)", this); actionCut->setShortcut(QKeySequence::Cut);
    actionCopy = new QAction("复制(&C)", this); actionCopy->setShortcut(QKeySequence::Copy);
    actionPaste = new QAction("粘贴(&P)", this); actionPaste->setShortcut(QKeySequence::Paste);
    actionSelectAll = new QAction("全选(&A)", this); actionSelectAll->setShortcut(QKeySequence::SelectAll);
    actionFind = new QAction("查找(&F)", this); actionFind->setShortcut(QKeySequence::Find);

    // 【新增】查找下一个
    actionFindNext = new QAction("查找下一个(&N)", this); actionFindNext->setShortcut(Qt::Key_F3);

    actionReplace = new QAction("替换(&R)", this); actionReplace->setShortcut(QKeySequence::Replace);

    connect(actionUndo, &QAction::triggered, editor, &QPlainTextEdit::undo);
    connect(actionRedo, &QAction::triggered, editor, &QPlainTextEdit::redo);
    connect(actionCut, &QAction::triggered, editor, &QPlainTextEdit::cut);
    connect(actionCopy, &QAction::triggered, editor, &QPlainTextEdit::copy);
    connect(actionPaste, &QAction::triggered, editor, &QPlainTextEdit::paste);
    connect(actionSelectAll, &QAction::triggered, editor, &QPlainTextEdit::selectAll);

    connect(actionFind, &QAction::triggered, this, &MainWindow::findText);
    connect(actionFindNext, &QAction::triggered, this, &MainWindow::findNext); // 【新增】
    connect(actionReplace, &QAction::triggered, this, &MainWindow::replaceText);

    // 格式
    actionWrap = new QAction("自动换行(&W)", this);
    actionWrap->setCheckable(true);
    actionWrap->setChecked(true);
    connect(actionWrap, &QAction::toggled, this, &MainWindow::toggleWrap);

    actionShowLine = new QAction("显示行号(&L)", this);
    actionShowLine->setCheckable(true);
    actionShowLine->setChecked(true);
    connect(actionShowLine, &QAction::toggled, this, [this](bool checked){
        editor->setLineNumberVisible(checked);
    });

    actionFont = new QAction("字体(&F)...", this);
    connect(actionFont, &QAction::triggered, this, &MainWindow::setFont);
    actionFontColor = new QAction("字体颜色...", this);
    connect(actionFontColor, &QAction::triggered, this, &MainWindow::setFontColor);
    actionFontBgColor = new QAction("字体背景色...", this);
    connect(actionFontBgColor, &QAction::triggered, this, &MainWindow::setFontBgColor);
    actionEditorBgColor = new QAction("编辑器背景色...", this);
    connect(actionEditorBgColor, &QAction::triggered, this, &MainWindow::setEditorBgColor);

    // 查看
    actionToolBar = new QAction("工具栏(&T)", this);
    actionToolBar->setCheckable(true);
    actionToolBar->setChecked(true);

    actionStatusBar = new QAction("状态栏(&S)", this);
    actionStatusBar->setCheckable(true);
    actionStatusBar->setChecked(true);
    connect(actionStatusBar, &QAction::toggled, this, &MainWindow::toggleStatusBar);

    // 帮助
    actionAbout = new QAction("关于(&A)", this);
    connect(actionAbout, &QAction::triggered, this, &MainWindow::showAbout);
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu("文件(&F)");
    fileMenu->addAction(actionNew);
    fileMenu->addAction(actionOpen);
    fileMenu->addAction(actionSave);
    fileMenu->addAction(actionSaveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(actionExit);

    editMenu = menuBar()->addMenu("编辑(&E)");
    editMenu->addAction(actionUndo);
    editMenu->addAction(actionRedo);
    editMenu->addSeparator();
    editMenu->addAction(actionCut);
    editMenu->addAction(actionCopy);
    editMenu->addAction(actionPaste);
    editMenu->addSeparator();
    editMenu->addAction(actionFind);
    editMenu->addAction(actionFindNext); // 【新增】
    editMenu->addAction(actionReplace);
    editMenu->addSeparator();
    editMenu->addAction(actionSelectAll);

    formatMenu = menuBar()->addMenu("格式(&O)");
    formatMenu->addAction(actionWrap);
    formatMenu->addAction(actionShowLine);
    formatMenu->addSeparator();
    formatMenu->addAction(actionFont);
    formatMenu->addAction(actionFontColor);
    formatMenu->addAction(actionFontBgColor);
    formatMenu->addAction(actionEditorBgColor);

    viewMenu = menuBar()->addMenu("查看(&V)");
    viewMenu->addAction(actionToolBar);
    viewMenu->addAction(actionStatusBar);

    helpMenu = menuBar()->addMenu("帮助(&H)");
    helpMenu->addAction(actionAbout);
}

void MainWindow::createToolBars() {
    mainToolBar = addToolBar("主工具栏");
    mainToolBar->setObjectName("MainToolBar");
    mainToolBar->addAction(actionNew);
    mainToolBar->addAction(actionOpen);
    mainToolBar->addAction(actionSave);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionUndo);
    mainToolBar->addAction(actionRedo);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionCut);
    mainToolBar->addAction(actionCopy);
    mainToolBar->addAction(actionPaste);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionFont);
    mainToolBar->addAction(actionWrap);
    mainToolBar->addAction(actionShowLine);
    mainToolBar->addAction(actionFindNext); // 【新增】放到工具栏最后

    viewMenu->addAction(mainToolBar->toggleViewAction());
    connect(mainToolBar->toggleViewAction(), &QAction::toggled, actionToolBar, &QAction::setChecked);
    connect(actionToolBar, &QAction::toggled, mainToolBar, &QToolBar::setVisible);
}

void MainWindow::createStatusBar() {
    statusLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusLabel);
    updateStatusBar();
}

// ================= 槽函数实现 =================

void MainWindow::newFile() {
    if (maybeSave()) {
        editor->clear();
        currentFilePath.clear();
        isUntitled = true;
        editor->document()->setModified(false);
        setWindowTitle("无标题 - 简单文本编辑器");
        lastSearchString.clear();
    }
}

void MainWindow::openFile() {
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, "打开文件", "", "文本文件 (*.txt);;所有文件 (*)");
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

bool MainWindow::saveFile() {
    if (isUntitled) {
        return saveAsFile();
    } else {
        QFile file(currentFilePath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QMessageBox::warning(this, "错误", "无法保存文件！");
            return false;
        }
        QTextStream out(&file);
        out << editor->toPlainText();
        editor->document()->setModified(false);
        updateWindowTitle();
        return true;
    }
}

bool MainWindow::saveAsFile() {
    // 加入第三个参数和第四个参数，指定默认路径和文件过滤器
    QString fileName = QFileDialog::getSaveFileName(this, "另存为", currentFilePath, "文本文件 (*.txt);;所有文件 (*)");
    if (fileName.isEmpty()) return false;

    currentFilePath = fileName;
    isUntitled = false;
    return saveFile();
}

void MainWindow::loadFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "错误", "无法打开文件！\n可能文件不存在或没有权限。");
        return;
    }
    QTextStream in(&file);
    editor->setPlainText(in.readAll());

    currentFilePath = fileName;
    isUntitled = false;
    editor->document()->setModified(false);
    updateWindowTitle();
    lastSearchString.clear();
}

void MainWindow::updateWindowTitle() {
    QString title = isUntitled ? "无标题" : QFileInfo(currentFilePath).fileName();
    if (editor->document()->isModified()) {
        title += "*";
    }
    setWindowTitle(title + " - 简单文本编辑器");
}

void MainWindow::updateStatusBar() {
    QTextCursor cursor = editor->textCursor();
    int line = cursor.blockNumber() + 1;      // 当前行号（从1开始）
    int col = cursor.columnNumber() + 1;      // 当前列号（从1开始）
    int len = editor->toPlainText().length(); // 文本总长度

    // 拼接信息并显示在状态栏右侧的 Label 中
    statusLabel->setText(QString("length: %1  lines: %2  Ln: %3  Col: %4")
                             .arg(len).arg(editor->blockCount()).arg(line).arg(col));
}

void MainWindow::updateActionStates() {
    actionUndo->setEnabled(editor->document()->isUndoAvailable());
    actionRedo->setEnabled(editor->document()->isRedoAvailable());
    actionPaste->setEnabled(editor->canPaste());
}

void MainWindow::toggleWrap(bool checked) {
    editor->setLineWrapMode(checked ? QPlainTextEdit::WidgetWidth : QPlainTextEdit::NoWrap);
}

void MainWindow::setFont() {
    // 1. 在弹窗之前，保存当前的选区（拍照留证）
    QTextCursor cursor = editor->textCursor();
    bool hasSelection = cursor.hasSelection();

    // 2. 弹出字体选择框（先不管它底层怎么闹）
    bool ok;
    QFont font = QFontDialog::getFont(&ok, editor->font(), this);

    if (ok) {
        if (hasSelection) {
            // 【情况A：只改了选中的部分】
            // 注意：在纯文本里，必须用 CharFormat 把字号和字体族一起设置进去，否则字号会漏掉
            QTextCharFormat fmt;
            fmt.setFont(font);
            fmt.setFontPointSize(font.pointSize()); // 强制写入字号

            // 把选中的部分替换成新格式
            cursor.mergeCharFormat(fmt);

        } else {
            // 【情况B：没选中，改了全部】
            // 1. 改默认字体，让以后敲的字也是这个字体
            editor->setFont(font);

            // 2. 强制遍历整个文档，把已经存在的字全改了
            cursor.select(QTextCursor::Document);

            QTextCharFormat fmt;
            fmt.setFont(font);
            fmt.setFontPointSize(font.pointSize());
            cursor.mergeCharFormat(fmt);

            // 恢复原来的光标位置
            editor->setTextCursor(cursor);
        }
    }
}

void MainWindow::setFontColor() {
    QColor color = QColorDialog::getColor(Qt::black, this, "选择字体颜色");
    if (color.isValid()) {
        QTextCharFormat fmt;
        fmt.setForeground(color);
        editor->mergeCurrentCharFormat(fmt);
    }
}

void MainWindow::setFontBgColor() {
    QColor color = QColorDialog::getColor(Qt::white, this, "选择字体背景色");
    if (color.isValid()) {
        QTextCharFormat fmt;
        fmt.setBackground(color);
        editor->mergeCurrentCharFormat(fmt);
    }
}

void MainWindow::setEditorBgColor() {
    QColor color = QColorDialog::getColor(editor->palette().color(QPalette::Base), this, "选择编辑器背景色");
    if (color.isValid()) {
        QPalette p = editor->palette();
        p.setColor(QPalette::Base, color);
        editor->setPalette(p);
    }
}

void MainWindow::toggleStatusBar(bool checked) {
    statusBar()->setVisible(checked);
}

// 【优化】查找
void MainWindow::findText() {
    bool ok;
    QString text = QInputDialog::getText(this, "查找", "查找目标：", QLineEdit::Normal, lastSearchString, &ok);

    if (ok && !text.isEmpty()) {
        lastSearchString = text;
        if (!editor->find(text)) {
            QTextCursor cursor = editor->textCursor();
            cursor.movePosition(QTextCursor::Start);
            editor->setTextCursor(cursor);
            if (!editor->find(text)) {
                QMessageBox::information(this, "提示", "未找到目标文本");
            }
        }
    }
}

// 【新增】查找下一个
void MainWindow::findNext() {
    if (lastSearchString.isEmpty()) {
        findText();
        return;
    }
    if (!editor->find(lastSearchString)) {
        QTextCursor cursor = editor->textCursor();
        cursor.movePosition(QTextCursor::Start);
        editor->setTextCursor(cursor);
        if (!editor->find(lastSearchString)) {
            QMessageBox::information(this, "提示", "未找到更多匹配项");
        }
    }
}

void MainWindow::replaceText() {
    bool ok;
    QString findStr = QInputDialog::getText(this, "替换", "查找目标：", QLineEdit::Normal, lastSearchString, &ok);
    if (!ok || findStr.isEmpty()) return;
    QString replaceStr = QInputDialog::getText(this, "替换", "替换为：", QLineEdit::Normal, "", &ok);
    if (!ok) return;

    QTextCursor cursor = editor->textCursor();
    cursor.movePosition(QTextCursor::Start);
    editor->setTextCursor(cursor);

    cursor.beginEditBlock();
    int count = 0;
    while (editor->find(findStr)) {
        QTextCursor currentCursor = editor->textCursor();
        currentCursor.insertText(replaceStr);
        count++;
    }
    cursor.endEditBlock();

    if (count == 0) {
        QMessageBox::information(this, "提示", "未找到目标文本");
    } else {
        QMessageBox::information(this, "提示", QString("成功替换了 %1 处文本").arg(count));
    }
}

void MainWindow::showAbout() {
    QMessageBox::about(this, "关于",
                       "大牛文本编辑器\n\n"
                       "版本：V1.0\n"
                       "开发者：俞基杰\n"
                       "学号：2024414300122\n"
                       "email: 3506537232@qq.com");
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}

bool MainWindow::maybeSave() {
    if (editor->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, "提示", "文档已被修改。\n是否保存修改？",
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) return saveFile();
        else if (ret == QMessageBox::Cancel) return false;
    }
    return true;
}