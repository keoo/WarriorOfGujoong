#include <QtCore/QString>
#include <AutoTest.hpp>

class ModelTest : public QObject
{
    Q_OBJECT

public:
    ModelTest();

private Q_SLOTS:
    void testCase1();
};

ModelTest::ModelTest()
{
}

void ModelTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

DECLARE_TEST(ModelTest)

#include "tst_modeltest.moc"

