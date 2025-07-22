#ifndef MAINWINDOW_H
#define MAINWINDOW_H


// 包含必要的QT头文件
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPolygonItem>
#include <QPointF>
#include <QVector>
#include <QPolygonF>
#include <QtWidgets>
#include <QtMath>


class coord {
public:
    int r;
    int s;
    int t;

    // Default constructor
    coord() : r(0), s(0), t(0) {}
    // Parameterized constructor
    coord(int rValue, int sValue, int tValue) : r(rValue), s(sValue), t(tValue) {}

    // Operator== overload
    bool operator==(const coord& other) const {
        return r == other.r && s == other.s && t == other.t;
    }
};

class rulesDialog : public QDialog
{
    Q_OBJECT

public:
    rulesDialog(QWidget* parent = nullptr) : QDialog(parent)
    {


        this->setWindowTitle("Game Rule");
        // 设置窗口为模态，防止用户点击其他窗口
        this->setModal(true);
        QVBoxLayout *layout = new QVBoxLayout(this);

        QTextEdit *rulesTextEdit = new QTextEdit(this);
        rulesTextEdit->setPlainText("<游戏目标>\n玩家轮流移动棋子，将自己颜色的所有棋子从起始角移动到正对面的角，最先完成这个目标的玩家获胜。\n\n<游戏人数>\n支持2~6人同时游戏。\n玩家可以选择自己喜欢颜色的棋子，之后只需在自己选择的颜色的回合移动棋子，没有被选择的棋子颜色的回合直接跳过即可。\n\n<走棋规则>\n每回合每位玩家只能移动至多一个棋子。\n移动:棋子可以沿着与其相邻的六个方向中的任意一个方向移动一格到相邻的空格。\n跳跃:如果相邻的位置上有任何颜色的棋子，且该棋子直线方向的下一个位置为空，则可以跳到该空位上.\n连续跳跃:在一次跳跃后，如果当前依然满足跳跃条件，可以继续跳跃，跳跃次数没有限制。\n\n<按钮说明>\n[New Game]:重新的一局，会更新棋盘。\n[Game Rule]:打开游戏规则页面，即当前的界面。\n[End Round]:结束这一回合，在一名玩家完成一回合的棋子移动后点击。\n\n<界面信息>\n游戏界面会显示当前是哪位玩家的回合，在一名玩家点击[End Round]后会转移到下一名玩家的回合。\n只能在界面上选中当前回合对应的玩家的棋子，选中的棋子会被红色边框圈出。\n\n<道德风尚>\n游戏过程中，请不要对其他玩家恶语相向。\n请不要因为输掉游戏而毁坏设备，或者殴打其他玩家。\n");
        rulesTextEdit->setReadOnly(true); // 设置为只读
        rulesTextEdit->setFocusPolicy(Qt::NoFocus);
        layout->addWidget(rulesTextEdit); // 将文本编辑框添加到布局中

        QPushButton *okButton = new QPushButton("OK", this);
        connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
        layout->addWidget(okButton); // 将OK按钮添加到布局中

        // 设置布局到对话框
        this->setLayout(layout);

        // 设置对话框的初始大小
        this->resize(500, 600);

    }
};

class Checkerboard : public QWidget {
public:
    Checkerboard(QWidget *parent = nullptr) : QWidget(parent) {
        // 设置窗口标题
        setWindowTitle(tr("Chinese Checkers"));
        boardSize = 750;
        setFixedSize(2*boardSize, boardSize);

        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 4; j++ ){
                for(int k = 0; k < 5; k++){

                    if( j+k >= 4){
                        state[i][j][k]=i;
                    }else{
                        state[i][j][k]=-1;
                    }

                }

            }
        }

        QPushButton *refleshButton = new QPushButton("New Game", this);
        QPushButton *ruleButton = new QPushButton("Game Rule", this);
        QPushButton *nextButton = new QPushButton("End Round", this);
        QPushButton *turnButton = new QPushButton("Current Turn", this);

        refleshButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        ruleButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        nextButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        turnButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        refleshButton->setFixedSize(200, 30);
        ruleButton->setFixedSize(200, 30);
        nextButton->setFixedSize(200, 30);
        turnButton->setFixedSize(200, 30);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setSpacing(1);
        layout->setContentsMargins(10, 10, 10, 10);



        layout->addWidget(refleshButton);
        layout->addWidget(ruleButton);
        layout->addWidget(nextButton);
        layout->addStretch();
        layout->addWidget(turnButton);
        layout->addStretch();

        connect(refleshButton, &QPushButton::clicked, this, &Checkerboard::reflesh);
        connect(nextButton, &QPushButton::clicked, this, &Checkerboard::nextround);
        connect(ruleButton, &QPushButton::clicked,this,&Checkerboard::showrule);

        turn=0;
        centrestate=-1;
        present=coord(6,6,6);
        jump=false;
        movement=false;

    }

protected:
    void paintEvent(QPaintEvent *event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        // 绘制棋盘
        drawCheckerboard(painter);

    }

    void drawCheckerboard(QPainter &painter) {

        double centerX = boardSize;
        double centerY = boardSize / 2;
        double radius = 3*boardSize / 50;

        painter.setPen(Qt::black); // 绘制黑色边框
        QPointF turing = QPointF(centerX/3,9*centerY/8);
        switch (turn) {
        case 0:
            painter.setBrush(Qt::red);
            break; // 结束switch语句
        case 1:
            painter.setBrush(Qt::green);
            break; // 结束switch语句
        case 2:
            painter.setBrush(Qt::blue);
            break; // 结束switch语句
        case 3:
            painter.setBrush(Qt::yellow);
            break; // 结束switch语句
        case 4:
            painter.setBrush(Qt::cyan);
            break; // 结束switch语句
        case 5:
            painter.setBrush(Qt::gray);
            break; // 结束switch语句
        default:
            painter.setBrush(Qt::white);
        }

        painter.drawEllipse(turing, radius/3, radius/3);

        centre = QPointF(centerX,centerY);
        switch (centrestate) {
        case 0:
            painter.setBrush(Qt::red);
            break; // 结束switch语句
        case 1:
            painter.setBrush(Qt::green);
            break; // 结束switch语句
        case 2:
            painter.setBrush(Qt::blue);
            break; // 结束switch语句
        case 3:
            painter.setBrush(Qt::yellow);
            break; // 结束switch语句
        case 4:
            painter.setBrush(Qt::cyan);
            break; // 结束switch语句
        case 5:
            painter.setBrush(Qt::gray);
            break; // 结束switch语句
        default:
            painter.setBrush(Qt::white);
        }
        if(present==coord(-1,-1,-1)){
            painter.setPen(Qt::magenta); // 绘制黑色边框
        }else{
            painter.setPen(Qt::black); // 绘制黑色边框
        }
        painter.drawEllipse(centre, radius/3, radius/3);

        for (int i = 0; i < 6; ++i) {
            double angle1 = M_PI / 3 * i;
            double angle2 = M_PI / 3 * (i+1);

            for (int j = 0; j < 4; j++ ){
                for(int k = 0; k < 5; k++){

                    double x=centerX+(j+1)*radius*qCos(angle1)+k*radius*qCos(angle2);
                    double y=centerY+(j+1)*radius*qSin(angle1)+k*radius*qSin(angle2);
                    position[i][j][k]= QPointF(x,y);

                    switch (state[i][j][k]) {

                    case 0:
                        painter.setBrush(Qt::red);
                        break; // 结束switch语句
                    case 1:
                        painter.setBrush(Qt::green);
                        break; // 结束switch语句
                    case 2:
                        painter.setBrush(Qt::blue);
                        break; // 结束switch语句
                    case 3:
                        painter.setBrush(Qt::yellow);
                        break; // 结束switch语句
                    case 4:
                        painter.setBrush(Qt::cyan);
                        break; // 结束switch语句
                    case 5:
                        painter.setBrush(Qt::gray);
                        break; // 结束switch语句
                    default:
                        painter.setBrush(Qt::white);
                    }
                    if(present==coord(i,j,k)){
                        painter.setPen(Qt::magenta); // 绘制黑色边框
                    }else{
                        painter.setPen(Qt::black); // 绘制黑色边框
                    }
                    painter.drawEllipse(position[i][j][k], radius/3, radius/3);
                }

            }

        }
    }


    coord fetch(QPointF point){

        double centerX = boardSize;
        double centerY = boardSize / 2;
        double radius = 3 * boardSize / 50;

        for (int i = 0; i < 6; ++i) {

            double angle1 = M_PI / 3 * i;
            double angle2 = M_PI / 3 * (i+1);

            for (int j = 0; j < 4; j++ ){
                for(int k = 0; k < 5; k++){

                    double x=centerX+(j+1)*radius*qCos(angle1)+k*radius*qCos(angle2);
                    double y=centerY+(j+1)*radius*qSin(angle1)+k*radius*qSin(angle2);
                    position[i][j][k]= QPointF(x,y);
                    if ((QPointF(x,y) - point).manhattanLength() < 30) {
                        return coord(i,j,k);
                        break;
                    }


                }

            }
        }
        if ((QPointF(centerX,centerY) - point).manhattanLength() < 30) {
            return coord(-1,-1,-1);
        }
        return coord(6,6,6);
    }


    bool isValidMove(coord present, coord now)  {

        QPointF p;
        QPointF n;
        if((movement==true)&&(jump==false)){
            return false;
        }

        if(present==coord(-1,-1,-1)){
            p=centre;
        }else{
            p=position[present.r][present.s][present.t];
        }
        if(now==coord(-1,-1,-1)){
            n=centre;
        }else{
            n=position[now.r][now.s][now.t];
        }

        double distance=(p.x()-n.x())*(p.x()-n.x())+(p.y()-n.y())*(p.y()-n.y());

        if(distance<=3000){
            if((movement==true)&&(jump==true)){
                return false;
            }
            return true;
        }else if(distance>=9000){
            return false;
        }else if(distance>=7000){
            coord mid=fetch(QPointF((p.x()+n.x())/2,(p.y()+ n.y())/2));
            if(state[mid.r][mid.s][mid.t]!=-1){
                jump=true;
                return true;
            }else{
                return false;
            }
        }else{
            return false;
        }
        return false;
    }

    void mousePressEvent(QMouseEvent* event) override
    {
        if(present==coord(6,6,6)){
            if(fetch(event->pos())==coord(-1,-1,-1)){
                if(centrestate==turn){
                    present=coord(-1,-1,-1);
                }
            }else if(fetch(event->pos())==coord(6,6,6)){
                return;
            }else{
                coord now=fetch(event->pos());
                if(state[now.r][now.s][now.t]==turn){
                    present=fetch(event->pos());
                }
            }
        }else if(present==coord(-1,-1,-1)){
            if(fetch(event->pos())==coord(-1,-1,-1)){
                return;
            }else if(fetch(event->pos())==coord(6,6,6)){
                return;
            }else{
                coord now=fetch(event->pos());
                if(state[now.r][now.s][now.t]==turn){
                    if(movement==false){
                        present=fetch(event->pos());
                    }
                }else if(state[now.r][now.s][now.t]==-1){
                    if(isValidMove(present,now)){
                        centrestate=-1;
                        present=fetch(event->pos());
                        state[present.r][present.s][present.t]=turn;
                        movement=true;
                    }else{
                        return;
                    }

                }else{
                    return;
                }
            }
        }else{
            if(fetch(event->pos())==coord(-1,-1,-1)){
                if(centrestate==turn){
                    if(movement==false){
                        present=fetch(event->pos());
                    }
                }else if(centrestate==-1){
                    if(isValidMove(present,coord(-1,-1,-1))){
                        state[present.r][present.s][present.t]=-1;
                        present=coord(-1,-1,-1);
                        centrestate=turn;
                        movement=true;
                    }else{
                        return;
                    }

                }else{
                    return;
                }
            }else if(fetch(event->pos())==coord(6,6,6)){
                return;
            }else{
                coord now=fetch(event->pos());
                if(state[now.r][now.s][now.t]==turn){
                    if(movement==false){
                        present=fetch(event->pos());
                    }
                }else if(state[now.r][now.s][now.t]==-1){
                    if(isValidMove(present,now)){
                        state[present.r][present.s][present.t]=-1;
                        present=fetch(event->pos());
                        state[present.r][present.s][present.t]=turn;
                        movement=true;
                    }else{
                        return;
                    }

                }else{
                    return;
                }
            }
        }

        update();
        return;
    }


    void reflesh(){
        for (int i = 0; i < 6; ++i) {
            for (int j = 0; j < 4; j++ ){
                for(int k = 0; k < 5; k++){

                    if( j+k >= 4){
                        state[i][j][k]=i;
                    }else{
                        state[i][j][k]=-1;
                    }

                }

            }
        }
        turn=0;
        centrestate=-1;
        present=coord(6,6,6);
        jump=false;
        movement=false;
        update();
        return;
    }

    void nextround(){
        turn++;
        if(turn==6){
            turn=0;
        }
        present=coord(6,6,6);
        jump=false;
        movement=false;
        update();
        return;
    }

    void showrule(){
        rulesDialog dialog(this);
        if(dialog.exec()== QDialog::Accepted){
            return;
        }
    }


private:
    int boardSize; // 棋盘大小

    coord present;
    int turn;
    bool jump;
    bool movement;

    QPointF centre;
    QPointF position[6][4][5];
    int centrestate;
    int state[6][4][5];
    };

#endif // MAINWINDOW_H

