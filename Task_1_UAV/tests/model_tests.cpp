#include "../src/uav.h"
#include <QtTest>
#include <memory>
#include <type_traits>
class Tests : public QObject {
 Q_OBJECT
private slots:
 void polymorphism() {
  static_assert(std::is_abstract_v<UAV>);
  static_assert(std::has_virtual_destructor_v<UAV>);
  std::vector<std::unique_ptr<UAV>> fleet;
  fleet.push_back(std::make_unique<Quadcopter>());
  fleet.push_back(std::make_unique<Hexacopter>());
  fleet.push_back(std::make_unique<Octocopter>());
  for (int i=0;i<3;++i) {
   UAV &v=*fleet[i]; QCOMPARE(v.motor_count(),4+2*i);
   QVERIFY(!v.type().isEmpty()); QVERIFY(!v.takeoff_type().isEmpty()); QVERIFY(!v.landing_type().isEmpty());
   QVERIFY_EXCEPTION_THROWN(v.land(),std::logic_error);
   v.takeoff(); QVERIFY(v.isFlying());
   QVERIFY_EXCEPTION_THROWN(v.takeoff(),std::logic_error);
   const UAV &c=v; print_vehicle_info(c); QVERIFY(v.isFlying());
   v.land(); QVERIFY(!v.isFlying());
   QVERIFY_EXCEPTION_THROWN(v.land(),std::logic_error);
  }
 }
};
QTEST_APPLESS_MAIN(Tests)
#include "model_tests.moc"
