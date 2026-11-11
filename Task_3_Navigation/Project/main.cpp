#include <QtWidgets>
#include <cmath>
#include "navigation.h"
#ifdef AUTOPILOT
#include "autopilot.h"
#endif
class Scene:public QWidget{public:int stage=0;QString active="GPS";double altitude=150;bool available=true;Scene(){setMinimumHeight(300);}void paintEvent(QPaintEvent*)override{QPainter p(this);p.setRenderHint(QPainter::Antialiasing);p.fillRect(rect(),QColor("#101e32"));
QRectF area=rect().adjusted(30,30,-30,-30);p.setPen(QPen(QColor("#243650"),1));for(int i=0;i<=10;++i){double x=area.left()+area.width()*i/10;p.drawLine(QPointF(x,area.top()),QPointF(x,area.bottom()));double y=area.top()+area.height()*i/10;p.drawLine(QPointF(area.left(),y),QPointF(area.right(),y));}
#ifdef AUTOPILOT
double base=area.bottom()-20,top=area.top()+30;double target=base-(base-top)*200/260.;double current=base-(base-top)*std::clamp(altitude,0.,260.)/260.;p.setPen(QPen(QColor("#31d2c0"),2,Qt::DashLine));p.drawLine(QPointF(area.left(),target),QPointF(area.right(),target));p.drawText(QPointF(area.left()+10,target-10),"ЦЕЛЕВАЯ ВЫСОТА 200 м");
p.setPen(QPen(QColor("#6ca8ff"),4));QPointF drone(area.center().x(),current);p.drawLine(drone+QPointF(-28,-8),drone+QPointF(28,8));p.drawLine(drone+QPointF(-28,8),drone+QPointF(28,-8));for(auto d:{QPointF(-28,-8),QPointF(28,8),QPointF(-28,8),QPointF(28,-8)})p.drawEllipse(drone+d,10,5);p.drawText(drone+QPointF(45,4),QString::number(altitude,'f',1)+" м");p.setPen(QPen(available?QColor("#31d2c0"):QColor("#ff6f83"),2));p.drawLine(drone+QPointF(0,-15),QPointF(drone.x(),target));p.drawText(QPointF(area.left()+10,base),"СХЕМА ОШИБКИ ПО ВЫСОТЕ · НЕ СИМУЛЯЦИЯ ПОЛЁТА");
#else
QStringList names{"GPS","INS","Visual"};for(int i=0;i<3;++i){QPointF c(area.left()+area.width()*(i+.5)/3,area.center().y());bool selected=available&&active==names[i];p.setBrush(selected?QColor("#124a50"):QColor("#172a43"));p.setPen(QPen(selected?QColor("#31d2c0"):QColor("#456182"),selected?3:1));p.drawEllipse(c,62,62);p.setPen(Qt::white);QFont f=p.font();f.setPointSize(17);f.setBold(true);p.setFont(f);p.drawText(QRectF(c-QPointF(65,22),QSizeF(130,40)),Qt::AlignCenter,names[i]);f.setPointSize(9);f.setBold(false);p.setFont(f);p.drawText(QRectF(c+QPointF(-70,30),QSizeF(140,25)),Qt::AlignCenter,selected?"АКТИВНЫЙ ИСТОЧНИК":"ПРИОРИТЕТ "+QString::number(i+1));if(i<2){p.setPen(QPen(QColor("#5c7c9e"),2));p.drawLine(c+QPointF(73,0),c+QPointF(area.width()/3-73,0));}}
p.setPen(QColor("#86a1c3"));p.drawText(QPointF(area.left(),area.top()+15),"АВТОМАТИЧЕСКИЙ ВЫБОР ПЕРВОГО ДОСТУПНОГО ИСТОЧНИКА");p.drawText(QPointF(area.left(),area.bottom()-10),available?"Потеря GPS → INS → Visual. Восстановление GPS возвращает высший приоритет.":"КРИТИЧЕСКИЙ ОТКАЗ: НИ ОДИН ИСТОЧНИК НЕ ДОСТУПЕН");
#endif
}};
int main(int argc,char**argv){QApplication app(argc,argv);app.setStyle("Fusion");QWidget window;window.resize(1180,780);
window.setStyleSheet("QWidget{background:#0b1423;color:#dce9fc;font-family:'Segoe UI';font-size:10pt;}QLabel#title{font-size:27pt;font-weight:700;}QLabel#muted{color:#829bbc;}QFrame#card{background:#132238;border:1px solid #263c59;border-radius:10px;}QPushButton{background:#1b3554;border:1px solid #34577d;border-radius:6px;padding:10px;}QPushButton:hover{background:#275077;}QPushButton#primary{background:#087f83;border-color:#20b4b5;}QPlainTextEdit{background:#08101c;border:1px solid #263c59;border-radius:8px;color:#a1c5df;font-family:Consolas;font-size:9pt;}QDoubleSpinBox,QSpinBox{padding:6px;border:1px solid #355171;border-radius:5px;background:#15283e;}QCheckBox{padding:8px;}QCheckBox::indicator{width:18px;height:18px;}");
auto* root=new QVBoxLayout(&window);root->setContentsMargins(24,20,24,20);root->setSpacing(12);
auto* eyebrow=new QLabel("КОНТРОЛЬНАЯ 02 / ВАРИАНТ 2 / ПОЛИМОРФИЗМ");eyebrow->setObjectName("muted");root->addWidget(eyebrow);
#ifdef AUTOPILOT
QString title="Autopilot / контроль цикла";QString subtitle="Интеграция модулей · предполётные проверки · управление высотой";
#else
QString title="Navigation / резервирование";QString subtitle="Задание 3 · GPS → INS → Visual · автоматическое переключение";
#endif
window.setWindowTitle(title);auto* titleLabel=new QLabel(title);titleLabel->setObjectName("title");root->addWidget(titleLabel);auto* sub=new QLabel(subtitle);sub->setObjectName("muted");root->addWidget(sub);
auto* status=new QLabel;status->setStyleSheet("font-size:18pt;color:#3cdec9;padding:12px;background:#132238;border-radius:8px;");root->addWidget(status);
auto* body=new QHBoxLayout;root->addLayout(body,1);auto* controls=new QFrame;controls->setObjectName("card");controls->setFixedWidth(300);auto* form=new QVBoxLayout(controls);body->addWidget(controls);auto* scene=new Scene;body->addWidget(scene,1);
GPSNavigation gps;InertialNavigation ins;VisualNavigation visual;NavigationManager manager;manager.set_sources({&gps,&ins,&visual});
auto* gpsReady=new QCheckBox("GPS готов");gpsReady->setChecked(true);auto* insReady=new QCheckBox("INS готов");insReady->setChecked(true);auto* visualReady=new QCheckBox("Visual: цель отслеживается");visualReady->setChecked(true);
auto* satellites=new QSpinBox;satellites->setRange(0,40);satellites->setValue(10);auto* altitude=new QDoubleSpinBox;altitude->setRange(-1000,10000);altitude->setValue(150);altitude->setSuffix(" м");
auto* vehicleReady=new QCheckBox("БПЛА подключён");vehicleReady->setChecked(true);auto* sensorReady=new QCheckBox("Сенсор готов");sensorReady->setChecked(true);auto* controllerReady=new QCheckBox("Регулятор подключён");controllerReady->setChecked(true);
#ifdef AUTOPILOT
form->addWidget(vehicleReady);form->addWidget(sensorReady);form->addWidget(gpsReady);form->addWidget(controllerReady);form->addWidget(new QLabel("Фактическая высота"));form->addWidget(altitude);
#else
form->addWidget(gpsReady);form->addWidget(insReady);form->addWidget(visualReady);form->addWidget(new QLabel("Спутники GPS (минимум 4)"));form->addWidget(satellites);form->addWidget(new QLabel("Высота GPS"));form->addWidget(altitude);
#endif
auto* run=new QPushButton("Выполнить проверку");run->setObjectName("primary");form->addWidget(run);auto* fail=new QPushButton("Имитировать отказ GPS");form->addWidget(fail);auto* reset=new QPushButton("Штатный сценарий");form->addWidget(reset);form->addStretch();
auto* note=new QLabel("Данные учебные. Изменения доступны вручную; реальная аппаратура не подключена.");note->setWordWrap(true);note->setObjectName("muted");form->addWidget(note);
auto* log=new QPlainTextEdit;log->setReadOnly(true);log->setMaximumBlockCount(500);log->setMinimumHeight(135);log->setMaximumHeight(160);root->addWidget(log);
auto* footer=new QLabel;footer->setObjectName("muted");root->addWidget(footer);
auto refresh=[&]{gps.set_valid(gpsReady->isChecked());gps.set_satellites(satellites->value());gps.set_position({55.75,37.61,altitude->value()});ins.set_valid(insReady->isChecked());visual.set_valid(visualReady->isChecked());
#ifdef AUTOPILOT
Quadcopter vehicle;GPS sensor;SimpleController controller;sensor.set_ready(sensorReady->isChecked());Autopilot autopilot(vehicleReady->isChecked()?&vehicle:nullptr,{&sensor},&gps,controllerReady->isChecked()?&controller:nullptr);autopilot.run_cycle();scene->available=autopilot.completed();scene->altitude=altitude->value();status->setText(autopilot.completed()?QString("ЦИКЛ ЗАВЕРШЁН  /  Ошибка: %1 м  /  Управление: %2").arg(200-altitude->value()).arg(autopilot.control_output()):"ЦИКЛ ПРЕРВАН  /  Проверьте журнал");log->appendPlainText(autopilot.result()+"\n");footer->setText("Цель 200 м · dt = 0,1 с · SimpleController: output = 0,5 × error · Каждый запуск — один цикл");
#else
manager.print_status();auto* active=manager.active();scene->available=active;scene->active=active?active->system_name():"—";status->setText(active?"АКТИВНАЯ НАВИГАЦИЯ: "+active->system_name():"КРИТИЧЕСКАЯ ОШИБКА / Навигация недоступна");QString message=QString("GPS: %1 | INS: %2 | Visual: %3\n").arg(gps.is_valid()?"valid":"invalid",ins.is_valid()?"valid":"invalid",visual.is_valid()?"valid":"invalid");if(active){auto p=manager.get_position();message+=QString("Active navigation system: %1\nPosition: %2, %3, %4 m").arg(active->system_name()).arg(p.latitude,0,'f',5).arg(p.longitude,0,'f',5).arg(p.altitude);}else message+="Critical error: no navigation system available";log->appendPlainText(message+"\n");footer->setText("GPS: 55,75000 / 37,61000 · INS: 55,75010 / 37,61020 · Visual: 55,74990 / 37,60980 · координаты учебные");
#endif
status->setStyleSheet(QString("font-size:18pt;padding:12px;background:#132238;border-radius:8px;color:%1;").arg(scene->available?"#3cdec9":"#ff7183"));scene->update();};
QObject::connect(run,&QPushButton::clicked,&window,refresh);QObject::connect(fail,&QPushButton::clicked,&window,[&]{gpsReady->setChecked(false);refresh();});QObject::connect(reset,&QPushButton::clicked,&window,[&]{gpsReady->setChecked(true);insReady->setChecked(true);visualReady->setChecked(true);vehicleReady->setChecked(true);sensorReady->setChecked(true);controllerReady->setChecked(true);satellites->setValue(10);altitude->setValue(150);refresh();});
#ifndef AUTOPILOT
for(auto* box:{gpsReady,insReady,visualReady})QObject::connect(box,&QCheckBox::toggled,&window,refresh);QObject::connect(satellites,qOverload<int>(&QSpinBox::valueChanged),&window,refresh);QObject::connect(altitude,qOverload<double>(&QDoubleSpinBox::valueChanged),&window,refresh);
#endif
refresh();window.show();auto args=app.arguments();int shot=args.indexOf("--screenshot");if(shot>=0&&shot+1<args.size())QTimer::singleShot(350,&window,[&]{app.exit(window.grab().save(args[shot+1])?0:2);});return app.exec();}
