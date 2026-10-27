#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
#include <QLabel>
#include <QAction>
#include <QTextBlock>

// 自定义带行号的文本框
class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    explicit CodeEditor(QWidget *parent = nullptr);
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    // 提供给主窗口调用的接口，用于显示/隐藏行号
    void setLineNumberVisible(bool visible) {
        setViewportMargins(visible ? lineNumberAreaWidth() : 0, 0, 0, 0);
    }

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
};

// 自定义行号区域
class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}
    QSize sizeHint() const override { return QSize(codeEditor->lineNumberAreaWidth(), 0); }
protected:
    void paintEvent(QPaintEvent *event) override { codeEditor->lineNumberAreaPaintEvent(event); }
private:
    CodeEditor *codeEditor;
};

// 主窗口类
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    // 文件操作
    void newFile();
    void openFile();
    bool saveFile();
    bool saveAsFile();

    // 编辑操作
    void findText();
    void findNext(); // 【新增】查找下一个
    void replaceText();

    // 格式操作
    void setFont();
    void setFontColor();
    void setFontBgColor();
    void setEditorBgColor();
    void toggleWrap(bool checked);

    // 查看操作
    void toggleStatusBar(bool checked);

    // 帮助及其他
    void showAbout();
    void updateStatusBar();
    void updateWindowTitle();
    void updateActionStates();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void setupEditor();
    bool maybeSave();
    void loadFile(const QString &fileName);

    CodeEditor *editor;
    QLabel *statusLabel;

    // 菜单和工具栏
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *formatMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *mainToolBar;

    // Actions
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSaveAs;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionRedo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSelectAll;
    QAction *actionFind;
    QAction *actionFindNext; // 【新增】
    QAction *actionReplace;
    QAction *actionWrap;
    QAction *actionShowLine;
    QAction *actionFont;
    QAction *actionFontColor;
    QAction *actionFontBgColor;
    QAction *actionEditorBgColor;
    QAction *actionToolBar;
    QAction *actionStatusBar;
    QAction *actionAbout;

    QString currentFilePath;
    bool isUntitled;
    QString lastSearchString; // 【新增】记录上次搜索内容
};

#endif // MAINWINDOW_H