#include "uav.h"
#include <QtWidgets>
#include <cmath>
#include <memory>
#include <array>
class Diagram : public QWidget {
public:
 int motors=4; bool flying=false; double phase=0;
 explicit Diagram(QWidget *p=nullptr):QWidget(p){setMinimumSize(480,400); auto*t=new QTimer(this);connect(t,&QTimer::timeout,this,[this]{phase+=flying?18:0.25;update();});t->start(30);}
protected:
 void paintEvent(QPaintEvent*) override {
  QPainter p(this);p.setRenderHint(QPainter::Antialiasing);
  p.fillRect(rect(),QColor("#0b1728"));
  p.setPen(QColor("#14273b"));for(int x=0;x<width();x+=32)p.drawLine(x,0,x,height());for(int y=0;y<height();y+=32)p.drawLine(0,y,width(),y);
  QPointF c(width()/2.,height()/2.);p.translate(c);
  p.setPen(QPen(QColor("#213c50"),1,Qt::DashLine));p.drawEllipse(QPointF(),165,165);p.drawEllipse(QPointF(),100,100);
  p.setPen(QColor("#688398"));p.drawText(QRectF(-150,-195,300,25),Qt::AlignCenter,"ВИД СВЕРХУ  /  "+QString::number(motors)+" ДВИГАТЕЛЕЙ");
  if(flying)p.scale(1.06+0.015*std::sin(phase/25),1.06+0.015*std::sin(phase/25));
  for(int i=0;i<motors;++i){double a=(i*360./motors-45)*3.141592653589793/180;QPointF r(120*std::cos(a),120*std::sin(a));
   p.setPen(QPen(QColor("#25465a"),16,Qt::SolidLine,Qt::RoundCap));p.drawLine(QPointF(),r);p.setPen(QPen(QColor("#3e7184"),2));p.drawLine(QPointF(),r);
   p.setPen(QPen(QColor(flying?"#54e8d2":"#3d7187"),2));p.setBrush(QColor("#102c3c"));p.drawEllipse(r,38,38);
   p.save();p.translate(r);p.rotate(phase*(i%2?-1:1));p.setPen(Qt::NoPen);p.setBrush(QColor(flying?"#53dac9":"#6894a7"));p.drawRoundedRect(QRectF(-31,-5,62,10),5,5);p.drawRoundedRect(QRectF(-5,-31,10,62),5,5);p.restore();
   p.setBrush(QColor("#e2f6ff"));p.drawEllipse(r,5,5);
  }
  p.setPen(QPen(QColor("#60e4d2"),2));p.setBrush(QColor("#173e50"));p.drawRoundedRect(QRectF(-37,-37,74,74),19,19);p.setPen(QColor("#defdf7"));p.setFont(QFont("Segoe UI",16,QFont::Bold));p.drawText(QRectF(-35,-30,70,60),Qt::AlignCenter,"UAV");
  p.setFont(QFont("Segoe UI",10));p.setPen(QColor(flying?"#65e7cd":"#8ca6b9"));p.drawText(QRectF(-180,178,360,25),Qt::AlignCenter,flying?"●  РОТОРЫ АКТИВНЫ  •  В ПОЛЁТЕ":"●  ГОТОВ К ВЗЛЁТУ  •  НА ЗЕМЛЕ");
 }
};
class Window : public QWidget {
 std::array<std::unique_ptr<UAV>,3> fleet;
 QComboBox *select; QLabel *state,*count,*mode; QPushButton *up,*down; QTextEdit *journal; Diagram *diagram;
 UAV* current()const{return fleet.at(select->currentIndex()).get();}
 void log(const QString&s){journal->append("<span style='color:#678399'>"+QTime::currentTime().toString("HH:mm:ss")+"</span>  "+s.toHtmlEscaped());}
 void refresh(){auto*v=current();if(!v)return;diagram->motors=v->motor_count();diagram->flying=v->isFlying();count->setText(QString::number(v->motor_count())+(v->motor_count()==4?"  двигателя":"  двигателей"));state->setText(v->isFlying()?"В ПОЛЁТЕ":"НА ЗЕМЛЕ");mode->setText(v->takeoff_type()+"\n"+v->landing_type());up->setEnabled(!v->isFlying());down->setEnabled(v->isFlying());diagram->update();}
 void command(bool takeoff){auto*v=current();if(!v){log("Ошибка: аппарат не выбран");return;}try{if(takeoff)v->takeoff();else v->land();log(v->type()+": "+(takeoff?v->takeoff_type():v->landing_type()));}catch(const std::exception&e){log(QString("Ошибка: ")+e.what());}refresh();}
public:
 Window(){fleet[0]=std::make_unique<Quadcopter>();fleet[1]=std::make_unique<Hexacopter>();fleet[2]=std::make_unique<Octocopter>();setWindowTitle("UAV · Полиморфизм | Контрольная работа 2");resize(1100,770);
 setStyleSheet("QWidget{background:#081220;color:#dfebf5;font-family:'Segoe UI';font-size:14px;} QLabel{background:transparent;} QLabel#title{font-size:30px;font-weight:700;} QLabel#muted{color:#7f9aaf;} QComboBox{background:#14283c;border:1px solid #28455d;border-radius:8px;padding:12px;} QPushButton{background:#19364c;border:1px solid #35546b;border-radius:8px;padding:14px;font-weight:600;} QPushButton:hover{background:#244b61;} QPushButton#primary{background:#57dfca;color:#082b30;border:0;} QPushButton:disabled{background:#102031;color:#4b6377;border-color:#1e3041;} QTextEdit{background:#0b1728;border:1px solid #22374c;border-radius:8px;padding:10px;font-family:'Consolas';font-size:13px;} QFrame#card{background:#0e1c2d;border:1px solid #21364b;border-radius:12px;} ");
 auto*root=new QVBoxLayout(this);root->setContentsMargins(30,24,30,24);root->setSpacing(16);
 auto*eyebrow=new QLabel("КОНТРОЛЬНАЯ РАБОТА 02  /  ВАРИАНТ 2  /  ЗАДАНИЕ 1");eyebrow->setObjectName("muted");root->addWidget(eyebrow);
 auto*title=new QLabel("Пульт управления БПЛА");title->setObjectName("title");root->addWidget(title);auto*sub=new QLabel("Один интерфейс UAV — три реализации поведения. Выберите аппарат и выполните команду.");sub->setObjectName("muted");root->addWidget(sub);
 auto*middle=new QHBoxLayout;middle->setSpacing(22);root->addLayout(middle,1);diagram=new Diagram;middle->addWidget(diagram,3);
 auto*card=new QFrame;card->setObjectName("card");auto*side=new QVBoxLayout(card);side->setContentsMargins(22,22,22,22);side->setSpacing(17);middle->addWidget(card,2);
 side->addWidget(new QLabel("АКТИВНЫЙ АППАРАТ"));select=new QComboBox;select->setObjectName("vehicleSelector");select->addItems({"Квадрокоптер / 4 ротора","Гексакоптер / 6 роторов","Октокоптер / 8 роторов"});side->addWidget(select);
 count=new QLabel;count->setStyleSheet("font-size:27px;font-weight:600;color:#64e4cf;");side->addWidget(count);state=new QLabel;side->addWidget(state);mode=new QLabel;mode->setWordWrap(true);mode->setStyleSheet("color:#8ba7bb;line-height:150%;");side->addWidget(mode);side->addStretch();
 up=new QPushButton("↑  Взлёт");up->setObjectName("primary");down=new QPushButton("↓  Посадка");down->setObjectName("landButton");side->addWidget(up);side->addWidget(down);auto*info=new QPushButton("Сведения об аппарате");side->addWidget(info);
 auto*jl=new QLabel("ЖУРНАЛ КОМАНД");jl->setObjectName("muted");root->addWidget(jl);journal=new QTextEdit;journal->setReadOnly(true);journal->setFixedHeight(125);root->addWidget(journal);auto*footer=new QLabel("UAV& → virtual → override     •     Состояние каждого аппарата сохраняется при переключении");footer->setObjectName("muted");root->addWidget(footer);
 connect(select,&QComboBox::currentIndexChanged,this,[this]{refresh();log("Выбран: "+current()->type());});connect(up,&QPushButton::clicked,this,[this]{command(true);});connect(down,&QPushButton::clicked,this,[this]{command(false);});connect(info,&QPushButton::clicked,this,[this]{auto*v=current();if(!v)return;print_vehicle_info(*v);log(v->type()+" | Двигателей: "+QString::number(v->motor_count())+" | "+v->takeoff_type()+" | "+v->landing_type());});refresh();log("Система готова. Квадрокоптер выбран.");
 }
 bool smoke(){for(int i=0;i<3;++i){select->setCurrentIndex(i);if(current()->motor_count()!=4+2*i)return false;up->click();if(!current()->isFlying()||up->isEnabled()||!down->isEnabled())return false;down->click();if(current()->isFlying()||!up->isEnabled()||down->isEnabled())return false;}select->setCurrentIndex(0);return true;}
};
int main(int argc,char**argv){QApplication a(argc,argv);Window w;w.show();const auto args=a.arguments();if(args.contains("--smoke-test")){QTimer::singleShot(100,&a,[&]{a.exit(w.smoke()?0:1);});}int i=args.indexOf("--screenshot");if(i>=0&&i+1<args.size())QTimer::singleShot(400,&a,[&]{a.exit(w.grab().save(args[i+1])?0:2);});return a.exec();}

