#include "Imprimeries.h"
int Imprimerie::MotherOfFactories::nbInstances = 0;
QMap<QString, Imprimerie::TheAbstractFactoryOfNotes*>* Imprimerie::MotherOfFactories::factories = 0;
